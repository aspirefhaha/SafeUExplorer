#ifndef CFSPRIVATE_H
#define CFSPRIVATE_H

#include "FsItemType.h"
#include <QString>
#include <QList>

struct FSPrivate
{
    FSITEMTYPE fstype;
    QString absPath;
    QString m_label;
    int m_col;
    int m_row;
    struct exfat * m_pexfatRoot;
    FSPrivate * m_pParent;

    QList<FSPrivate *> m_lsChildren;
	bool firsted;

    bool match(QString name,FSITEMTYPE fstype){
        return name==absPath && fstype==this->fstype;
    }

    explicit FSPrivate(FSITEMTYPE tt,QString label,int row,int col,QString abspath,FSPrivate * parent, void *exfat=NULL)
        :fstype(tt)
        ,m_row(row)
        ,m_col(col)
        ,m_pParent(parent)
        ,absPath(abspath)
        ,m_label(label)
		,firsted(true)
        ,m_pexfatRoot((struct exfat *)exfat)
    {
            if(parent!=NULL){
                //if(parent->m_pexfatRoot != NULL)
                //	m_pexfatRoot = parent->m_pexfatRoot;
            }
    }
};


#endif // CFSPRIVATE_H
