#include "UI_Login_Dialog.h"
#include "ui_UI_Login_Dialog.h"
#include <QMessageBox>

UI_Login_Dialog::UI_Login_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UI_Login_Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("中电环宇WEB2.0");
    QRegExp ipRx("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-4]|[01]?\\d\\d?)");
    QRegExpValidator *pIpValidator = new QRegExpValidator(ipRx,this);
    ui->lineEdit->setValidator(pIpValidator);//m_leIP = new QLineEdit(this);
    m_bLoginStatus = false;
    m_pManager = new QNetworkAccessManager(this);
    m_pManager->setCookieJar(&network_cookie_);
    connect(m_pManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
    qDebug()<<"是否支持SSL："<<QSslSocket::supportsSsl();
}

UI_Login_Dialog::~UI_Login_Dialog()
{
    delete ui;
}

bool UI_Login_Dialog::GetLoginStatus()
{
    return m_bLoginStatus;
}

QByteArray UI_Login_Dialog::GetLoginCookie()
{
    return m_baCookie;
}

QStringList UI_Login_Dialog::GetTreeItemDllName()
{
    QStringList strDllList;
#if defined(TIAOSHI)
    strDllList  <<"CGI_SCADA_DLL_DeviceMessage_d"<<"设备信息"
                <<"CGI_SCADA_DLL_ChannelMessage_d"<<"通道信息"
                <<"CGI_SCADA_DLL_HistoryMessage_d"<<"历史数据"
                <<"CGI_SCADA_DLL_ControlMessage_d"<<"控制逻辑"
                <<"CGI_SCADA_DLL_AlarmMessage_d"<<"警报服务"
                <<"CGI_SCADA_DLL_WebUIMessage_d"<<"Web UI"
                <<"CGI_SCADA_DLL_AppUIMessage_d"<<"App UI"
                <<"CGI_SCADA_DLL_UserManageMessage_d"<<"用户管理"
                <<"CGI_SCADA_DLL_AboutMessage_d"<<"关于";
#else
    strDllList  <<"CGI_SCADA_DLL_DeviceMessage"<<"设备信息"
                <<"CGI_SCADA_DLL_ChannelMessage"<<"通道信息"
                <<"CGI_SCADA_DLL_HistoryMessage"<<"历史数据"
                <<"CGI_SCADA_DLL_ControlMessage"<<"控制逻辑"
                <<"CGI_SCADA_DLL_AlarmMessage"<<"警报服务"
                <<"CGI_SCADA_DLL_WebUIMessage"<<"Web UI"
                <<"CGI_SCADA_DLL_AppUIMessage"<<"App UI"
                <<"CGI_SCADA_DLL_UserManageMessage"<<"用户管理"
                <<"CGI_SCADA_DLL_AboutMessage"<<"关于";
#endif
    return strDllList;
}

void UI_Login_Dialog::replyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        qDebug()<<__func__<<__LINE__<<bytes;
        QString string = QString::fromUtf8(bytes);
//        ui->textBrowser->setText(string);
    }
    else
    {
        qDebug()<<"handle errors here";
//        ui->textBrowser->append("DEBUG Message:");
//        ui->textBrowser->append("handle errors here");
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
//        ui->textBrowser->append(QString("found error ....code: %1 %2\n").arg(statusCodeV.toInt()).arg((int)reply->error()));
        qDebug(qPrintable(reply->errorString()));
//        ui->textBrowser->append(reply->errorString());
    }
    reply->deleteLater();

//    qDebug()<<__func__<<__LINE__<<"_______________start";
    QList<QNetworkCookie> cookies = network_cookie_.GetCookies();
    qDebug()<<__func__<<__LINE__<<"cookie个数"<<cookies.count();
    if (cookies.isEmpty())
    {
        QMessageBox::warning(this, tr("登录失败"),
                                   tr("登录管理机失败，\n"
                                      "请检查IP地址、用户名和密码是否正确。"),
                                   QMessageBox::Ok ,
                                   QMessageBox::Ok);
    }else
    {
        for (int i = 0; i < cookies.count(); ++i)
        {
            m_bLoginStatus = true;
            this->hide();
            qDebug()<<__func__<<__LINE__<<"cookie值"<<cookies.at(i).value();
            m_baCookie = cookies.at(i).value();
            CGI_SCADA_define::s_baCookie = m_baCookie;
            CGI_SCADA_define::s_strIPAddress = ui->lineEdit->text();
        }
    }
//    qDebug()<<__func__<<__LINE__<<"_______________end";
}

/*!
 \brief 登录按钮

 \fn UI_Login_Dialog::on_pushButton_clicked
*/
void UI_Login_Dialog::on_pushButton_clicked()
{
//    QString strUrl = QString("http://%1/cgi-bin/login.cgi?web=operating_status").arg(ui->lineEdit->text());/// web 2.0 cgi方式
    QString strUrl = QString("http://%1:8000/bin/login.wk?web=operating_status").arg(ui->lineEdit->text());/// web 2.0 cgi方式
//    QString strUrl = QString("http://%1/bin/login?web=login").arg(ui->lineEdit->text());/// web 3.0
//    QString strUrl = QString("http://%1/bin/op_qt").arg(ui->lineEdit->text());/// web 3.0
    QUrl url(strUrl);
    QNetworkRequest  networkRequest(url);
    QString strPost = QString("name=%1&password=%2&remember=false")
            .arg(ui->lineEdit_2->text())
            .arg(QString(QCryptographicHash::hash(ui->lineEdit_3->text().toLatin1(), QCryptographicHash::Md5).toHex()));
    qDebug()<<__func__<<__LINE__<<strUrl<<strPost.toLatin1();
    QByteArray baPost = strPost.toLatin1();
    m_pReply = m_pManager->post(networkRequest,baPost);
}
