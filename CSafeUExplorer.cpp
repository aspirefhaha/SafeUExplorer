#include "CSafeUExplorer.h"
#include "CommonTools.h"
#include "ui_CSafeUExplorer.h"
#include <QStringList>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QIcon>
#include <QDir>

CSafeUExplorer::CSafeUExplorer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CSafeUExplorer)
{
    ui->setupUi(this);

    ui->mainToolBar->addAction(ui->actionDesktop);
    ui->mainToolBar->addAction(ui->actionUpFolder);
    ui->mainToolBar->addAction(ui->actionRefresh);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionChgPassword);
    ui->mainToolBar->addAction(ui->actionFormatUDisk);
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
    ui->twLocal->setRowCount(0);
    ui->twLocal->horizontalHeader()->setStretchLastSection(true);
    ui->twLocal->verticalHeader()->hide();
    ui->twLocal->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twLocal->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->twLocal->setShowGrid(false);
    //ui->twLocal->resizeColumnsToContents();
    ui->twLocal->setAlternatingRowColors(true);
    ui->twLocal->setHorizontalHeaderLabels(header);
    ui->twLocal->show();

    m_pSafeUModel = new CLocalModel(this);
    ui->tvSafeUDisk->setModel(m_pSafeUModel);
    ui->tvSafeUDisk->header()->setStretchLastSection(true);

}

void CSafeUExplorer::sltItemClicked(QModelIndex index)
{
    FSPrivate * itemPtr = (FSPrivate *)index.internalPointer();
    if(itemPtr!= nullptr){
        ui->leLocal->setText(itemPtr->absPath);
        refreshLocalFs(itemPtr->absPath);
    }

}

void CSafeUExplorer::refreshLocalFs(QString dirpath)
{
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
            ui->twLocal->setItem(j,2,new QTableWidgetItem(tr("Dir")));
            ui->twLocal->setItem(j,3, new QTableWidgetItem(fileinfo.lastModified().toString("yyyy-MM-dd HH:mm::ss")));
            j++;
        }

    }
    foreach(QFileInfo fileinfo,finfolist){
        if(!fileinfo.isDir()){
            col1Item = new QTableWidgetItem(icon_provider.icon(QFileIconProvider::File),fileinfo.fileName());
            ui->twLocal->setItem(j,0,col1Item);
            QTableWidgetItem * sizeItem = new QTableWidgetItem(covertHumanString(fileinfo.size()));
            sizeItem->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            ui->twLocal->setItem(j,1,sizeItem);

            ui->twLocal->setItem(j,2,new QTableWidgetItem(tr("File")));
            ui->twLocal->setItem(j,3, new QTableWidgetItem(fileinfo.lastModified().toString("yyyy-MM-dd HH:mm::ss")));
            j++;

        }

    }

    ui->twLocal->resizeColumnsToContents();
    ui->tvSafeUDisk->header()->setStretchLastSection(true);
}

void CSafeUExplorer::sltLocalItemClicked(QModelIndex index)
{
    QTableWidgetItem * item = ui->twLocal->item(index.row(),0);
    QString absPath = item->data(Qt::UserRole).toString();
    //qDebug() << absPath;
    if(absPath.isEmpty())
    {
        return;
    }
    ui->leLocal->setText(absPath);
    refreshLocalFs(absPath);
}

CSafeUExplorer::~CSafeUExplorer()
{
    delete ui;
}
