#include "CGI_SCADA_DLL_TreeItemHistorySelect.h"



CGI_SCADA_DLL_TreeItemHistorySelect::CGI_SCADA_DLL_TreeItemHistorySelect(QTreeWidgetItem *parentTreeItem_, const QStringList &strings, int type, QObject *parent):
    CGI_SCADA_DLL_TreeItemBase(parentTreeItem_,strings,type,parent)
{
    m_pMainView = new QWebView;
    m_pCookieJar = new QNetworkCookieJar();
    m_pMainView->page()->networkAccessManager()->setCookieJar(m_pCookieJar);
    m_bIP_CookieIsInit = false;
}

CGI_SCADA_DLL_TreeItemHistorySelect::~CGI_SCADA_DLL_TreeItemHistorySelect()
{
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

QWidget *CGI_SCADA_DLL_TreeItemHistorySelect::GetWidget(QTreeWidgetItem *)
{
    if (!m_bIP_CookieIsInit)
    {
        QList<QNetworkCookie> cookies;
        cookies.append(QNetworkCookie("CMMSUSS", CGI_SCADA_define::s_baCookie));
        qDebug()<<__func__<<__LINE__<<CGI_SCADA_define::s_strIPAddress<<CGI_SCADA_define::s_baCookie;
        m_pCookieJar->setCookiesFromUrl(cookies, QString(URL_TreeItemType_HistorySelect).arg(CGI_SCADA_define::s_strIPAddress));
        m_bIP_CookieIsInit = true;
    }
    m_pMainView->load(QUrl(QString(URL_TreeItemType_HistorySelect).arg(CGI_SCADA_define::s_strIPAddress)));
    return m_pMainView;
}
