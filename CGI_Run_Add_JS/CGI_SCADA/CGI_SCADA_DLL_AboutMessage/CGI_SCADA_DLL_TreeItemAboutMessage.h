#ifndef CGI_SCADA_DLL_TREEITEMABOUTMESSAGE_H
#define CGI_SCADA_DLL_TREEITEMABOUTMESSAGE_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "UI_AboutMessage.h"
#include <QWebView>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include "../CGI_SCADA_include/CGI_SCADA_define.h"

/// web 2.0
//#define URL_TreeItemType_About_M "http://%1/cgi-bin/web.cgi?web=About"

/// web 3.0
#define URL_TreeItemType_About_M "http://%1:8000/bin/Qt.wk?web=About"

class CGI_SCADA_DLL_TreeItemAboutMessage : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    explicit CGI_SCADA_DLL_TreeItemAboutMessage(const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_TreeItemAboutMessage();
    QWidget *GetWidget(QTreeWidgetItem *);
signals:

public slots:
private:
//    UI_AboutMessage *m_pWidget;

    QWebView *m_pMainView;
    QNetworkCookieJar *m_pCookieJar;
    bool m_bIP_CookieIsInit;
};

#endif // CGI_SCADA_DLL_TREEITEMABOUTMESSAGE_H
