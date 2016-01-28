#ifndef CGI_SCADA_DLL_DEFINE_H
#define CGI_SCADA_DLL_DEFINE_H
#include <QString>
#include <QMap>
#include <QDateTime>
#include <QPushButton>

#include <QTableView>
#include <QStandardItem>
#include <QModelIndex>
#include <QStandardItemModel>

#include <QDomElement>
#include <QDomDocument>

#include <QGridLayout>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QTreeWidgetItem>
#include <QInputDialog>
#include <QDir>


#define  COM_Attribute_ConfigFileName           "conf/CommunicationMode/COM.xml"
#define  CAN_Attribute_ConfigFileName           "conf/CommunicationMode/CAN.xml"
#define  COM_Attribute_ProtocolFileName         "conf/8.xml"
#define  Attribute_ProtocolFileName_New         "conf/xml/Protocol.xml"
#define  DeviceFileName                          "conf/xml/DeviceModel.xml"

#define  GUANLIANBIANLIANGMING_COLUMN   0   ///< 关联变量名所在列号
#define  BIANLIANGMING_COLUMN           1   ///< 名字所在列号

enum ProtocolType{
    ProtocolType_Undefine = 0,///< 未定义的通道类型
    ProtocolType_Collect,       ///< 采集驱动类型
    ProtocolType_Transform      ///< 转发驱动类型
};


class AttributeParameter{
public:
    AttributeParameter(){}

    AttributeParameter(const AttributeParameter &other)
    {
        m_strName           = other.m_strName;
        m_strDisplay        = other.m_strDisplay;
        m_StrEditType       = other.m_StrEditType;
        m_strInitValue      = other.m_strInitValue;
        m_strListEnumNames  = other.m_strListEnumNames;
    }

    QString m_strName;///< 属性名
    QString m_strDisplay;///< 显示名
    QString m_StrEditType;///< 编辑类型
    QString m_strInitValue;///< 初始值
    QMap<int,QString> m_strListEnumNames;///< 枚举类型map
};
class Device_Att_Parameter{
public:
    Device_Att_Parameter(){}

    Device_Att_Parameter(const Device_Att_Parameter &other)
    {
        m_strName           = other.m_strName;
        m_strvar            = other.m_strvar;
        m_strType           = other.m_strType;
        m_strInitValue      = other.m_strInitValue;
        m_strEnumNamesList  = other.m_strEnumNamesList;
    }

    QString m_strName;  ///< 显示名
    QString m_strvar;   ///< 属性名
    QString m_strType;///< 编辑类型
    QString m_strInitValue;///< 初始值
    QStringList m_strEnumNamesList;///< 枚举类型map
};

class Device_Type_Property{
public:
    Device_Type_Property(){}

    Device_Type_Property(const Device_Type_Property &other)
    {
        m_strName           = other.m_strName;
        m_strvar            = other.m_strvar;
        m_StrEditType       = other.m_StrEditType;
        m_strInitValue      = other.m_strInitValue;
        m_strDisplayType    = other.m_strDisplayType;
        m_strListEnumNames  = other.m_strListEnumNames;
    }

    QString m_strName;  ///< 显示名
    QString m_strvar;   ///< 属性名
    QString m_StrEditType;///< 编辑类型
    QString m_strInitValue;///< 初始值
    QString m_strDisplayType;
    QStringList m_strListEnumNames;///< 枚举类型map
};
class Device_Type {
public:
    Device_Type(){}

    Device_Type(const Device_Type& other)
    {
        m_strTypeName  = other.m_strTypeName;
        m_strValue     = other.m_strValue;
        m_PropertyList = other.m_PropertyList;
    }

    QString m_strTypeName;  ///< 显示名字
    QString m_strValue;     ///< 写入文件的名字
    QList<Device_Type_Property> m_PropertyList;///< property节点的list
};
class Device{
public:
    Device(){}

    Device(const Device &other)
    {
        m_strDevice_Factory = other.m_strDevice_Factory;
        m_strDeviceName     = other.m_strDeviceName;
        m_strDeviceDesc     = other.m_strDeviceDesc;
        m_Typelist          = other.m_Typelist;
        m_Att_ParameterList = other.m_Att_ParameterList;
    }
    QString m_strDevice_Factory;/// 设备厂家
    QString m_strDeviceName;///< 设备名称
    QString m_strDeviceDesc;///< 设备描述
    QList<Device_Type> m_Typelist;
    QList<Device_Att_Parameter> m_Att_ParameterList;
};
#endif // CGI_SCADA_DLL_DEFINE_H
