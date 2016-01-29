#include "CCGI_RTDB_Start.h"
#include <QDateTime>
#include <QDebug>

CCGI_RTDB_Start::CCGI_RTDB_Start(QObject *parent) :
    QObject(parent)
{
    m_nIsConnect = false;
}
bool CCGI_RTDB_Start::Open()
{
//    qDebug()<<"m_libname ="<<m_LibName<<currentThreadId();
#if defined(TIAOSHI)
    m_LibName = QString("Protocol/RTDBSharedMemoryLib_d");///< 库文件的路径
#else
    m_LibName = QString("Protocol/RTDBSharedMemoryLib");///< 库文件的路径
#endif
    qDebug()<<m_LibName;
    mylib.setFileName(m_LibName);//文件名

    if (mylib.load())
    {
#if defined(PRINTF)
        printf("DLL load is OK! %s\n",m_LibName.toStdString().data());
#else

#endif
        qDebug()<<"DLL load is OK!"<<m_LibName;
        typedef CRTDBSharedMemoryLibI * (DLLAPI_CreateDriver)( );//初始化协议
        DLLAPI_CreateDriver *pCreateDriver = (DLLAPI_CreateDriver *)mylib.resolve("CreateDriver");//获取 用于获取类对象的全局函数
        if (pCreateDriver)
        {
#if defined(PRINTF)
        printf("Link to Function is OK! %s\n",m_LibName.toStdString().data());
#else

#endif
            qDebug()<<"Link to Function is OK!"<<m_LibName;
            m_pRTDBSharedMemoryLibI = pCreateDriver();///< 库中导出类的初始化

            if (m_pRTDBSharedMemoryLibI->Open())
            {
                m_nIsConnect = true;
                return true;
            }else
            {
#if defined(PRINTF)
        printf("Create Function Return false. %s\n",m_LibName.toStdString().data());
#else

#endif
                qDebug()<<"Create Function Return false."<<m_LibName;
                return false;
            }
        }else                       //ShowMessageSignal
        {
#if defined(PRINTF)
        printf("Link to Function is not OK!!!! %s %s\n",mylib.errorString().toStdString().data(),m_LibName.toStdString().data());
#else

#endif
            qDebug() << mylib.errorString();
            qDebug()<<"Link to Function is not OK!!!!"<<m_LibName;
            return false;
        }
    }else
    {
#if defined(PRINTF)
        printf("DLL is not loaded! %s\n",m_LibName.toStdString().data());
#else

#endif
        qDebug()<<"DLL is not loaded!"<<m_LibName;
        return false;
    }
    return true;
}

bool CCGI_RTDB_Start::SetValue(int nID_, QVariant varValue_)
{
    if (m_nIsConnect)
    {
        return m_pRTDBSharedMemoryLibI->YKYSSetValue(nID_,varValue_);
    }else{
        return false;
    }

}

bool CCGI_RTDB_Start::SetValue(QString TagName_, QVariant varValue_)
{
    if (m_nIsConnect)
    {
        return m_pRTDBSharedMemoryLibI->YKYSSetValue(TagName_,varValue_);
    }else{
        return false;
    }
}

QList<CSharedMemoryPoint> CCGI_RTDB_Start::GetValue(QList<int> nIDList_)
{
    if (m_nIsConnect)
    {
        return m_pRTDBSharedMemoryLibI->GetValue(nIDList_);
    }else{
        QList<CSharedMemoryPoint> aaa;
        return aaa;
    }

}

CSharedMemoryPoint CCGI_RTDB_Start::GetValue(int nID_)
{
    if (m_nIsConnect)
    {
        return m_pRTDBSharedMemoryLibI->GetValue(nID_);

    }else{
        return CSharedMemoryPoint();
    }

}

CSharedMemoryPoint CCGI_RTDB_Start::GetValue(QString strRTDBName_)
{
    qDebug()<<__func__<<__LINE__<<"m_nIsConnect"<<m_nIsConnect;
    if (m_nIsConnect)
    {
        qDebug()<<__func__<<__LINE__<<"m_nIsConnect"<<m_nIsConnect;
        return m_pRTDBSharedMemoryLibI->GetValue(strRTDBName_);
    }else{
        qDebug()<<__func__<<__LINE__<<"m_nIsConnect"<<m_nIsConnect;
        return CSharedMemoryPoint();
    }

}
