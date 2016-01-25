#ifndef CCHANNELMESSAGE_H
#define CCHANNELMESSAGE_H

#include <QObject>
#include <QDomElement>
#include <QString>
#include <QStandardItemModel>
#include <QDebug>
#include "EnumClass.h"
#include "CDeviceModelMessage.h"
#include "CProtocolFileMessage.h"

class CChannelMessage : public QObject
{
    Q_OBJECT
public:
    explicit CChannelMessage(const QDomElement &element_,QObject *parent = 0);
    QStandardItemModel *GetModel(ChannelMessage_TAG_TYPE nTagType_);

signals:

public slots:
private:
    void SetElement(const QDomElement &element_);
    void SetRootElement(const QDomElement &element_);
    void SetLinkElement(const QDomElement &element_);
    void SetDeviceElement(const QDomElement &element_);
    QList<Property *> m_Link_Property;      ///< Link节点的属性
    QList<Property *> m_Channel_Property;   ///< Channel节点的属性
    QList<Property *> m_Top_Property;       ///< Top节点的属性
    QList<Property *> m_Protocol_Property;  ///< Protocol节点的属性
    QList<Property *> m_MainPort_Property;  ///< MainPort节点的属性
    QList<Property *> m_Attribute_property;///< Attribute节点的属性

    QList<Property *> m_YX_property;
    QList<Property *> m_YC_property;
    QList<Property *> m_YM_property;
    QList<Property *> m_YK_property;
    QList<Property *> m_YS_property;

    QStandardItemModel *m_pPro_Model;/// 属性
    QStandardItemModel *m_pYX_Model;
    QStandardItemModel *m_pYC_Model;
    QStandardItemModel *m_pYM_Model;
    QStandardItemModel *m_pYK_Model;
    QStandardItemModel *m_pYS_Model;

    ChannelMessage_Type m_nChannelMessage_Type;
};

#endif // CCHANNELMESSAGE_H
