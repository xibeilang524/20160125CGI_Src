/*!
 * \file    cpointiec_104.h
 * \brief   概述 CPointIEC_104类的头文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/25
 *
 * \b       修改记录：
 * \li      2015/5/25
 *  添加注释
 */

#ifndef CPOINTIEC_104_H
#define CPOINTIEC_104_H
#include "../PortInclude/cpointbase.h"

/*!
 * \class  CPointIEC_104
 * \brief  概述 IEC-104协议中点的参数类
 * \author zzy
 * \date   2015/5/25
 * 详细概述
 */
class CPointIEC_104 : public CPointBase
{
public:
    CPointIEC_104();
    virtual int GetDeviceAddress();
    int m_nDeviceAddr;      ///< 设备地址
    int m_nPublicAddress;   ///< 公共地址
    int m_nPointNumber;     ///< 点号
    int m_nYK_Type;
};

#endif // CPOINTIEC_104_H
