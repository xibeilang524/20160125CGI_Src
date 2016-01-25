/*!
 * \file    CPointBaseModbusRtu.cpp
 * \brief   概述 CPointBaseModbusRtu类的源文件
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

#include "cpointbasemodbusrtu.h"

#include <QDebug>

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
CPointBaseModbusRtu::CPointBaseModbusRtu()
{
}

/*!
 * \brief  功能概述 获取该点的设备地址
 * \param  参数描述 无
 * \return 返回值描述 返回该点的设备地址
 * \author zzy
 * \date   2015/5/25
 */
int CPointBaseModbusRtu::GetDeviceAddress()
{
    return m_nDeviceAddr;
}
