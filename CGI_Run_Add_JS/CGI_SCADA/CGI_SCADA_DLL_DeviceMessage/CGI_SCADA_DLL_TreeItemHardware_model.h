#ifndef CGI_SCADA_DLL_TREEITEMHARDWARE_MODEL_H
#define CGI_SCADA_DLL_TREEITEMHARDWARE_MODEL_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "UI_Hardware_model.h"
#include <QWebView>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include "CGI_SCADA_DLL_DeviceMessageDefine.h"
#include "../CGI_SCADA_include/CGI_SCADA_define.h"

class CGI_SCADA_DLL_TreeItemHardware_model : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    CGI_SCADA_DLL_TreeItemHardware_model(QTreeWidgetItem *parentTreeItem_,const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_TreeItemHardware_model();
    QWidget *GetWidget(QTreeWidgetItem *);

signals:

public slots:
private:
//    UI_Hardware_model *m_pWidget;
    QWebView *m_pMainView;
    QNetworkCookieJar *m_pCookieJar;
    bool m_bIP_CookieIsInit;
};

#endif // CGI_SCADA_DLL_TREEITEMHARDWARE_MODEL_H
