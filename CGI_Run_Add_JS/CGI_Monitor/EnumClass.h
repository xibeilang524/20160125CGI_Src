#ifndef ENUMCLASS_H
#define ENUMCLASS_H
#include <QString>
#include <QList>
#include <QDebug>
#include "../PortInclude/XmlParser/CXmlParser.h"

#define TableViewColumn 9//tableview的列数
#define TagLinkColumn 2
#define TagNameColumn 1
#define TagDescColumn 2
#define TagIDColumn   3
#define TagOriginalColumn 1 //原始值
#define TagProjectColumn  0 //工程值
#define TagKValueColumn   6 //系数
#define TagBaseValueColumn 7 //基数
#define TagTimeColumn 8 //时间戳

#ifndef TAG_TYPE_ENUM
#define TAG_TYPE_ENUM
/*!
 \brief Tag类型

 \enum TAG_TYPE
*/
enum ChannelMessage_TAG_TYPE
{
    ChannelMessage_TAG_TYPE_DI = 1,///< 遥信
    ChannelMessage_TAG_TYPE_AI,    ///< 遥测
    ChannelMessage_TAG_TYPE_MEM,   ///< 遥脉
    ChannelMessage_TAG_TYPE_DO,    ///< 遥控
    ChannelMessage_TAG_TYPE_AO,    ///< 遥设
    ChannelMessage_TAG_TYPE_PRO,   ///< 属性
};
#endif


#ifndef ChannelType_H
#define ChannelType_H
enum ChannelType{
    ChannelType_COM = 0,
    ChannelType_NET,
    ChannelType_CAN,
    ChannelType_Monitor104,
    ChannelType_DeliverMonitor104,
};
#endif


#ifndef TREEWIDGETITEM_TYPE
#define TREEWIDGETITEM_TYPE
enum TreeWidgetItem_Type{
    TreeWidgetItem_File = 0,
    TreeWidgetItem_Channel,
    TreeWidgetItem_Device,
};

#endif


/*!
 \brief 潘工DeviceModel.xml文件中的点的信息类

 \class property CChannelMessage.h "CChannelMessage.h"
*/
class Property
{
public:

    QString m_name;
    QString m_var;

};
typedef QList<Property *> PropertyList;
class Device_Type_Property
{
public:
    Device_Type_Property()
    {
        m_pYXPropertyList = NULL;
        m_pYCPropertyList = NULL;
        m_pYMPropertyList = NULL;
        m_pYKPropertyList = NULL;
        m_pYSPropertyList = NULL;
    }
    PropertyList *m_pYXPropertyList;
    PropertyList *m_pYCPropertyList;
    PropertyList *m_pYMPropertyList;
    PropertyList *m_pYKPropertyList;
    PropertyList *m_pYSPropertyList;
};


#ifndef ENUM_CHANNELMESSAGETYPE
#define ENUM_CHANNELMESSAGETYPE
enum ChannelMessage_Type
{
    ChannelMessage_Type_ComChannel=0,
    ChannelMessage_Type_NetChannel,
    ChannelMessage_Type_CanChannel,
    ChannelMessage_Type_Device,
};
#endif

#endif // ENUMCLASS_H
