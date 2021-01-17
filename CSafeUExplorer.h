#ifndef CSAFEUEXPLORER_H
#define CSAFEUEXPLORER_H

#include <QMainWindow>
#include <QActionGroup>
#include <QToolBar>
#include "BGCopyThread.h"
#include "CGlobalModel.h"
#include "CopyDlg.h"
#include "DelDlg.h"

namespace Ui {
class CSafeUExplorer;
}

class CSafeUExplorer : public QMainWindow
{
    Q_OBJECT

public:
    explicit CSafeUExplorer(QWidget *parent = 0);
    ~CSafeUExplorer();


protected Q_SLOTS:
    void sltItemClicked(QModelIndex);
    void sltLocalItemClicked(QModelIndex);
	void sltUDiskItemClicked(QModelIndex);
    void sltRefresh(bool);
	void sltDesktop(bool);
	void sltUpfolder(bool);
	void sltLocalCellClick(int, int);
	void sltUDiskCellClick(int, int);
	void sltQuit(bool);
    void sltLocalStartDrag();
    void sltUDiskStartDrag();
	void sltAcceptLocalItemList(QList<int> list);
	void sltAcceptUDiskItemList(QList<int> list);
	void sltWantCancelCopy();
	void sltFormat(bool);
	void sltDelUDiskFile(QList<QModelIndex>);
protected:
	bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:
    Ui::CSafeUExplorer *ui;
	bool udiskFocused;

    CGlobalModel * m_pGlobalModel;

    void refreshLocalFs(QString &dirpath);
	void refreshUDiskFs(QString &dirpath);
	void findLocalItem(QString &abspath);
	BGCopyThread copyThread;
	CopyDlg copyDlg;
    DelDlg delDlg;
};

#endif // CSAFEUEXPLORER_H
