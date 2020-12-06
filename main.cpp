
#include "exfat.h"
#include "CSafeUExplorer.h"
#include "CInputKeyDlg.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator * pTranslator = new QTranslator();
    if(pTranslator->load("./SafeUExplorer.qm")){
        a.installTranslator(pTranslator);
    }
	CInputKeyDlg inputDlg;
	int ret = 0;
	int getenctype = getEncType();
	if (getenctype == 0) {
		CSafeUExplorer w;
		w.show();
		ret = a.exec();
	}
	else {
		inputDlg.setModal(true);
		if (inputDlg.exec() == QDialog::Accepted) {
			CSafeUExplorer w;
			w.show();
			ret = a.exec();
		}
		
	}
	
    
    return ret;
}

