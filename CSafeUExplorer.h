#ifndef CSAFEUEXPLORER_H
#define CSAFEUEXPLORER_H

#include <QMainWindow>
#include <QActionGroup>
#include <QToolBar>
#include "CGlobalModel.h"

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
	void sltQuit(bool);

private:
    Ui::CSafeUExplorer *ui;

    //CLocalModel * m_pLocalModel;
    //CLocalModel * m_pSafeUModel;
    CGlobalModel * m_pGlobalModel;

    void refreshLocalFs(QString &dirpath);
	void refreshUDiskFs(QString &dirpath);
	void findLocalItem(QString &abspath);
};

#endif // CSAFEUEXPLORER_H
