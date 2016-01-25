#ifndef CPOINTCDT_H
#define CPOINTCDT_H

#include "../PortInclude/cpointbase.h"

class CPointCDT : public CPointBase
{
public:
    CPointCDT();
    ///获取设备地址
    int GetDeviceAddress();
    int m_nSrcStationAdd;       ///< 源站地址即从站地址
    int m_nTargetStationAdd;    ///< 目的站地址即主站地址
    int m_nPointNO;             ///< 点号
    int m_nStartFunctionalGroup;///< 起始功能码
    void SetValue(CPointI *pPoint_, QVariant VarProject_, QVariant VarOriginal_);///< 设置值
};

#endif // CPOINTCDT_H
