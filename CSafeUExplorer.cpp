#include "CSafeUExplorer.h"
#include "ui_CSafeUExplorer.h"

CSafeUExplorer::CSafeUExplorer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CSafeUExplorer)
{
    ui->setupUi(this);
//    m_ToolBarActionGroup = new QActionGroup(this);
//    QList<QAction*> m_ToolBarList;

//    m_ActionToolBar1 = new QAction(QStringLiteral("ToolBar1"));
//    m_ActionToolBar2 = new QAction(QStringLiteral("ToolBar2"));
//    m_ActionToolBar1->setCheckable(true);
//    m_ActionToolBar2->setCheckable(true);
//    m_ToolBarList.append(m_ActionToolBar1);
//    m_ToolBarList.append(m_ActionToolBar2);
//    m_ToolBarActionGroup->addAction(m_ActionToolBar1);
//    m_ToolBarActionGroup->addAction(m_ActionToolBar2);

    //ui->mainToolBar->addActions(m_ToolBarList);
    //ui->mainToolBar->setStyleSheet("background-color:rgb(200,40,43);color:rgb(204,204,204)");
    ui->mainToolBar->addAction(ui->actionDesktop);
    ui->mainToolBar->addAction(ui->actionUpFolder);
    ui->mainToolBar->addAction(ui->actionRefresh);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionChgPassword);
    ui->mainToolBar->addAction(ui->actionFormatUDisk);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionQuit);
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

CSafeUExplorer::~CSafeUExplorer()
{
    delete ui;
}
