/*!
 * \file    CPRTVBase.h
 * \brief   概述 CPRTVBase类的头文件
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

#ifndef CPRTVBASE_H
#define CPRTVBASE_H

#include "../PortInclude/CPRTI.h"

/*!
 * \class  CPRTVBase
 * \brief  概述 通道的类
 * \author zzy
 * \date   2015/3/18
 * 详细概述
 */
class CPRTVBase : public CPRTI
{
public:
    CPRTVBase();
    bool SetDriverType(const int nDriverType_);
    int GetDriverType();

    bool SetProtocol(CProtocolI *pProtocolI_);
     CProtocolI *GetProcotolI();

    bool SetChargePileNumber(const QVariant varChargePileNumber_);
    QVariant GetChargePileNumber();

    bool SetChargePileNumber(const int nChargePileNumber_);
    int GetnChargePileNumber();

    bool SetStationAddress(const QVariant varStationAddress_);
    QVariant GetStationAddress();

    bool SetStationAddress(const int nStationAddress_);
    int GetnStationAddress();

    bool SetChargingPileID(const QByteArray baChargingPileID_);
    const QByteArray GetChargingPileID();

    int GetInterfaceMark() const;
    void SetNInterfaceMark(int nInterfaceMark_);

private:
    CProtocolI  *m_pProtocolI;          ///< 驱动导出类指针
    int         m_nDriverType;          ///< 采集/转发类型标识
    QVariant    m_VarChargePileNumber;  ///< 充电桩编号The charging pile number
    int         m_nChargePileNumber;    ///< 充电桩编号The charging pile number
    QVariant    m_VarStationAddress;    ///< 站地址  The station address
    int         m_nStationAddress;      ///< 站地址  The station address
    QByteArray  m_baChargingPileID;     ///< 充电桩ID
    int         m_nInterfaceMark;       ///< 充电机接口标识
};

#endif // CPRTVBASE_H
