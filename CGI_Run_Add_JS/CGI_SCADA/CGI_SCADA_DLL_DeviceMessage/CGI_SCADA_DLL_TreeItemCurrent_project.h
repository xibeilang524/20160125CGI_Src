#ifndef CGI_SCADA_DLL_TREEITEMCURRENT_PROJECT_H
#define CGI_SCADA_DLL_TREEITEMCURRENT_PROJECT_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "UI_Current_project.h"
#include <QWebView>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include "CGI_SCADA_DLL_DeviceMessageDefine.h"
#include "../CGI_SCADA_include/CGI_SCADA_define.h"

class CGI_SCADA_DLL_TreeItemCurrent_project : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    CGI_SCADA_DLL_TreeItemCurrent_project(QTreeWidgetItem *parentTreeItem_,const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_TreeItemCurrent_project();
    QWidget *GetWidget(QTreeWidgetItem *);

signals:

public slots:
private:
//    UI_Current_project *m_pWidget;

    QWebView *m_pMainView;
    QNetworkCookieJar *m_pCookieJar;
    bool m_bIP_CookieIsInit;

};

#endif // CGI_SCADA_DLL_TREEITEMCURRENT_PROJECT_H
