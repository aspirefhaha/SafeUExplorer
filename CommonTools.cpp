#ifndef _COMMONTOOLS_H
#define _COMMONTOOLS_H

#define Q_COMPILER_UNICODE_STRINGS

#include "CommonTools.h"
#include <Windows.h>

#include <QDebug>

//char* GBKToUTF8( const char* chGBK ){
//        DWORD dWideBufSize=MultiByteToWideChar(CP_ACP, 0,(LPCSTR)chGBK,-1, NULL, 0);
//        wchar_t * pWideBuf=new wchar_t[dWideBufSize];
//        wmemset(pWideBuf, 0, dWideBufSize);
//        MultiByteToWideChar(CP_ACP,0,(LPCSTR)chGBK,-1,pWideBuf,dWideBufSize);
//        DWORD dUTF8BufSize=WideCharToMultiByte(CP_UTF8,0,(LPCWSTR)pWideBuf,-1,NULL,0,NULL,NULL);
//        char * pUTF8Buf=new char[dUTF8BufSize];
//        memset(pUTF8Buf, 0, dUTF8BufSize);
//        DWORD chgSize = WideCharToMultiByte( CP_UTF8,0,(LPCWSTR)pWideBuf,-1,pUTF8Buf,dUTF8BufSize,NULL,NULL);
//        //char tmpstr[256];
//        //sprintf(tmpstr,"widechartomb ret %d\n",chgSize);
//        //OutputDebugString(tmpstr);
//        delete[]pWideBuf;
//        return pUTF8Buf;
//}

QString GetVolumeLabel(QString path)
{
    LPTSTR lpVolumeNameBuffer=new TCHAR[12];//磁盘卷标
    DWORD nVolumeNameSize=12;// 卷标的字符串长度
    DWORD VolumeSerialNumber;//硬盘序列号
    DWORD MaximumComponentLength;// 最大的文件长度
    LPTSTR lpFileSystemNameBuffer=new TCHAR[10];// 存储所在盘符的分区类型的长指针变量
    DWORD nFileSystemNameSize=10;// 分区类型的长指针变量所指向的字符串长度
    DWORD FileSystemFlags;// 文件系统的一此标志

    GetVolumeInformation((LPTSTR)path.utf16(),
      lpVolumeNameBuffer, nVolumeNameSize,
      &VolumeSerialNumber, &MaximumComponentLength,
      &FileSystemFlags,
      lpFileSystemNameBuffer, nFileSystemNameSize);

    //qDebug() << "SerialNumber:" << QString::number(VolumeSerialNumber,16);

    return QString::fromUtf16((ushort *)lpVolumeNameBuffer);

}

QString covertHumanString(qlonglong orisize){
    double ksize = orisize / 1024.0;
    double msize = 0;
    if(orisize>4096LL){

        if(ksize > 1000){
            msize = ksize / 1024;
            double gsize = 0;
            if(msize > 1000){
                gsize = msize / 1024;
                return QString::number(gsize) + " GB";
            }
            return QString::number(msize) + " MB";
        }
        else{
             return QString::number(ksize) + " KB";
        }
    }
    return QString::number(orisize) + " B";
}
#endif
