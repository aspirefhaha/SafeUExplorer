#ifndef CSAFEUEXPLORER_H
#define CSAFEUEXPLORER_H

#include <QMainWindow>
#include <QActionGroup>
#include <QToolBar>
#include "CLocalModel.h"
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

private:
    Ui::CSafeUExplorer *ui;

    CLocalModel * m_pLocalModel;
    CLocalModel * m_pSafeUModel;
    CGlobalModel * m_pGlobalModel;
};

#endif // CSAFEUEXPLORER_H
