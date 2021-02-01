
#include "exfat.h"
#include "exfatfs.h"
#include "CSafeUExplorer.h"
#include "CommonTools.h"
#include "ui_CSafeUExplorer.h"
#include <QStringList>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QIcon>
#include <QDir>
#include <QDebug>
#include "CopyItem.h"
#include "CopyDlg.h"
#include <QMessageBox>
#include <QInputDialog>

#include <Windows.h>
#include <dbt.h>
#include "CFormatDlg.h"
#include "mkexfat.h"
#include "sdcryptostor.h"
#include <windowsx.h>

/**
1.0.1 发布
1.0.2 加快读写速度
1.0.3 增加对USB插拔设备的监视
1.0.4 修改了输入框不显示的问题
1.0.5 输入框密码改成*
1.0.6 拷贝的时候判断剩余空间，增加了文件系统剩余空间显示
1.0.7 支持AES256加密算法
*/

#define SIZECOLWIDTH 70
#define NAMECOLWIDTH 200
#define TYPECOLWIDTH 70

CSafeUExplorer::CSafeUExplorer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CSafeUExplorer),
	copyThread(this),
	udiskFocused(false)
{
    ui->setupUi(this);
	

    ui->mainToolBar->addAction(ui->actionDesktop);
    ui->mainToolBar->addAction(ui->actionUpFolder);
    ui->mainToolBar->addAction(ui->actionRefresh);
	//if (m_pGlobalModel->ef && m_pGlobalModel->ef->dev) {
	ui->mainToolBar->addSeparator();
	//ui->mainToolBar->addAction(ui->actionChgPassword);
	ui->mainToolBar->addAction(ui->actionFormatUDisk);
	//}

	ui->mainToolBar->addSeparator();
	ui->mainToolBar->addAction(ui->actionQuit);
	ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->splitter->setStretchFactor(0,20);
    ui->splitter->setStretchFactor(1,40);
    ui->splitter->setStretchFactor(2,40);

    m_pGlobalModel = new CGlobalModel(this);

	
    ui->tvGlobal->setHeaderHidden(true);
    //ui->tvGlobal->verticalHeader()->setVisible(false);
    ui->tvGlobal->header()->setStretchLastSection(true);
    ui->tvGlobal->setModel(m_pGlobalModel);

    //m_pLocalModel = new CLocalModel(this);
    //ui->tvLocalDisks->setModel(m_pLocalModel);
    //ui->tvLocalDisks->header()->setStretchLastSection(true);
    QStringList header;
    header<<tr("Name")<<tr("Size")<<tr("Type")<<tr("Date Modified");
    ui->twLocal->setColumnCount(4);
    ui->twLocal->setColumnWidth(0,NAMECOLWIDTH);
    ui->twLocal->setColumnWidth(1,SIZECOLWIDTH);
    ui->twLocal->setColumnWidth(2, TYPECOLWIDTH);
    ui->twLocal->setRowCount(0);
    ui->twLocal->horizontalHeader()->setStretchLastSection(true);
    ui->twLocal->verticalHeader()->hide();
    ui->twLocal->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twLocal->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->twLocal->setShowGrid(false);
    //ui->twLocal->resizeColumnsToContents();
    ui->twLocal->setAlternatingRowColors(true);
    ui->twLocal->setHorizontalHeaderLabels(header);

    ui->twLocal->setDragDropMode(QAbstractItemView::DragDrop);
    ui->twLocal->setDragEnabled(true);
    ui->twLocal->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twLocal->show();

    //m_pSafeUModel = new CLocalModel(this);
	ui->twSafeUDisk->setColumnCount(4);
	ui->twSafeUDisk->setRowCount(0);
    ui->twSafeUDisk->setColumnWidth(0,NAMECOLWIDTH);
    ui->twSafeUDisk->setColumnWidth(1,SIZECOLWIDTH);
    ui->twSafeUDisk->setColumnWidth(2, TYPECOLWIDTH);
	ui->twSafeUDisk->horizontalHeader()->setStretchLastSection(true);
	ui->twSafeUDisk->verticalHeader()->hide();
	ui->twSafeUDisk->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->twSafeUDisk->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->twSafeUDisk->setShowGrid(false);
	//ui->twSafeUDisk->resizeColumnsToContents();
	ui->twSafeUDisk->setAlternatingRowColors(true);
	ui->twSafeUDisk->setHorizontalHeaderLabels(header);
    ui->twSafeUDisk->setDragDropMode(QAbstractItemView::DragDrop);
    ui->twSafeUDisk->setDragEnabled(true);
    ui->twSafeUDisk->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->twSafeUDisk->show();

    connect(ui->twSafeUDisk, SIGNAL(startDrag()), this, SLOT(sltUDiskStartDrag()));
    connect(ui->twLocal, SIGNAL(startDrag()), this , SLOT(sltLocalStartDrag()));
	connect(ui->twLocal, SIGNAL(acceptItemList(QList<int>)), this, SLOT(sltAcceptUDiskItemList(QList<int>)));
	connect(ui->twSafeUDisk, SIGNAL(acceptItemList(QList<int>)), this, SLOT(sltAcceptLocalItemList(QList<int>)));

	connect(ui->twSafeUDisk, SIGNAL(DelUdiskItem(QList<QModelIndex>)), this, SLOT(sltDelUDiskFile(QList<QModelIndex>)));

    connect(ui->actionRefresh,SIGNAL(triggered(bool)),this,SLOT(sltRefresh(bool)));
	connect(ui->actionDesktop, SIGNAL(triggered(bool)), this, SLOT(sltDesktop(bool)));
	connect(ui->actionUpFolder, SIGNAL(triggered(bool)), this, SLOT(sltUpfolder(bool)));
	connect(ui->actionQuit, SIGNAL(triggered(bool)), this, SLOT(sltQuit(bool)));
	connect(ui->actionFormatUDisk, SIGNAL(triggered(bool)), this, SLOT(sltFormat(bool)));

	connect(&copyThread, SIGNAL(total(int, qint64)), &copyDlg, SLOT(setTotal(int, qint64)));
	connect(&copyThread, SIGNAL(curFinished(qint64, int)), &copyDlg, SLOT(setCurPos(qint64, int)));
	connect(&copyThread, SIGNAL(curFileProg(qint64, qint64)), &copyDlg, SLOT(setCurFileProg(qint64, qint64)));
	connect(&copyThread, SIGNAL(curItem(QString, QString)), &copyDlg, SLOT(setCurItem(QString, QString)));
	connect(&copyThread, SIGNAL(workFinished()), &copyDlg, SLOT(sltQuit()));
	connect(&copyThread, SIGNAL(speed(qreal)), &copyDlg, SLOT(sltSpeed(qreal)));
	connect(&copyThread, SIGNAL(LeftSizeError(qint64, qint64)), &copyDlg, SLOT(sltLeftSizeError(qint64, qint64)));


	connect(&copyDlg, SIGNAL(finished()), this, SLOT(sltUpdateSize()));


	connect(&copyThread, SIGNAL(workFinished()), &delDlg, SLOT(sltQuit()));
	connect(&copyThread, SIGNAL(total(int, qint64)), &delDlg, SLOT(setTotal(int, qint64)));
	connect(&copyThread, SIGNAL(curItem(QString, QString)), &delDlg, SLOT(setCurItem(QString, QString)));
	connect(&copyThread, SIGNAL(curFinished(qint64, int)), &delDlg, SLOT(setCurPos(qint64, int)));

	connect(&delDlg, SIGNAL(finished()), this, SLOT(sltUpdateSize()));
	
	connect(&copyDlg, SIGNAL(wantQuit()), this, SLOT(sltWantCancelCopy()));

    if(m_pGlobalModel->ef && m_pGlobalModel->ef->dev){
		updateLeftSize();
        QString udiskrootdir = "/";
        refreshUDiskFs(udiskrootdir);
		copyThread.ef = m_pGlobalModel->ef;
    }
	else {
		QMessageBox::warning(this, tr("Mount Failed!"), tr("Hide Partition Not Formated!"));
	}

    QString localDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    refreshLocalFs(localDesktop);
	QLabel *msgLabel = new QLabel;

	msgLabel->setStyleSheet(" QLabel{ color: grey }");

	msgLabel->setText("V1.0.7");

	statusBar()->addWidget(msgLabel);
}

void CSafeUExplorer::sltDelUDiskFile(QList<QModelIndex> indexs)
{
	foreach(auto index , indexs) {
		QTableWidgetItem * item = ui->twSafeUDisk->item(index.row(), 0);
		copyThread.m_DelItemList.append(item->data(Qt::UserRole).toString());
	}
    //qDebug() << "Del" <<  item->data(Qt::UserRole).toString();
    //copyThread.RemovePath(item->data(Qt::UserRole).toString());
	copyThread.m_bQuit = false;
    delDlg.init();
    delDlg.setModal(true);
	copyThread.start();
	delDlg.exec();
    QString uCurPath = ui->leUDisk->text();
    refreshUDiskFs(uCurPath);
}

void CSafeUExplorer::updateLeftSize()
{
	QString firststr = tr("SafeUDisk(%1GB)").arg(exfat_get_size(m_pGlobalModel->ef->dev) / 1024LL / 1024LL / 1024LL + 1);
	long long leftsize = exfat_count_free_clusters(m_pGlobalModel->ef);
	leftsize <<= (m_pGlobalModel->ef->sb->spc_bits + m_pGlobalModel->ef->sb->sector_bits);
	QString secondstr = tr("Left %1").arg(covertHumanString(leftsize));
	ui->lbSafeUDisk->setText(firststr  + " " + secondstr);
}

void CSafeUExplorer::sltUpdateSize()
{
	updateLeftSize();
}

void CSafeUExplorer::sltFormat(bool)
{
	if (QMessageBox::Yes == QMessageBox::warning(this, tr("Data Lost Warning"), tr("Format Fs will Lost All Data in both Normal and Hide Partition,Continue?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)) {
		ui->twSafeUDisk->clearContents();
		ui->twSafeUDisk->setRowCount(0);
		if (m_pGlobalModel->ef && m_pGlobalModel->ef->dev) {
			exfat_unmount(m_pGlobalModel->ef);
			m_pGlobalModel->ef->dev = NULL;
			copyThread.ef = NULL;
		}
        format_fs();
        if (0 == exfat_mount(m_pGlobalModel->ef, "nothing", "rw")) {
            if (m_pGlobalModel->ef && m_pGlobalModel->ef->dev) {
				updateLeftSize();
				copyThread.ef = m_pGlobalModel->ef;
                QString rootDir = "/";
                refreshUDiskFs(rootDir);
                QMessageBox::information(this, tr("Succeed"), tr("Format Operation Succeed!"), QMessageBox::Yes, QMessageBox::Yes);
            }
        }
        else {
            QMessageBox::warning(this, tr("Format Failed"), tr("TF Card Failed or  Not Authoried!!!"), QMessageBox::Yes, QMessageBox::Yes);
        }

	}
	
}

void CSafeUExplorer::sltUpfolder(bool checked)
{
	Q_UNUSED(checked)
	if (udiskFocused) {
		if (m_pGlobalModel->ef==NULL)
			return;
		QString dirpath = ui->leUDisk->text();
		int pos1 = dirpath.lastIndexOf('/');
		if (pos1 == -1) {
			QString udiskrootdir = "/";
			refreshUDiskFs(udiskrootdir);
			return;
		}
		QString parentpath = dirpath.mid(0, pos1);
		if (parentpath == "")
			parentpath = "/";
		refreshUDiskFs(parentpath);
	}
	else {
		QString localcurfolder = ui->leLocal->text() + "/..";
		QDir dir(localcurfolder);
		QString upfolder =dir.absolutePath();
		refreshLocalFs(upfolder);
	}
}

bool CSafeUExplorer::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	MSG* msg = reinterpret_cast<MSG*>(message);
	int msgType = msg->message;
	if (msgType == WM_DEVICECHANGE)
	{
		PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
		switch (msg->wParam) {
		case DBT_DEVICEARRIVAL:

			if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
			{
				PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
				if (lpdbv->dbcv_flags == 0)
				{
					qDebug() << __FUNCTION__ << "u disk come";
					//emit sigUDiskCome();
				}
			}
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
			{
				PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
				if (lpdbv->dbcv_flags == 0)
				{
					qDebug() << __FUNCTION__ << "u disk remove";
					char kName[256] = { 0 };
					char * sName = kName;
					DWORD len = 0;
					int retenctype = -1;
					int enuret = sd_EnumDevice(&sName, &len);
					if (enuret != SDR_OK) {
						QMessageBox::warning(this, tr("Unplug Unexpected"), tr("Unplug SafeUDisk at Inappropriate Time! Data May be Damanged!!!!"));
						this->close();

					}
					//emit sigUDiskRemove();
				}
			}
			break;
		case DBT_DEVNODES_CHANGED:
		{
			qDebug() << "USB_change";
		}
		break;
		}
	}
	else if (msg->message == WM_GETMINMAXINFO || msg->message == WM_NCCREATE
				|| msg->message == WM_NCCALCSIZE
				|| msg->message == WM_CREATE
				|| msg->message == WM_SIZE
				|| msg->message == WM_MOVE)
	{
		return false;
	}
	
	return QMainWindow::nativeEvent(eventType, message, result);

}

void CSafeUExplorer::sltDesktop(bool checked)
{
	Q_UNUSED(checked)
	QString localDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	refreshLocalFs(localDesktop);
}

void CSafeUExplorer::sltLocalStartDrag()
{
    ui->twSafeUDisk->setAcceptDrops(true);
}
void CSafeUExplorer::sltUDiskStartDrag()
{
    ui->twLocal->setAcceptDrops(true);
}


void CSafeUExplorer::sltQuit(bool checked)
{
	Q_UNUSED(checked)
	QApplication::exit();
}
void CSafeUExplorer::sltRefresh(bool checked)
{
	Q_UNUSED(checked)
	if (m_pGlobalModel->ef && m_pGlobalModel->ef->dev) {
		QString udiskrootdir = "/";
		refreshUDiskFs(udiskrootdir);
		copyThread.ef = m_pGlobalModel->ef;
	}
	else {
		QMessageBox::warning(this, tr("Not Formated!"), tr("Hide Partition Not Formated!"));
	}
	QString localDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	refreshLocalFs(localDesktop);
}

void CSafeUExplorer::sltAcceptLocalItemList(QList<int> list)
{
	//qDebug() << "Copy to UDisk";
	copyThread.copyItems.clear();
	for (auto row : list) {
		//qDebug() << row;
		QTableWidgetItem * item = ui->twLocal->item(row, 0);
        //qDebug() << item->data(Qt::UserRole).toString();
		CopyItem copyItem;
		copyItem.source = item->data(Qt::UserRole).toString();
		copyItem.sourceType = FTLDRIVE;
		copyItem.sourceDir = ui->leLocal->text();
		copyItem.size = 0;
		copyItem.targetDir = ui->leUDisk->text();
		copyItem.targetType = FTUSAFE;
		copyThread.copyItems.append(copyItem);
	}
	copyThread.m_bQuit = false;

	copyDlg.init();
	copyThread.start();
	copyDlg.setModal(true);
	copyDlg.exec();
    QString curUPath = ui->leUDisk->text();
    refreshUDiskFs(curUPath);
}


void CSafeUExplorer::sltAcceptUDiskItemList(QList<int> list)
{
	//qDebug() << "Copy to Local";
	copyThread.copyItems.clear();
	for (auto row : list) {
		//qDebug() << row;
		QTableWidgetItem * item = ui->twSafeUDisk->item(row, 0);
		qDebug() << item->data(Qt::UserRole).toString();
		CopyItem copyItem;
		copyItem.source = item->data(Qt::UserRole).toString();
		copyItem.sourceType = FTUSAFE;
		copyItem.sourceDir = ui->leUDisk->text();
		copyItem.size = 0;
		copyItem.targetDir = ui->leLocal->text();
		copyItem.targetType = FTLDRIVE;
		copyThread.copyItems.append(copyItem);
	}
	copyThread.m_bQuit = false;
	copyThread.start();
	copyDlg.init();
	copyDlg.setModal(true);
	copyDlg.exec();
    QString lcurPath = ui->leLocal->text();
    refreshLocalFs(lcurPath);
}

void CSafeUExplorer::sltWantCancelCopy()
{
	copyThread.try_quit();
	copyThread.wait();
	copyDlg.sltQuit();
}


void CSafeUExplorer::sltItemClicked(QModelIndex index)
{
    FSPrivate * itemPtr = (FSPrivate *)index.internalPointer();
    if(itemPtr!= nullptr){
		switch (itemPtr->fstype) {
		case FTLDIR:
		case FTLDRIVE:
			ui->leLocal->setText(itemPtr->absPath);
			refreshLocalFs(itemPtr->absPath);
			break;
		case FTDIR:
		case FTUSAFE:
			ui->leUDisk->setText(itemPtr->absPath);
			refreshUDiskFs(itemPtr->absPath);
			break;
		}
    }

}


void CSafeUExplorer::refreshUDiskFs(QString &dirpath)
{
	struct exfat * ef = m_pGlobalModel->ef;
	int rc;
	struct exfat_node * pdir;
    //char utf8str[EXFAT_UTF8_NAME_BUFFER_MAX] = { 0 };
	int j = 0;
	QFileIconProvider icon_provider;
	QTableWidgetItem * col1Item = nullptr;
	//ui->twSafeUDisk->clear();
	if (ef==NULL || ef->dev == NULL)
		return;
	QString realpath = dirpath;
	if (dirpath != "/") {
		if (dirpath.endsWith("/"))
			realpath = dirpath.mid(0, dirpath.length() - 1);
	}
	ui->leUDisk->setText(realpath);
	rc = exfat_lookup(ef, &pdir, realpath.toUtf8().data());
	if (rc != 0)
		return;
	struct exfat_node* node;
	FSPrivate * newItem = nullptr;
	do {
		struct exfat_iterator it;
		rc = exfat_opendir(ef, pdir, &it);
		if (rc != 0)
			break;
		int itemcount = 0;
		while ((node = exfat_readdir(&it))) {
			exfat_put_node(ef, node);
			itemcount++;
		}
		exfat_closedir(ef, &it);
		ui->twSafeUDisk->setRowCount(itemcount+1);
		col1Item = new QTableWidgetItem(icon_provider.icon(QFileIconProvider::Folder), "..");
		//col1Item->setData(Qt::UserRole, realpath + "/..");
        QString parentpath ;
        int pos1 = realpath.lastIndexOf('/');
        parentpath = realpath.mid(0,pos1+1);
        col1Item->setData(Qt::UserRole, parentpath);
		ui->twSafeUDisk->setItem(j, 0, col1Item);
		j++;
		exfat_opendir(ef, pdir, &it);
		if (rc != 0)
			break;
		QList<struct exfat_node *> filenodes;
		while ((node = exfat_readdir(&it)))
		{
			if (node->attrib & EXFAT_ATTRIB_DIR) {
				int len = exfat_utf16_length((const le16_t *)&node->name);
				QString itemname = QString::fromUtf16((const  unsigned short *)&node->name, len);
				col1Item = new QTableWidgetItem(icon_provider.icon(QFileIconProvider::Folder), itemname);
				QString absname = dirpath + "/" + itemname;
				if (dirpath == "/") {
					absname = dirpath + itemname;
				}
				col1Item->setData(Qt::UserRole, absname);
				ui->twSafeUDisk->setItem(j, 0, col1Item);
                QTableWidgetItem * typeItem = new QTableWidgetItem(tr("Dir"));
                typeItem->setTextAlignment(Qt::AlignCenter);
                ui->twSafeUDisk->setItem(j, 2, typeItem);
				ui->twSafeUDisk->setItem(j, 1, new QTableWidgetItem(""));
				ui->twSafeUDisk->setItem(j, 3, new QTableWidgetItem(QDateTime::fromTime_t(node->mtime).toString("yyyy-MM-dd hh:mm:ss")));
                j++;
				
			}
			else {
                filenodes.append(node);
			}
            exfat_put_node(ef, node);
		}
		foreach(struct exfat_node * node, filenodes) {
            exfat_get_node(node);
			int len = exfat_utf16_length((const le16_t *)&node->name);
			QString itemname = QString::fromUtf16((const  unsigned short *)&node->name, len);
			col1Item = new QTableWidgetItem(icon_provider.icon(QFileIconProvider::File), itemname);
			QString absname = dirpath + "/" + itemname;
			if (dirpath == "/") {
				absname = dirpath + itemname;
			}
			col1Item->setData(Qt::UserRole, absname);
            ui->twSafeUDisk->setItem(j, 0, col1Item);
            QTableWidgetItem * typeItem = new QTableWidgetItem(tr("File"));
            typeItem->setTextAlignment(Qt::AlignCenter);
            ui->twSafeUDisk->setItem(j, 2, typeItem);
            QTableWidgetItem * sizeItem = new QTableWidgetItem(covertHumanString(node->size));
            sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            ui->twSafeUDisk->setItem(j, 1, sizeItem);
			ui->twSafeUDisk->setItem(j, 3, new QTableWidgetItem(QDateTime::fromTime_t(node->mtime).toString("yyyy-MM-dd hh:mm:ss")));
			exfat_put_node(ef, node);
			j++;
		}
		exfat_closedir(ef, &it);
	} while (0);
	exfat_put_node(ef, pdir);
	return;
}

void CSafeUExplorer::refreshLocalFs(QString &dirpath)
{
	ui->leLocal->setText(dirpath);
    QDir dir(dirpath);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Time | QDir::Reversed);

    QFileInfoList finfolist =  dir.entryInfoList();
    int itemcount = finfolist.size();
    ui->twLocal->setRowCount(itemcount+1);
    QFileIconProvider icon_provider;
    int j = 0;
    QTableWidgetItem * col1Item = new QTableWidgetItem(icon_provider.icon(QFileIconProvider::Folder),"..");
    col1Item->setData(Qt::UserRole,QDir(dirpath + "/..").absolutePath());
    ui->twLocal->setItem(j,0,col1Item);
    j++;
    foreach(QFileInfo fileinfo,finfolist){
        if(fileinfo.isDir()){
            col1Item = new QTableWidgetItem(icon_provider.icon(QFileIconProvider::Folder),fileinfo.fileName());
            col1Item->setData(Qt::UserRole,fileinfo.absoluteFilePath());
            ui->twLocal->setItem(j,0,col1Item);
            ui->twLocal->setItem(j, 1, new QTableWidgetItem(""));

            QTableWidgetItem * typeItem = new QTableWidgetItem(tr("Dir"));
            typeItem->setTextAlignment(Qt::AlignCenter);
            ui->twLocal->setItem(j, 2, typeItem);
            ui->twLocal->setItem(j,3, new QTableWidgetItem(fileinfo.lastModified().toString("yyyy-MM-dd HH:mm::ss")));
            j++;
        }

    }
    foreach(QFileInfo fileinfo,finfolist){
        if(!fileinfo.isDir()){
            col1Item = new QTableWidgetItem(icon_provider.icon(QFileIconProvider::File),fileinfo.fileName());
			col1Item->setData(Qt::UserRole, fileinfo.absoluteFilePath());
            ui->twLocal->setItem(j,0,col1Item);
            QTableWidgetItem * sizeItem = new QTableWidgetItem(covertHumanString(fileinfo.size()));
            sizeItem->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            ui->twLocal->setItem(j,1,sizeItem);

            QTableWidgetItem * typeItem = new QTableWidgetItem(tr("File"));
            typeItem->setTextAlignment(Qt::AlignCenter);
            ui->twLocal->setItem(j, 2, typeItem);
            ui->twLocal->setItem(j,3, new QTableWidgetItem(fileinfo.lastModified().toString("yyyy-MM-dd HH:mm::ss")));
            j++;

        }
    }
    //ui->twLocal->resizeColumnsToContents();
    //ui->twSafeUDisk->header()->setStretchLastSection(true);
}

void CSafeUExplorer::sltUDiskItemClicked(QModelIndex index)
{
	udiskFocused = true;
	QTableWidgetItem * item = ui->twSafeUDisk->item(index.row(), 0);
	QString absPath = item->data(Qt::UserRole).toString();
	if (absPath == "/")
	{

	}
	else if (absPath.endsWith("/"))
		absPath = absPath.left(absPath.length() - 1);
    //qDebug() << absPath;
	struct exfat_node * pdir;
	struct exfat * ef = m_pGlobalModel->ef;
	if (ef && ef->dev) {
		int rc = exfat_lookup(ef, &pdir, absPath.toUtf8().data());
		if (rc != 0)
			return;
		if (pdir->attrib & EXFAT_ATTRIB_DIR)
			refreshUDiskFs(absPath);
		exfat_put_node(ef, pdir);
	}
	
		
}

void CSafeUExplorer::sltLocalCellClick(int, int)
{
	qDebug() << "local cell";
	udiskFocused = false;
}
void CSafeUExplorer::sltUDiskCellClick(int, int)
{
	//qDebug() << "disk cell";
	udiskFocused = true;
}

void CSafeUExplorer::sltLocalItemClicked(QModelIndex index)
{
	udiskFocused = false;
    QTableWidgetItem * item = ui->twLocal->item(index.row(),0);
    QString absPath = item->data(Qt::UserRole).toString();
    //qDebug() << absPath;
    if(absPath.isEmpty())
    {
        return;
    }
	QFileInfo fileinfo(absPath);
	if (!fileinfo.isDir())
		return;
    refreshLocalFs(absPath);
	//findLocalItem(absPath);
}

CSafeUExplorer::~CSafeUExplorer()
{
	if (copyThread.isRunning()) {
		copyThread.try_quit();
		copyThread.wait();
	}
    delete ui;
}

void CSafeUExplorer::findLocalItem(QString & abspath)
{
    //QModelIndex findidx = m_pGlobalModel->findLocalItem(abspath);
}
