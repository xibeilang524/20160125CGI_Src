/*!
 * \file    cpointbasemodbus.h
 * \brief   概述 CPointBaseModbus类的头文件
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

#ifndef CPOINTBASEMODBUS_H
#define CPOINTBASEMODBUS_H
#include "../PortInclude/cpointbase.h"

/*!
 * \class  CPointBaseModbus
 * \brief  概述 Modbus协议中点的参数类
 * \author zzy
 * \date   2015/5/25
 * 详细概述
 */
class CPointBaseModbus : public CPointBase
{
public:
    CPointBaseModbus();
    virtual int GetDeviceAddress();
public:
    int m_nDeviceAddr;      ///< 设备地址
    int m_nRegisterAddress; ///< 寄存器地址
    int m_nFunction;        ///< 功能码
    int m_nRegisterCount;   ///< 寄存器数
    int m_nDataType;        ///< 数据类型
};

#endif // CPOINTBASEMODBUS_H
