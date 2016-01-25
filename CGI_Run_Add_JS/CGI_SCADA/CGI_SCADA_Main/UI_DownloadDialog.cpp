#include "UI_DownloadDialog.h"
#include "ui_UI_DownloadDialog.h"
#include <QtNetwork>

UI_DownloadDialog::UI_DownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UI_DownloadDialog)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    ui->progressBar->setValue(0);
    ui->label->hide();
    ui->lineEdit->hide();

//    ui->progressBar->hide();
}

UI_DownloadDialog::~UI_DownloadDialog()
{
    delete ui;
}
void UI_DownloadDialog::replyFinished(QNetworkReply *reply)
{
    QString all = reply->readAll();
    ui->textBrowser->setText(all);
    reply->deleteLater();
}

void UI_DownloadDialog::startRequest(QUrl url)
{
    reply = manager->get(QNetworkRequest(url));
    connect(reply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(updateDataReadProgress(qint64,qint64)));
    connect(reply,SIGNAL(finished()),this,SLOT(httpFinished()));
}

void UI_DownloadDialog::httpReadyRead()
{
    qDebug()<<__func__<<__LINE__;
    if(file)file->write(reply->readAll());
}

void UI_DownloadDialog::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    qDebug()<<__func__<<__LINE__<<bytesRead<<totalBytes;
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesRead);
}

void UI_DownloadDialog::httpFinished()
{
//    ui->progressBar->hide();
    qDebug()<<__func__<<__LINE__;
//    ui->progressBar->setValue(100);
    file->flush();
    file->close();
    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;
}


void UI_DownloadDialog::on_pushButton_clicked()
{
    qDebug()<<__func__<<__LINE__;
//    url = ui->lineEdit->text();
    url = QString("http://%1:%2/%3").arg(ui->lineEdit_2->text()).arg(ui->lineEdit_3->text()).arg(ui->lineEdit_4->text());
    QFileInfo info(url.path());
    QString fileName(info.fileName());
    file = new QFile(fileName);
    if(!file->open(QIODevice::WriteOnly))
    {
        qDebug()<<"file open error";
        delete file;
        file = 0;
        return;
    }
    startRequest(url);
    ui->progressBar->setValue(0);
//    ui->progressBar->show();
}
