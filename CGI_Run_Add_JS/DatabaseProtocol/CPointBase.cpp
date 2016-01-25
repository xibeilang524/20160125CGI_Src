/*!
 * \file    CPointBase.cpp
 * \brief   概述 CPointBase类的源文件
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

#include "CPointBase.h"
#include "../PortInclude/cvaluebase.h"
#include "DatabaseProtocol/CDatabaseProtocolDatabase.h"

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
CPointBase::CPointBase()
{
    m_pProtocol = NULL;
    m_pTag= NULL;
    m_pValue = new CValueBase;
}
/*!
 * \brief 功能概述 处理当值发生变化时
 * \param 参数描述 valueSet_是发生变化的值
 * \return 返回值描述 返回true
 * \author zzy
 * \date 2015/1/12
 */
bool CPointBase::OnValueChange(QVariant valueSet_)
{

    //    功能:处理当值发生变化时
    //    功能:处理遥控、遥设变量
    //    qDebug()<<"转发";
    Q_ASSERT(m_pValue != NULL);
    return m_pProtocol->OnTagChange(this,valueSet_);
}

/*!
 * \brief  功能概述 获取值
 * \param  参数描述 无
 * \return 返回值描述 返回值指针
 * \author zzy
 * \date   2015/5/27
 */
CValueI *CPointBase::GetValue()
{
    Q_ASSERT(m_pValue != NULL);
    return m_pValue;
}

/*!
 * \brief  功能概述 对于数据库来说没有设备地址
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/2/5
 */
int CPointBase::GetDeviceAddress()
{
    return 0;
}
