#ifndef CPOINTDEMOBASE_H
#define CPOINTDEMOBASE_H

#include "../PortInclude/cpointbase.h"

class CPointDemoBase : public CPointBase
{
public:
    CPointDemoBase();
    int m_nDeviceAddr;
    int m_nFunction;
    int m_nPara1;
    int m_nPara2;
    int m_nPara3;
    int GetDeviceAddress();
    int m_nStartRunTime;
    int GetSysAbsTime();
};

#endif // CPOINTDEMOBASE_H
