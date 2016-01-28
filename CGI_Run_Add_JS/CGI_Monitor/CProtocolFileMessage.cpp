#include "CProtocolFileMessage.h"
#include <QDomNodeList>
#include <QDomElement>

QMap<QString, PropertyList*> CProtocolFileMessage::m_ProtocolMessageMap;

CProtocolFileMessage::CProtocolFileMessage(QObject *parent) :
    QObject(parent)
{
}

void CProtocolFileMessage::InitProtocolFileMessage(QString strFileName_)
{
    strFileName_.append("/Protocol.xml");
    qDebug()<<__FILE__<<__func__<<strFileName_;
    QDomDocument doc;
    if (CXmlParser::readFile(strFileName_,doc))
    {
        QDomNodeList ProtocolNodeList = doc.elementsByTagName("Protocol");
        for (int i = 0; i < ProtocolNodeList.count(); ++i)
        {
            QDomElement ProtocolElement = ProtocolNodeList.at(i).toElement();
            QDomNodeList ParaNodeList = ProtocolElement.elementsByTagName("Para");
            PropertyList *pPropertyList = new PropertyList;
            Property *pProperty = NULL;
            for (int j = 0; j < ParaNodeList.count(); ++j)
            {
                QDomElement ParaElement = ParaNodeList.at(j).toElement();
                pProperty = new Property;
                pProperty->m_name = ParaElement.attribute("name");
                pProperty->m_var  = ParaElement.attribute("var");
                pPropertyList->append(pProperty);
            }

            if (!m_ProtocolMessageMap.contains(ProtocolElement.attribute("value")))
            {
                qDebug()<<__FILE__<<__func__<<ProtocolElement.attribute("value")<<"succeed";
                m_ProtocolMessageMap.insert(ProtocolElement.attribute("value"),pPropertyList);
            }else
            {
                qDebug()<<__FILE__<<__func__<<ProtocolElement.attribute("value")<<"fail";
            }
        }
    }
}
