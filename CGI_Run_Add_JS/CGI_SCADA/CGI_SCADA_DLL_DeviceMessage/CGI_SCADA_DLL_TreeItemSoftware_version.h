#ifndef CGI_SCADA_DLL_TREEITEMSOFTWARE_VERSION_H
#define CGI_SCADA_DLL_TREEITEMSOFTWARE_VERSION_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "UI_Software_version.h"

class CGI_SCADA_DLL_TreeItemSoftware_version : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    CGI_SCADA_DLL_TreeItemSoftware_version(QTreeWidgetItem *parentTreeItem_,const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_TreeItemSoftware_version();
    QWidget *GetWidget(QTreeWidgetItem *);
signals:

public slots:
private:
    UI_Software_version *m_pWidget;
signals:

public slots:

};

#endif // CGI_SCADA_DLL_TREEITEMSOFTWARE_VERSION_H
