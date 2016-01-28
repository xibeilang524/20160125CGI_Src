#include "CPointDemoBase.h"
#include <QDateTime>

CPointDemoBase::CPointDemoBase()
{
    m_nDeviceAddr = 0;
    m_nStartRunTime =GetSysAbsTime();
}

int CPointDemoBase::GetDeviceAddress()
{
    return m_nDeviceAddr;
}

int CPointDemoBase::GetSysAbsTime()
{
    ///获取绝对秒
    int nTime = QDateTime::currentDateTime().secsTo(QDateTime(QDate(1970,1,1),QTime(0,0,0,0)));
    if(nTime < 0)
    {
        nTime = -nTime;
    }
    return nTime;
}
