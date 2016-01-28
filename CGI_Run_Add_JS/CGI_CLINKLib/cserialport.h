/*!
 * \file    cserialport.h
 * \brief   概述 CSerialPort类的头文件
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

#ifndef CSERIALPORT_H
#define CSERIALPORT_H

#include <QSerialPort>
#include "../PortInclude/typedef.h"

/*!
 * \class  CSerialPort
 * \brief  概述 继承于QSerialPort的串口类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CSerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit CSerialPort(QObject *parent = 0);
    bool OpenSerialPort(const QString strPortName_, const QString strBaudNo_,
                        const QString strDataBits_, const QString strStopBits_, const QString strParity_);
private:
    int m_nBaudNo;///< 波特率
    void InitBaudNo(const QString &strBaudNo_);
    int m_nDataBits;///< 数据位
    void InitDataBits(const QString &strDataBits_);
    int m_nStopBits;///< 停止位
    void InitStopBits(const QString &strStopBits_);
    int m_nParity;///< 校验方式
    void InitParity(const QString &strParity_);
//! end 串口部分操作******************************************

    qint64 SetWriteData(const char *data, qint64 maxSize);
    qint64 SetWriteData(QByteArray &dataArray);
    qint64 SetWriteData(const BYTE *data,qint64 maxSize);
};

#endif // CSERIALPORT_H
