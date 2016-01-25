#include "CRTFrameRead.h"
#include <QBuffer>
#include <QDebug>
#include "../PortInclude/CRTFrameSharedMemoryLibI.h"

CRTFrameRead::CRTFrameRead(QObject *parent) :
    QObject(parent)
{
    m_pRTFrameSharedMemoryLibI = NULL;
#if defined(TIAOSHI)
    m_LibName = QString("Protocol/RTFrameSharedMemoryLib_d");///< 库文件的路径
#else
    m_LibName = QString("Protocol/RTFrameSharedMemoryLib");///< 库文件的路径
#endif
//    mylib;///< 库文件
    mylib.setFileName(m_LibName);//文件名

    if (mylib.load())
    {
        qDebug()<<"DLL load is OK!"<<m_LibName;
        typedef CRTFrameSharedMemoryLibI * (DLLAPI_CreateDriver)( );//初始化协议
        DLLAPI_CreateDriver *pCreateDriver = (DLLAPI_CreateDriver *)mylib.resolve("CreateDriver");//获取 用于获取类对象的全局函数
        if (pCreateDriver)
        {
            qDebug()<<"Link to Function is OK!"<<m_LibName;
            m_pRTFrameSharedMemoryLibI = pCreateDriver();///< 库中导出类的初始化
        }else                       //ShowMessageSignal
        {
            qDebug() << mylib.errorString();
            qDebug()<<"Linke to Function is not OK!!!!"<<m_LibName;
        }
    }else
    {
        qDebug()<<"DLL is not loaded!"<<m_LibName;
    }
}
QString CRTFrameRead::GetFrame(QString strChannelName_)
{
    if (m_pRTFrameSharedMemoryLibI)
    {
        QString strFrame_ = m_pRTFrameSharedMemoryLibI->GetFrame(strChannelName_);
        return strFrame_;
    }
    return QString();
}
