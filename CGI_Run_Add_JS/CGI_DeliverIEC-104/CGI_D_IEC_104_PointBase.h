#ifndef CGI_D_IEC_104_POINTBASE_H
#define CGI_D_IEC_104_POINTBASE_H

#include "../PortInclude/cpointbase.h"
#include "IEC_104.h"

class CGI_D_IEC_104_PointBase : public CPointBase
{
public:
    CGI_D_IEC_104_PointBase();
    virtual int Get104Type();/// 遥信和遥测有获取104点的类型，遥脉、遥控、遥设没有
    int m_nPublicAddress;   ///< 公共地址
    int m_nPointNumber;     ///< 点号
};

#endif // CGI_D_IEC_104_POINTBASE_H
