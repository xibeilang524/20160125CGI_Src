#include "UI_COMCAN_Attribute.h"
#include "ui_UI_COM_Attribute.h"
#include "../CGI_SCADA_include/XmlParser/CXmlParser.h"

UI_COMCAN_Attribute::UI_COMCAN_Attribute(int nType_, QWidget *parent) :
    m_nType(nType_),
    QWidget(parent),
    ui(new Ui::UI_COM_Attribute)
{
    ui->setupUi(this);
    m_pParameterMap = new AttributeParameterMap;
    m_pComParMap = new AttributeParameterMap;
    m_pProtocolParMap = new AttributeParameterMap;
    m_pLeftItem = NULL;
    m_pRightItem = NULL;
    m_pLayout = new QGridLayout;
    this->setLayout(m_pLayout);

    m_pTopVariantEditor = new QtTreePropertyBrowser;
    m_pTopVariantEditor->setPropertiesWithoutValueMarked(true);
    m_pTopVariantManager = new QtVariantPropertyManager;
    QtVariantEditorFactory *TopvariantFactory = new QtVariantEditorFactory();
    connect(m_pTopVariantManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(slot_propertyChange(QtProperty*)));
    m_pTopVariantEditor->setFactoryForManager(m_pTopVariantManager, TopvariantFactory);//使属性可以编辑，否则属性不能在窗口改变

    m_pLeftVariantEditor = new QtTreePropertyBrowser;
    m_pLeftVariantEditor->setPropertiesWithoutValueMarked(true);
    m_pLeftVariantManager = new QtVariantPropertyManager;
    QtVariantEditorFactory *LeftvariantFactory = new QtVariantEditorFactory();
//    connect(m_pLeftVariantManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(LeftPropertyChange(QtProperty*)));
    m_pLeftVariantEditor->setFactoryForManager(m_pLeftVariantManager, LeftvariantFactory);//使属性可以编辑，否则属性不能在窗口改变

    m_pRightVariantEditor = new QtTreePropertyBrowser;
    m_pRightVariantEditor->setPropertiesWithoutValueMarked(true);
    m_pRightVariantManager = new QtVariantPropertyManager;
    QtVariantEditorFactory *RightvariantFactory = new QtVariantEditorFactory();
//    connect(m_pRightVariantManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(RightPropertyChange(QtProperty*)));
    m_pRightVariantEditor->setFactoryForManager(m_pRightVariantManager, RightvariantFactory);//使属性可以编辑，否则属性不能在窗口改变


    m_pLayout->addWidget(m_pTopVariantEditor,0,0,1,2);
    m_pLayout->addWidget(m_pLeftVariantEditor,1,0,1,1);
    m_pLayout->addWidget(m_pRightVariantEditor,1,1,1,1);

    SetFileName_New();
    SetConfigFileName("conf/CommunicationMode/请选择通讯方式.xml");
}

UI_COMCAN_Attribute::~UI_COMCAN_Attribute()
{
    qDebug()<<__func__<<__FILE__<<__LINE__<<"start"<<"m_nType"<<m_nType;
    delete ui;
    if (m_pParameterMap != NULL)
        delete m_pParameterMap;
    m_pParameterMap = NULL;

    if (m_pComParMap != NULL)
        delete m_pComParMap;
    m_pComParMap = NULL;
    if (m_pProtocolParMap != NULL)
        delete m_pProtocolParMap;
    m_pProtocolParMap = NULL;
    if (m_pLayout != NULL)
        m_pLayout->deleteLater();
//        delete m_pLayout;
    m_pLayout = NULL;
    if (m_pTopVariantEditor != NULL)
        m_pTopVariantEditor->deleteLater();
//        delete m_pTopVariantEditor;
    m_pTopVariantEditor = NULL;
    if (m_pTopVariantManager != NULL)
        m_pTopVariantManager->deleteLater();
//        delete m_pTopVariantManager;
    m_pTopVariantManager = NULL;
    if (m_pLeftVariantEditor != NULL)
        m_pLeftVariantEditor->deleteLater();
//        delete m_pLeftVariantEditor;
    m_pLeftVariantEditor = NULL;
    if (m_pLeftVariantManager != NULL)
        m_pLeftVariantManager->deleteLater();
//        delete m_pLeftVariantManager;
    m_pLeftVariantManager = NULL;
    if (m_pRightVariantEditor != NULL)
        m_pRightVariantEditor->deleteLater();
//        delete m_pRightVariantEditor;
    m_pRightVariantEditor = NULL;
    if (m_pRightVariantManager != NULL)
        m_pRightVariantManager->deleteLater();
//       delete m_pRightVariantManager;
    m_pRightVariantManager = NULL;
    qDebug()<<__func__<<__FILE__<<__LINE__<<"end"<<"m_nType"<<m_nType;
}

bool UI_COMCAN_Attribute::SaveAction(QDomDocument &doc, QDomElement &parentElem)
{
    QDomElement ChannelElem     = doc.createElement("Channel");
    ChannelElem.setAttribute("Name","");

    QDomElement TopElem         = doc.createElement("Top");
//    TopElem.setAttribute("Name","");
    switch (m_nType) {
    case CGI_SCADA_DLL_I::TreeItemType_ChanneM_COM_Attribute:
        TopElem.setAttribute("ComType","SerialPort");
        break;
    case CGI_SCADA_DLL_I::TreeItemType_ChanneM_CAN_Attribute:
        TopElem.setAttribute("ComType","CanPort");
        break;
    case CGI_SCADA_DLL_I::TreeItemType_ChanneM_NET_Link_Attribute:
        TopElem.setAttribute("ComType","NetPort");
        break;
    default:
        break;
    }
    QString strProtocolName;
    for (int i = 0; i < topItem->subProperties().count(); ++i)
    {
        if (m_pParameterMap->value(i)->m_strName == "Protocol")
        {
            strProtocolName = topItem->subProperties().at(i)->valueText();
        }
        if (m_pParameterMap->value(i)->m_strName == "ComMode")///< 只有NETlink才会有ComeMode节点
        {
            parentElem.setAttribute("ComMode",topItem->subProperties().at(i)->valueText());
        }else
        {
            TopElem.setAttribute(m_pParameterMap->value(i)->m_strName,topItem->subProperties().at(i)->valueText());
        }
    }
    QDomElement ProtocolElem    = doc.createElement("Protocol");
    ProtocolElem.setAttribute("Name",strProtocolName);
    for (int i = 0; i < m_pRightItem->subProperties().count(); ++i)
    {
        ProtocolElem.setAttribute(m_pProtocolParMap->value(i)->m_strName,m_pRightItem->subProperties().at(i)->valueText());
    }
    QDomElement MainPortElem    = doc.createElement("MainPort");
    switch (m_nType) {
    case CGI_SCADA_DLL_I::TreeItemType_ChanneM_COM_Attribute:
    case CGI_SCADA_DLL_I::TreeItemType_ChanneM_CAN_Attribute:
        for (int i = 0; i < m_pLeftItem->subProperties().count(); ++i)
        {
            MainPortElem.setAttribute(m_pComParMap->value(i)->m_strName,m_pLeftItem->subProperties().at(i)->valueText());
        }
        break;
    case CGI_SCADA_DLL_I::TreeItemType_ChanneM_NET_Link_Attribute:
    {
        int i = 0;
        for (i = 0; i < m_pLeftItem->subProperties().count(); ++i)
        {
            parentElem.setAttribute(m_pComParMap->value(i)->m_strName,m_pLeftItem->subProperties().at(i)->valueText());
        }
        if (i == 0)
        {
            parentElem.setAttribute("ComMode","");
        }
    }
        break;
    default:
        break;
    }

    parentElem.appendChild(ChannelElem);
    parentElem.appendChild(TopElem);
    parentElem.appendChild(ProtocolElem);
    parentElem.appendChild(MainPortElem);

    return true;
}

/*!
 \brief Com 和 Can通道打开文件

 \fn UI_COMCAN_Attribute::SetComCanChannelTopProtocolMainPortElem
 \param ChannelElem_
 \param TopElem_
 \param ProtocolElem_
 \param MainPortElem_
 \return bool
*/
bool UI_COMCAN_Attribute::SetComCanChannelTopProtocolMainPortElem(const QDomElement &ChannelElem_, const QDomElement &TopElem_, const QDomElement &ProtocolElem_, const QDomElement &MainPortElem_)
{
    QtVariantProperty *pItem;
    ///< 设置上方属性的属性值
    for (int i = 0; i < m_pParameterMap->count(); ++i)
    {
        pItem = (QtVariantProperty *)topItem->subProperties().at(i);
        qDebug()<<__func__<<__LINE__<<m_pParameterMap->value(i)->m_strDisplay<<m_pParameterMap->value(i)->m_StrEditType;
        if (m_pParameterMap->value(i)->m_StrEditType == "select")
        {
            qDebug()<<__func__<<__LINE__<<m_pParameterMap->value(i)->m_strDisplay<<m_pParameterMap->value(i)->m_strName;
            int num = m_pParameterMap->value(i)->m_strListEnumNames.key(TopElem_.attribute(m_pParameterMap->value(i)->m_strName));
            qDebug()<<__func__<<__LINE__<<m_pParameterMap->value(i)->m_strDisplay<<m_pParameterMap->value(i)->m_strName;
            pItem->setValue(num);
            continue;
        }
        if (m_pParameterMap->value(i)->m_StrEditType == "AutoDate")
        {
            QString strDateTime = TopElem_.attribute(m_pParameterMap->value(i)->m_strName);
            int nYear,nMonths,nDay,nHour,nMinute;
            sscanf(strDateTime.toLatin1().data(),"%d/%d/%d %d:%d",&nYear,&nMonths,&nDay,&nHour,&nMinute);
            QDate Date(nYear,nMonths,nDay);
            QTime Time(nHour,nMinute);
            QDateTime DateTime(Date,Time);
            pItem->setValue(DateTime);
            continue;
        }
        if (TopElem_.attribute(m_pParameterMap->value(i)->m_strName).isEmpty())
        {
            pItem->setValue(m_pParameterMap->value(i)->m_strInitValue);
        }else
        {
            pItem->setValue(TopElem_.attribute(m_pParameterMap->value(i)->m_strName));
        }
    }

    ///< 设置左侧属性的属性值
    for (int i = 0; i < m_pComParMap->count(); ++i)
    {
        pItem = (QtVariantProperty *)m_pLeftItem->subProperties().at(i);
        if (m_pComParMap->value(i)->m_StrEditType == "Select")
        {
            int num = m_pComParMap->value(i)->m_strListEnumNames.key(MainPortElem_.attribute(m_pComParMap->value(i)->m_strName));
            pItem->setValue(num);
            continue;
        }
        if (m_pComParMap->value(i)->m_StrEditType == "AutoDate")
        {
            QString strDateTime = MainPortElem_.attribute(m_pComParMap->value(i)->m_strName);
            int nYear,nMonths,nDay,nHour,nMinute;
            sscanf(strDateTime.toLatin1().data(),"%d/%d/%d %d:%d",&nYear,&nMonths,&nDay,&nHour,&nMinute);
            QDate Date(nYear,nMonths,nDay);
            QTime Time(nHour,nMinute);
            QDateTime DateTime(Date,Time);
            pItem->setValue(DateTime);
            continue;
        }
        if (MainPortElem_.attribute(m_pComParMap->value(i)->m_strName).isEmpty())
        {
            pItem->setValue(m_pComParMap->value(i)->m_strInitValue);
        }else
        {
            pItem->setValue(MainPortElem_.attribute(m_pComParMap->value(i)->m_strName));
        }
    }

    ///< 设置右侧属性的属性值
    for (int i = 0; i < m_pProtocolParMap->count(); ++i)
    {
        pItem = (QtVariantProperty *)m_pRightItem->subProperties().at(i);
        if (m_pProtocolParMap->value(i)->m_StrEditType == "select")
        {
            int num = m_pProtocolParMap->value(i)->m_strListEnumNames.key(ProtocolElem_.attribute(m_pProtocolParMap->value(i)->m_strName));
            pItem->setValue(num);
            continue;
        }
        if (m_pProtocolParMap->value(i)->m_StrEditType == "AutoDate")
        {
            QString strDateTime = ProtocolElem_.attribute(m_pProtocolParMap->value(i)->m_strName);
            int nYear,nMonths,nDay,nHour,nMinute;
            sscanf(strDateTime.toLatin1().data(),"%d/%d/%d %d:%d",&nYear,&nMonths,&nDay,&nHour,&nMinute);
            QDate Date(nYear,nMonths,nDay);
            QTime Time(nHour,nMinute);
            QDateTime DateTime(Date,Time);
            pItem->setValue(DateTime);
            continue;
        }
        if (ProtocolElem_.attribute(m_pProtocolParMap->value(i)->m_strName).isEmpty())
        {
            pItem->setValue(m_pProtocolParMap->value(i)->m_strInitValue);
        }else
        {
            pItem->setValue(ProtocolElem_.attribute(m_pProtocolParMap->value(i)->m_strName));
        }
    }
    return true;
}

/*!
 \brief Net Link 打开文件使用的函数

 \fn UI_COMCAN_Attribute::SetNetLinkElem
 \param LinkElem_
 \return bool
*/
bool UI_COMCAN_Attribute::SetNetLinkElem(const QDomElement &LinkElem_)
{
    QtVariantProperty *pItem;
    if (LinkElem_.elementsByTagName("Top").isEmpty())
    {

    }else
    {
        ///< 设置上方属性的属性值
        QDomElement TopElem = LinkElem_.elementsByTagName("Top").at(0).toElement();
        for (int i = 0; i < m_pParameterMap->count(); ++i)
        {
            pItem = (QtVariantProperty *)topItem->subProperties().at(i);
            qDebug()<<__func__<<__LINE__
                   <<m_pParameterMap->value(i)->m_strName
                   <<m_pParameterMap->value(i)->m_strDisplay
                  <<m_pParameterMap->value(i)->m_StrEditType;
            if (m_pParameterMap->value(i)->m_strName == "ComMode")
            {
                if (m_pParameterMap->value(i)->m_StrEditType == "select")
                {
                    qDebug()<<__func__<<__LINE__<<m_pParameterMap->value(i)->m_strDisplay<<m_pParameterMap->value(i)->m_strName;
                    int num = m_pParameterMap->value(i)->m_strListEnumNames.key(LinkElem_.attribute(m_pParameterMap->value(i)->m_strName));
                    qDebug()<<__func__<<__LINE__<<m_pParameterMap->value(i)->m_strDisplay<<m_pParameterMap->value(i)->m_strName;
                    pItem->setValue(num);
                    continue;
                }
                if (m_pParameterMap->value(i)->m_StrEditType == "AutoDate")
                {
                    QString strDateTime = LinkElem_.attribute(m_pParameterMap->value(i)->m_strName);
                    int nYear,nMonths,nDay,nHour,nMinute;
                    sscanf(strDateTime.toLatin1().data(),"%d/%d/%d %d:%d",&nYear,&nMonths,&nDay,&nHour,&nMinute);
                    QDate Date(nYear,nMonths,nDay);
                    QTime Time(nHour,nMinute);
                    QDateTime DateTime(Date,Time);
                    pItem->setValue(DateTime);
                    continue;
                }
                if (LinkElem_.attribute(m_pParameterMap->value(i)->m_strName).isEmpty())
                {
                    pItem->setValue(m_pParameterMap->value(i)->m_strInitValue);
                }else
                {
                    pItem->setValue(LinkElem_.attribute(m_pParameterMap->value(i)->m_strName));
                }
            }else
            {
                if (m_pParameterMap->value(i)->m_StrEditType == "select")
                {
                    qDebug()<<__func__<<__LINE__<<m_pParameterMap->value(i)->m_strDisplay<<m_pParameterMap->value(i)->m_strName;
                    int num = m_pParameterMap->value(i)->m_strListEnumNames.key(TopElem.attribute(m_pParameterMap->value(i)->m_strName));
                    qDebug()<<__func__<<__LINE__<<m_pParameterMap->value(i)->m_strDisplay<<m_pParameterMap->value(i)->m_strName;
                    pItem->setValue(num);
                    continue;
                }
                if (m_pParameterMap->value(i)->m_StrEditType == "AutoDate")
                {
                    QString strDateTime = TopElem.attribute(m_pParameterMap->value(i)->m_strName);
                    int nYear,nMonths,nDay,nHour,nMinute;
                    sscanf(strDateTime.toLatin1().data(),"%d/%d/%d %d:%d",&nYear,&nMonths,&nDay,&nHour,&nMinute);
                    QDate Date(nYear,nMonths,nDay);
                    QTime Time(nHour,nMinute);
                    QDateTime DateTime(Date,Time);
                    pItem->setValue(DateTime);
                    continue;
                }
                if (TopElem.attribute(m_pParameterMap->value(i)->m_strName).isEmpty())
                {
                    pItem->setValue(m_pParameterMap->value(i)->m_strInitValue);
                }else
                {
                    pItem->setValue(TopElem.attribute(m_pParameterMap->value(i)->m_strName));
                }
            }

        }
    }

    ///< 设置左侧属性的属性值
    for (int i = 0; i < m_pComParMap->count(); ++i)
    {
        pItem = (QtVariantProperty *)m_pLeftItem->subProperties().at(i);
        if (m_pComParMap->value(i)->m_StrEditType == "Select")
        {
            int num = m_pComParMap->value(i)->m_strListEnumNames.key(LinkElem_.attribute(m_pComParMap->value(i)->m_strName));
            pItem->setValue(num);
            continue;
        }
        if (m_pComParMap->value(i)->m_StrEditType == "AutoDate")
        {
            QString strDateTime = LinkElem_.attribute(m_pComParMap->value(i)->m_strName);
            int nYear,nMonths,nDay,nHour,nMinute;
            sscanf(strDateTime.toLatin1().data(),"%d/%d/%d %d:%d",&nYear,&nMonths,&nDay,&nHour,&nMinute);
            QDate Date(nYear,nMonths,nDay);
            QTime Time(nHour,nMinute);
            QDateTime DateTime(Date,Time);
            pItem->setValue(DateTime);
            continue;
        }
        if (LinkElem_.attribute(m_pComParMap->value(i)->m_strName).isEmpty())
        {
            pItem->setValue(m_pComParMap->value(i)->m_strInitValue);
        }else
        {
            pItem->setValue(LinkElem_.attribute(m_pComParMap->value(i)->m_strName));
        }
    }


    if (LinkElem_.elementsByTagName("Protocol").isEmpty())
    {

    }else
    {
        QDomElement ProtocolElem = LinkElem_.elementsByTagName("Protocol").at(0).toElement();
        ///< 设置右侧属性的属性值
        for (int i = 0; i < m_pProtocolParMap->count(); ++i)
        {
            pItem = (QtVariantProperty *)m_pRightItem->subProperties().at(i);
            if (m_pProtocolParMap->value(i)->m_StrEditType == "select")
            {
                int num = m_pProtocolParMap->value(i)->m_strListEnumNames.key(ProtocolElem.attribute(m_pProtocolParMap->value(i)->m_strName));
                pItem->setValue(num);
                continue;
            }
            if (m_pProtocolParMap->value(i)->m_StrEditType == "AutoDate")
            {
                QString strDateTime = ProtocolElem.attribute(m_pProtocolParMap->value(i)->m_strName);
                int nYear,nMonths,nDay,nHour,nMinute;
                sscanf(strDateTime.toLatin1().data(),"%d/%d/%d %d:%d",&nYear,&nMonths,&nDay,&nHour,&nMinute);
                QDate Date(nYear,nMonths,nDay);
                QTime Time(nHour,nMinute);
                QDateTime DateTime(Date,Time);
                pItem->setValue(DateTime);
                continue;
            }
            if (ProtocolElem.attribute(m_pProtocolParMap->value(i)->m_strName).isEmpty())
            {
                pItem->setValue(m_pProtocolParMap->value(i)->m_strInitValue);
            }else
            {
                pItem->setValue(ProtocolElem.attribute(m_pProtocolParMap->value(i)->m_strName));
            }
        }
    }



    return true;
}

void UI_COMCAN_Attribute::SetFileName_New()
{
    qDebug()<<__func__<<__LINE__;
    QString strFileName_(Attribute_ProtocolFileName_New);

    QtVariantProperty *item;

    QDomDocument doc;
    CXmlParser::readFile(strFileName_,doc);
    QDomElement domElement;
    domElement = doc.documentElement();
    if ("Root" != domElement.tagName())
    {
        qDebug()<<"Error: Not a project file";
        return;
    }
    QString PortName("通道");
    m_strTabName = PortName;

    topItem = m_pTopVariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                     PortName);
    m_pTopVariantEditor->addProperty(topItem);

    int nRowNumber = 0;

    {
        QDomNodeList ParameterNodes = domElement.elementsByTagName("Parameter");
        for(nRowNumber = 0;nRowNumber < ParameterNodes.count();nRowNumber++)
        {
            QDomElement ParameterElement = ParameterNodes.at(nRowNumber).toElement();
            QString _Name = ParameterElement.attribute("Display");
            QString _EditType = ParameterElement.attribute("EditType");
            QString _InitValue = ParameterElement.attribute("InitValue");

            AttributeParameter *pParameter = new AttributeParameter;
            pParameter->m_strName = ParameterElement.attribute("Name");
            pParameter->m_strDisplay = ParameterElement.attribute("Display");
            pParameter->m_StrEditType = ParameterElement.attribute("EditType");
            pParameter->m_strInitValue = ParameterElement.attribute("InitValue");
            m_pParameterMap->insert(nRowNumber,pParameter);
            if(_EditType == "Edit")
            {
                item = m_pTopVariantManager->addProperty(QVariant::String, _Name);
                item->setValue(_InitValue);
                topItem->addSubProperty(item);
                continue;
            }else if(_EditType == "AutoAuthor")
            {
                item = m_pTopVariantManager->addProperty(QVariant::String, _Name);
                item->setAttribute(QLatin1String("readOnly"), true);
                item->setValue(_InitValue);
                topItem->addSubProperty(item);
                continue;
            }else if(_EditType == "AutoDate")
            {
                item = m_pTopVariantManager->addProperty(QVariant::DateTime, _Name);
                item->setValue(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm"));
                topItem->addSubProperty(item);
                continue;
            }else if (_EditType == "BoolEdit")
            {
                item = m_pTopVariantManager->addProperty(QVariant::Bool, _Name);
                if(_InitValue == "1")
                    item->setValue(true);
                else if(_InitValue == "0")
                    item->setValue(false);
                topItem->addSubProperty(item);
                continue;
            }else if(_EditType == "select")
            {
                QDomNodeList SelectNodes = ParameterElement.childNodes();
                QStringList _SelectList;
                int nInitValue = 0;
                for(int num = 0;num < SelectNodes.count();num++)
                {
                    QDomElement SelectElement = SelectNodes.at(num).toElement();
                    if(SelectElement.tagName() == "select")
                    {
                        pParameter->m_strListEnumNames.insert(num,SelectElement.attribute("Display"));
                        _SelectList.append(SelectElement.attribute("Display"));
                        if (ParameterElement.attribute("InitValue") == SelectElement.attribute("Value"))
                        {
                            nInitValue = num;
                            pParameter->m_strInitValue = SelectElement.attribute("Display");
                        }
                    }
                }
                item = m_pTopVariantManager->addProperty(QtVariantPropertyManager::enumTypeId(),_Name);//combox控件
                item->setAttribute(QLatin1String("enumNames"), _SelectList);
                item->setValue(nInitValue);
                topItem->addSubProperty(item);
                continue;
            }else
            {
                item = m_pTopVariantManager->addProperty(QVariant::String, _Name);
                item->setValue(_InitValue);
                topItem->addSubProperty(item);
                continue;
            }
        }
    }
    qDebug()<<__func__<<__LINE__<<m_nType<<"TreeItemType_ChanneM_NET_Link_Attribute";

    if (m_nType == CGI_SCADA_DLL_I::TreeItemType_ChanneM_NET_Link_Attribute)
    {
        qDebug()<<__func__<<__LINE__<<"TreeItemType_ChanneM_NET_Link_Attribute";

        AttributeParameter *pParameter = new AttributeParameter;
        pParameter->m_strName = QString("ComMode");
        pParameter->m_strDisplay = QString("通讯方式");
        pParameter->m_StrEditType = QString("select");
        pParameter->m_strInitValue = QString("");
        qDebug()<<__func__<<__LINE__;
        m_pParameterMap->insert(nRowNumber++,pParameter);

        QStringList _SelectList = QStringList()<<"请选择通讯方式"<<"UDP"<<"TCPClient"<<"TCPServer";
        for (int i = 0; i < _SelectList.count(); ++i)
        {
            pParameter->m_strListEnumNames.insert(i,_SelectList.at(i));
        }

        item = m_pTopVariantManager->addProperty(QtVariantPropertyManager::enumTypeId(),pParameter->m_strDisplay);//combox控件
        item->setValue(0);
        item->setAttribute(QLatin1String("enumNames"), _SelectList);
        topItem->addSubProperty(item);
    }

    {///< 通道参数配置

        QDomNodeList ProtocolNodes = domElement.elementsByTagName("Protocol");
        qDebug()<<__func__<<__LINE__;

        AttributeParameter *pParameter = new AttributeParameter;
        pParameter->m_strName = QString("Protocol");
        pParameter->m_strDisplay = QString("驱动");
        pParameter->m_StrEditType = QString("select");
        pParameter->m_strInitValue = QString("");
        qDebug()<<__func__<<__LINE__;
        m_pParameterMap->insert(nRowNumber++,pParameter);

        qDebug()<<__func__<<__LINE__;
        QStringList _SelectList;
        int nInitValue = -1;
        qDebug()<<__func__<<__LINE__;
        for(int num = 0;num < ProtocolNodes.count();num++)
        {
            qDebug()<<__func__<<__LINE__;
            QDomElement ProtocolElement = ProtocolNodes.at(num).toElement();
            QString strText = ProtocolElement.attribute("text");
            pParameter->m_strListEnumNames.insert(num,strText);
            _SelectList.append(strText);
            nInitValue = 0;
        }

        qDebug()<<__func__<<__LINE__;
        item = m_pTopVariantManager->addProperty(QtVariantPropertyManager::enumTypeId(),pParameter->m_strDisplay);//combox控件
        if (nInitValue == 0)
        {
            item->setValue(nInitValue);
        }
        qDebug()<<__func__<<__LINE__;
        item->setAttribute(QLatin1String("enumNames"), _SelectList);
        topItem->addSubProperty(item);
        qDebug()<<__func__<<__LINE__;
    }

}

/*!
 * \brief  功能概述 设置通讯参数窗口
 * \param  参数描述 strFileName_是保存有属性名、属性显示名、属性编辑类型、属性初始值的文件名
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/1/19
 */
void UI_COMCAN_Attribute::SetConfigFileName(QString strFileName_)
{
//    QString strFileName_(COM_Attribute_ConfigFileName);
    QString strFileName;
    switch (m_nType) {
    case CGI_SCADA_DLL_I::TreeItemType_ChanneM_COM_Attribute:
        strFileName = QString(COM_Attribute_ConfigFileName);
        break;
    case CGI_SCADA_DLL_I::TreeItemType_ChanneM_CAN_Attribute:
        strFileName = QString(CAN_Attribute_ConfigFileName);
        break;
    case CGI_SCADA_DLL_I::TreeItemType_ChanneM_NET_Link_Attribute:
        strFileName = strFileName_;
        break;
    default:
        break;
    }
    if (m_pLeftItem)
        delete m_pLeftItem;
    m_pLeftItem = NULL;
    qDebug()<<__func__<<__LINE__<<strFileName<<m_nType;

    QtVariantProperty *item;

    QDomDocument doc;
    CXmlParser::readFile(strFileName,doc);
    QDomElement domElement;
    domElement = doc.documentElement();
    if ("Root" != domElement.tagName())
    {
        qDebug()<<__func__<<__LINE__<<"Error: Not a project file";
        return;
    }
    QDomElement PortElement = domElement.childNodes().at(0).toElement();
    QString PortName = PortElement.attribute("Display");

    m_pLeftItem = m_pLeftVariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                     PortName);
    m_pLeftVariantEditor->addProperty(m_pLeftItem);
    QDomNodeList ParameterNodes = PortElement.childNodes();
    for(int i = 0;i < ParameterNodes.count();i++)
    {
        QDomElement ParameterElement = ParameterNodes.at(i).toElement();
        QString _Name = ParameterElement.attribute("Display");
        QString _EditType = ParameterElement.attribute("EditType");
        QString _InitValue = ParameterElement.attribute("InitValue");

        AttributeParameter *pParameter = new AttributeParameter;
        pParameter->m_strName = ParameterElement.attribute("Name");
        pParameter->m_strDisplay = ParameterElement.attribute("Display");
        pParameter->m_StrEditType = ParameterElement.attribute("EditType");
        pParameter->m_strInitValue = ParameterElement.attribute("InitValue");
        m_pComParMap->insert(i,pParameter);
        if(_EditType == "Edit")
        {
            item = m_pLeftVariantManager->addProperty(QVariant::String, _Name);
            item->setValue(_InitValue);
            m_pLeftItem->addSubProperty(item);
            continue;
        }else if(_EditType == "AutoAuthor")
        {
            item = m_pLeftVariantManager->addProperty(QVariant::String, _Name);
            item->setAttribute(QLatin1String("readOnly"), true);
            item->setValue(_InitValue);
            m_pLeftItem->addSubProperty(item);
            continue;
        }else if(_EditType == "AutoDate")
        {
            item = m_pLeftVariantManager->addProperty(QVariant::DateTime, _Name);
            item->setValue(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm"));
            m_pLeftItem->addSubProperty(item);
            continue;
        }else if (_EditType == "BoolEdit")
        {
            item = m_pLeftVariantManager->addProperty(QVariant::Bool, _Name);
            if(_InitValue == "1")
                item->setValue(true);
            else if(_InitValue == "0")
                item->setValue(false);
            m_pLeftItem->addSubProperty(item);
            continue;
        }else if(_EditType == "Select")
        {
            QDomNodeList SelectNodes = ParameterElement.childNodes();
            QStringList _SelectList;
            int nInitValue = 0;
            for(int num = 0;num < SelectNodes.count();num++)
            {
                QDomElement SelectElement = SelectNodes.at(num).toElement();
                if(SelectElement.tagName() == "Select")
                {
                    pParameter->m_strListEnumNames.insert(num,SelectElement.attribute("Display"));
                    _SelectList.append(SelectElement.attribute("Display"));
                    if (ParameterElement.attribute("InitValue") == SelectElement.attribute("Value"))
                    {
                        nInitValue = num;
                        pParameter->m_strInitValue = SelectElement.attribute("Display");
                    }
                }
            }
            item = m_pLeftVariantManager->addProperty(QtVariantPropertyManager::enumTypeId(),_Name);//combox控件
            item->setAttribute(QLatin1String("enumNames"), _SelectList);
            item->setValue(nInitValue);
            m_pLeftItem->addSubProperty(item);
            continue;
        }else
        {
            item = m_pLeftVariantManager->addProperty(QVariant::String, _Name);
            item->setValue(_InitValue);
            m_pLeftItem->addSubProperty(item);
            continue;
        }
    }
}


void UI_COMCAN_Attribute::SetProtocolName(QString strProtocolName_)
{
    qDebug()<<__func__<<__LINE__<<__FILE__<<strProtocolName_;
    if (m_pRightItem)
        delete m_pRightItem;
    m_pRightItem = NULL;
    qDebug()<<__func__<<__LINE__<<__FILE__<<strProtocolName_;

    QString strFileName_(Attribute_ProtocolFileName_New);
    m_strProtocolName = strFileName_;
    m_strProtocolName = m_strProtocolName.remove(tr(".xml"));
    QtVariantProperty *item;

    QDomDocument doc;
    CXmlParser::readFile(strFileName_,doc);
    QDomElement domElement;
    domElement = doc.documentElement();
    if ("Root" != domElement.tagName())
    {
        qDebug()<<"Error: Not a project file";
        return;
    }

    QString PortName(strProtocolName_);

    m_pRightItem = m_pRightVariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                     PortName);
    m_pRightVariantEditor->addProperty(m_pRightItem);

    QDomNodeList ProtocolNodes = domElement.elementsByTagName("Protocol");
    for(int nProtocolCount = 0;nProtocolCount < ProtocolNodes.count();nProtocolCount++)
    {
        if (ProtocolNodes.at(nProtocolCount).toElement().attribute("text") == strProtocolName_)
        {
            QDomNodeList ParaNodeList = ProtocolNodes.at(nProtocolCount).toElement().elementsByTagName("Para");

            for (int nParaCount = 0; nParaCount < ParaNodeList.count(); ++nParaCount)
            {
                qDebug()<<__func__<<ParaNodeList.at(nParaCount).toElement().attribute("var");
                QDomElement ParaElement = ParaNodeList.at(nParaCount).toElement();

                AttributeParameter *pParameter = new AttributeParameter;
                pParameter->m_strName = ParaElement.attribute("var");
                pParameter->m_strDisplay = ParaElement.attribute("name");
                pParameter->m_StrEditType = ParaElement.attribute("type");
                pParameter->m_strInitValue = ParaElement.attribute("value");
                m_pProtocolParMap->insert(nParaCount,pParameter);
                if(pParameter->m_StrEditType == "text")
                {
                    item = m_pRightVariantManager->addProperty(QVariant::String, pParameter->m_strDisplay);
                    item->setValue(pParameter->m_strInitValue);
                    m_pRightItem->addSubProperty(item);
                    continue;
                }else if(pParameter->m_StrEditType == "AutoAuthor")
                {
                    item = m_pRightVariantManager->addProperty(QVariant::String, pParameter->m_strDisplay);
                    item->setAttribute(QLatin1String("readOnly"), true);
                    item->setValue(pParameter->m_strInitValue);
                    m_pRightItem->addSubProperty(item);
                    continue;
                }else if(pParameter->m_StrEditType == "AutoDate")
                {
                    item = m_pRightVariantManager->addProperty(QVariant::DateTime, pParameter->m_strDisplay);
                    item->setValue(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm"));
                    m_pRightItem->addSubProperty(item);
                    continue;
                }else if (pParameter->m_StrEditType == "BoolEdit")
                {
                    item = m_pRightVariantManager->addProperty(QVariant::Bool, pParameter->m_strDisplay);
                    if(pParameter->m_strInitValue == "1")
                        item->setValue(true);
                    else if(pParameter->m_strInitValue == "0")
                        item->setValue(false);
                    m_pRightItem->addSubProperty(item);
                    continue;
                }else if(pParameter->m_StrEditType == "select")
                {
                    QDomNodeList SelectNodes = ParaElement.childNodes();
                    QStringList _SelectList;
                    int nInitValue = 0;
                    for(int num = 0;num < SelectNodes.count();num++)
                    {
                        QDomElement SelectElement = SelectNodes.at(num).toElement();
                        if(SelectElement.tagName() == "Option")
                        {
                            pParameter->m_strListEnumNames.insert(num,SelectElement.attribute("name"));
                            _SelectList.append(SelectElement.attribute("name"));
                            if (ParaElement.attribute("value") == SelectElement.attribute("value"))
                            {
                                nInitValue = num;
                                pParameter->m_strInitValue = SelectElement.attribute("name");
                            }
                        }
                    }
                    item = m_pRightVariantManager->addProperty(QtVariantPropertyManager::enumTypeId(),pParameter->m_strDisplay);//combox控件
                    item->setAttribute(QLatin1String("enumNames"), _SelectList);
        //            qDebug()<<nInitValue<<_InitValue<<_Name;
                    item->setValue(nInitValue);
                    m_pRightItem->addSubProperty(item);
                    continue;
                }else
                {
                    item = m_pRightVariantManager->addProperty(QVariant::String, pParameter->m_strDisplay);
                    item->setValue(pParameter->m_strInitValue);
                    m_pRightItem->addSubProperty(item);
                    continue;
                }
            }

        }
    }
}
void UI_COMCAN_Attribute::slot_propertyChange(QtProperty *property)
{
    qDebug()<<__func__<<__FILE__<<__LINE__<<property->valueText();
    for (int i = 0; i < m_pParameterMap->count(); ++i)
    {
        if (m_pParameterMap->value(i)->m_strDisplay == property->propertyName())
        {
            if (m_pParameterMap->value(i)->m_strName == "Protocol")///协议类型
            {
                if (property->valueText().isEmpty())
                    continue;
                qDebug()<<__func__<<__LINE__<<__FILE__<<property->valueText();
                QString strProtocolName = property->valueText();
                emit signal_ChangeProtocolName(strProtocolName);
                SetProtocolName(strProtocolName);
                if (strProtocolName.contains("Deliver"))
                {
                    emit signal_ProtocolTypeChange(ProtocolType_Transform);
                }else
                {
                    emit signal_ProtocolTypeChange(ProtocolType_Collect);
                }
            }else if (m_pParameterMap->value(i)->m_strName == "ComMode")
            {
                if (property->valueText().isEmpty())
                    continue;
                qDebug()<<__func__<<__LINE__<<__FILE__<<property->valueText();
                QString strProtocolName = property->valueText();
//                SetProtocolName(strProtocolName);
                strProtocolName = QString("conf/CommunicationMode/%1.xml").arg(strProtocolName);
                SetConfigFileName(strProtocolName);
            }
        }
    }
}
