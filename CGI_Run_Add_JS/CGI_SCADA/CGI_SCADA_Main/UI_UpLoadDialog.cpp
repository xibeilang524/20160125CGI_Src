#include "UI_UpLoadDialog.h"
#include "ui_UI_UpLoadDialog.h"

UI_UpLoadDialog::UI_UpLoadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UI_UpLoadDialog)
{
    ui->setupUi(this);
    m_pWebView = new QWebView(this);
    connect(m_pWebView,SIGNAL(urlChanged(QUrl)),this,SLOT(slot_urlChanged(QUrl)));
//    m_pWebView->setStyleSheet("background-color: white;");
    ui->verticalLayout->addWidget(m_pWebView);
}

UI_UpLoadDialog::~UI_UpLoadDialog()
{
    delete ui;
}

void UI_UpLoadDialog::on_pushButton_clicked()
{
    qDebug()<<__func__<<ui->lineEdit->text();
    QString strUrl = QString("http://%1:%2/upload").arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text());

    m_pWebView->setUrl(strUrl);
}
