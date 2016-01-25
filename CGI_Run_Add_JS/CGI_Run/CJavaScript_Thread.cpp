#include "CJavaScript_Thread.h"
#include <QDebug>


CJavaScript_Thread::CJavaScript_Thread(CRTDBBase *pRealTimeDB_, const QDomElement &ControlElement_, CJavaScript_ProtocolI::JAVASCRIPT_CONTROL_TYPE nType_, QObject *parent) :
    QThread(parent),
    m_pRealTimeDB(pRealTimeDB_),
    m_ControlElement(ControlElement_),
    m_nType(nType_),
    m_bIsValid(false)
{
    m_pProtocolI = NULL;

    switch (m_nType) {
    case CJavaScript_ProtocolI::Control_Undefined:
        break;
    case CJavaScript_ProtocolI::Control_Start:
        m_bIsValid = true;
        break;
    case CJavaScript_ProtocolI::Control_Cycle:
        m_bIsValid = true;

        break;
    case CJavaScript_ProtocolI::Control_Timing:
        m_bIsValid = true;

        break;
    case CJavaScript_ProtocolI::Control_DateChange:
        m_bIsValid = true;
#if defined(TIAOSHI)
            m_LibName = QString("Protocol/JavaScriptProtocol_DataChange_d");
#else
            m_LibName = QString("Protocol/JavaScriptProtocol_DataChange");
#endif
        break;
    default:
        break;
    }

}

void CJavaScript_Thread::run()
{
    mylib.setFileName(m_LibName);//文件名

    if (mylib.load())
    {
        qDebug()<<"DLL load is OK!"<<m_LibName;
        typedef CJavaScript_ProtocolI * (DLLAPI_CreateDriver)(QObject *parent);//初始化协议
        DLLAPI_CreateDriver *pCreateDriver = NULL;
        pCreateDriver = (DLLAPI_CreateDriver *)mylib.resolve("CreateDriver_JavaScript");//获取 用于获取类对象的全局函数
        if (pCreateDriver != NULL)
        {
            qDebug()<<"Link to Function is OK!"<<m_LibName;
            qDebug()<<"Link to Function is OK3!"<<m_LibName;
            m_pProtocolI = pCreateDriver(NULL);///< 库中导出类的初始化
            qDebug()<<"Link to Function is OK2!"<<m_LibName;
            if (m_pProtocolI != NULL)
            {
                qDebug()<<"Link to Function is OK! 2"<<m_LibName<<m_pProtocolI;
                m_pProtocolI->OnCreateChannel(m_nType,m_ControlElement,m_pRealTimeDB);
            }else
            {
                qDebug()<<"Link to Function isn't OK!"<<m_LibName<<m_pProtocolI;
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
bool CJavaScript_Thread::GetbIsValid() const
{
    return m_bIsValid;
}

void CJavaScript_Thread::SetBIsValid(bool bIsValid)
{
    m_bIsValid = bIsValid;
}

