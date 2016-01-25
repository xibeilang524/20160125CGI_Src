#ifndef CGI_SCADA_DLL_TREEITEMEQUIPMENT_TIME_H
#define CGI_SCADA_DLL_TREEITEMEQUIPMENT_TIME_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "UI_Equipment_time.h"

class CGI_SCADA_DLL_TreeItemEquipment_time : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    CGI_SCADA_DLL_TreeItemEquipment_time(QTreeWidgetItem *parentTreeItem_,const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_TreeItemEquipment_time();
    QWidget *GetWidget(QTreeWidgetItem *);

signals:

public slots:
private:
    UI_Equipment_time *m_pWidget;

};

#endif // CGI_SCADA_DLL_TREEITEMEQUIPMENT_TIME_H
