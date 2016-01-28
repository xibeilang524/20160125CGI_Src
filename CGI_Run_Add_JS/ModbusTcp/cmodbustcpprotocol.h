/*!
 * \file    CModbusTcpProtocol.h
 * \brief   概述 CModbusTcpProtocol类的头文件
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

#ifndef CModbusTcpProtocol_H
#define CModbusTcpProtocol_H

#include <QMutex>
#include <QMap>
#include <QTcpSocket>
#include <QFile>
#include "../PortInclude/cprotocolbase.h"
#include "cpointbasemodbusrtu.h"
#include <QTimerEvent>
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
};

/*!
 * \class  CModbusTcpProtocol
 * \brief  概述 ModbusTcp驱动类
 * \author zzy
 * \date   2015/5/25
 * 详细概述
 */
class CModbusTcpProtocol : public CProtocolBase
{
    Q_OBJECT
public:
    explicit CModbusTcpProtocol(QObject *parent);
    ~CModbusTcpProtocol();
    void timerEvent(QTimerEvent *evnet);
public:
//! 供运行系统调用的函数
    bool OnCreateChannel(const QString strChannelFileName_,CRTDBI *pDatabaseReturn);
    bool OnCreateChannel(const QDomElement &ChannelElement_,ChannelType nChannelType_, int nComOrCanNumber_,CRTDBI *pDatabaseReturn_);
    bool OnTagChange(CPointBase *pPointBase_, QVariant VarSet_);
public slots:
    void ProcessRespond(QByteArray Respond);
    void slotConnect(bool);
//! end 供运行系统调用的函数
//!
//! **************************************** Modbus Rtu驱动部分
//!
    /////////////////////////////////////////////////

private:
    void SetInitSend_RX_RC_FrameList();
    WORD CRC(const BYTE *pInfo, int nLen);
    void Make_RX_RC_Query(QMap<int, CPointBaseModbusRtu *> *pFunction_);
    void Make_RK_Query(QString strLink_,int nDeviceAdd_,bool bOn_);
    void Make_RS_Query(QString strLink_, int nDeviceAdd_, QVariant varValue_);
    void Make_RX_TCP_Query(const char *szSend_, int length_, ModbusRtuQuery *pQuery_);
    void Make_RC_TCP_Query(const char *szSend_, int length_, ModbusRtuQuery *pQuery_);
    void Make_RS_TCP_Query(const char *szSend_, int length_, ModbusRtuQuery *pQuery_);
    void Make_RK_TCP_Query(const char *szSend_, int length_, ModbusRtuQuery *pQuery_);
    void SetFunctionMap(const QDomElement &DeviceListElement_);
    void CGI_SetFunctionMap(const QDomElement &DeviceListElement_);
private:
    ModbusRtuQuery *m_pModbusRtuQuery;///< 下发的报文
    typedef QMap<int,CPointBaseModbusRtu *> FunctinMap;///< 功能码Map
    QList<ModbusRtuQuery *> Send_RX_RC_FrameList;///< 遥信、遥测报文  采集信息报文
    QList<ModbusRtuQuery *> Send_RK_RS_FrameList;///< 遥控、遥设报文  控制、设置报文
    QMap<int,FunctinMap *> Device_RX_Map;///< 设备地址，功能码1、2的设备Map
    QMap<int,FunctinMap *> Device_RC_Map;///< 设备地址，功能码3、4的设备Map
    QMap<int,FunctinMap *> Device_RK_Map;///< 设备地址，功能码5的设备Map
    QMap<int,FunctinMap *> Device_RS_Map;///< 设备地址，功能码16的设备Map
    QString m_strLink;///< 记录遥控、遥设的Link名字,用于设置值得处理
    int SendFrameListNumber;
    QMutex ShowMessageMutex;
    QMutex m_mutexProcessData;
    QTimer *m_pTimer;
    int m_nTimerID;
//! end Modbus Rtu驱动部分*********************************
/// 本库中自己调用的函数和变量

    void ProcessRespond_SetValue(QByteArray &Respond_);
private slots:
    void OnTimeOut();
    void Polling(); ///周期调用此函数,下发命令帧和等待返回帧
};

#endif // CModbusTcpProtocol_H
