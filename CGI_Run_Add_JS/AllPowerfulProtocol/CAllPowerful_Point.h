#ifndef CALLPOWERFUL_POINT_H
#define CALLPOWERFUL_POINT_H

#include "../PortInclude/cpointbase.h"

class CAllPowerful_Point : public CPointBase
{
public:
    CAllPowerful_Point();
    int GetDeviceAddress();

    int m_nDeviceAddress;
    int m_nStartByte;
    int m_nByteCount;
    int m_nValueType; ///< 数据类型
};

#endif // CALLPOWERFUL_POINT_H
