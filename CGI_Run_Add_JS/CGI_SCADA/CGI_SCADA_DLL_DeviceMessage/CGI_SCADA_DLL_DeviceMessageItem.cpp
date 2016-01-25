#include "CGI_SCADA_DLL_DeviceMessageItem.h"

/*!
 \brief 库文件导出接口函数

 \fn CreateDriver
 \param parent QObject对象指针
 \return CGI_SCADA_DLL_I 返回创建的驱动指针
*/
CGI_SCADA_DLL_I * CreateDriver(QObject *parent)
{
    QStringList strings;
    strings << "设备信息";
    qDebug()<<__func__<<"设备信息";
    CGI_SCADA_DLL_DeviceMessageItem *testProtocol = new CGI_SCADA_DLL_DeviceMessageItem(strings,CGI_SCADA_DLL_I::TreeItemType_DeviceM,parent);
    qDebug()<<testProtocol->text(0);
    return  testProtocol;
}

CGI_SCADA_DLL_DeviceMessageItem::CGI_SCADA_DLL_DeviceMessageItem(const QStringList &strings, int type,QObject *parent) :
    CGI_SCADA_DLL_TreeItemBase(strings,type,parent)
{
    QString str = QString("aaaaaaaaaaaa dev%1").arg(QString::number(type));
    m_pLabel = new QLabel(str);

    QStringList strNameList;

//    strNameList << "运行状态";
//    m_pOperating_status = new CGI_SCADA_DLL_TreeItemOperating_status(this,strNameList,TreeItemType_DeviceM_operating_status);
//    this->addChild(m_pOperating_status);

//    strNameList.clear();
//    strNameList << "硬件型号";
//    m_pHardware_model = new CGI_SCADA_DLL_TreeItemHardware_model(this,strNameList,TreeItemType_DeviceM_hardware_model);
//    this->addChild(m_pHardware_model);

//    strNameList.clear();
//    strNameList << "软件版本";
//    m_pSoftware_version = new CGI_SCADA_DLL_TreeItemSoftware_version(this,strNameList,TreeItemType_DeviceM_software_version);
//    this->addChild(m_pSoftware_version);

//    strNameList.clear();
//    strNameList << "当前用户";
//    m_pCurrent_users = new CGI_SCADA_DLL_TreeItemCurrent_users(this,strNameList,TreeItemType_DeviceM_current_users);
//    this->addChild(m_pCurrent_users);

//    strNameList.clear();
//    strNameList << "设备时间";
//    m_pEquipment_time = new CGI_SCADA_DLL_TreeItemEquipment_time(this,strNameList,TreeItemType_DeviceM_equipment_time);
//    this->addChild(m_pEquipment_time);

//    strNameList.clear();
//    strNameList << "当前工程";
//    m_pCurrent_project = new CGI_SCADA_DLL_TreeItemCurrent_project(this,strNameList,TreeItemType_DeviceM_current_project);
//    this->addChild(m_pCurrent_project);

//    strNameList.clear();
//    strNameList << "产权信息";
//    m_pProperty_information = new CGI_SCADA_DLL_TreeItemProperty_information(this,strNameList,TreeItemType_DeviceM_property_information);
//    this->addChild(m_pProperty_information);


    strNameList << "运行状态";
    m_pOperating_status = new CGI_SCADA_DLL_TreeItemChildItem(this,strNameList,TreeItemType_DeviceM_operating_status);
    this->addChild(m_pOperating_status);

    strNameList.clear();
    strNameList << "硬件型号";
    m_pHardware_model = new CGI_SCADA_DLL_TreeItemChildItem(this,strNameList,TreeItemType_DeviceM_hardware_model);
    this->addChild(m_pHardware_model);

    strNameList.clear();
    strNameList << "软件版本";
    m_pSoftware_version = new CGI_SCADA_DLL_TreeItemChildItem(this,strNameList,TreeItemType_DeviceM_software_version);
    this->addChild(m_pSoftware_version);

    strNameList.clear();
    strNameList << "当前用户";
    m_pCurrent_users = new CGI_SCADA_DLL_TreeItemChildItem(this,strNameList,TreeItemType_DeviceM_current_users);
    this->addChild(m_pCurrent_users);

    strNameList.clear();
    strNameList << "设备时间";
    m_pEquipment_time = new CGI_SCADA_DLL_TreeItemChildItem(this,strNameList,TreeItemType_DeviceM_equipment_time);
    this->addChild(m_pEquipment_time);

    strNameList.clear();
    strNameList << "当前工程";
    m_pCurrent_project = new CGI_SCADA_DLL_TreeItemChildItem(this,strNameList,TreeItemType_DeviceM_current_project);
    this->addChild(m_pCurrent_project);

    strNameList.clear();
    strNameList << "产权信息";
    m_pProperty_information = new CGI_SCADA_DLL_TreeItemChildItem(this,strNameList,TreeItemType_DeviceM_property_information);
    this->addChild(m_pProperty_information);
}

CGI_SCADA_DLL_DeviceMessageItem::~CGI_SCADA_DLL_DeviceMessageItem()
{
    qDebug()<<__func__;
}

/*!
 \brief 获取树节点的顺序

 \fn CGI_SCADA_DLL_DeviceMessageItem::GetItemNumber
 \return int 这个号码代表的是树节点的顺序，这个号码在所有节点驱动中是唯一的
*/
int CGI_SCADA_DLL_DeviceMessageItem::GetItemNumber()
{
    return 0;
}

QWidget *CGI_SCADA_DLL_DeviceMessageItem::GetWidget(QTreeWidgetItem *pItem_)
{
//    QString str = QString("aaaaaaaaaaaa dev%1").arg(QString::number(pItem_->type()));
//    m_pLabel = new QLabel(str);
//    QWidget *pwidget = new QWidget;
    return m_pLabel;
}

