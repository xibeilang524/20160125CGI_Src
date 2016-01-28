#include "UI_DeviceAttribute.h"
#include "ui_UI_DeviceAttribute.h"

UI_DeviceAttribute::UI_DeviceAttribute(QList<Device_Att_Parameter> deviceAttParameterList_, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UI_DeviceAttribute)
{
    ui->setupUi(this);
    m_DeviceAttParameterList = deviceAttParameterList_;

    m_pLayout = new QGridLayout(this);
    this->setLayout(m_pLayout);

    m_pTopVariantEditor = new QtTreePropertyBrowser(this);
    m_pTopVariantEditor->setPropertiesWithoutValueMarked(true);
    m_pTopVariantManager = new QtVariantPropertyManager(this);
    QtVariantEditorFactory *TopvariantFactory = new QtVariantEditorFactory();
//    connect(m_pTopVariantManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(propertyChange(QtProperty*)));
    m_pTopVariantEditor->setFactoryForManager(m_pTopVariantManager, TopvariantFactory);//使属性可以编辑，否则属性不能在窗口改变
    m_pLayout->addWidget(m_pTopVariantEditor,0,0,1,2);
    SetParameterList();

}

UI_DeviceAttribute::~UI_DeviceAttribute()
{
    delete ui;
}

/*!
 \brief 保存设备属性节点中的属性

 \fn UI_DeviceAttribute::SaveAction
 \param doc
 \param parentElem
 \return bool
*/
bool UI_DeviceAttribute::SaveAction(QDomDocument &/*doc*/, QDomElement &parentElem)
{
    qDebug()<<__func__<<__FILE__<<__LINE__<<"topItem->subProperties().count()"<<topItem->subProperties().count();
    for (int i = 0; i < topItem->subProperties().count(); ++i)
    {
        parentElem.setAttribute(m_DeviceAttParameterList.at(i).m_strvar,topItem->subProperties().at(i)->valueText());
    }
    return true;
}
void UI_DeviceAttribute::SetParameterList()
{
    QtVariantProperty *item;
    topItem = m_pTopVariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                     "属性");
    m_pTopVariantEditor->addProperty(topItem);
    qDebug()<<__func__<<__LINE__;
    for(int i = 0;i < m_DeviceAttParameterList.count();i++)
    {
        Device_Att_Parameter *pDeviceAttParameter = &(m_DeviceAttParameterList[i]);
        QString _Name       = pDeviceAttParameter->m_strName;
        QString _EditType   = pDeviceAttParameter->m_strType;
        QString _InitValue  = pDeviceAttParameter->m_strInitValue;
        qDebug()<<__func__<<__LINE__<<m_DeviceAttParameterList[i].m_strName;
        qDebug()<<__func__<<__LINE__<<pDeviceAttParameter->m_strName;
        if(_EditType == "text")
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
            item = m_pTopVariantManager->addProperty(QtVariantPropertyManager::enumTypeId(),_Name);//combox控件
            QStringList _SelectList = pDeviceAttParameter->m_strEnumNamesList;
            qDebug()<<pDeviceAttParameter->m_strEnumNamesList;
            int nInitValue = pDeviceAttParameter->m_strInitValue.toInt();

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

bool UI_DeviceAttribute::SetDeviceAttributeElem(const QDomElement &AttributeElem_)
{
    QtVariantProperty *pItem;
    for (int i = 0; i < m_DeviceAttParameterList.count(); ++i)
    {
        qDebug()<<__func__<<__LINE__<<m_DeviceAttParameterList.at(i).m_strType
               <<m_DeviceAttParameterList.at(i).m_strName
              <<m_DeviceAttParameterList.at(i).m_strvar
                  <<m_DeviceAttParameterList.at(i).m_strInitValue
                 <<m_DeviceAttParameterList.at(i).m_strEnumNamesList;
        pItem = (QtVariantProperty *)topItem->subProperties().at(i);
        if (m_DeviceAttParameterList.at(i).m_strType == "select")
        {
            int num = m_DeviceAttParameterList.at(i).m_strEnumNamesList.indexOf(AttributeElem_.attribute(m_DeviceAttParameterList.at(i).m_strvar));
            qDebug()<<__func__<<__LINE__<<num;
            pItem->setValue(num);
            continue;
        }
        if (m_DeviceAttParameterList.at(i).m_strType == "AutoDate")
        {
            QString strDateTime = AttributeElem_.attribute(m_DeviceAttParameterList.at(i).m_strvar);
            int nYear,nMonths,nDay,nHour,nMinute;
            sscanf(strDateTime.toLatin1().data(),"%d/%d/%d %d:%d",&nYear,&nMonths,&nDay,&nHour,&nMinute);
            QDate Date(nYear,nMonths,nDay);
            QTime Time(nHour,nMinute);
            QDateTime DateTime(Date,Time);
            pItem->setValue(DateTime);
            continue;
        }
        if (AttributeElem_.attribute(m_DeviceAttParameterList.at(i).m_strvar).isEmpty())
        {
            pItem->setValue(m_DeviceAttParameterList.at(i).m_strInitValue);
        }else
        {
            pItem->setValue(AttributeElem_.attribute(m_DeviceAttParameterList.at(i).m_strvar));
        }
        qDebug()<<"for (int i = 0; i < m_pParameterMap->count(); ++i) 2";
    }
    return true;
}
