#ifndef CGI_SCADA_DLL_DEVICEMESSAGEITEM_H
#define CGI_SCADA_DLL_DEVICEMESSAGEITEM_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "CGI_SCADA_DLL_TreeItemOperating_status.h"
#include "CGI_SCADA_DLL_TreeItemHardware_model.h"
#include "CGI_SCADA_DLL_TreeItemSoftware_version.h"
#include "CGI_SCADA_DLL_TreeItemCurrent_users.h"
#include "CGI_SCADA_DLL_TreeItemEquipment_time.h"
#include "CGI_SCADA_DLL_TreeItemCurrent_project.h"
#include "CGI_SCADA_DLL_TreeItemProperty_information.h"

#include "CGI_SCADA_DLL_TreeItemChildItem.h"

class CGI_SCADA_DLL_DeviceMessageItem : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    explicit CGI_SCADA_DLL_DeviceMessageItem(const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_DeviceMessageItem();
    int GetItemNumber();
    QWidget *GetWidget(QTreeWidgetItem *pItem_);
signals:

public slots:
private:
    QLabel *m_pLabel;
//    CGI_SCADA_DLL_TreeItemOperating_status      *m_pOperating_status;
//    CGI_SCADA_DLL_TreeItemHardware_model        *m_pHardware_model;
//    CGI_SCADA_DLL_TreeItemSoftware_version      *m_pSoftware_version;
//    CGI_SCADA_DLL_TreeItemCurrent_users         *m_pCurrent_users;
//    CGI_SCADA_DLL_TreeItemEquipment_time        *m_pEquipment_time;
//    CGI_SCADA_DLL_TreeItemCurrent_project       *m_pCurrent_project;
//    CGI_SCADA_DLL_TreeItemProperty_information  *m_pProperty_information;

    CGI_SCADA_DLL_TreeItemChildItem *m_pOperating_status;
    CGI_SCADA_DLL_TreeItemChildItem *m_pHardware_model;
    CGI_SCADA_DLL_TreeItemChildItem *m_pSoftware_version;
    CGI_SCADA_DLL_TreeItemChildItem *m_pCurrent_users;
    CGI_SCADA_DLL_TreeItemChildItem *m_pEquipment_time;
    CGI_SCADA_DLL_TreeItemChildItem *m_pCurrent_project;
    CGI_SCADA_DLL_TreeItemChildItem *m_pProperty_information;
};

#endif // CGI_SCADA_DLL_DEVICEMESSAGEITEM_H
