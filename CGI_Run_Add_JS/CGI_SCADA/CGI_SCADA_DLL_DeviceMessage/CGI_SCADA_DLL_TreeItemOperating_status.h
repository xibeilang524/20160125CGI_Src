#ifndef CGI_SCADA_DLL_TREEITEMOPERATING_STATUS_H
#define CGI_SCADA_DLL_TREEITEMOPERATING_STATUS_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "UI_operating_status.h"
#include <QWebView>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include "CGI_SCADA_DLL_DeviceMessageDefine.h"

class CGI_SCADA_DLL_TreeItemOperating_status : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    CGI_SCADA_DLL_TreeItemOperating_status(QTreeWidgetItem *parentTreeItem_,const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_TreeItemOperating_status();
    QWidget *GetWidget(QTreeWidgetItem *);
signals:

public slots:
private:
//    UI_operating_status *m_pWidget;
    QWebView *m_pMainView;
    QNetworkCookieJar *m_pCookieJar;
    bool m_bIP_CookieIsInit;


};

#endif // CGI_SCADA_DLL_TREEITEMOPERATING_STATUS_H
