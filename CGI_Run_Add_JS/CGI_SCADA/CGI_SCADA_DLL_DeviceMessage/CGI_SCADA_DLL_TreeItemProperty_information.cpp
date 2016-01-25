#include "CGI_SCADA_DLL_TreeItemProperty_information.h"

CGI_SCADA_DLL_TreeItemProperty_information::CGI_SCADA_DLL_TreeItemProperty_information(QTreeWidgetItem *parentTreeItem_, const QStringList &strings, int type, QObject *parent):
    CGI_SCADA_DLL_TreeItemBase(parentTreeItem_,strings,type,parent)
{
    m_pWidget = NULL;
}

CGI_SCADA_DLL_TreeItemProperty_information::~CGI_SCADA_DLL_TreeItemProperty_information()
{
    if (m_pWidget != NULL)
    {
        m_pWidget->deleteLater();
        m_pWidget = NULL;
    }
}

QWidget *CGI_SCADA_DLL_TreeItemProperty_information::GetWidget(QTreeWidgetItem *)
{
    if (m_pWidget == NULL)
    {
        m_pWidget = new UI_Property_information(NULL);
    }
    return m_pWidget;
}
