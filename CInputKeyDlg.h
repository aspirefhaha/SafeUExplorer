#ifndef CINPUTKEYDLG_H
#define CINPUTKEYDLG_H

#include <QDialog>

namespace Ui {
class CInputKeyDlg;
}

class CInputKeyDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CInputKeyDlg(QWidget *parent = nullptr);
    ~CInputKeyDlg();


protected slots:
    void sltConfirm();

private:
    Ui::CInputKeyDlg *ui;
};

#endif // CINPUTKEYDLG_H
