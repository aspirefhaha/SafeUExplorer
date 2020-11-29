#include "FileTableWidget.h"
#include <QDebug>
#include <QDrag>
#include <QDragEnterEvent>
#include <QEvent>
#include <QMimeData>
#include <QStandardItemModel>

FileTableWidget::FileTableWidget(QWidget * parent)
    :QTableWidget(parent)
{

}

FileTableWidget::~FileTableWidget()
{

}

void FileTableWidget::mousePressEvent(QMouseEvent *event)
{
    //qDebug() << "mouse press";
    this->setAcceptDrops(false);
    emit startDrag();
    QTableWidget::mousePressEvent(event);
}

void FileTableWidget::dropEvent(QDropEvent *ev) 
{
	//qDebug() << ev->mimeData()->formats();
	if (ev->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
		
		QByteArray encodedata = ev->mimeData()->data("application/x-qabstractitemmodeldatalist");
		if (encodedata.isEmpty())
		{
			return ;
		}
		QDataStream stream(&encodedata, QIODevice::ReadOnly);
		//qDebug() << "from udisk";
		QList<int> sourceidxs;
		while (!stream.atEnd())
		{
			int row, col;
			QMap<int, QVariant> roleDataMap;
			///ÍÏµÄrowºÍcol
			stream >> row >> col >> roleDataMap;
            //qDebug() << "from udisk" <<  row << col;
			if (row == 0)
				continue;
			if(!sourceidxs.contains(row))
				sourceidxs.append(row);

            
		}
		emit acceptItemList(sourceidxs);

	}
		
}
