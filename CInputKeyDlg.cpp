#include "exfat.h"
#include "CInputKeyDlg.h"
#include "ui_CInputKeyDlg.h"
#include <QMessageBox>
#include <QCryptographicHash>


CInputKeyDlg::CInputKeyDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CInputKeyDlg)
{
    ui->setupUi(this);
}

CInputKeyDlg::~CInputKeyDlg()
{
    delete ui;
}


void CInputKeyDlg::sltConfirm()
{
	int getenctype = getEncType();
	if (getenctype == 0) {
		accept();
	}
	if (ui->leKey->text().isEmpty()) {
		QMessageBox::warning(this,tr("Input"),tr("Please Input Key!"));
		return;
	}

	if (getenctype != 0) {
		if (getenctype > 0) {
			QString md5;
			QByteArray bb;
			bb = QCryptographicHash::hash(ui->leKey->text().toUtf8(), QCryptographicHash::Md5);
			//bb.at(0)==0x3f;
			md5.append(bb.toHex());
			unsigned char * pdata = (unsigned char *)bb.data();
			//setEncKey(pdata);
			if (memcmp(pdata, encKey, 16) == 0) {

				accept();
			}
			else {
				QMessageBox::warning(this, tr("Key Error"), tr("Please Input Correct Key!"));
			}
		}
		else {
			switch (getenctype) {
			case -1:
			case -2:
				QMessageBox::warning(this, tr("Open Failed"), tr("Open TF Card Failed Or Hide Partition Not Exists!"));
				break;
			case -3:
				QMessageBox::warning(this, tr("Open Failed"), tr("GetCap Failed!"));
				break;
			case -4:
				QMessageBox::warning(this, tr("Open Failed"), tr("Not Authorized!"));
				break;
			default:
				QMessageBox::warning(this, tr("Open Failed"), tr("Unknown Error"));
				break;
			}
		}
	}
}