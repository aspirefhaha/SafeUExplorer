#include "UDiskFileTableWidget.h"

#include <QDebug>
#include <QDrag>
#include <QDragEnterEvent>
#include <QEvent>
#include <QMimeData>
#include <QStandardItemModel>

UDiskFileTableWidget::UDiskFileTableWidget(QWidget * parent)
    :QTableWidget(parent)
{

}

UDiskFileTableWidget::~UDiskFileTableWidget()
{

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
