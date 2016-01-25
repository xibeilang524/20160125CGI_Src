#include "CStart.h"
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QLockFile>
#include "../PortInclude/defined.h"

CStart::CStart(QObject *parent) :
    QObject(parent)
{
    m_nTimerID = 0;
    m_nChongQiShiJian = 1*60*60;///< 6个小时重新启动一下机器
//    m_nChongQiShiJian = 30;
    m_nStartTime = GetSysAbsTime();

}
void CStart::SetStart()
{
    qDebug()<<"start "<<__func__<<__FILE__<<__LINE__;
    int nQiDong = system(START_START_SH);
    if (0 == nQiDong)
    {
        qDebug()<<"Qi Dong."<<nQiDong;
    }else
    {
        qDebug()<<"Qi Dong."<<nQiDong;
    }
#if defined(Q_OS_LINUX)
    {
        #if defined(Q_PROCESSOR_ARM)
        m_nTimerID = startTimer(10000);/// 10miao钟重新判断一下运行系统是否在运行
        #endif
    }
#else
    {
        #if defined(Q_PROCESSOR_ARM)

        #else defined()

        #endif
    }

#endif
}

void CStart::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nTimerID)
    {
        qDebug()<<GetSysAbsTime()<<m_nStartTime<<m_nChongQiShiJian;
        QLockFile lockfile(LOCKFILE);
        if (lockfile.tryLock())
        {
            lockfile.unlock();
            int nQiDong = system(START_START_SH);
            if (0 == nQiDong)
            {
                qDebug()<<"Qi Dong."<<nQiDong;
            }else
            {
                qDebug()<<"Qi Dong."<<nQiDong;
            }
        }else
        {

        }
    }

}
/*!
 * \brief  功能概述 获取绝对秒
 * \param  参数描述 无
 * \return 返回值描述 返回绝对秒
 * \author zzy
 * \date   2015/5/27
 */
int CStart::GetSysAbsTime()
{
    ///获取绝对秒
    int nTime = QDateTime::currentDateTime().secsTo(QDateTime(QDate(1970,1,1),QTime(0,0,0,0)));
    if(nTime < 0)
    {
        nTime = -nTime;
    }
    return nTime;
}

