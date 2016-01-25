/*!
 * \file    CPRTVBase.cpp
 * \brief   概述 CPRTVBase类的源文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/3/18
 *
 * \b       修改记录：
 * \li      2015/3/18
 *  添加注释
 */

#include "CPRTVBase.h"

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/3/18
 */
CPRTVBase::CPRTVBase()
{
    m_pProtocolI = NULL;                ///< 驱动导出类指针
    m_nDriverType = Undefinition_Type;  ///< 采集/转发类型标识
//    m_VarChargePileNumber;  ///充电桩编号The charging pile number
    m_nChargePileNumber = -1;           ///< 充电桩编号The charging pile number
//    m_VarStationAddress;    ///站地址  The station address
    m_nStationAddress = -1;             ///< 站地址  The station address
}

/*!
 * \brief  功能概述 设定驱动类型 采集/转发
 * \param  参数描述 nDriverType_是驱动的类型
 * \return 返回值描述 设置成功返回true，否则返回false
 * \author zzy
 * \date   2015/3/18
 */
bool CPRTVBase::SetDriverType(const int nDriverType_)
{
    Q_ASSERT( (Collect_Type==nDriverType_) || (Transform_Type==nDriverType_) );
//    assert( (Collect_Type==nDriverType_) || (Transform_Type==nDriverType_) );
    m_nDriverType = nDriverType_;
    return true;
}

/*!
 * \brief  功能概述 获取驱动的类型
 * \param  参数描述 无
 * \return 返回值描述 返回驱动的类型，采集或转发
 * \author zzy
 * \date   2015/3/18
 */
int CPRTVBase::GetDriverType()
{
    return m_nDriverType;
}
/*!
 * \brief  功能概述 设置驱动的指针
 * \param  参数描述 pProtocolI_是驱动的指针
 * \return 返回值描述 成功返回true
 * \author zzy
 * \date   2015/3/18
 */
bool CPRTVBase::SetProtocol(CProtocolI *pProtocolI_)
{
    m_pProtocolI = pProtocolI_;
    return true;
}

/*!
 * \brief  功能概述 获取驱动的指针
 * \param  参数描述 无
 * \return 返回值描述 返回驱动的指针
 * \author zzy
 * \date   2015/3/18
 */
CProtocolI *CPRTVBase::GetProcotolI()
{
    return m_pProtocolI;
}

/*!
 * \brief  功能概述
 * \param  参数描述
 * \return 返回值描述
 * \author zzy
 * \date   2015/3/18
 */
bool CPRTVBase::SetChargePileNumber(const QVariant varChargePileNumber_)
{
    m_VarChargePileNumber = varChargePileNumber_;
    return true;
}

/*!
 * \brief  功能概述
 * \param  参数描述
 * \return 返回值描述
 * \author zzy
 * \date   2015/3/18
 */
QVariant CPRTVBase::GetChargePileNumber()
{
    return m_VarChargePileNumber;
}

/*!
 * \brief  功能概述
 * \param  参数描述
 * \return 返回值描述
 * \author zzy
 * \date   2015/3/18
 */
bool CPRTVBase::SetChargePileNumber(const int nChargePileNumber_)
{
    m_nChargePileNumber = nChargePileNumber_;
    return true;
}

/*!
 * \brief  功能概述
 * \param  参数描述
 * \return 返回值描述
 * \author zzy
 * \date   2015/3/18
 */
int CPRTVBase::GetnChargePileNumber()
{
    return m_nChargePileNumber;
}

/*!
 * \brief  功能概述 设置站地址
 * \param  参数描述 varStationAddress_是站地址
 * \return 返回值描述 成功返回true
 * \author zzy
 * \date   2015/3/18
 */
bool CPRTVBase::SetStationAddress(const QVariant varStationAddress_)
{
    m_VarStationAddress = varStationAddress_;
    return true;
}

/*!
 * \brief  功能概述 获取站地址
 * \param  参数描述 无
 * \return 返回值描述 返回站地址
 * \author zzy
 * \date   2015/3/18
 */
QVariant CPRTVBase::GetStationAddress()
{
    return m_VarStationAddress;
}

/*!
 * \brief  功能概述 设置站地址
 * \param  参数描述 nStationAddress_是站地址
 * \return 返回值描述 成功返回true
 * \author zzy
 * \date   2015/3/18
 */
bool CPRTVBase::SetStationAddress(const int nStationAddress_)
{
    m_nStationAddress = nStationAddress_;
    return true;
}

/*!
 * \brief  功能概述 获取站地址
 * \param  参数描述 无
 * \return 返回值描述 返回站地址
 * \author zzy
 * \date   2015/3/18
 */
int CPRTVBase::GetnStationAddress()
{
    return m_nStationAddress;
}

bool CPRTVBase::SetChargingPileID(const QByteArray baChargingPileID_)
{
    m_baChargingPileID = baChargingPileID_;
    return true;
}

const QByteArray CPRTVBase::GetChargingPileID()
{
    return m_baChargingPileID;
}
int CPRTVBase::GetInterfaceMark() const
{
    return m_nInterfaceMark;
}

void CPRTVBase::SetNInterfaceMark(int nInterfaceMark_)
{
    m_nInterfaceMark = nInterfaceMark_;
}

