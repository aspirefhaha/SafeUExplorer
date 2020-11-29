#include "CopyDlg.h"
#include "ui_CopyDlg.h"

CopyDlg::CopyDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyDlg),
	m_bCouldQuit(false)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
	m_TotalSize = 1;
	m_TotalCount = 0;
}

CopyDlg::~CopyDlg()
{
    delete ui;
}

void CopyDlg::init()
{
	m_TotalSize = 1;
	m_TotalCount = 0;
	ui->pbTotal->setValue(0);
	ui->pbCurItem->setValue(0);
	ui->lbSource->setText(tr("Source"));
	ui->lbTarget->setText(tr("Target"));

}

void CopyDlg::reject()
{
	if (m_bCouldQuit)
		QDialog::reject();
	else
		emit wantQuit();
}

void CopyDlg::sltCancel()
{
	m_bCouldQuit = true;
	emit wantQuit();
}
void CopyDlg::sltQuit()
{
	ui->lbTotal->setText(tr("Calculating..."));
	QDialog::reject();
}
void CopyDlg::setTotal(int total, qint64 totalsize)
{
	m_TotalCount = total;
	ui->lbTotal->setText(tr("Total %1 Current %2").arg(total).arg(0));
	ui->pbTotal->setRange(0, total);
	ui->pbTotal->setValue(0);
	ui->pbCurItem->setRange(0, 100);
	ui->pbCurItem->setValue(0);
	m_bCouldQuit = false;
	m_TotalSize = totalsize;
}
void CopyDlg::setCurPos(qint64 size, int finished)
{
	ui->pbTotal->setValue(finished);
	ui->lbTotal->setText(tr("Total %1 Current %2").arg(m_TotalCount).arg(finished));
	ui->pbCurItem->setValue((int)(size* 100LL / m_TotalSize));
}
void CopyDlg::setCurItem(QString source, QString target)
{
	ui->lbSource->setText(source);
	ui->lbTarget->setText(target);
}