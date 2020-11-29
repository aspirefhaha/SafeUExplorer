#ifndef COPYITEM_H
#define COPYITEM_H

#include <QtCore>
#include "FsItemType.h"

struct CopyItem {
    QString source;
	QString sourceDir;
    FSITEMTYPE sourceType;
    QString target;
	QString targetDir;
    FSITEMTYPE targetType;
    quint64 size;

};

#endif // COPYITEM_H
