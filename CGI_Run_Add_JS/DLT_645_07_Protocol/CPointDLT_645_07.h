/*!
 * \file    CPointDLT_645_07.h
 * \brief   概述 CPointDLT_645_07类的头文件
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

#ifndef CPOINTDLT_645_07_H
#define CPOINTDLT_645_07_H

#include "../PortInclude/cpointbase.h"

/*!
 * \class  CPointDLT_645_07
 * \brief  概述 DLT/645-07协议点的参数类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CPointDLT_645_07 : public CPointBase
{
public:
    CPointDLT_645_07();
    int GetDeviceAddress();

    BYTE m_bDI3;
    BYTE m_bDI2;
    BYTE m_bDI1;
    BYTE m_bDI0;
    BYTE m_bDeviceAddr[6];///< 设备地址
    int m_nByteCount;///< 字节长度
    int m_nDecimalPlaces;///< 小数点位数
    unsigned int m_unDI;///< 4个DI强转无符号整形
    QByteArray m_arrayDeviceAddr;///< 设备地址 与报文中的设备地址一致 6字节
    QString m_strDeviceAddr;///< 设备地址 与报文中的设备地址一致 6字节
    QString m_strDI;///< 点在报文中的数据标识
    int m_nStartByte;///< 起始字节

};

#endif // CPOINTDLT_645_07_H
