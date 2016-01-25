#include "CGI_SCADA_DLL_TreeItemCAN.h"
#include "UI_CreateDeviceDialog.h"
#include "../CGI_SCADA_include/XmlParser/CXmlParser.h"



CGI_SCADA_DLL_TreeItemCAN::CGI_SCADA_DLL_TreeItemCAN(int nCanNumber_, QTreeWidgetItem *parentItem, const QStringList &strings, int type, QObject *parent) :
    CGI_SCADA_DLL_TreeItemBase(parentItem,strings,type,parent),
    m_nCanNumber(nCanNumber_),
    m_nProtocolType(ProtocolType_Undefine)
{
    this->setDisabled(true);

    m_pMainWidget = new UI_COMCAN_Attribute(TreeItemType_ChanneM_CAN_Attribute);
    connect(m_pMainWidget,SIGNAL(signal_ProtocolTypeChange(int)),this,SIGNAL(signal_ProtocolTypeChange(int)));
    connect(m_pMainWidget,SIGNAL(signal_ProtocolTypeChange(int)),this,SLOT(slot_ProtocolTypeChange(int)));

//    m_pAttributeItem = new CGI_SCADA_DLL_TreeItemComCanAttribute(this,QStringList()<<"属性",TreeItemType_ChanneM_CAN_Attribute,this);
//    connect(m_pAttributeItem,SIGNAL(signal_ProtocolTypeChange(int)),this,SLOT(slot_ProtocolTypeChange(int)));
//    connect(m_pAttributeItem,SIGNAL(signal_ProtocolTypeChange(int)),this,SIGNAL(signal_ProtocolTypeChange(int)));
//    this->addChild(m_pAttributeItem);
}

CGI_SCADA_DLL_TreeItemCAN::~CGI_SCADA_DLL_TreeItemCAN()
{
    qDebug()<<__func__<<__FILE__<<__LINE__<<"start"<<this->text(0);
    if (m_pMainWidget != NULL)
    {
        m_pMainWidget->deleteLater();
//        delete m_pMainWidget;
        m_pMainWidget = NULL;
    }
    qDebug()<<__func__<<__FILE__<<__LINE__<<"end"<<this->text(0);
}

void CGI_SCADA_DLL_TreeItemCAN::GetPopUpMenu()
{
    QMenu *SwitchMenu = new QMenu();
    //->添加网络节点
//    AddColChannel->setIcon(QIcon(":/ico/networkc.ico"));

    QAction *pAddDevice = new QAction(tr("新建设备"), SwitchMenu);
    connect(pAddDevice, SIGNAL(triggered()), this, SLOT(slot_AddDevice()));

    QAction *pDeleteChannel = new QAction(tr("删除通道"), SwitchMenu);
    connect(pDeleteChannel, SIGNAL(triggered()), this, SLOT(slot_DeleteChannel()));
    QAction *pSetEnable = new QAction(tr("使能通道"), SwitchMenu);
    pSetEnable->setCheckable(true);
    if (this->isDisabled())
    {
        pSetEnable->setChecked(false);
    }else
    {
        pSetEnable->setChecked(true);
    }
//    this->isDisabled();
    connect(pSetEnable, SIGNAL(triggered(bool)), this, SLOT(slot_SetEnable(bool)));

//    QAction *pAddNET = new QAction(tr("添加NET"),SwitchMenu);
//    connect(pAddNET,SIGNAL(triggered()),this,SLOT(slot_AddNET()));

    SwitchMenu->addAction(pAddDevice);
    SwitchMenu->addAction(pDeleteChannel);
    SwitchMenu->addAction(pSetEnable);
//    SwitchMenu->addAction(pAddNET);
    SwitchMenu->addSeparator();
    //-> 运行菜单
    SwitchMenu->exec(QCursor::pos());
//    delete SwitchMenu;//防止内存泄露
    SwitchMenu->deleteLater();
    qDebug()<<__func__<<__LINE__<<__FILE__<<this->childCount();
    return ;
}

int CGI_SCADA_DLL_TreeItemCAN::GetCanNumber() const
{
    return m_nCanNumber;
}

bool CGI_SCADA_DLL_TreeItemCAN::SaveAction()
{
    QDomDocument SCADApro;
    QDomElement ProRoot = SCADApro.createElement(tr("Root"));
    SCADApro.appendChild(ProRoot);

    m_pMainWidget->SaveAction(SCADApro,ProRoot);

    QDomElement DeviceListElem  = SCADApro.createElement("DeviceList");
    for (int nDeviceIndex = 0; nDeviceIndex < this->childCount(); ++nDeviceIndex)
    {
        CGI_SCADA_DLL_I *pTreeItem = (CGI_SCADA_DLL_I*)this->child(nDeviceIndex);
        /*if (pTreeItem == m_pAttributeItem)/// 第一个是属性节点
            continue;
        else */
        if (pTreeItem->type() == CGI_SCADA_DLL_I::TreeItemType_Undefined)
            continue;
        else {
            pTreeItem->SaveAction(SCADApro,DeviceListElem);
        }
    }
    ProRoot.appendChild(DeviceListElem);
    QString strFileName;
//    qDebug()<<__func__<<__FILE__<<__LINE__<<m_strProjectPaths;
//    if (CGI_SCADA_DLL_TreeItemBase::m_strProjectPaths.isEmpty())
//    {
//        strFileName = "project/"+this->text(0)+"_.xml";
//    }else
//    {
//        strFileName = CGI_SCADA_DLL_TreeItemBase::m_strProjectPaths+"/"+this->text(0)+"_.xml";
//    }
    strFileName = "temp_project/"+this->text(0)+"_.xml";
    CXmlParser::writeFile(strFileName,SCADApro);
    return true;
}

bool CGI_SCADA_DLL_TreeItemCAN::SetInitTagMap(QMap<QString, QStringList> &TagMap_)
{
    switch (m_nProtocolType) {
    case ProtocolType_Transform:

        break;
    default:
        for (int nDeviceIndex = 0; nDeviceIndex < this->childCount(); ++nDeviceIndex)
        {
            CGI_SCADA_DLL_I *pTreeItem = (CGI_SCADA_DLL_I*)this->child(nDeviceIndex);
            /*if (pTreeItem == m_pAttributeItem)/// 第一个是属性节点
                continue;
            else */
            if (pTreeItem->type() == CGI_SCADA_DLL_I::TreeItemType_Undefined)
                continue;
            else {
                pTreeItem->SetInitTagMap(TagMap_);
            }
        }
        break;
    }
}

bool CGI_SCADA_DLL_TreeItemCAN::SetBuildTagMap(const QMap<QString, QStringList> &TagMap_)
{
    switch (m_nProtocolType) {
    case ProtocolType_Transform:
        for (int nDeviceIndex = 0; nDeviceIndex < this->childCount(); ++nDeviceIndex)
        {
            CGI_SCADA_DLL_I *pTreeItem = (CGI_SCADA_DLL_I*)this->child(nDeviceIndex);
            /*if (pTreeItem == m_pAttributeItem)/// 第一个是属性节点
                continue;
            else */
            if (pTreeItem->type() == CGI_SCADA_DLL_I::TreeItemType_Undefined)
                continue;
            else {
                pTreeItem->SetBuildTagMap(TagMap_);
            }
        }
        break;
    default:
        break;
    }
}

QWidget *CGI_SCADA_DLL_TreeItemCAN::GetWidget(QTreeWidgetItem *pItem_)
{
    return m_pMainWidget;
}

bool CGI_SCADA_DLL_TreeItemCAN::SetChannelFile(QString strFileName_)
{
    qDebug()<<__func__<<__LINE__<<__FILE__<<strFileName_;
    QDomDocument doc;
    if (!CXmlParser::readFile(strFileName_,doc))
    {
        return false;
    }

    QDomElement ChannelElem;
    if (doc.elementsByTagName("Channel").isEmpty())
    {
        return false;
    }else
    {
        ChannelElem = doc.elementsByTagName("Channel").at(0).toElement();
    }
    QDomElement TopElem;
    if (doc.elementsByTagName("Top").isEmpty())
    {
        return false;
    }else
    {
        TopElem = doc.elementsByTagName("Top").at(0).toElement();
    }
    QDomElement ProtocolElem;
    if (doc.elementsByTagName("Protocol").isEmpty())
    {
        return false;
    }else
    {
        ProtocolElem = doc.elementsByTagName("Protocol").at(0).toElement();
    }
    QDomElement MainPortElem;
    if (doc.elementsByTagName("MainPort").isEmpty())
    {
        return false;
    }else
    {
        MainPortElem = doc.elementsByTagName("MainPort").at(0).toElement();
    }
    QDomElement DeviceListElem;
    if (doc.elementsByTagName("DeviceList").isEmpty())
    {
        return false;
    }else
    {
        DeviceListElem = doc.elementsByTagName("DeviceList").at(0).toElement();
    }
    m_pMainWidget->SetComCanChannelTopProtocolMainPortElem(ChannelElem,TopElem,ProtocolElem,MainPortElem);
    QDomNodeList DeviceNodeList = DeviceListElem.elementsByTagName("Device");
    qDebug()<<__func__<<__LINE__<<__FILE__<<DeviceNodeList.count();
    for (int nDeviceIndex = 0; nDeviceIndex < DeviceNodeList.count(); ++nDeviceIndex)
    {
        QDomElement DeviceElem = DeviceNodeList.at(nDeviceIndex).toElement();
        QDomElement AttributeElem;
        if (DeviceElem.elementsByTagName("Attribute").isEmpty())
        {
            continue;
        }else
        {
            AttributeElem = DeviceElem.elementsByTagName("Attribute").at(0).toElement();
        }
        Device device = CGI_SCADA_DeviceConfig::GetDevice(AttributeElem.attribute("Device_Factory"),AttributeElem.attribute("Device_Model"));
        qDebug()<<__func__<<__LINE__<<__FILE__<<"m_nProtocolType"<<m_nProtocolType;
        CGI_SCADA_DLL_TreeItemDevice *pDeviceItem = new CGI_SCADA_DLL_TreeItemDevice(m_nProtocolType,device,this,QStringList()<<DeviceElem.attribute("Name"),TreeItemType_ChanneM_CAN_Device,this);
        qDebug()<<__func__<<__LINE__<<__FILE__<<AttributeElem.attribute("Device_Factory")<<AttributeElem.attribute("Device_Model")<<device.m_strDeviceName;
        connect(this,SIGNAL(signal_ProtocolTypeChange(int)),pDeviceItem,SIGNAL(signal_ProtocolTypeChange(int)));
        connect(pDeviceItem,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
        qDebug()<<__func__<<__LINE__<<__FILE__<<AttributeElem.attribute("Device_Factory")<<AttributeElem.attribute("Device_Model")<<device.m_strDeviceName;
        this->addChild(pDeviceItem);
        qDebug()<<__func__<<__LINE__<<__FILE__<<AttributeElem.attribute("Device_Factory")<<AttributeElem.attribute("Device_Model")<<device.m_strDeviceName;
        pDeviceItem->SetDeviceElem(DeviceElem);
    }
    qDebug()<<__func__<<__LINE__<<__FILE__;
    return true;
}

void CGI_SCADA_DLL_TreeItemCAN::slot_AddDevice()
{
    qDebug()<<__func__;
    UI_CreateDeviceDialog dialog;
    int ret = dialog.exec();
    if (ret)
    {
        qDebug()<<__func__<<"创建设备";
        Device  device = dialog.GetNewCreateDevice();
        qDebug()<<__func__<<__LINE__<<"m_Device.m_Att_ParameterList.count()"<<device.m_Att_ParameterList.count();
        for (int i = 0; i < device.m_Att_ParameterList.count(); ++i)
        {
            qDebug()<<__func__<<__LINE__<<device.m_Att_ParameterList.at(i).m_strName;
        }
        QString strDeviceName = dialog.GetDeviceName();
        qDebug()<<__func__<<__LINE__<<device.m_strDeviceName<<device.m_strDeviceDesc;
        for (int i = 0 ; i < device.m_Typelist.count(); ++i)
        {
            qDebug()<<__func__<<__LINE__<<device.m_Typelist.at(i).m_strValue;
        }
        CGI_SCADA_DLL_TreeItemDevice *pDeviceItem = new CGI_SCADA_DLL_TreeItemDevice(m_nProtocolType,device,this,QStringList()<<strDeviceName,TreeItemType_ChanneM_CAN_Device,this);
        connect(pDeviceItem,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
        connect(this,SIGNAL(signal_ProtocolTypeChange(int)),pDeviceItem,SIGNAL(signal_ProtocolTypeChange(int)));
        this->addChild(pDeviceItem);
    }else
    {
        qDebug()<<__func__<<"不创建设备";
    }
}

void CGI_SCADA_DLL_TreeItemCAN::slot_DeleteChannel()
{
    qDebug()<<__func__;
    int ret = QMessageBox::warning(NULL, tr("删除通道"),
                                             tr("即将删除此通道，\n"
                                                "确定删除此通道吗？"),
                                             QMessageBox::Cancel|QMessageBox::Ok,
                                             QMessageBox::Cancel);
    if (ret == QMessageBox::Ok)
    {
        emit signal_DeleteChannl();
    }
}

void CGI_SCADA_DLL_TreeItemCAN::slot_ProtocolTypeChange(int nProtocolType_)
{
    m_nProtocolType = (ProtocolType)nProtocolType_;
    qDebug()<<__func__<<__LINE__<<m_nProtocolType;
}

void CGI_SCADA_DLL_TreeItemCAN::slot_SetEnable(bool enable)
{
    qDebug()<<__func__<<__FILE__<<__LINE__<<enable;
//    if (QAction* pAction = dynamic_cast<QAction*>(sender()))
//    {
        if (enable)
        {
            this->setDisabled(false);
        }else
        {
            this->setDisabled(true);
        }
//    }
}
