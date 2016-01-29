#include "CRTDBSharedMemoryThread.h"
#include <QDebug>

CRTDBSharedMemoryThread::CRTDBSharedMemoryThread(QObject *parent) :
    QThread(parent)
{
    m_pRTDBSharedMemoryLibI = NULL;
#if defined(TIAOSHI)
    m_LibName = QString("Protocol/RTDBSharedMemoryLib_d");///< 库文件的路径
#else
    m_LibName = QString("Protocol/RTDBSharedMemoryLib");///< 库文件的路径
#endif
            qDebug()<<m_LibName;
}

void CRTDBSharedMemoryThread::SetpRTDB(QMap<int, CSharedMemoryTag *> *pRTDB_)
{
    m_pRTDB = pRTDB_;
}

void CRTDBSharedMemoryThread::SetValue(int nID_, QVariant VarProject_)
{
    qDebug()<<__func__<<__FILE__<<1;
    if (m_pRTDBSharedMemoryLibI)
    {
        qDebug()<<__func__<<__FILE__<<2;
        m_pRTDBSharedMemoryLibI->SetValue(nID_, VarProject_);
        qDebug()<<__func__<<__FILE__<<4;
    }
}

void CRTDBSharedMemoryThread::run()
{
    qDebug()<<"m_libname ="<<m_LibName<<currentThreadId();
    mylib.setFileName(m_LibName);//文件名

    if (mylib.load())
    {
        qDebug()<<"DLL load is OK!"<<m_LibName;
        typedef CRTDBSharedMemoryLibI * (DLLAPI_CreateDriver)(QObject *parent);//初始化协议
        DLLAPI_CreateDriver *pCreateDriver = (DLLAPI_CreateDriver *)mylib.resolve("CreateDriver");//获取 用于获取类对象的全局函数
        if (pCreateDriver)
        {
            qDebug()<<"Link to Function is OK!"<<m_LibName;
            m_pRTDBSharedMemoryLibI = pCreateDriver(NULL);///< 库中导出类的初始化
            if (m_pRTDBSharedMemoryLibI->Create(m_pRTDB,QString()))
            {

            }else
            {
                qDebug()<<"Create Function Return false."<<m_LibName;
            }
        }else                       //ShowMessageSignal
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
