#include "CGI_SCADA_DeviceConfig.h"
#include "../CGI_SCADA_include/XmlParser/CXmlParser.h"

QMap<QString,Factory_Device > CGI_SCADA_DeviceConfig::s_ManufacturerMap;
CGI_SCADA_DeviceConfig::CGI_SCADA_DeviceConfig()
{
}

void CGI_SCADA_DeviceConfig::InitManufactureMap()
{
    QString strFileName(DeviceFileName);
    QDomDocument doc;
    if (!CXmlParser::readFile(strFileName,doc))
    {
        QString message = QString("没有找到设备配置文件.\n文件名为%1").arg(DeviceFileName);
        QMessageBox::warning(0, tr("InitManufactureMap"),
                                          message,
                                       QMessageBox::Cancel,
                                       QMessageBox::Cancel);
        return ;
    }
    QDomElement domElement;
    domElement = doc.documentElement();
    if ("Root" != domElement.tagName())
    {
        qDebug()<<"Error: Not a project file";
        QString message = QString("无效的设备配置文件.\n文件名为%1\nError: Not a project file").arg(DeviceFileName);
        QMessageBox::warning(0, tr("创建新设备"),
                                          message,
                                       QMessageBox::Cancel,
                                       QMessageBox::Cancel);
        return;
    }
    QDomNodeList FactoryNodeList = doc.elementsByTagName("Factory");
    QStringList strList;
    for (int nFactoryCount = 0; nFactoryCount < FactoryNodeList.count(); ++nFactoryCount)
    {
//        QList<Device> list;
        Factory_Device DeviceMap;
        QDomElement ManufacturerElement = FactoryNodeList.at(nFactoryCount).toElement();
        if (FactoryNodeList.at(nFactoryCount).hasChildNodes())
        {
            QDomNodeList DeviceList = FactoryNodeList.at(nFactoryCount).toElement().elementsByTagName("Device");
            for (int nDeviceCount = 0; nDeviceCount < DeviceList.count(); ++nDeviceCount)
            {
                QDomElement DeviceElement = DeviceList.at(nDeviceCount).toElement();
                Device device;
                device.m_strDevice_Factory = ManufacturerElement.attribute("value");
                device.m_strDeviceName = DeviceElement.attribute("name");
                device.m_strDeviceDesc = DeviceElement.attribute("value");

                QDomNodeList Att_ParameterNodeList = DeviceElement.elementsByTagName("Att_Parameter");
//                qDebug()<<"Att_ParameterNodeList"<<__LINE__<<Att_ParameterNodeList.count();
                for (int nAtt_ParameterCount = 0; nAtt_ParameterCount < Att_ParameterNodeList.count(); ++nAtt_ParameterCount)
                {
                    QDomElement Att_ParameterEle = Att_ParameterNodeList.at(nAtt_ParameterCount).toElement();
                    Device_Att_Parameter att_Parameter;
                    att_Parameter.m_strName = Att_ParameterEle.attribute("name");
                    att_Parameter.m_strvar  = Att_ParameterEle.attribute("var");
                    att_Parameter.m_strType = Att_ParameterEle.attribute("type");
                    att_Parameter.m_strInitValue = Att_ParameterEle.attribute("value");
//                    qDebug()<<"Att_ParameterEle"<<__LINE__<<Att_ParameterEle.attribute("name");
                    if (att_Parameter.m_strType == "select")
                    {
                        QDomNodeList selectNodeList = Att_ParameterEle.elementsByTagName("Select");
                        for (int nSelectCount = 0; nSelectCount < selectNodeList.count(); ++nSelectCount)
                        {
                            QDomElement selectEle = selectNodeList.at(nSelectCount).toElement();
                            att_Parameter.m_strEnumNamesList.append(selectEle.attribute("name"));
                        }
                    }
                    device.m_Att_ParameterList.append(att_Parameter);
                }

                QDomNodeList TypeNodeList = DeviceElement.elementsByTagName("Type");
                for (int nTypeCount = 0; nTypeCount < TypeNodeList.count(); ++nTypeCount)
                {
                    QDomElement TypeElement = TypeNodeList.at(nTypeCount).toElement();
                    Device_Type type;
                    type.m_strTypeName = TypeElement.attribute("name");
                    type.m_strValue = TypeElement.attribute("value");
                    QDomNodeList propertyNodeList = TypeElement.elementsByTagName("property");
                    for (int npropertyCount = 0; npropertyCount < propertyNodeList.count(); ++npropertyCount)
                    {
                        QDomElement propertyElement = propertyNodeList.at(npropertyCount).toElement();
                        Device_Type_Property property;
                        property.m_strName = propertyElement.attribute("name");
                        property.m_strvar = propertyElement.attribute("var");
                        property.m_StrEditType = propertyElement.attribute("type");
                        property.m_strInitValue = propertyElement.attribute("value");
                        property.m_strDisplayType = propertyElement.attribute("displayType");
                        if (property.m_StrEditType == "link")
                        {
//                            property.m_strInitValue = propertyElement.attribute("value");
                        }else if (property.m_StrEditType == "text")
                        {
//                            property.m_strInitValue = propertyElement.attribute("value");
                        }else if (property.m_StrEditType == "select")
                        {
                            QString strSource = propertyElement.attribute("source");
//                            qDebug()<<"strSource"<<__LINE__<<strSource<<"strInitValue"<<property.m_strInitValue;
                            strSource.remove('[');
                            strSource.remove(']');
//                            strSource.remove('{');
//                            qDebug()<<"strSource"<<__LINE__<<strSource;
//                            qDebug()<<"strSource"<<__LINE__<<strSource.split('}');
                            QStringList sourceList = strSource.split("},{");
                            for (int nvalue = 0; nvalue < sourceList.count(); ++nvalue)
                            {
                                sourceList[nvalue].remove('{');
                                sourceList[nvalue].remove('}');
                                sourceList[nvalue].remove("value: '");
                                sourceList[nvalue].remove(" text: '");
                                sourceList[nvalue].remove('\'');
//                                qDebug()<<"strSource"<<__LINE__<<sourceList[nvalue];
//                                qDebug()<<sourceList[nvalue].split(',');
                                QStringList valueList = sourceList[nvalue].split(',');
                                if (valueList.count() == 2)
                                {
                                    if (valueList.at(0) == property.m_strInitValue)
                                    {
                                        property.m_strInitValue = valueList.at(1);
//                                        qDebug()<<__LINE__<<"property.m_strInitValue"<<property.m_strInitValue;
                                    }
                                    property.m_strListEnumNames.append(valueList.at(1));
                                }

                            }
                        }
                        type.m_PropertyList.append(property);
                    }
                    device.m_Typelist.append(type);
                }
//                list.append(device);
                qDebug()<<__func__<<__LINE__<<"device.m_strDeviceDesc"<<device.m_strDeviceDesc;
                DeviceMap.insert(device.m_strDeviceDesc,device);
            }
        }
        QString ManufacturerName = ManufacturerElement.attribute("value");
        strList<<ManufacturerName;
//        ManufacturerMap.insert(ManufacturerName,list);
        qDebug()<<__func__<<__LINE__<<"ManufacturerName"<<ManufacturerName;
        s_ManufacturerMap.insert(ManufacturerName,DeviceMap);
    }
}

Device CGI_SCADA_DeviceConfig::GetDevice(QString strFactoryName_, QString strDeviceName_)
{
    if (s_ManufacturerMap.contains(strFactoryName_))
    {
        if (s_ManufacturerMap.value(strFactoryName_).contains(strDeviceName_))
        {
            return s_ManufacturerMap.value(strFactoryName_).value(strDeviceName_);
        }
    }
    return Device();
}
