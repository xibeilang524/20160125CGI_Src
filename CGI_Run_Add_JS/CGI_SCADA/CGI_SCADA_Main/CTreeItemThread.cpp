#include "CTreeItemThread.h"
#include "../CGI_SCADA_include/CGI_SCADA_DLL_I.h"
#include "../CGI_SCADA_include/CGI_SCADA_define.h"

CTreeItemThread::CTreeItemThread(QObject *parent) :
    QThread(parent),
    m_pProtocolI(NULL)
{
}
void CTreeItemThread::run()
{
//    qDebug()<<"run"<<m_LibName;
//    mylib.setFileName(m_LibName);//文件名

//    if (mylib.load())
//    {
//        qDebug()<<"DLL load is OK!"<<m_LibName;
//        typedef CGI_SCADA_DLL_I * (DLLAPI_CreateDriver)(QObject *parent);//初始化协议
//        DLLAPI_CreateDriver *pCreateDriver = NULL;
//        pCreateDriver = (DLLAPI_CreateDriver *)mylib.resolve("CreateDriver");//获取 用于获取类对象的全局函数
//        if (pCreateDriver != NULL)
//        {
//            qDebug()<<"Link to Function is OK!"<<m_LibName;
//            m_pProtocolI = pCreateDriver(NULL);///< 库中导出类的初始化
//        }else
//        {
//            qDebug() << mylib.errorString();
//            qDebug()<<"Linke to Function is not OK!!!!"<<m_LibName;
//        }
//    }else
//    {
//        qDebug()<<"DLL is not loaded!"<<m_LibName;
//    }
//    exec();///< 线程中使用定时器时需要加上此行
}

void CTreeItemThread::SetLibraryName(const QString &strLibName_)
{
    m_LibName = strLibName_;
}

CGI_SCADA_DLL_I *CTreeItemThread::GetTreeItem()
{
    return m_pProtocolI;
}

bool CTreeItemThread::SetStart()
{
    qDebug()<<"run"<<m_LibName;
    m_LibName = m_LibName.remove(".dll");
    bool result = false;
    qDebug()<<"run"<<m_LibName;
    mylib.setFileName(m_LibName);//文件名

    if (mylib.load())
    {
        qDebug()<<"DLL load is OK!"<<m_LibName;
        typedef CGI_SCADA_DLL_I * (DLLAPI_CreateDriver)(QObject *parent);//初始化协议
        DLLAPI_CreateDriver *pCreateDriver = NULL;
        pCreateDriver = (DLLAPI_CreateDriver *)mylib.resolve("CreateDriver");//获取 用于获取类对象的全局函数
        if (pCreateDriver != NULL)
        {
            qDebug()<<"Link to Function is OK!"<<m_LibName;
            m_pProtocolI = pCreateDriver(NULL);///< 库中导出类的初始化
            connect(this,SIGNAL(signal_OpenProject(QString)),m_pProtocolI,SLOT(slot_OpenProject(QString)));
            connect(m_pProtocolI,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
            m_pProtocolI->SetIPAddress_Cookie(CGI_SCADA_define::s_strIPAddress,CGI_SCADA_define::s_baCookie);
            result = true;
        }else
        {
            qDebug() << mylib.errorString();
            qDebug()<<"Linke to Function is not OK!!!!"<<m_LibName;
        }
    }else
    {
        qDebug()<<"DLL is not loaded!"<<m_LibName;
    }
    return result;
}
