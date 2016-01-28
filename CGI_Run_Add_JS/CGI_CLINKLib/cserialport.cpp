/*!
 * \file    cserialport.cpp
 * \brief   概述 CSerialPort类的源文件
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

#include "cserialport.h"
#include <QDebug>

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 parent是父对象的指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
CSerialPort::CSerialPort(QObject *parent) :
    QSerialPort(parent)
{
}

/*!
 * \brief  功能概述 打开串口
 * \param  参数描述 strPortName_是串口名，strBaudNo_是波特率，strDataBits_是数据位，strStopBits_是停止位，strParity_是校验方式
 * \return 返回值描述 成功打开返回true，若打开失败则返回false
 * \author zzy
 * \date   2015/5/27
 */
bool CSerialPort::OpenSerialPort(const QString strPortName_, const QString strBaudNo_,
                                 const QString strDataBits_, const QString strStopBits_, const QString strParity_)
{
    InitBaudNo(strBaudNo_);
    InitDataBits(strDataBits_);
    InitStopBits(strStopBits_);
    InitParity(strParity_);

    ///打开串口
    QString tempPortName = strPortName_;
    setPortName(strPortName_);
    if (!open(QIODevice::ReadWrite)) {
        qDebug()<<QString("Can't open %1, error code %2").arg(portName()).arg(error());
        int nPortNumber = tempPortName.remove("COM").toInt();
        QString strPortName2 = tr("ttyS%1").arg(nPortNumber);
        setPortName(strPortName2);
        if (!open(QIODevice::ReadWrite)) {
            qDebug()<<QString("Can't open %1, error code %2").arg(portName()).arg(error());
            QString strPortName3 = tr("ttyO%1").arg(nPortNumber);
            setPortName(strPortName3);
            if (!open(QIODevice::ReadWrite)) {
                qDebug()<<QString("Can't open %1, error code %2").arg(portName()).arg(error());
                return false;
            }
        }
    }
    ///  设置波特率
    setBaudRate((QSerialPort::BaudRate)m_nBaudNo);

    ///  设置数据位
    setDataBits((QSerialPort::DataBits)m_nDataBits);

    ///  设置停止位
    setStopBits((QSerialPort::StopBits)m_nStopBits);

    ///  设置校验方式
    setParity((QSerialPort::Parity)m_nParity);

    if (!setFlowControl(QSerialPort::NoFlowControl)) {
        qDebug()<<QString("Can't set no flow control to port %1, error code %2").arg(portName()).arg(error());
        return false;
    }
    return true;

}

/*!
 * \brief  功能概述 初始化波特率
 * \param  参数描述 strBaudNo_是组态时组态的串口波特率
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CSerialPort::InitBaudNo(const QString &strBaudNo_)
{
    if (strBaudNo_ == "1200")
    {
        m_nBaudNo = QSerialPort::Baud1200;
        return;
    }else if (strBaudNo_ == "2400")
    {
        m_nBaudNo = QSerialPort::Baud2400;
        return;
    }else if (strBaudNo_ == "4800")
    {
        m_nBaudNo = QSerialPort::Baud4800;
        return;
    }else if (strBaudNo_ == "9600")
    {
        m_nBaudNo = QSerialPort::Baud9600;
        return;
    }else if (strBaudNo_ == "19200")
    {
        m_nBaudNo = QSerialPort::Baud19200;
        return;
    }else if (strBaudNo_ == "38400")
    {
        m_nBaudNo = QSerialPort::Baud38400;
        return;
    }else if (strBaudNo_ == "57600")
    {
        m_nBaudNo = QSerialPort::Baud57600;
        return;
    }else if (strBaudNo_ == "115200")
    {
        m_nBaudNo = QSerialPort::Baud115200;
        return;
    }else
    {
        m_nBaudNo = QSerialPort::UnknownBaud;
        return;
    }
}

/*!
 * \brief  功能概述 初始化停止位
 * \param  参数描述 strDataBits_是组态时组态的数据位
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CSerialPort::InitDataBits(const QString &strDataBits_)
{
    if (strDataBits_ == "5")
    {
        m_nDataBits = QSerialPort::Data5;
        return;
    }else if (strDataBits_ == "6")
    {
        m_nDataBits = QSerialPort::Data6;
        return;
    }else if (strDataBits_ == "7")
    {
        m_nDataBits = QSerialPort::Data7;
        return;
    }else if (strDataBits_ == "8")
    {
        m_nDataBits = QSerialPort::Data8;
        return;
    }else
    {
        Q_ASSERT(false);
        m_nDataBits = QSerialPort::UnknownDataBits;
        return;
    }
}

/*!
 * \brief  功能概述 初始化停止位
 * \param  参数描述 strStopBits_是组态时组态的停止位
 * \return 返回值描述
 * \author zzy
 * \date   2015/5/27
 */
void CSerialPort::InitStopBits(const QString &strStopBits_)
{
    if (strStopBits_ == "1")
    {
        m_nStopBits = QSerialPort::OneStop;
        return;
    }else if (strStopBits_ == "1.5")
    {
        m_nStopBits = QSerialPort::OneAndHalfStop;
        return;
    }else if (strStopBits_ == "2")
    {
        m_nStopBits = QSerialPort::TwoStop;
        return;
    }else
    {
        Q_ASSERT(false);
        m_nStopBits = QSerialPort::UnknownStopBits;
        return;
    }
}

/*!
 * \brief  功能概述 初始化校验位
 * \param  参数描述 strParity_是组态时组态的校验位
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CSerialPort::InitParity(const QString &strParity_)
{
    if (strParity_ == "NONE")///<zzy 2015/1/14 修改:无校验
//    if (strParity_ == "无校验")///<zzy 2015/1/14 修改:无校验
    {
        m_nParity = QSerialPort::NoParity;
        return;
    }else if (strParity_ == "EVEN")///<zzy 2015/1/14 修改:偶校验
//    }else if (strParity_ == "偶校验")///<zzy 2015/1/14 修改:偶校验
    {
        m_nParity = QSerialPort::EvenParity;
        return;
    }else if (strParity_ == "ODD")///<zzy 2015/1/14 修改:奇校验
//    }else if (strParity_ == "奇校验")///<zzy 2015/1/14 修改:奇校验
    {
        m_nParity = QSerialPort::OddParity;
        return;
    }else if (strParity_ == "Space")///<zzy 2015/1/14 修改:
    {
        m_nParity = QSerialPort::SpaceParity;
        return;
    }else if (strParity_ == "Mark")///<zzy 2015/1/14 修改:
    {
        m_nParity = QSerialPort::MarkParity;
        return;
    }else
    {
        Q_ASSERT(false);
        m_nParity = QSerialPort::UnknownParity;
        return;
    }
}

/*!
 * \brief  功能概述 发送报文
 * \param  参数描述 data是发送数据的头指针，maxSize是发送的数据字节个数
 * \return 返回值描述 返回发送的字节个数
 * \author zzy
 * \date   2015/5/27
 */
qint64 CSerialPort::SetWriteData(const char *data, qint64 maxSize)
{
    return writeData(data,maxSize);
}

/*!
 * \brief  功能概述 发送报文
 * \param  参数描述 dataArray是发送的数据
 * \return 返回值描述 返回发送的字节个数
 * \author zzy
 * \date   2015/5/27
 */
qint64 CSerialPort::SetWriteData(QByteArray &dataArray)
{
    return writeData(dataArray.data(), dataArray.count());
}

/*!
 * \brief  功能概述 发送报文
 * \param  参数描述 data是发送数据的头指针，maxSize是发送的数据字节个数
 * \return 返回值描述 返回发送的字节个数
 * \author zzy
 * \date   2015/5/27
 */
qint64 CSerialPort::SetWriteData(const BYTE *data, qint64 maxSize)
{
    return writeData((char *)data,maxSize);
}
