/*!
 * \file    CPRTI.h
 * \brief   概述 CPRTI类的头文件
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

#ifndef CPRT_H
#define CPRT_H
#include <QVariant>


class CProtocolI;

/*!
 * \enum   PROTOCOL_TYPE
 * \brief  概述 采集和转发驱动标识
 * \author zzy
 * \date   2015/3/18
 * 详细概述
 */
enum PROTOCOL_TYPE{
    Undefinition_Type = -1,
    Collect_Type,///< 采集类型
    Transform_Type///< 转发类型
};
/*!
 * \class  CPRTI
 * \brief  概述 记录驱动的导出类指针，该类在驱动中创建、初始化
 * \author zzy
 * \date   2015/3/18
 * 详细概述
 */
class CPRTI
{
public:
    virtual bool SetDriverType(const int ) = 0;            ///< 设置驱动类型(采集/转发)
    virtual int GetDriverType() = 0;                        ///< 获取驱动类型(采集/转发)

    virtual bool SetProtocol(CProtocolI *) = 0;             ///< 设置驱动导出类指针
    virtual  CProtocolI *GetProcotolI() = 0;                ///< 获取驱动导出类指针

    virtual bool SetChargePileNumber(const QVariant ) = 0; ///< 设置充电桩编号Set the charging pile number
    virtual QVariant GetChargePileNumber() = 0;             ///< 获取充电桩编号Get the charging pile number

    virtual bool SetChargePileNumber(const int) = 0;      ///< 设置充电桩编号Set the charging pile number
    virtual int GetnChargePileNumber() = 0;                  ///< 获取充电桩编号Get the charging pile number

    virtual bool SetStationAddress(const QVariant ) = 0;   ///< 设置站地址 Set the station address
    virtual QVariant GetStationAddress() = 0;               ///< 获取站地址Get the station address

    virtual bool SetStationAddress(const int ) = 0;        ///< 设置站地址 Set the station address
    virtual int GetnStationAddress() = 0;                    ///< 获取站地址Get the station address

    virtual bool SetChargingPileID(const QByteArray strChargingPileID_) = 0;
    virtual const QByteArray GetChargingPileID() = 0;

    virtual int GetInterfaceMark() const = 0;
    virtual void SetNInterfaceMark(int GetInterfaceMark) = 0;

};

#endif // CPRT_H
