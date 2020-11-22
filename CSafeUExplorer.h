#ifndef CSAFEUEXPLORER_H
#define CSAFEUEXPLORER_H

#include <QMainWindow>
#include <QActionGroup>
#include <QToolBar>
#include "CLocalModel.h"

namespace Ui {
class CSafeUExplorer;
}

class CSafeUExplorer : public QMainWindow
{
    Q_OBJECT

public:
    explicit CSafeUExplorer(QWidget *parent = 0);
    ~CSafeUExplorer();

private:
    Ui::CSafeUExplorer *ui;

    CLocalModel * m_pLocalModel;
    CLocalModel * m_pSafeUModel;
};

#endif // CSAFEUEXPLORER_H
