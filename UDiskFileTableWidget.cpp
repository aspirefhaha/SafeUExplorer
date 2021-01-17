#include "UDiskFileTableWidget.h"

#include <QDebug>
#include <QDrag>
#include <QDragEnterEvent>
#include <QEvent>
#include <QMimeData>
#include <QStandardItemModel>
#include <QAction>
#include <QMenu>

UDiskFileTableWidget::UDiskFileTableWidget(QWidget * parent)
    :QTableWidget(parent)
{

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(show_menu(QPoint)));
	menu = new QMenu(this);
	pnew = new QAction(tr("Del"), this);
	connect(pnew, SIGNAL(triggered()), this, SLOT(clickgoose()));
	menu->addAction(pnew);
	menuProperty = new QMenu(this);
	propertyAction = new QAction(tr("Property"), this);
	//connect(propertyAction, SIGNAL(triggered()), this, SLOT(clickproperty()));
	menuProperty->addAction(propertyAction);
}

UDiskFileTableWidget::~UDiskFileTableWidget()
{

}

void UDiskFileTableWidget::show_menu(QPoint pt)
{
	//获得鼠标点击的x，y坐标点 
	/*int x = pt.x();
	int y = pt.y();*/
	QList<QTableWidgetItem*>items = selectedItems();

	int count = items.count() / this->columnCount();
	/*if (count > 1)
	{
		menuProperty->move(cursor().pos());
		menuProperty->show();
		return;
	}*/
	
	//std::sort(vecItemIndex.begin(), vecItemIndex.end());
	m_selIndex = this->indexAt(pt);
	QTableWidgetItem * pitem = item(m_selIndex.row(), 0);
	if (pitem) {
		qDebug() << "Del" << pitem->data(Qt::UserRole).toString();
		if (pitem->text() == "..")
			return;
		menu->move(cursor().pos());
		menu->show();
	}
	
}

void UDiskFileTableWidget::clickgoose()
{
	//int row = index.row();//获得QTableWidget列表点击的行数 
	//std::vector<int> vecItemIndex;//保存选中行的索引
	QItemSelectionModel *selections = this->selectionModel(); //返回当前的选择模式  
	QModelIndexList selectedsList = selections->selectedIndexes(); //返回所有选定的模型项目索引列表  

	//for (int i = 0; i < selectedsList.count(); i++)
	//{
	//	//vecItemIndex.push_back(selectedsList.at(i).row());
	//}
	emit DelUdiskItem(selectedsList);
}

void UDiskFileTableWidget::mousePressEvent(QMouseEvent *event)
{
    this->setAcceptDrops(false);
    emit startDrag();
    QTableWidget::mousePressEvent(event);
}

void UDiskFileTableWidget::dropEvent(QDropEvent *ev)
{
    //qDebug() << ev->mimeData()->formats();
    if (ev->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {

        QByteArray encodedata = ev->mimeData()->data("application/x-qabstractitemmodeldatalist");
        if (encodedata.isEmpty())
        {
            return ;
        }
        QDataStream stream(&encodedata, QIODevice::ReadOnly);
		//qDebug() << "from local";
		QList<int> sourceidxs;
		while (!stream.atEnd())
		{
			int row, col;
			QMap<int, QVariant> roleDataMap;
			///拖的row和col
			stream >> row >> col >> roleDataMap;
			//qDebug() << "from udisk" <<  row << col;
			if (row == 0)
				continue;
			if (!sourceidxs.contains(row))
				sourceidxs.append(row);


		}
		emit acceptItemList(sourceidxs);

    }

}
