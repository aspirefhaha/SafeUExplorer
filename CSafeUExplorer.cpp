#include "CSafeUExplorer.h"
#include "ui_CSafeUExplorer.h"

CSafeUExplorer::CSafeUExplorer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CSafeUExplorer)
{
    ui->setupUi(this);

    ui->mainToolBar->addAction(ui->actionDesktop);
    ui->mainToolBar->addAction(ui->actionUpFolder);
    ui->mainToolBar->addAction(ui->actionRefresh);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionChgPassword);
    ui->mainToolBar->addAction(ui->actionFormatUDisk);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionQuit);
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->splitter->setStretchFactor(0,20);
    ui->splitter->setStretchFactor(1,40);
    ui->splitter->setStretchFactor(2,40);

    m_pLocalModel = new CLocalModel(this);
    ui->tvLocalDisks->setModel(m_pLocalModel);

    m_pSafeUModel = new CLocalModel(this);
    ui->tvSafeUDisk->setModel(m_pSafeUModel);
}

CSafeUExplorer::~CSafeUExplorer()
{
    delete ui;
}
