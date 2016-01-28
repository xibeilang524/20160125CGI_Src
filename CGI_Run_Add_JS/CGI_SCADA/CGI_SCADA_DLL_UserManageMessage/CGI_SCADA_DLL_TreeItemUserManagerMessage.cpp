#include "CGI_SCADA_DLL_TreeItemUserManagerMessage.h"

/*!
 \brief 库文件导出接口函数

 \fn CreateDriver
 \param parent QObject对象指针
 \return CGI_SCADA_DLL_I 返回创建的驱动指针
*/
CGI_SCADA_DLL_I * CreateDriver(QObject *parent)
{
    QStringList strings;
    strings << "用户管理";
    qDebug()<<__func__<<strings;
    CGI_SCADA_DLL_TreeItemUserManagerMessage *testProtocol = new CGI_SCADA_DLL_TreeItemUserManagerMessage(strings,CGI_SCADA_DLL_I::TreeItemType_UserMaM,parent);
    qDebug()<<testProtocol->text(0);
    return  testProtocol;
}

CGI_SCADA_DLL_TreeItemUserManagerMessage::CGI_SCADA_DLL_TreeItemUserManagerMessage(const QStringList &strings, int type, QObject *parent):
    CGI_SCADA_DLL_TreeItemBase(strings,type,parent)
{
//    m_pTableWidget = NULL;
//    m_pTableWidget = new CGI_SCADA_DLL_UserManageTableWidget(NULL);
    m_pMainView = new QWebView;
    m_pCookieJar = new QNetworkCookieJar();
    m_pMainView->page()->networkAccessManager()->setCookieJar(m_pCookieJar);
    m_bIP_CookieIsInit = false;
}

CGI_SCADA_DLL_TreeItemUserManagerMessage::~CGI_SCADA_DLL_TreeItemUserManagerMessage()
{
//    if (m_pTableWidget != NULL)
//    {
//        m_pTableWidget->deleteLater();
//        m_pTableWidget = NULL;
//    }
    if (m_pMainView != NULL)
    {
        m_pMainView->deleteLater();
        m_pMainView = NULL;
    }
    if (m_pCookieJar != NULL)
    {
        m_pCookieJar->deleteLater();
        m_pCookieJar = NULL;
    }
}

QWidget *CGI_SCADA_DLL_TreeItemUserManagerMessage::GetWidget(QTreeWidgetItem *)
{
//    return m_pTableWidget;
    if (!m_bIP_CookieIsInit)
    {
        QList<QNetworkCookie> cookies;
        cookies.append(QNetworkCookie("CMMSUSS", CGI_SCADA_define::s_baCookie));
        qDebug()<<__func__<<__LINE__<<CGI_SCADA_define::s_strIPAddress<<CGI_SCADA_define::s_baCookie;
        m_pCookieJar->setCookiesFromUrl(cookies, QString(URL_TreeItemType_UserMaM).arg(CGI_SCADA_define::s_strIPAddress));
        m_bIP_CookieIsInit = true;
    }
    m_pMainView->load(QUrl(QString(URL_TreeItemType_UserMaM).arg(CGI_SCADA_define::s_strIPAddress)));
    return m_pMainView;
}
