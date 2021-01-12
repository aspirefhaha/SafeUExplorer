#ifndef DELDLG_H
#define DELDLG_H

#include <QDialog>

namespace Ui {
class DelDlg;
}

class DelDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DelDlg(QWidget *parent = nullptr);
    ~DelDlg();
    void init();
public slots:
	void sltQuit();
	void sltCancel() {
		emit wantQuit();
	}
	void setTotal(int total, qint64 totalsize);
	void setCurItem(QString source, QString target);
	void setCurPos(qint64 size, int finished);

Q_SIGNALS:
	void wantQuit();

private:
    Ui::DelDlg *ui;

	int totalcount;
};

#endif // DELDLG_H
