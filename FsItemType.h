#ifndef FSITEMTYPE_H
#define FSITEMTYPE_H


enum FSITEMTYPE {
    FTLDRIVE=1, //本地磁盘
    FTLDIR,     //本地目录
    FTLFILE,    //本地文件
    FTSPEC,     //安全U盘根目录
    FTDIR,      //安全U盘目录
    FTFILE,     //安全U盘文件
    FTUNKNOWN
};

#endif // FSITEMTYPE_H
