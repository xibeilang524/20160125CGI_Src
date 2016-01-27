#include "CGI_SCADA_DLL_TreeItemNetLink.h"
#include "UI_CreateDeviceDialog.h"
#include <QThread>
#include <QProcess>


CGI_SCADA_DLL_TreeItemNetLink::CGI_SCADA_DLL_TreeItemNetLink(QTreeWidgetItem *parentItem, const QStringList &strings, int type, QObject *parent) :
    CGI_SCADA_DLL_TreeItemBase(parentItem,strings,type,parent)
{
    m_pMainWidget = new UI_COMCAN_Attribute(TreeItemType_ChanneM_NET_Link_Attribute);
    connect(m_pMainWidget,SIGNAL(signal_ProtocolTypeChange(int)),this,SLOT(slot_ProtocolTypeChange(int)));
    connect(m_pMainWidget,SIGNAL(signal_ProtocolTypeChange(int)),this,SIGNAL(signal_ProtocolTypeChange(int)));
    connect(m_pMainWidget,SIGNAL(signal_ChangeProtocolName(QString)),this,SLOT(slot_ChangeProtocolName(QString)));
}

QWidget *CGI_SCADA_DLL_TreeItemNetLink::GetWidget(QTreeWidgetItem */*pItem_*/)
{
    return m_pMainWidget;
}

void CGI_SCADA_DLL_TreeItemNetLink::GetPopUpMenu()
{
    QMenu *SwitchMenu = new QMenu();
    //->添加网络节点
//    AddColChannel->setIcon(QIcon(":/ico/networkc.ico"));

    QAction *pAddDevice = new QAction(tr("新建设备"), SwitchMenu);
    connect(pAddDevice, SIGNAL(triggered()), this, SLOT(slot_AddDevice()));

    QAction *pDeleteChannel = new QAction(tr("删除链接"), SwitchMenu);
    connect(pDeleteChannel, SIGNAL(triggered()), this, SLOT(slot_DeleteChannel()));

    QAction *pProtocolExplanation = new QAction(tr("驱动说明"),SwitchMenu);
    connect(pProtocolExplanation,SIGNAL(triggered()),this,SLOT(slot_ProtocolExplanation()));

    SwitchMenu->addAction(pAddDevice);
    SwitchMenu->addAction(pDeleteChannel);
    SwitchMenu->addAction(pProtocolExplanation);
    SwitchMenu->addSeparator();
    //-> 运行菜单
    SwitchMenu->exec(QCursor::pos());
//    delete SwitchMenu;//防止内存泄露
    SwitchMenu->deleteLater();
    qDebug()<<__func__<<__LINE__<<__FILE__<<this->childCount();
    return ;
}

bool CGI_SCADA_DLL_TreeItemNetLink::SaveAction(QDomDocument &doc, QDomElement &parentElem)
{
    QDomElement LinkElem = doc.createElement("Link");
    parentElem.appendChild(LinkElem);
    LinkElem.setAttribute("Name",this->text(0));
    m_pMainWidget->SaveAction(doc,LinkElem);
    QDomElement DeviceListElem  = doc.createElement("DeviceList");
    for (int nDeviceIndex = 0; nDeviceIndex < this->childCount(); ++nDeviceIndex)
    {
        if (this->child(nDeviceIndex)->type() != TreeItemType_Undefined)
        {
            ((CGI_SCADA_DLL_I*)this->child(nDeviceIndex))->SaveAction(doc,DeviceListElem);
        }
    }
    LinkElem.appendChild(DeviceListElem);
    return true;
}

bool CGI_SCADA_DLL_TreeItemNetLink::SetInitTagMap(QMap<QString, QStringList> &TagMap_)
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

bool CGI_SCADA_DLL_TreeItemNetLink::SetBuildTagMap(const QMap<QString, QStringList> &TagMap_)
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

bool CGI_SCADA_DLL_TreeItemNetLink::setLinkElem(const QDomElement &LinkElem_)
{
    m_pMainWidget->SetNetLinkElem(LinkElem_);

    QDomNodeList DeviceNodeList = LinkElem_.elementsByTagName("Device");
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
        qDebug()<<__func__<<__LINE__<<__FILE__<<AttributeElem.attribute("Device_Factory")<<AttributeElem.attribute("Device_Model")<<device.m_strDeviceName;
        CGI_SCADA_DLL_TreeItemDevice *pDeviceItem = new CGI_SCADA_DLL_TreeItemDevice(m_nProtocolType,device,this,QStringList()<<DeviceElem.attribute("Name"),TreeItemType_ChanneM_NET_Link_Device,this);
        qDebug()<<__func__<<__LINE__<<__FILE__<<AttributeElem.attribute("Device_Factory")<<AttributeElem.attribute("Device_Model")<<device.m_strDeviceName;
        connect(pDeviceItem,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
        connect(this,SIGNAL(signal_ProtocolTypeChange(int)),pDeviceItem,SIGNAL(signal_ProtocolTypeChange(int)));
        qDebug()<<__func__<<__LINE__<<__FILE__<<AttributeElem.attribute("Device_Factory")<<AttributeElem.attribute("Device_Model")<<device.m_strDeviceName;
        this->addChild(pDeviceItem);
        qDebug()<<__func__<<__LINE__<<__FILE__<<AttributeElem.attribute("Device_Factory")<<AttributeElem.attribute("Device_Model")<<device.m_strDeviceName;
        pDeviceItem->SetDeviceElem(DeviceElem);
    }
    return true;
}

void CGI_SCADA_DLL_TreeItemNetLink::slot_ChangeProtocolName(QString strProtocolName)
{
    m_strProtocolName = QString("conf/Explanation/%1.txt").arg(strProtocolName);
}
void CGI_SCADA_DLL_TreeItemNetLink::slot_AddDevice()
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
        CGI_SCADA_DLL_TreeItemDevice *pDeviceItem = new CGI_SCADA_DLL_TreeItemDevice(m_nProtocolType,device,this,QStringList()<<strDeviceName,TreeItemType_ChanneM_NET_Link_Device,this);
        connect(pDeviceItem,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
        connect(this,SIGNAL(signal_ProtocolTypeChange(int)),pDeviceItem,SIGNAL(signal_ProtocolTypeChange(int)));
        this->addChild(pDeviceItem);
    }else
    {
        qDebug()<<__func__<<"不创建设备";
    }
}
void CGI_SCADA_DLL_TreeItemNetLink::slot_DeleteChannel()
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
        deleteLater();
    }
}

void CGI_SCADA_DLL_TreeItemNetLink::slot_ProtocolExplanation()
{
    QString strCommand = QString("notepad ") + m_strProtocolName;
    QFile file(m_strProtocolName);

    if (file.exists())
    {
        qDebug()<<__func__<<__LINE__<<strCommand<<m_strProtocolName;
//        QProcess process;
        if (m_Process.isOpen())
        {
            m_Process.close();
        }else
        {

        }
        m_Process.start(QString("notepad"),QStringList()<<m_strProtocolName);
//        system(strCommand.toStdString().data());/// 需要将此运行放入新的线程中，解决界面卡死的问题,或者使用QProcess试试看可以不
    }else
    {
        qDebug()<<__func__<<__LINE__<<strCommand<<m_strProtocolName;
    }
}
CGI_SCADA_DLL_TreeItemNetLink::~CGI_SCADA_DLL_TreeItemNetLink()
{
    qDebug()<<__func__<<__FILE__<<__LINE__<<"start"<<this->text(0);
    if (m_pMainWidget != NULL)
    {
        m_pMainWidget->deleteLater();///< 继承于QObject的类最好用deleteLater()函数来删除，否则容易出现doublefree的情况
//        delete m_pMainWidget;
        m_pMainWidget = NULL;
    }
    qDebug()<<__func__<<__FILE__<<__LINE__<<"end"<<this->text(0);;
}

void CGI_SCADA_DLL_TreeItemNetLink::slot_ProtocolTypeChange(int nProtocolType_)
{
    m_nProtocolType = (ProtocolType)nProtocolType_;
    qDebug()<<__func__<<__LINE__<<m_nProtocolType;
}
