#ifndef CGLOBALMODEL_H
#define CGLOBALMODEL_H

#include <QAbstractItemModel>
#include <qthread.h>
#include <Windows.h>
#include <QList>
#include <QtCore>
#include "CFSPrivate.h"

class CGlobalModel : public QAbstractItemModel
{
	Q_OBJECT

public:
    CGlobalModel(QObject *parent);
    ~CGlobalModel();
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

	void notifyChange(const QModelIndex & someindex,const QModelIndex &rbindex){
		emit dataChanged(someindex,rbindex);
	}

public slots:

	
private:
    QList<FSPrivate *> m_rootDrives;
//	QList<ExfatFSPrivate *> m_allItems;
//	QString m_FSFileName;
};

#endif // CGLOBALMODEL_H
