
#include "exfat.h"
#include "CSafeUExplorer.h"
#include "CInputKeyDlg.h"
#include <QMessageBox>
#include <QApplication>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator * pTranslator = new QTranslator();
    if(pTranslator->load("./SafeUExplorer.qm")){
        a.installTranslator(pTranslator);
    }
	
	CInputKeyDlg inputDlg;
	int ret = 0;
	if (!isHidePartitionExists())
	{
		QMessageBox::warning(NULL, QObject::tr("No TF Card"), QObject::tr("Can Not Found a TF Card,or Hide Partition Size is Zero!"));
		return 0;
	}
	int getenctype = getEncType();
	if (getenctype == 0) {
		CSafeUExplorer w;

		//qApp->installNativeEventFilter((QAbstractNativeEventFilter*)&w);
		w.show();
		ret = a.exec();
	}
	else {
		inputDlg.setModal(true);
		if (inputDlg.exec() == QDialog::Accepted) {
			CSafeUExplorer w;

			//qApp->installNativeEventFilter((QAbstractNativeEventFilter*)&w);
			w.show();
			ret = a.exec();
		}
		
	}
	
    
    return ret;
}



extern "C" void MyOutputDebugString(
	char * lpOutputString
)
{
	qDebug() << lpOutputString;
}

