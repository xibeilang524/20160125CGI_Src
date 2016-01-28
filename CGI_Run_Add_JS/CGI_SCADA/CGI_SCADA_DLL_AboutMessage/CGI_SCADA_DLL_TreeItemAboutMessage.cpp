#include "CGI_SCADA_DLL_TreeItemAboutMessage.h"

/*!
 \brief 库文件导出接口函数

 \fn CreateDriver
 \param parent QObject对象指针
 \return CGI_SCADA_DLL_I 返回创建的驱动指针
*/
CGI_SCADA_DLL_I * CreateDriver(QObject *parent)
{
    QStringList strings;
    strings << "关于";
    qDebug()<<__func__<<strings;
    CGI_SCADA_DLL_TreeItemAboutMessage *testProtocol = new CGI_SCADA_DLL_TreeItemAboutMessage(strings,CGI_SCADA_DLL_I::TreeItemType_About_M,parent);
    qDebug()<<testProtocol->text(0);
    return  testProtocol;
}

CGI_SCADA_DLL_TreeItemAboutMessage::CGI_SCADA_DLL_TreeItemAboutMessage(const QStringList &strings, int type, QObject *parent):
    CGI_SCADA_DLL_TreeItemBase(strings,type,parent)
{
//    m_pWidget = NULL;
    m_pMainView = new QWebView;
    m_pCookieJar = new QNetworkCookieJar();
    m_pMainView->page()->networkAccessManager()->setCookieJar(m_pCookieJar);
    m_bIP_CookieIsInit = false;
}

CGI_SCADA_DLL_TreeItemAboutMessage::~CGI_SCADA_DLL_TreeItemAboutMessage()
{
//    if (m_pWidget != NULL)
//    {
//        m_pWidget->deleteLater();
//        m_pWidget = NULL;
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

QWidget *CGI_SCADA_DLL_TreeItemAboutMessage::GetWidget(QTreeWidgetItem *)
{
//    if (m_pWidget == NULL)
//    {
//        m_pWidget = new UI_AboutMessage(NULL);
//    }
//    return m_pWidget;
    if (!m_bIP_CookieIsInit)
    {
        QList<QNetworkCookie> cookies;
        cookies.append(QNetworkCookie("CMMSUSS", CGI_SCADA_define::s_baCookie));
        qDebug()<<__func__<<__LINE__<<CGI_SCADA_define::s_strIPAddress<<CGI_SCADA_define::s_baCookie;
        m_pCookieJar->setCookiesFromUrl(cookies, QString(URL_TreeItemType_About_M).arg(CGI_SCADA_define::s_strIPAddress));
        m_bIP_CookieIsInit = true;
    }
    m_pMainView->load(QUrl(QString(URL_TreeItemType_About_M).arg(CGI_SCADA_define::s_strIPAddress)));
    return m_pMainView;
}
