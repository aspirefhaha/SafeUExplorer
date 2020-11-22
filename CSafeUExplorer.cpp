#include "CSafeUExplorer.h"
#include "ui_CSafeUExplorer.h"
#include <QStringList>
#include <QFileInfo>
#include <QDir>

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

    m_pGlobalModel = new CGlobalModel(this);
    ui->tvGlobal->setHeaderHidden(true);
    //ui->tvGlobal->verticalHeader()->setVisible(false);
    ui->tvGlobal->header()->setStretchLastSection(true);
    ui->tvGlobal->setModel(m_pGlobalModel);

    m_pLocalModel = new CLocalModel(this);
    ui->tvLocalDisks->setModel(m_pLocalModel);
    ui->tvLocalDisks->header()->setStretchLastSection(true);

    m_pSafeUModel = new CLocalModel(this);
    ui->tvSafeUDisk->setModel(m_pSafeUModel);
    ui->tvSafeUDisk->header()->setStretchLastSection(true);


}

CSafeUExplorer::~CSafeUExplorer()
{
    delete ui;
}
