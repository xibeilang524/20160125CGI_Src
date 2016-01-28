#include "CJavaScript_Protocol.h"
#include <QDebug>
#include "../PortInclude/CTagI.h"
#include <QScriptEngine>
/*!
 \brief 库文件导出接口函数

 \fn CreateDriver
 \param parent parent是QObject对象指针
 \return CProtocolI 返回创建的驱动指针
*/
CJavaScript_ProtocolI * CreateDriver_JavaScript(QObject *parent)
{
    printf("aaaaaaaaaaaaaaCreateDriver_JavaScript");
//    qDebug()<"aaaaaaaaaaaaaaCreateDriver_JavaScript";
    CJavaScript_Protocol *testProtocol = new CJavaScript_Protocol(parent);
    return  testProtocol;
}
CJavaScript_Protocol::CJavaScript_Protocol(QObject *parent):
    CJavaScript_ProtocolI(parent)
{
    m_pRTDB = NULL;
    connect(this,SIGNAL(signal_TagChange(CPointI*,QVariant)),this,SLOT(slot_TagChange(CPointI*,QVariant)));
}

bool CJavaScript_Protocol::OnCreateChannel(CJavaScript_ProtocolI::JAVASCRIPT_CONTROL_TYPE nType_, const QDomElement elemControl_, CRTDBI *pRealDatabase)
{
    m_pRTDB = pRealDatabase;
    m_nType = nType_;
    qDebug()<<__func__<<__LINE__<<"CJavaScript_Protocol"<<nType_;
//    QString elemControlTagName = elemControl_.tagName();
    switch (m_nType) {
    case CJavaScript_ProtocolI::Control_Undefined:
        return false;
        break;
    case CJavaScript_ProtocolI::Control_Start:
        return false;
        break;
    case CJavaScript_ProtocolI::Control_Cycle:
    {

    }
        break;
    case CJavaScript_ProtocolI::Control_Timing:
    {

    }
        break;
    case CJavaScript_ProtocolI::Control_DateChange:
    {///< 数据改变
        qDebug()<<__func__<<__LINE__<<"CJavaScript_Protocol 2"<<nType_<<elemControl_.tagName();
        QDomNodeList JavaScriptList = elemControl_.elementsByTagName("JavaScript");
        /// 添加点
        for (int nJavaScriptCount = 0; nJavaScriptCount < JavaScriptList.count(); ++nJavaScriptCount)
        {
            QDomElement Elem_JavaScript = JavaScriptList.at(nJavaScriptCount).toElement();
            qDebug()<<__func__<<__LINE__<<"CJavaScript_Protocol 3"<<Elem_JavaScript.attribute("Script");
            QDomNodeList PointNodeList = Elem_JavaScript.elementsByTagName("Point");
//            QString strScript = Elem_JavaScript.attribute("Script");
//            QString strDesc = Elem_JavaScript.attribute("Desc");
            for (int nPointCount = 0; nPointCount < PointNodeList.count(); ++nPointCount)
            {
                QDomElement elemPoint = PointNodeList.at(nPointCount).toElement();
                qDebug()<<__func__<<__LINE__<<"CJavaScript_Protocol 4"<<elemPoint.attribute("Link");

                Point *pPoint = new Point;

                pPoint->m_strLink = elemPoint.attribute("Link");
                CTagI *pTag = m_pRTDB->FindTag(pPoint->m_strLink);
                if (pTag)
                {
                    pPoint->m_pTag = pTag;
//                    pPoint->m_pTag->SetNotify(pPoint);///设置转发点 这里不设置转发点
                    AddPoint(pPoint);
                }
            }
        }/// 添加点结束

        /// 初始化JS脚本
        for (int nJavaScriptCount = 0; nJavaScriptCount < JavaScriptList.count(); ++nJavaScriptCount)
        {
            QDomElement Elem_JavaScript = JavaScriptList.at(nJavaScriptCount).toElement();
            QDomNodeList PointNodeList = Elem_JavaScript.elementsByTagName("Point");
            QString strScript = Elem_JavaScript.attribute("Script");
            QString strDesc = Elem_JavaScript.attribute("Desc");
            qDebug()<<__func__<<__LINE__<<strScript<<strDesc;

            JavaScript *pJavaScript = NULL;
            for (int nPointCount = 0; nPointCount < PointNodeList.count(); ++nPointCount)
            {
                QDomElement elemPoint = PointNodeList.at(nPointCount).toElement();
                QString strLink = elemPoint.attribute("Link");

                CTagI *pTag = m_pRTDB->FindTag(strLink);
                if (pTag == NULL)
                {
                    break;
                }
                if (FindPoint(strLink) == NULL)
                {
                    break;
                }
                if (pJavaScript == NULL)
                {
                    pJavaScript = new JavaScript;
                    pJavaScript->m_strScript = strScript;
                }
                CJavaScript_DataChange_Point *pPoint = new CJavaScript_DataChange_Point;
                pPoint->m_strLink = strLink;
                pPoint->m_pProtocol = this;
                pPoint->m_pTag = pTag;
                pPoint->m_pTag->SetNotify(pPoint);///设置转发点
                pPoint->AddJavaScript(pJavaScript);

                PointStruct *pPointStruct = new PointStruct;
                pPointStruct->m_strLink = strLink;
                pPointStruct->m_strName = elemPoint.attribute("Name");
//                pPointStruct->m_pPoint = FindPoint(strLink);
                pPointStruct->m_pPoint = pPoint;


                pJavaScript->m_PointStructMap.insert(strLink,pPointStruct);
                qDebug()<<"111111111"<<__func__<<__LINE__<<strScript<<strDesc<<strLink;
            }
            qDebug()<<"SCRIPT="<<Elem_JavaScript.attribute("Script");
        }/// 初始化JS脚本 结束
    }
        break;
    default:
        break;
    }
//    if (elemControlTagName == "ControlCycle")
//    {///< 循环

//    }else if (elemControlTagName == "ControlTiming")
//    {///< 定时

//    }else if (elemControlTagName == "ControlDateChange")
//    {///< 数据改变
//        QDomNodeList JavaScriptList = elemControl_.elementsByTagName("JavaScript");
//        /// 添加点
//        for (int nJavaScriptCount = 0; nJavaScriptCount < JavaScriptList.count(); ++nJavaScriptCount)
//        {
//            QDomElement Elem_JavaScript = JavaScriptList.at(nJavaScriptCount).toElement();
//            QDomNodeList PointNodeList = Elem_JavaScript.elementsByTagName("Point");
////            QString strScript = Elem_JavaScript.attribute("Script");
////            QString strDesc = Elem_JavaScript.attribute("Desc");
//            for (int nPointCount = 0; nPointCount < PointNodeList.count(); ++nPointCount)
//            {
//                QDomElement elemPoint = PointNodeList.at(nPointCount).toElement();

//                Point *pPoint = new Point;

//                pPoint->m_strLink = elemPoint.attribute("Link");
//                CTagI *pTag = m_pRTDB->FindTag(pPoint->m_strLink);
//                if (pTag)
//                {
//                    pPoint->m_pTag = pTag;
////                    pPoint->m_pTag->SetNotify(pPoint);///设置转发点 这里不设置转发点
//                    AddPoint(pPoint);
//                }
//            }
//        }/// 添加点结束

//        /// 初始化JS脚本
//        for (int nJavaScriptCount = 0; nJavaScriptCount < JavaScriptList.count(); ++nJavaScriptCount)
//        {
//            QDomElement Elem_JavaScript = JavaScriptList.at(nJavaScriptCount).toElement();
//            QDomNodeList PointNodeList = Elem_JavaScript.elementsByTagName("Point");
//            QString strScript = Elem_JavaScript.attribute("Script");
//            QString strDesc = Elem_JavaScript.attribute("Desc");
//            qDebug()<<__func__<<__LINE__<<strScript<<strDesc;

//            JavaScript *pJavaScript = NULL;
//            for (int nPointCount = 0; nPointCount < PointNodeList.count(); ++nPointCount)
//            {
//                QDomElement elemPoint = PointNodeList.at(nPointCount).toElement();
//                QString strLink = elemPoint.attribute("Link");

//                CTagI *pTag = m_pRTDB->FindTag(strLink);
//                if (pTag == NULL)
//                {
//                    break;
//                }
//                if (FindPoint(strLink) == NULL)
//                {
//                    break;
//                }
//                if (pJavaScript == NULL)
//                {
//                    pJavaScript = new JavaScript;
//                    pJavaScript->m_strScript = strScript;
//                }
//                CJavaScript_DataChange_Point *pPoint = new CJavaScript_DataChange_Point;
//                pPoint->m_strLink = strLink;
//                pPoint->m_pProtocol = this;
//                pPoint->m_pTag = pTag;
//                pPoint->m_pTag->SetNotify(pPoint);///设置转发点
//                pPoint->AddJavaScript(pJavaScript);

//                PointStruct *pPointStruct = new PointStruct;
//                pPointStruct->m_strLink = strLink;
//                pPointStruct->m_strName = elemPoint.attribute("Name");
//                pPointStruct->m_pPoint = FindPoint(strLink);

//                pJavaScript->m_PointStructMap.insert(strLink,pPointStruct);
//            }
//            qDebug()<<"SCRIPT="<<Elem_JavaScript.attribute("Script");
//        }/// 初始化JS脚本 结束
//    }else
//    {
//        return false;
//    }
    return true;
}

bool CJavaScript_Protocol::OnTagChange(CPointI *pPoint_, QVariant VarSet_)
{
    emit signal_TagChange(pPoint_,VarSet_);
    return true;
}

void CJavaScript_Protocol::slot_TagChange(CPointI *pPoint_, QVariant /*VarSet_*/)
{
    CJavaScript_DataChange_Point *pPoint = (CJavaScript_DataChange_Point *)pPoint_;

    qDebug()<<"222222222222222"<<__func__<<__LINE__<<pPoint->m_strLink;
    for (int nJavaScriptCount = 0; nJavaScriptCount < pPoint->m_JavaScriptList.count(); ++nJavaScriptCount)
    {
        JavaScript *pJavaScript = pPoint->m_JavaScriptList.at(nJavaScriptCount);
        qDebug()<<__func__<<__LINE__<<pPoint->m_strLink<<pJavaScript->m_strScript;
        QScriptEngine engine;
        QMap<QString,PointStruct*>::iterator iterator;
        for (iterator = pJavaScript->m_PointStructMap.begin(); iterator != pJavaScript->m_PointStructMap.end(); ++iterator)
        {
            QString strName = iterator.value()->m_strName;
            qDebug()<<__func__<<__LINE__<<pPoint->m_strLink<<pJavaScript->m_strScript<<strName;
            CTagI *pTag = iterator.value()->m_pPoint->m_pTag;
            if (pTag)
            {
                iterator.value()->m_pPoint->m_SrcValue = pTag->GetProjectValue()->GetVarValue();
                engine.globalObject().setProperty(strName,pTag->GetProjectValue()->GetVarValue().toFloat());
                qDebug()<<__func__<<__LINE__<<pPoint->m_strLink<<pJavaScript->m_strScript<<strName<<pTag->GetProjectValue()->GetVarValue().toFloat();
            }else
            {
                qDebug()<<__func__<<__LINE__<<pPoint->m_strLink<<pJavaScript->m_strScript<<strName<<"return;";
//                return;
            }
        }
        engine.evaluate(pJavaScript->m_strScript);
        for (iterator = pJavaScript->m_PointStructMap.begin(); iterator != pJavaScript->m_PointStructMap.end(); ++iterator)
        {
            QString strName = iterator.value()->m_strName;
            qDebug()<<__func__<<__LINE__<<pPoint->m_strLink<<pJavaScript->m_strScript<<strName;
            CTagI *pTag = iterator.value()->m_pPoint->m_pTag;
            if (pTag)
            {
                qDebug()<<__func__<<__LINE__<<pTag->GetName();
                QVariant value = engine.globalObject().property(strName).toVariant();
                if (value != iterator.value()->m_pPoint->m_SrcValue)
                {
                    engine.globalObject().setProperty(strName,pTag->GetProjectValue()->GetVarValue().toFloat());
                    pTag->SetValue(iterator.value()->m_pPoint,value,value);
                    qDebug()<<__func__<<__LINE__<<pTag->GetName()<<"setvalue1"<<value;
                }else
                {
                    qDebug()<<__func__<<__LINE__<<pTag->GetName()<<"setvalue2"<<value;
                }
            }else
            {
//                return;
            }
        }
    }

}

bool CJavaScript_Protocol::AddPoint(Point *pPoint_)
{
    qDebug()<<__func__<<pPoint_->m_strLink;
    if (m_Protocol_PointMap.contains(pPoint_->m_strLink))
    {
        return false;
    }else
    {
        m_Protocol_PointMap.insert(pPoint_->m_strLink, pPoint_);
    }
    return true;
}

Point *CJavaScript_Protocol::FindPoint(QString strLink_)
{
    return m_Protocol_PointMap.value(strLink_);;
}
