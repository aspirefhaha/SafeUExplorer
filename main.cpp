#include "CSafeUExplorer.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator * pTranslator = new QTranslator();
    if(pTranslator->load("./SafeUExplorer.qm")){
        a.installTranslator(pTranslator);
    }
    CSafeUExplorer w;
    w.show();

    return a.exec();
}
