#ifndef CGI_SCADA_DLL_TREEITEMUSERMANAGERMESSAGE_H
#define CGI_SCADA_DLL_TREEITEMUSERMANAGERMESSAGE_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "CGI_SCADA_DLL_UserManageTableWidget.h"

#include <QWebView>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include "../CGI_SCADA_include/CGI_SCADA_define.h"

/// web2.0
//#define URL_TreeItemType_UserMaM "http://%1/cgi-bin/web.cgi?web=Users"

/// web 3.0
#define URL_TreeItemType_UserMaM "http://%1:8000/bin/Qt.wk?web=Users"

class CGI_SCADA_DLL_TreeItemUserManagerMessage : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    explicit CGI_SCADA_DLL_TreeItemUserManagerMessage(const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_TreeItemUserManagerMessage();
    QWidget *GetWidget(QTreeWidgetItem *);
signals:

public slots:
private:
//    CGI_SCADA_DLL_UserManageTableWidget *m_pTableWidget;
    QWebView *m_pMainView;
    QNetworkCookieJar *m_pCookieJar;
    bool m_bIP_CookieIsInit;

};

#endif // CGI_SCADA_DLL_TREEITEMUSERMANAGERMESSAGE_H
