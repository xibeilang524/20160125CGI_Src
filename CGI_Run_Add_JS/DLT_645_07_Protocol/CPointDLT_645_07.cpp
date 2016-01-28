/*!
 * \file    CPointDLT_645_07.cpp
 * \brief   概述 CPointDLT_645_07类的源文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/27
 *
 * \b       修改记录：
 * \li      2015/5/27
 *  添加注释
 */

#include "CPointDLT_645_07.h"

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
CPointDLT_645_07::CPointDLT_645_07()
{
    m_bDI3 = 0x00;
    m_bDI2 = 0x00;
    m_bDI1 = 0x00;
    m_bDI0 = 0x00;
    m_nByteCount = 0;/// @note 字节长度
    m_nDecimalPlaces = 0;/// @note 小数点位数
    m_strDeviceAddr = "";/// @note 设备地址 与报文中的设备地址一致 6字节
    m_strDI = "";/// @note 点在报文中的数据标识
}

/*!
 * \brief  功能概述 获取设备地址
 * \param  参数描述 无
 * \return 返回值描述 返回设备地址
 * \author zzy
 * \date   2015/5/27
 */
int CPointDLT_645_07::GetDeviceAddress()
{
    return 0;
}
