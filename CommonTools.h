#ifndef COMMONTOOLS_H
#define COMMONTOOLS_H
#include <QString>

char* GBKToUTF8( const char* chGBK );
QString GetVolumeLabel(QString path);
QString covertHumanString(qlonglong orisize);

#endif // COMMONTOOLS_H
