#ifndef CSAFEUEXPLORER_H
#define CSAFEUEXPLORER_H

#include <QMainWindow>
#include <QActionGroup>
#include <QToolBar>

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
    QActionGroup *m_MenuActionGroup;
    QActionGroup *m_ToolBarActionGroup;
    QAction *m_ActionMenu1;
    QAction *m_ActionMenu2;
    QAction *m_ActionToolBar1;
    QAction *m_ActionToolBar2;
};

#endif // CSAFEUEXPLORER_H
