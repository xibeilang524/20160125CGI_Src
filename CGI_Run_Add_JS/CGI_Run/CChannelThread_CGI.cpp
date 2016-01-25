#include "CChannelThread_CGI.h"
#include <QDebug>
#include <QDir>
#include <QMutex>
#include "Src/crtdbbase.h"
#include "Src/CPRTMapBase.h"
#include "Src/cmyfunction.h"
#include "Src/CPRTVBase.h"
#include "../PortInclude/cprotocoli.h"

//extern CRTDBBase g_RealTimeDB;
QMutex g_DisplayFramemutex;
CPRTMapBase g_PRTMap;


/*!
 * \brief  功能概述 各个驱动线程向主线程传输报文和Debug信息的函数
 * \param  参数描述 type_是包括发送、接收、Debug三种模式，message_是需要显示的信息，nChannelNumber_是调用此函数驱动的通道号
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/22
 */
void ShowMessageaaa(int type_,QString message_,int nChannelNumber_)
{
//    qDebug()<<"run:="<<type_<<message_<<nChannelNumber_;
    switch (type_) {
    case CProtocolI::SHOWMESSAGE_TYPE_SEND:
        fprintf(stderr, "Send: %u %s\n", nChannelNumber_, message_.toStdString().data());
        break;
    case CProtocolI::SHOWMESSAGE_TYPE_RECEIVE:
        fprintf(stderr, "Rece: %u %s\n", nChannelNumber_, message_.toStdString().data());
        break;
    case CProtocolI::SHOWMESSAGE_TYPE_DEBUG:
        fprintf(stderr, "Debu: %u %s\n", nChannelNumber_, message_.toStdString().data());
    default:
        break;
    }
    if (CMyFunction::pMonitorProtocol != NULL)
    {
//        qDebug()<<"run1:="<<type_<<message_<<nChannelNumber_;
        CMyFunction::pMonitorProtocol->WriteMessage(type_,message_,nChannelNumber_);
//        qDebug()<<"run2:="<<type_<<message_<<nChannelNumber_;
    }
}
CChannelThread_CGI::CChannelThread_CGI(CRTDBBase *pRealTimeDB_,int nChannelNumber_, ChannelType nChannelType_, QObject *parent) :
    QThread(parent),
    m_pRealTimeDB(pRealTimeDB_),
    m_nChannelNumber(nChannelNumber_),
    m_nChannelType(nChannelType_)
{
}

CChannelThread_CGI::CChannelThread_CGI(CRTDBBase *pRealTimeDB_,int nChannelNumber_, ChannelType nChannelType_, int nComOrCanNumber_, QObject *parent) :
    QThread(parent),
    m_pRealTimeDB(pRealTimeDB_),
    m_nChannelNumber(nChannelNumber_),
    m_nChannelType(nChannelType_),
    m_nComOrCanNumber(nComOrCanNumber_)
{
    qDebug()<<"COM "<<m_nComOrCanNumber;

}

void CChannelThread_CGI::SetMonitorIPAddress(QString strMonitorIPAddress_)
{
    m_strMonitorIPAddress = strMonitorIPAddress_;
}

void CChannelThread_CGI::SetDomElement(QDomElement ChannelElement_)
{
    m_ChannelElement = ChannelElement_;
    InitChannel();
}

void CChannelThread_CGI::InitChannel()
{
    switch (m_nChannelType) {
    case ChannelType_COM:
    case ChannelType_NET:
    case ChannelType_CAN:
    {
        QDomNodeList TopNodeList = m_ChannelElement.elementsByTagName("Top");
        for (int nTopCount = 0; nTopCount < TopNodeList.count(); ++nTopCount)
        {
#if defined(TIAOSHI)
            m_LibName = QString("Protocol/"+TopNodeList.at(0).toElement().attribute("Protocol")+"_d");
#else
            m_LibName = QString("Protocol/"+TopNodeList.at(0).toElement().attribute("Protocol"));
#endif
            qDebug()<<m_LibName;
        }
    }
        break;
    case ChannelType_Monitor104:
#if defined(TIAOSHI)
        m_LibName = QString("Protocol/MonitorDeliverIEC-104_d");
#else
        m_LibName = QString("Protocol/MonitorDeliverIEC-104");
#endif
        break;
    default:
        break;
    }

//    switch (m_nChannelType) {
//    case ChannelType_COM:
//    {
//        QDomNodeList TopNodeList = m_ChannelElement.elementsByTagName("Top");
//        for (int nTopCount = 0; nTopCount < TopNodeList.count(); ++nTopCount)
//        {
//            m_LibName = QString("Protocol/"+TopNodeList.at(0).toElement().attribute("Protocol"));
//            qDebug()<<m_LibName;
//        }

//    }
//        break;
//    case ChannelType_NET:
//    {
//        QDomNodeList TopNodeList = m_ChannelElement.elementsByTagName("Top");
//        for (int nTopCount = 0; nTopCount < TopNodeList.count(); ++nTopCount)
//        {
//            m_LibName = QString("Protocol/"+TopNodeList.at(0).toElement().attribute("Protocol"));
//            qDebug()<<m_LibName;
//        }
//    }
//        break;
//    case ChannelType_CAN:
//    {
//        QDomNodeList TopNodeList = m_ChannelElement.elementsByTagName("Top");
//        for (int nTopCount = 0; nTopCount < TopNodeList.count(); ++nTopCount)
//        {
//            m_LibName = QString("Protocol/"+TopNodeList.at(0).toElement().attribute("Protocol"));
//        }
//    }
//        break;
//    default:
//        qDebug()<<"Don't have this ChannelType. :"<<m_nChannelType;
//        break;
//    }
}

void CChannelThread_CGI::run()
{
    qDebug()<<"run"<<m_ChannelElement.tagName();
    mylib.setFileName(m_LibName);//文件名

    if (mylib.load())
    {
        qDebug()<<"DLL load is OK!"<<m_LibName;
        typedef CProtocolI * (DLLAPI_CreateDriver)(QObject *parent);//初始化协议
        DLLAPI_CreateDriver *pCreateDriver = NULL;
        pCreateDriver = (DLLAPI_CreateDriver *)mylib.resolve("CreateDriver");//获取 用于获取类对象的全局函数
        if (pCreateDriver != NULL)
        {
            qDebug()<<"Link to Function is OK!"<<m_LibName;
            m_pProtocolI = pCreateDriver(NULL);///< 库中导出类的初始化
            m_pProtocolI->SetShowMessage(ShowMessageaaa);///< 传递打印信息函数的 函数指针
            m_pProtocolI->SetChannelNumber(m_nChannelNumber);///< 设置通道号
            CPRTVBase *pPRTBase = new CPRTVBase;
            pPRTBase->SetProtocol(m_pProtocolI);
            if (ChannelType_Monitor104 == m_nChannelType)
            {
                qDebug()<<"chenggong chushihua monitor zhuanfa qudong zhizhen ================";
                CMyFunction::pMonitorProtocol = m_pProtocolI;
                m_pProtocolI->SetPRTMap(&g_PRTMap);
                m_pProtocolI->OnCreateChannel(m_strMonitorIPAddress,m_pRealTimeDB);
            }else
            {
                g_PRTMap.Add(m_nChannelNumber,pPRTBase);

                m_pProtocolI->SetPRTMap(&g_PRTMap);
                m_pProtocolI->OnCreateChannel(m_ChannelElement,(ChannelType)m_nChannelType,m_nComOrCanNumber,m_pRealTimeDB);
            }
        }else
        {
            qDebug() << mylib.errorString();
            qDebug()<<"Linke to Function is not OK!!!!"<<m_LibName;
        }
    }else
    {
        qDebug()<<"DLL is not loaded!"<<m_LibName;
    }
    exec();///< 线程中使用定时器时需要加上此行
}
