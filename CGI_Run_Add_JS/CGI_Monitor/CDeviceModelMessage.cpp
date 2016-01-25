#include "CDeviceModelMessage.h"
#include <QDomNodeList>
#include <QDomElement>
QMap<QString, Device_Type_Property* > CDeviceModelMessage::m_DeviceTypeMessageMap;

CDeviceModelMessage::CDeviceModelMessage(QObject *parent) :
    QObject(parent)
{
}

void CDeviceModelMessage::InitDeviceModelMessage(QString strFileName_)
{
    strFileName_.append("/DeviceModel.xml");
    qDebug()<<__FILE__<<__func__<<strFileName_;
    QDomDocument doc;
    if (CXmlParser::readFile(strFileName_,doc))
    {
        QDomNodeList DeviceNodeList = doc.elementsByTagName("Device");
        for (int i = 0; i < DeviceNodeList.count(); ++i)
        {
            QDomElement DeviceElement = DeviceNodeList.at(i).toElement();
            Device_Type_Property *pDevice_Type_Property = new Device_Type_Property;
            QDomNodeList TypeNodeList = DeviceElement.elementsByTagName("Type");
            for (int j = 0; j < TypeNodeList.count(); ++j)
            {
                QDomElement TypeElement = TypeNodeList.at(j).toElement();
                QString Type_value = TypeElement.attribute("value");
                QDomNodeList propertyNodeList = TypeElement.elementsByTagName("property");
                PropertyList *pPropertyList = new PropertyList;
                Property *pProperty = NULL;
                for (int k = 0; k < propertyNodeList.count(); ++k)
                {
                    QDomElement propertyElement = propertyNodeList.at(k).toElement();
                    pProperty = new Property;
                    pProperty->m_name = propertyElement.attribute("name");
                    pProperty->m_var  = propertyElement.attribute("var");
                    qDebug()<<"--"<<Type_value<<pProperty->m_name<<pProperty->m_var;
                    pPropertyList->append(pProperty);
                }
                if ("YX" == Type_value)
                {
                    qDebug()<<"----------------"<<pPropertyList;
                    pDevice_Type_Property->m_pYXPropertyList = pPropertyList;
                }else if ("YC" == Type_value)
                {

                    pDevice_Type_Property->m_pYCPropertyList = pPropertyList;
                }else if ("YM" == Type_value)
                {

                    pDevice_Type_Property->m_pYMPropertyList = pPropertyList;
                }else if ("YK" == Type_value)
                {
                    pDevice_Type_Property->m_pYKPropertyList = pPropertyList;

                }else if ("YS" == Type_value)
                {
                    pDevice_Type_Property->m_pYSPropertyList = pPropertyList;

                }
            }
            if (!m_DeviceTypeMessageMap.contains(DeviceElement.attribute("value")))
            {
                qDebug()<<__FILE__<<__func__<<DeviceElement.attribute("value")<<"succeed";
                m_DeviceTypeMessageMap.insert(DeviceElement.attribute("value"),pDevice_Type_Property);
            }else
            {
                qDebug()<<__FILE__<<__func__<<DeviceElement.attribute("value")<<"fail";
            }
        }
    }
}

