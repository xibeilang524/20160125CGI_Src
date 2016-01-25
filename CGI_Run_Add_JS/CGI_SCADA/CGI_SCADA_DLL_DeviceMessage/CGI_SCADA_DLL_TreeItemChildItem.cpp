#include "CGI_SCADA_DLL_TreeItemChildItem.h"


CGI_SCADA_DLL_TreeItemChildItem::CGI_SCADA_DLL_TreeItemChildItem(QTreeWidgetItem *parentTreeItem_, const QStringList &strings, int type, QObject *parent):
    CGI_SCADA_DLL_TreeItemBase(parentTreeItem_,strings,type,parent)
{
    m_pMainView = new QWebView;
    m_pCookieJar = new QNetworkCookieJar();
    m_pMainView->page()->networkAccessManager()->setCookieJar(m_pCookieJar);
    m_bIP_CookieIsInit = false;

}

CGI_SCADA_DLL_TreeItemChildItem::~CGI_SCADA_DLL_TreeItemChildItem()
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

QWidget *CGI_SCADA_DLL_TreeItemChildItem::GetWidget(QTreeWidgetItem *)
{
    QString url;
    switch (this->type()) {
    case TreeItemType_DeviceM_operating_status:
        url = QString(URL_TreeItemType_DeviceM_operating_status).arg(CGI_SCADA_define::s_strIPAddress);
        break;
    case TreeItemType_DeviceM_hardware_model:
        url = QString(URL_TreeItemType_DeviceM_hardware_model).arg(CGI_SCADA_define::s_strIPAddress);
        break;
    case TreeItemType_DeviceM_software_version:
        url = QString(URL_TreeItemType_DeviceM_software_version).arg(CGI_SCADA_define::s_strIPAddress);
        break;
    case TreeItemType_DeviceM_current_users:
        url = QString(URL_TreeItemType_DeviceM_current_users).arg(CGI_SCADA_define::s_strIPAddress);
        break;
    case TreeItemType_DeviceM_equipment_time:
        url = QString(URL_TreeItemType_DeviceM_equipment_time).arg(CGI_SCADA_define::s_strIPAddress);
        break;
    case TreeItemType_DeviceM_current_project:
        url = QString(URL_TreeItemType_DeviceM_current_project).arg(CGI_SCADA_define::s_strIPAddress);
        break;
    case TreeItemType_DeviceM_property_information:
        url = QString(URL_TreeItemType_DeviceM_property_information).arg(CGI_SCADA_define::s_strIPAddress);
        break;
    default:
        return NULL;
        break;
    }
    if (!m_bIP_CookieIsInit)
    {
        QList<QNetworkCookie> cookies;
        /// 设置cookie的名字和值
        cookies.append(QNetworkCookie("CMMSUSS", CGI_SCADA_define::s_baCookie));
//        qDebug()<<__func__<<__LINE__<<CGI_SCADA_define::s_strIPAddress<<CGI_SCADA_define::s_baCookie;
        m_pCookieJar->setCookiesFromUrl(cookies, url);
        m_bIP_CookieIsInit = true;
    }
    m_pMainView->load(QUrl(url));
    return m_pMainView;
}
