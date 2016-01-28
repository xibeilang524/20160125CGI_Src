/*!
 * \file    CModbusRtuProtocol.h
 * \brief   概述 CModbusRtuProtocol类的头文件
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

#ifndef CMODBUSRTUPROTOCOL_H
#define CMODBUSRTUPROTOCOL_H

#include <QMutex>
#include <QMap>
#include "../PortInclude/cprotocolbase.h"
#include "cpointbasemodbusrtu.h"
#include "../PortInclude/CBaseFrame.h"
class CSerialPort;
class CValueI;
class CRTDBI;
class CPointBase;
class QTimer;
class QDomElement;

/*!
 * \class  ModbusRtuQuery
 * \brief  概述 ModbusRtu报文类
 * \author zzy
 * \date   2015/5/25
 * 详细概述
 */
class ModbusRtuQuery
{
public:
    ModbusRtuQuery();

    QByteArray szSend;      ///< 发送帧
    int m_nResponseLength;  ///< 应返回帧长度
    int m_nStartAddress;    ///< 报文起始地址
    int m_nResponseZiJieChangDu;//// 数据区字节长度
    BYTE m_nFunction;
};

/*!
 * \class  CModbusRtuProtocol
 * \brief  概述 ModbusRtu驱动类
 * \author zzy
 * \date   2015/5/25
 * 详细概述
 */
class CModbusRtuProtocol : public CProtocolBase
{
    Q_OBJECT
public:
    explicit CModbusRtuProtocol(QObject *parent);
public:
//! 供运行系统调用的函数
    virtual bool OnCreateChannel(const QString strChannelFileName_, CRTDBI *pDatabaseReturn);//建立通道并返回通道自己的实时库  实时数据库
    virtual bool OnCreateChannel(const QDomElement &ChannelElement_,ChannelType nChannelType_, int nComOrCanNumber_,CRTDBI *pDatabaseReturn_);
    virtual bool OnTagChange(CPointBase *pPointBase_, QVariant VarSet_);
public slots:
    void ProcessRespond(QByteArray data);
    void slotConnect(bool);
//! end 供运行系统调用的函数
//!
//! **************************************** Modbus Rtu驱动部分
//!
public slots:
signals:
    void SendFrame();
private:
    void SetInitSend_RX_RC_FrameList();
    WORD CRC(const BYTE *pInfo, int nLen);
    void Make_RX_RC_Query(QMap<int, CPointBaseModbusRtu *> *pFunction_);
    void Make_RK_Query(QString strLink_,int nDeviceAdd_,bool bOn_);
    void Make_RS_Query(QString strLink_, int nDeviceAdd_, QVariant varValue_);
    void SetFunctionMap(const QDomElement &DeviceListElement_);
private:
    CBaseFrame  m_bufferRecieve;     ///< 接收充电桩报文的缓冲区
    ModbusRtuQuery *m_pModbusRtuQuery;///< 下发的报文
    typedef QMap<int,CPointBaseModbusRtu *> FunctinMap;///< 功能码Map
    QList<ModbusRtuQuery *> Send_RX_RC_FrameList;///< 遥信、遥测报文  采集信息报文
    QList<ModbusRtuQuery *> Send_RK_RS_FrameList;///< 遥控、遥设报文  控制、设置报文
    QMap<int,FunctinMap *> Device_RX_Map;///< 设备地址，功能码1、2的设备Map
    QMap<int,FunctinMap *> Device_RC_Map;///< 设备地址，功能码3、4的设备Map
    QMap<int,FunctinMap *> Device_RK_Map;///< 设备地址，功能码5的设备Map
    QMap<int,FunctinMap *> Device_RS_Map;///< 设备地址，功能码16的设备Map
    QString m_strLink;///< 记录遥控、遥设的Link名字,用于设置值得处理
    int m_nLastTime;///< 最后发送时间
    int SendFrameListNumber;///< 发送报文list号
    QMutex m_mutexProcessData;///< 解析锁
    QTimer *m_pTimer;///< 定时器
    QByteArray m_baReceiveFrame;///< 接收到的报文
//! end Modbus Rtu驱动部分*********************************
/// 本库中自己调用的函数和变量

private:
private slots:
    void OnTimeOut();
    void Polling();
};

#endif // CMODBUSRTUPROTOCOL_H
