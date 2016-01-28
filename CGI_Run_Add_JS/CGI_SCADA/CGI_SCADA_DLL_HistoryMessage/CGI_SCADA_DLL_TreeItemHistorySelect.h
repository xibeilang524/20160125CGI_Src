#ifndef CGI_SCADA_DLL_TREEITEMHISTORYSELECT_H
#define CGI_SCADA_DLL_TREEITEMHISTORYSELECT_H

#include <QWebView>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "../CGI_SCADA_include/CGI_SCADA_define.h"

/// web 2.0
//#define URL_TreeItemType_HistorySelect   "http://%1/cgi-bin/web.cgi?web=HistorySelect"

/// web 3.0
#define URL_TreeItemType_HistorySelect   "http://%1:8000/bin/Qt.wk?web=HistorySelect"

class CGI_SCADA_DLL_TreeItemHistorySelect : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    CGI_SCADA_DLL_TreeItemHistorySelect(QTreeWidgetItem *parentTreeItem_,const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_TreeItemHistorySelect();
    QWidget *GetWidget(QTreeWidgetItem *);

signals:

public slots:
private:
    QWebView *m_pMainView;
    QNetworkCookieJar *m_pCookieJar;
    bool m_bIP_CookieIsInit;

};

#endif // CGI_SCADA_DLL_TREEITEMHISTORYSELECT_H
