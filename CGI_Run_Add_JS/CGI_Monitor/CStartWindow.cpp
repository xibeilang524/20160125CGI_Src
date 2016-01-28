#include "CStartWindow.h"
#include "ui_CStartWindow.h"
#include "CDeviceModelMessage.h"
#include "CProtocolFileMessage.h"

CStartWindow::CStartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CStartWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("CGI 监视");
    QString strPath = QDir::currentPath().remove("cgi-bin")+"xml";
    qDebug()<<"配置文件路径"<<strPath;
    CDeviceModelMessage::InitDeviceModelMessage(strPath);
    CProtocolFileMessage::InitProtocolFileMessage(strPath);

    m_pMainWindow = new MainWindow(this);
    on_action_triggered();
}

CStartWindow::~CStartWindow()
{
    delete ui;
}

void CStartWindow::on_action_triggered()
{
    qDebug()<<"场站管理Action";
    if (m_pMainWindow == NULL)
        return;
    m_pMainWindow->show();
    m_pMainWindow->raise();
}
