#include "DelDlg.h"
#include "ui_DelDlg.h"

DelDlg::DelDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DelDlg)
	, totalcount(0)
{
    ui->setupUi(this);
}

DelDlg::~DelDlg()
{
    delete ui;
}

void DelDlg::sltQuit()
{
	init();
	QDialog::reject();
}

void DelDlg::setTotal(int total, qint64 totalsize)
{
	ui->progressBar->setRange(0, total);
	totalcount = total;
	ui->lbCurCount->setText(QString("%1/%2").arg(0).arg(total));
}

void DelDlg::init()
{
    ui->lbCurItem->setText("");
	ui->lbCurCount->setText("");
	totalcount = 0;
}

void DelDlg::setCurPos(qint64 size, int finished)
{
	ui->lbCurCount->setText(QString("%1/%2").arg(finished).arg(totalcount));
	ui->progressBar->setValue(finished);
}

void DelDlg::setCurItem(QString source, QString target)
{
	ui->lbCurItem->setText(source);
}
