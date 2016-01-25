/*!
 * \file    CDatabaseThread.cpp
 * \brief   概述 CDatabaseThread类源文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/2/4
 *
 * \b       修改记录：
 * \li      2015/2/4
 *  添加注释
 */

#include "CDatabaseThread.h"
#include "../PortInclude/CDatabaseProtocolI.h"
#include "Src/cmyfunction.h"
#include "Src/crtdbbase.h"
#include <QDebug>

//extern CRTDBBase g_RealTimeDB;

/*!
 * \brief  功能概述 构造函数，构造一个CDatabaseThread类对象
 * \param  参数描述 nSaveType_是存储类型，strFileName_是通道文件名，strLibFilePath_是库文件路径名，
 * nNumber是数据库编号，parent是父类的指针
 * \return 返回值描述
 * \author zzy
 * \date   2015/2/4
 */
CDatabaseThread::CDatabaseThread(CRTDBBase *pRealTimeDB_,const QString &strFileName_, int nNumber, QObject *parent) :
    QThread(parent),
    m_pRealTimeDB(pRealTimeDB_),
    m_strFileName(strFileName_),
    m_nNumber(nNumber)
{
    m_IsStart = false;
}

/*!
 * \brief  功能概述 线程类中的run函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/2/5
 */
void CDatabaseThread::run()
{
#if defined(TIAOSHI)
    m_strLibFilePath = QString("Protocol/DatabaseProtocol_d");
#else
    m_strLibFilePath = QString("Protocol/DatabaseProtocol");
#endif
    mylib.setFileName(m_strLibFilePath);
    if (mylib.load())
    {
        typedef CDatabaseProtocolI * (DLLAPI_CreaterDriver)( );///< 初始化协议
        DLLAPI_CreaterDriver *pCreateDriver = (DLLAPI_CreaterDriver *)mylib.resolve("CreateDriver");
        if (pCreateDriver)
        {
            qDebug()<<"Database : Link to Function is OK!"<<m_strLibFilePath;
            m_pDatabaseProtocol = pCreateDriver();///< 库中导出类的初始化
            if (m_pDatabaseProtocol != NULL)
            {
                m_IsStart = m_pDatabaseProtocol->OnCreateChannel(m_strFileName,m_pRealTimeDB,m_nNumber);
            }else
            {
                qDebug()<<"Database : pCreateDriver() is not OK!"<<m_strLibFilePath;
            }
        }else
        {
            qDebug()<<mylib.errorString();
            qDebug()<<"Database : Linke to Function is not OK!!!!"<<m_strLibFilePath;
        }
    }else
    {
        qDebug()<<"Database : DLL is not loaded!"<<m_strLibFilePath;
    }
    exec();///< 线程中使用定时器时需要加上此函数
}
