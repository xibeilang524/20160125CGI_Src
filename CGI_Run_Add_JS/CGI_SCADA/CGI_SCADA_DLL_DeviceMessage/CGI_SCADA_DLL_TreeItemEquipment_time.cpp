#include "CGI_SCADA_DLL_TreeItemEquipment_time.h"

//CGI_SCADA_DLL_TreeItemEquipment_time::CGI_SCADA_DLL_TreeItemEquipment_time(QObject *parent) :
//    CGI_SCADA_DLL_TreeItemBase(parent)
//{
//}


CGI_SCADA_DLL_TreeItemEquipment_time::CGI_SCADA_DLL_TreeItemEquipment_time(QTreeWidgetItem *parentTreeItem_, const QStringList &strings, int type, QObject *parent):
    CGI_SCADA_DLL_TreeItemBase(parentTreeItem_,strings,type,parent)
{
    m_pWidget = NULL;
}

CGI_SCADA_DLL_TreeItemEquipment_time::~CGI_SCADA_DLL_TreeItemEquipment_time()
{
    if (m_pWidget != NULL)
    {
        m_pWidget->deleteLater();
        m_pWidget = NULL;
    }
}

QWidget *CGI_SCADA_DLL_TreeItemEquipment_time::GetWidget(QTreeWidgetItem *)
{
    if (m_pWidget == NULL)
    {
        m_pWidget = new UI_Equipment_time(NULL);
    }
    return m_pWidget;
}
