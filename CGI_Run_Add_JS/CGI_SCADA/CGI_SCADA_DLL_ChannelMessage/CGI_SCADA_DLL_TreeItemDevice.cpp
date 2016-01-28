#include "CGI_SCADA_DLL_TreeItemDevice.h"


CGI_SCADA_DLL_TreeItemDevice::CGI_SCADA_DLL_TreeItemDevice(ProtocolType nProtocolType_,Device device_,QTreeWidgetItem *parentItem, const QStringList &strings, int type, QObject *parent) :
    CGI_SCADA_DLL_TreeItemBase(parentItem,strings,type,parent),
    m_nProtocolType(nProtocolType_)
{
    qDebug()<<__func__<<__LINE__<<"nProtocolType"<<nProtocolType_<<parentItem->text(0);
    m_Device = device_;
    QString strDeviceName;
    if (!strings.isEmpty())
    {
        strDeviceName = strings.at(0);
    }

//    qDebug()<<__func__<<__LINE__<<"m_Device.m_Att_ParameterList.count()"<<m_Device.m_Att_ParameterList.count();
//    for (int i = 0; i < m_Device.m_Att_ParameterList.count(); ++i)
//    {
//        qDebug()<<__func__<<__LINE__<<m_Device.m_Att_ParameterList.at(i).m_strName;
//    }
    m_pDeviceAttribute = new UI_DeviceAttribute(m_Device.m_Att_ParameterList);

    m_pTabWidget = new QTabWidget;
    m_pTabWidget->addTab(m_pDeviceAttribute,"属性");

    qDebug()<<__func__<<__LINE__<<m_Device.m_strDeviceName<<m_Device.m_strDeviceDesc;
    QString strLinkName;
    switch (type) {
    case TreeItemType_ChanneM_COM_Device:
    case TreeItemType_ChanneM_CAN_Device:
        strLinkName  = parentItem->text(0)+"."+strDeviceName+"@";
        break;
    case TreeItemType_ChanneM_NET_Link_Device:
        strLinkName  = parentItem->parent()->text(0)+"."+parentItem->text(0)+"."+strDeviceName+"@";
        break;
    default:
        break;
    }
    for (int i = 0 ; i < m_Device.m_Typelist.count(); ++i)
    {
        qDebug()<<__func__<<__LINE__<<m_Device.m_Typelist.at(i).m_strValue<<parentItem->text(0);
//        m_TabWidget.addTab(new QTableView(), m_Device.m_Typelist.at(i).m_strTypeName);
        CGI_SCADA_DLL_DeviceTableView *pTableView = new CGI_SCADA_DLL_DeviceTableView(m_nProtocolType,this,strLinkName,m_Device.m_Typelist.at(i),m_pTabWidget);
        m_TableViewMap.insert(m_Device.m_Typelist.at(i).m_strValue,pTableView);
//        m_TableViewList.append(pTableView);
        connect(this,SIGNAL(signal_ProtocolTypeChange(int)),pTableView,SLOT(slot_ProtocolTypeChange(int)));
        connect(pTableView,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
        m_pTabWidget->addTab(pTableView, m_Device.m_Typelist.at(i).m_strTypeName);

    }
}

CGI_SCADA_DLL_TreeItemDevice::~CGI_SCADA_DLL_TreeItemDevice()
{
    qDebug()<<__func__<<__FILE__<<__LINE__<<"start"<<this->text(0);
    if (m_pTabWidget != NULL)
    {
        m_pTabWidget->deleteLater();
        m_pTabWidget = NULL;
    }
    qDebug()<<__func__<<__FILE__<<__LINE__<<"end"<<this->text(0);
}

void CGI_SCADA_DLL_TreeItemDevice::GetPopUpMenu()
{
    QMenu *SwitchMenu = new QMenu();
    //->添加网络节点
//    AddColChannel->setIcon(QIcon(":/ico/networkc.ico"));

    QAction *pUploadDevice = new QAction(tr("提交设备"), SwitchMenu);
    connect(pUploadDevice, SIGNAL(triggered()), this, SLOT(slot_UploadDevice()));

    QAction *pDeleteDevice = new QAction(tr("删除设备"), SwitchMenu);
    connect(pDeleteDevice, SIGNAL(triggered()), this, SLOT(slot_DeleteDevice()));

    SwitchMenu->addAction(pUploadDevice);
    SwitchMenu->addAction(pDeleteDevice);
    SwitchMenu->addSeparator();
    //-> 运行菜单
    SwitchMenu->exec(QCursor::pos());
//    delete SwitchMenu;//防止内存泄露
    SwitchMenu->deleteLater();
    qDebug()<<__func__<<__LINE__<<__FILE__<<this->childCount();
    return ;
}

QWidget *CGI_SCADA_DLL_TreeItemDevice::GetWidget(QTreeWidgetItem */*pItem_*/)
{
    return m_pTabWidget;
}

bool CGI_SCADA_DLL_TreeItemDevice::SaveAction(QDomDocument &doc, QDomElement &parentElem)
{
    QDomElement DeviceElem = doc.createElement("Device");
    DeviceElem.setAttribute("ConfigFileName","");
    DeviceElem.setAttribute("Name",this->text(0));
    QDomElement AttributeElem = doc.createElement("Attribute");

    AttributeElem.setAttribute("Device_Name",this->text(0));
    AttributeElem.setAttribute("Device_Model",m_Device.m_strDeviceDesc);
    AttributeElem.setAttribute("Device_Factory",m_Device.m_strDevice_Factory);
    m_pDeviceAttribute->SaveAction(doc,AttributeElem);
    DeviceElem.appendChild(AttributeElem);

//    for (int nTypeIndex = 0;nTypeIndex < m_TableViewList.count(); ++nTypeIndex)
//    {
//        QDomElement TypeElem = doc.createElement("Type");
//        m_TableViewList.at(nTypeIndex)->SaveAction(doc,TypeElem);
//        DeviceElem.appendChild(TypeElem);
//    }

    QMap<QString ,CGI_SCADA_DLL_DeviceTableView*>::iterator iterator;
    for (iterator = m_TableViewMap.begin(); iterator != m_TableViewMap.end(); ++iterator)
    {
        QDomElement TypeElem = doc.createElement("Type");
        iterator.value()->SaveAction(doc,TypeElem);
        DeviceElem.appendChild(TypeElem);
    }

    parentElem.appendChild(DeviceElem);
    return true;
}

bool CGI_SCADA_DLL_TreeItemDevice::SetInitTagMap(QMap<QString, QStringList> &TagMap_)
{
    QMap<QString ,CGI_SCADA_DLL_DeviceTableView*>::iterator iterator;
    for (iterator = m_TableViewMap.begin(); iterator != m_TableViewMap.end(); ++iterator)
    {
        iterator.value()->SetInitTagMap(TagMap_);
    }
}

bool CGI_SCADA_DLL_TreeItemDevice::SetBuildTagMap(const QMap<QString, QStringList> &TagMap_)
{
    QMap<QString ,CGI_SCADA_DLL_DeviceTableView*>::iterator iterator;
    for (iterator = m_TableViewMap.begin(); iterator != m_TableViewMap.end(); ++iterator)
    {
        iterator.value()->SetBuildTagMap(TagMap_);
    }
}

bool CGI_SCADA_DLL_TreeItemDevice::SetDeviceElem(const QDomElement &DeviceElem)
{
    QDomElement AttributeElem;
    if (DeviceElem.elementsByTagName("Attribute").isEmpty())
    {
        return false;
    }else
    {
        AttributeElem = DeviceElem.elementsByTagName("Attribute").at(0).toElement();
    }

    m_pDeviceAttribute->SetDeviceAttributeElem(AttributeElem);

    QDomNodeList TypeNodeList = DeviceElem.elementsByTagName("Type");

    for (int nTypeIndex = 0; nTypeIndex < TypeNodeList.count(); ++nTypeIndex)
    {
        QDomElement TypeElem = TypeNodeList.at(nTypeIndex).toElement();
        CGI_SCADA_DLL_DeviceTableView *pTableView = m_TableViewMap.value(TypeElem.attribute("Name"));
        if (pTableView)
        {
            pTableView->SetTypeElem(TypeElem);
        }
    }
    return true;
}

QStringListModel *CGI_SCADA_DLL_TreeItemDevice::GetListModel(QStringListModel *pStringListModel)
{
    pStringListModel->removeRows(0,pStringListModel->rowCount());
    QMap<QString ,CGI_SCADA_DLL_DeviceTableView*>::iterator iterator;
    for (iterator = m_TableViewMap.begin(); iterator != m_TableViewMap.end(); ++iterator)
    {
        iterator.value()->SetInitStringListModel(pStringListModel);
    }
    return pStringListModel;
}

/*!
 \brief 提交设备

 \fn CGI_SCADA_DLL_TreeItemDevice::slot_UploadDevice
*/
void CGI_SCADA_DLL_TreeItemDevice::slot_UploadDevice()
{
    qDebug()<<__func__<<__LINE__<<__FILE__<<"提交设备";
}

/*!
 \brief 删除设备

 \fn CGI_SCADA_DLL_TreeItemDevice::slot_DeleteDevice
*/
void CGI_SCADA_DLL_TreeItemDevice::slot_DeleteDevice()
{
    qDebug()<<__func__<<__LINE__<<__FILE__<<"删除设备";
    deleteLater();
}
