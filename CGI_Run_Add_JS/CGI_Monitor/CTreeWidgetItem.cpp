#include "CTreeWidgetItem.h"
#include <QDebug>


//CTreeWidgetItem::CTreeWidgetItem(TreeWidgetItem_Type nTreeWidgetItemType_, const QStringList &strings) :
//    QTreeWidgetItem(strings),
//    m_nTreeWidgetItem_Type(nTreeWidgetItemType_)
//{

//}


/*!
 \brief 串口配置文件使用的构造函数

 \fn CTreeWidgetItem::CTreeWidgetItem
 \param nChannelNumber_
 \param strText_
 \param element_
 \param parent
 \param type
*/
CTreeWidgetItem::CTreeWidgetItem(int nChannelNumber_, QString strText_, QDomElement element_, QTreeWidget *parent, int type) :
    QTreeWidgetItem(parent,type),
    m_pChannelMessage(new CChannelMessage(element_)),
    m_nChannelNumber(nChannelNumber_)
{
    qDebug()<<strText_<<type;
    this->setText(0,strText_);
    this->setIcon(0,QIcon("://Image/ico/network.ico"));
    QString elementTagName = element_.tagName();
    if ("Root" == elementTagName)
    {
        QDomNodeList DeviceList = element_.elementsByTagName("Device");
        int DeviceCount = DeviceList.count();
        for (int i = 0; i < DeviceCount; ++i)
        {
            CTreeWidgetItem *pTreeWidgetItem = new CTreeWidgetItem(DeviceList.at(i).toElement(),this,TreeWidgetItem_Device);
        }
    }
}

/*!
 \brief 设备节点使用的构造函数

 \fn CTreeWidgetItem::CTreeWidgetItem
 \param element_
 \param parent
 \param type
*/
CTreeWidgetItem::CTreeWidgetItem(QDomElement element_, QTreeWidgetItem *parent, int type) :
    QTreeWidgetItem(parent,type),
    m_pChannelMessage(new CChannelMessage(element_)),
    m_nChannelNumber(-1)
{
    QString elementTagName = element_.tagName();
    if ("Device" == elementTagName)
    {
        this->setText(0,element_.attribute("Name"));
        qDebug()<<"Device"<<element_.attribute("Name")<<type;
        this->setIcon(0,QIcon("://Image/ico/networkc.ico"));
    }
}

/*!
 \brief 网络配置文件中link使用的构造函数

 \fn CTreeWidgetItem::CTreeWidgetItem
 \param nChannelNumber_
 \param element_
 \param parent
 \param type
*/
CTreeWidgetItem::CTreeWidgetItem(int nChannelNumber_, QDomElement element_, QTreeWidgetItem *parent, int type) :
    QTreeWidgetItem(parent,type),
    m_pChannelMessage(new CChannelMessage(element_)),
    m_nChannelNumber(nChannelNumber_)
{
    QString elementTagName = element_.tagName();
    if ( "Link" == elementTagName)
    {
        this->setText(0,element_.attribute("Name"));
        qDebug()<<"Link"<<element_.attribute("Name")<<type;
        this->setIcon(0,QIcon("://Image/ico/network.ico"));
        QDomNodeList DeviceList = element_.elementsByTagName("Device");
        int DeviceCount = DeviceList.count();
        for (int i = 0; i < DeviceCount; ++i)
        {
            CTreeWidgetItem *pTreeWidgetItem = new CTreeWidgetItem(DeviceList.at(i).toElement(),this,TreeWidgetItem_Device);
        }
    }
}

int CTreeWidgetItem::GetChannelNumber() const
{
    qDebug()<<"m_nTreeWidgetItem_Type"<<this->type();
    switch (this->type()) {
    case TreeWidgetItem_File:
        return -1;
    case TreeWidgetItem_Channel:
    case TreeWidgetItem_Device:
        return m_nChannelNumber;
    default:
        return -1;
        break;
    }
}

QStandardItemModel *CTreeWidgetItem::GetModel(ChannelMessage_TAG_TYPE nTagType_)
{
    return m_pChannelMessage->GetModel(nTagType_);
}
