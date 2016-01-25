/*!
 * \file    cpointbasemodbus.cpp
 * \brief   概述 CPointBaseModbus类的源文件
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

#include "cpointbasemodbus.h"

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
CPointBaseModbus::CPointBaseModbus()
{
    m_nDeviceAddr = -1;      //设备地址
    m_nRegisterAddress = -1; //寄存器地址
    m_nFunction = -1;        //功能码
    m_nRegisterCount = -1;   //寄存器数
    m_nDataType = -1;        //数据类型
}

/*!
 * \brief  功能概述 获取该点的设备地址
 * \param  参数描述 无
 * \return 返回值描述 返回该点的设备地址
 * \author zzy
 * \date   2015/5/25
 */
int CPointBaseModbus::GetDeviceAddress()
{
    return m_nDeviceAddr;
}
