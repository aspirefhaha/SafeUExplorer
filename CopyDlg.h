#ifndef COPYDLG_H
#define COPYDLG_H

#include <QDialog>

namespace Ui {
class CopyDlg;
}

class CopyDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CopyDlg(QWidget *parent = nullptr);
    ~CopyDlg();

	bool m_bCouldQuit;
	void init();
public slots:
	void sltQuit();

Q_SIGNALS:
	void wantQuit();

protected slots:
	void sltCancel(); 
	void setTotal(int total, qint64 totalsize);
	void setCurPos(qint64 size, int finished);
	void setCurFileProg(qint64 filesize, qint64 finished);
	void setCurItem(QString source, QString target);
	void sltSpeed(qreal  speed);
	virtual void reject();

private:
    Ui::CopyDlg *ui;
	qint64 m_TotalSize;
	int m_TotalCount;
};

#endif // COPYDLG_H
