#include "CChannelMessage.h"

CChannelMessage::CChannelMessage(const QDomElement &element_, QObject *parent) :
    QObject(parent),
    m_pPro_Model(NULL),
    m_pYX_Model(NULL),
    m_pYC_Model(NULL),
    m_pYM_Model(NULL),
    m_pYK_Model(NULL),
    m_pYS_Model(NULL)
{
    SetElement(element_);

}

void CChannelMessage::SetElement(const QDomElement &element_)
{
    QString elementTagName = element_.tagName();
    if ("Root" == elementTagName)
    {
        SetRootElement(element_);
    }else if ("Link" == elementTagName)
    {
        SetLinkElement(element_);
    }else if ("Device" == elementTagName)
    {
        SetDeviceElement(element_);
    }
}

void CChannelMessage::SetRootElement(const QDomElement &element_)
{
    m_nChannelMessage_Type = ChannelMessage_Type_ComChannel;
    if (element_.elementsByTagName("Channel").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"Channel\").isEmpty()";
        Q_ASSERT(false);
        return;
    }
    if (element_.elementsByTagName("Top").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"Top\").isEmpty()";
        Q_ASSERT(false);
        return;
    }
    if (element_.elementsByTagName("Protocol").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"Protocol\").isEmpty()";
        Q_ASSERT(false);
        return;
    }
    if (element_.elementsByTagName("MainPort").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"MainPort\").isEmpty()";
        Q_ASSERT(false);
        return;
    }
    if (element_.elementsByTagName("Device").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"Device\").isEmpty()";
        Q_ASSERT(false);
        return;
    }
    if (element_.elementsByTagName("Attribute").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"Attribute\").isEmpty()";
        Q_ASSERT(false);
        return;
    }
    if (element_.elementsByTagName("Type").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"Type\").isEmpty()";
        return;
    }
    QStringList strDisplayList;
    qDebug()<<"11111111111111";

    QString Device_Model = element_.elementsByTagName("Attribute").at(0).toElement().attribute("Device_Model");
    Device_Type_Property *pDevice_Type_Property = CDeviceModelMessage::m_DeviceTypeMessageMap.value(Device_Model);
    if (pDevice_Type_Property == NULL)
        return;
    QDomNodeList TypeNodeList = element_.elementsByTagName("Type");
    qDebug()<<"22222222222222"<<Device_Model<<pDevice_Type_Property->m_pYXPropertyList;
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYXPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYXPropertyList->count();
        qDebug()<<"33333333333333";

        strDisplayList<<"工程值"<<"原始值";
        int i = 0;
        for (i = 0; i < pDevice_Type_Property->m_pYXPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYXPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYXPropertyList->at(i)->m_name;

        }
        m_pYX_Model = new QStandardItemModel;
        m_pYX_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYX_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YX")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();
                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYXPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYXPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYX_Model->appendRow(items);
                }
            }
        }
    }
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYCPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYCPropertyList->count();
        qDebug()<<"33333333333333";

        int i = 0;
        strDisplayList<<"工程值"<<"原始值";
        for (i = 0; i < pDevice_Type_Property->m_pYCPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYCPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYCPropertyList->at(i)->m_name;

        }
        m_pYC_Model = new QStandardItemModel;
        m_pYC_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYC_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YC")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();
                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYCPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYCPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYC_Model->appendRow(items);
                }
            }
        }
    }
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYMPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYMPropertyList->count();
        qDebug()<<"33333333333333";

        int i = 0;
        strDisplayList<<"工程值"<<"原始值";
        for (i = 0; i < pDevice_Type_Property->m_pYMPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYMPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYMPropertyList->at(i)->m_name;

        }
        m_pYM_Model = new QStandardItemModel;
        m_pYM_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYM_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YM")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();
                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYMPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYMPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYM_Model->appendRow(items);
                }
            }
        }
    }
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYKPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYKPropertyList->count();
        qDebug()<<"33333333333333";

        int i = 0;
        strDisplayList<<"工程值"<<"原始值";
        for (i = 0; i < pDevice_Type_Property->m_pYKPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYKPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYKPropertyList->at(i)->m_name;
        }
        m_pYK_Model = new QStandardItemModel;
        m_pYK_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYK_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YK")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();
                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYKPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYKPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYK_Model->appendRow(items);
                }
            }
        }
    }
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYSPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYSPropertyList->count();
        qDebug()<<"33333333333333";

        int i = 0;
        strDisplayList<<"工程值"<<"原始值";
        for (i = 0; i < pDevice_Type_Property->m_pYSPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYSPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYSPropertyList->at(i)->m_name;

        }
        m_pYS_Model = new QStandardItemModel;
        m_pYS_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYS_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YS")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();
                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYSPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYSPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYS_Model->appendRow(items);
                }
            }
        }
    }
}

void CChannelMessage::SetLinkElement(const QDomElement &element_)
{
    m_nChannelMessage_Type = ChannelMessage_Type_NetChannel;
    if (element_.elementsByTagName("Channel").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"Channel\").isEmpty()";
        Q_ASSERT(false);
        return;
    }
    if (element_.elementsByTagName("Top").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"Top\").isEmpty()";
        Q_ASSERT(false);
        return;
    }
    if (element_.elementsByTagName("Protocol").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"Protocol\").isEmpty()";
        Q_ASSERT(false);
        return;
    }
    if (element_.elementsByTagName("MainPort").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"MainPort\").isEmpty()";
        Q_ASSERT(false);
        return;
    }
    if (element_.elementsByTagName("Device").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"Device\").isEmpty()";
        return;
    }
    if (element_.elementsByTagName("Attribute").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"Attribute\").isEmpty()";
        Q_ASSERT(false);
        return;
    }
    if (element_.elementsByTagName("Type").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"Type\").isEmpty()";
        return;
    }
    QStringList strDisplayList;
    qDebug()<<"11111111111111";

    QString Device_Model = element_.elementsByTagName("Attribute").at(0).toElement().attribute("Device_Model");
    Device_Type_Property *pDevice_Type_Property = CDeviceModelMessage::m_DeviceTypeMessageMap.value(Device_Model);
    if (pDevice_Type_Property == NULL)
        return;
    QDomNodeList TypeNodeList = element_.elementsByTagName("Type");
    qDebug()<<"22222222222222"<<Device_Model<<pDevice_Type_Property->m_pYXPropertyList;
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYXPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYXPropertyList->count();
        qDebug()<<"33333333333333";

        int i = 0;
        strDisplayList<<"工程值"<<"原始值";
        for (i = 0; i < pDevice_Type_Property->m_pYXPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYXPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYXPropertyList->at(i)->m_name;
        }
        m_pYX_Model = new QStandardItemModel;
        m_pYX_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYX_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YX")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();

                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYXPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYXPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYX_Model->appendRow(items);
                }
            }
        }
    }
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYCPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYCPropertyList->count();
        qDebug()<<"33333333333333";

        int i = 0;
        strDisplayList<<"工程值"<<"原始值";
        for (i = 0; i < pDevice_Type_Property->m_pYCPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYCPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYCPropertyList->at(i)->m_name;

        }
        m_pYC_Model = new QStandardItemModel;
        m_pYC_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYC_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YC")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();
                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYCPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYCPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYC_Model->appendRow(items);
                }
            }
        }
    }
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYMPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYMPropertyList->count();
        qDebug()<<"33333333333333";

        int i = 0;
        strDisplayList<<"工程值"<<"原始值";
        for (i = 0; i < pDevice_Type_Property->m_pYMPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYMPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYMPropertyList->at(i)->m_name;

        }
        m_pYM_Model = new QStandardItemModel;
        m_pYM_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYM_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YM")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();
                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYMPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYMPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYM_Model->appendRow(items);
                }
            }
        }
    }
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYKPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYKPropertyList->count();
        qDebug()<<"33333333333333";

        int i = 0;
        strDisplayList<<"工程值"<<"原始值";
        for (i = 0; i < pDevice_Type_Property->m_pYKPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYKPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYKPropertyList->at(i)->m_name;
        }
        m_pYK_Model = new QStandardItemModel;
        m_pYK_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYK_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YK")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();
                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYKPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYKPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYK_Model->appendRow(items);
                }
            }
        }
    }
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYSPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYSPropertyList->count();
        qDebug()<<"33333333333333";

        int i = 0;
        strDisplayList<<"工程值"<<"原始值";
        for (i = 0; i < pDevice_Type_Property->m_pYSPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYSPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYSPropertyList->at(i)->m_name;

        }
        m_pYS_Model = new QStandardItemModel;
        m_pYS_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYS_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YS")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();
                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYSPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYSPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYS_Model->appendRow(items);
                }
            }
        }
    }


}

void CChannelMessage::SetDeviceElement(const QDomElement &element_)
{
    m_nChannelMessage_Type = ChannelMessage_Type_Device;
    if (element_.elementsByTagName("Attribute").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"Attribute\").isEmpty()";
        Q_ASSERT(false);
        return;
    }
    if (element_.elementsByTagName("Type").isEmpty())
    {
        qDebug()<<"element_.elementsByTagName(\"Type\").isEmpty()";
        return;
    }
    QStringList strDisplayList;
    qDebug()<<"11111111111111";

    QString Device_Model = element_.elementsByTagName("Attribute").at(0).toElement().attribute("Device_Model");
    Device_Type_Property *pDevice_Type_Property = CDeviceModelMessage::m_DeviceTypeMessageMap.value(Device_Model);
    if (pDevice_Type_Property == NULL)
        return;
    QDomNodeList TypeNodeList = element_.elementsByTagName("Type");
    qDebug()<<"22222222222222"<<Device_Model<<pDevice_Type_Property->m_pYXPropertyList;
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYXPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYXPropertyList->count();
        qDebug()<<"33333333333333";

        int i = 0;
        strDisplayList<<"工程值"<<"原始值";
        for (i = 0; i < pDevice_Type_Property->m_pYXPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYXPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYXPropertyList->at(i)->m_name;

        }
        m_pYX_Model = new QStandardItemModel;
        m_pYX_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYX_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YX")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();
                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYXPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYXPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYX_Model->appendRow(items);
                }
            }
        }
    }
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYCPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYCPropertyList->count();
        qDebug()<<"33333333333333";

        int i = 0;
        strDisplayList<<"工程值"<<"原始值";
        for (i = 0; i < pDevice_Type_Property->m_pYCPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYCPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYCPropertyList->at(i)->m_name;

        }
        m_pYC_Model = new QStandardItemModel;
        m_pYC_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYC_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YC")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();
                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYCPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYCPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYC_Model->appendRow(items);
                }
            }
        }
    }
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYMPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYMPropertyList->count();
        qDebug()<<"33333333333333";

        int i = 0;
        strDisplayList<<"工程值"<<"原始值";
        for (i = 0; i < pDevice_Type_Property->m_pYMPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYMPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYMPropertyList->at(i)->m_name;

        }
        m_pYM_Model = new QStandardItemModel;
        m_pYM_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYM_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YM")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();
                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYMPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYMPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYM_Model->appendRow(items);
                }
            }
        }
    }
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYKPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYKPropertyList->count();
        qDebug()<<"33333333333333";

        int i = 0;
        strDisplayList<<"工程值"<<"原始值";
        for (i = 0; i < pDevice_Type_Property->m_pYKPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYKPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYKPropertyList->at(i)->m_name;
        }
        m_pYK_Model = new QStandardItemModel;
        m_pYK_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYK_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YK")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();
                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYKPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYKPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYK_Model->appendRow(items);
                }
            }
        }
    }
    strDisplayList.clear();
    if (pDevice_Type_Property->m_pYSPropertyList)
    {
        qDebug()<<pDevice_Type_Property->m_pYSPropertyList->count();
        qDebug()<<"33333333333333";

        int i = 0;
        strDisplayList<<"工程值"<<"原始值";
        for (i = 0; i < pDevice_Type_Property->m_pYSPropertyList->count(); ++i)
        {
            qDebug()<<"44444444444444444444"<<pDevice_Type_Property->m_pYSPropertyList->at(i)->m_name;
            strDisplayList<<pDevice_Type_Property->m_pYSPropertyList->at(i)->m_name;

        }
        m_pYS_Model = new QStandardItemModel;
        m_pYS_Model->setColumnCount(i+2);
        qDebug()<<strDisplayList;
        m_pYS_Model->setHorizontalHeaderLabels(strDisplayList);
        for (int j = 0; j < TypeNodeList.count(); ++j)
        {
            if (TypeNodeList.at(j).toElement().attribute("Name") == "YS")
            {
                QDomNodeList PointNodeList = TypeNodeList.at(j).toElement().childNodes();
                for (int k = 0; k < PointNodeList.count(); ++k)
                {
                    QDomElement PointElement = PointNodeList.at(k).toElement();
                    QList<QStandardItem *> items; items << new QStandardItem()<< new QStandardItem();
                    for (int attributeCount = 0; attributeCount < pDevice_Type_Property->m_pYSPropertyList->count(); ++attributeCount)
                    {
                        QStandardItem *item = new QStandardItem(PointElement.attribute(pDevice_Type_Property->m_pYSPropertyList->at(attributeCount)->m_var));
                        items<<item;
                    }
                    m_pYS_Model->appendRow(items);
                }
            }
        }
    }
}

QStandardItemModel *CChannelMessage::GetModel(ChannelMessage_TAG_TYPE nTagType_)
{
    QStandardItemModel *result = NULL;
    switch (nTagType_) {
    case ChannelMessage_TAG_TYPE_DI:
        result = m_pYX_Model;
        break;
    case ChannelMessage_TAG_TYPE_AI:
        result = m_pYC_Model;
        break;
    case ChannelMessage_TAG_TYPE_MEM:
        result = m_pYM_Model;
        break;
    case ChannelMessage_TAG_TYPE_DO:
        result = m_pYK_Model;
        break;
    case ChannelMessage_TAG_TYPE_AO:
        result = m_pYS_Model;
        break;
    case ChannelMessage_TAG_TYPE_PRO:
        result = m_pPro_Model;
    default:
        return NULL;
        break;
    }
    return result;
}
