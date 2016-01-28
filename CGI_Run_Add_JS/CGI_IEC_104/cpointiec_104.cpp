/*!
 * \file    cpointiec_104.cpp
 * \brief   概述 CPointIEC_104类的源文件
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

#include "cpointiec_104.h"

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
CPointIEC_104::CPointIEC_104()
{
    m_nS_E = 5;///< init this value
}

/*!
 * \brief  功能概述 获取该点设备地址
 * \param  参数描述 无
 * \return 返回值描述 返回该点的设备地址
 * \author zzy
 * \date   2015/5/25
 */
int CPointIEC_104::GetDeviceAddress()
{
    return m_nDeviceAddr;
}
