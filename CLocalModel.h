#ifndef CLOCALMODEL_H
#define CLOCALMODEL_H

#include <QAbstractItemModel>
#include <qthread.h>
#include <Windows.h>
#include <QtCore>
#include "FsItemType.h"


struct ExfatFSPrivate 
{
    FSITEMTYPE fstype;
    QString absPath;
	int m_col;
	int m_row;
    //struct exfat * m_pexfatRoot;
	ExfatFSPrivate * m_pParent;
	
	//QList<ExfatFSPrivate *> m_lsChildren;

    bool match(QString name,FSITEMTYPE fstype){
		return name==absPath && fstype==this->fstype;
	}

    explicit ExfatFSPrivate(FSITEMTYPE tt,int row,int col,QString abspath,ExfatFSPrivate * parent)
        :fstype(tt)
        ,m_row(row)
        ,m_col(col)
        ,m_pParent(parent)
        ,absPath(abspath)
        //,m_pexfatRoot(NULL)
    {
			if(parent!=NULL){
                //if(parent->m_pexfatRoot != NULL)
                //	m_pexfatRoot = parent->m_pexfatRoot;
			}
	}
};

class CLocalModel : public QAbstractItemModel
{
	Q_OBJECT

public:
    CLocalModel(QObject *parent);
    ~CLocalModel();
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

//	void setFsFilename(QString & filename){
//		m_FSFileName = filename;
//	}

//	int resetfs();

//	int formatfs(const char * spec);

//  void addRootDevice(QString,FSITEMTYPE);

//    ExfatFSPrivate * findOutFSChild (QString abspath,FSITEMTYPE type)const;

//	void refreshRootDevice();
//	void editFile(const QString&);
//	const static int ONCEBLOCK = 512;

//	bool isDirectory(const char * dirpath);
//	bool isRootItem(ExfatFSPrivate * priv)const;

	void notifyChange(const QModelIndex & someindex,const QModelIndex &rbindex){
		emit dataChanged(someindex,rbindex);
	}



public slots:

	
private:
    QList<ExfatFSPrivate *> m_rootDrives;
//	QList<ExfatFSPrivate *> m_allItems;
//	QString m_FSFileName;
};

#endif // CLOCALMODEL_H
