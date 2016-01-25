/*!
 * \file
 * \brief   概述
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/3/24
 *
 * \b       修改记录：
 * \li      2015/3/24
 *  添加注释
 */

#ifndef CPOINTIEC_104_H
#define CPOINTIEC_104_H
#include "cpointbase.h"

class CPointIEC_104 : public CPointBase
{
public:
    CPointIEC_104();
    virtual int GetDeviceAddress();
    int m_nDeviceAddr;      //设备地址
    int m_nPublicAddress;   //公共地址
    int m_nPointNumber;     //点号
    int m_nYK_Type;
    float m_fChangePercentage;//变化百分比
};

#endif // CPOINTIEC_104_H
