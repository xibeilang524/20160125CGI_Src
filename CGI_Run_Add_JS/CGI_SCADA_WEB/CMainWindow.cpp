#include "CMainWindow.h"
#include "ui_CMainWindow.h"
#include <QDebug>
#include <QNetworkCookieJar>
#include <QList>
#include <QNetworkCookie>

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    m_IsBackStatus = false;
    m_IsBackStatus_2 = false;
    m_nBackspaceTimes = 0;
//    m_SrcUrl = QUrl();
    m_pWebView = new QWebView(this);
    connect(m_pWebView,SIGNAL(urlChanged(QUrl)),this,SLOT(slot_urlChanged(QUrl)));
    ui->gridLayout->addWidget(m_pWebView);
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::slot_urlChanged(QUrl url)
{
    qDebug()<<__func__<<url;
    m_nCurrentUrl = m_UrlList.count() - 1;
    ui->lineEdit->setText(url.toString());

    if (m_IsBackStatus)
    {
        m_IsBackStatus_2 = true;
    }else
    {
        m_UrlList.append(url);
        m_nBackspaceTimes = 0;
        m_IsBackStatus_2 = false;
    }
    m_IsBackStatus = false;
}

void CMainWindow::on_pushButton_clicked()
{
//    m_PreviousUrl = m_CurrentUrl;
    m_IsBackStatus = false;
    qDebug()<<__func__<<ui->lineEdit->text();
    m_pWebView->setUrl(ui->lineEdit->text());
//    m_CurrentUrl = QUrl(ui->lineEdit->text());
    m_UrlList.append(ui->lineEdit->text());
    m_nCurrentUrl = m_UrlList.count() - 1;
}

void CMainWindow::on_pushButton_2_clicked()
{
    if (m_UrlList.count() <= 0)
    {
        return ;
    }
    m_IsBackStatus = true;
    if (m_IsBackStatus_2)
    {
        m_nBackspaceTimes++;
        int nBackCount = m_UrlList.count() - m_nBackspaceTimes-1;
        if (nBackCount >= m_UrlList.count())
            return;
        qDebug()<<__func__<<ui->lineEdit->text()<<m_nBackspaceTimes;
        m_pWebView->setUrl(m_UrlList.at(nBackCount));

    }else
    {
        m_nBackspaceTimes++;
        int nBackCount = m_UrlList.count() - m_nBackspaceTimes-1;
        if (nBackCount >= m_UrlList.count())
            return;
        qDebug()<<__func__<<ui->lineEdit->text()<<m_nBackspaceTimes;
        m_pWebView->setUrl(m_UrlList.at(nBackCount));
    }
}

void CMainWindow::on_pushButton_3_clicked()
{

    QNetworkCookieJar *m_pCookieJar = new QNetworkCookieJar();
    m_pWebView->page()->networkAccessManager()->setCookieJar(m_pCookieJar);
    QList<QNetworkCookie> cookies;
    cookies.append(QNetworkCookie("key1", ui->lineEdit_2->text().toLocal8Bit()));
    //更多cookie按照上面的方法依次添加...
    m_pCookieJar->setCookiesFromUrl(cookies, ui->lineEdit->text());
//    m_pCookieJar->setCookiesFromUrl(cookies, strUrl);
    m_pWebView->load(ui->lineEdit->text());
}
