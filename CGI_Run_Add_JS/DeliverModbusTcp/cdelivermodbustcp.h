/*!
 * \file    cdelivermodbustcp.h
 * \brief   概述 CProtocolDeliverModbusTcp类的头文件
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

#ifndef CDELIVERMODBUSTCP_H
#define CDELIVERMODBUSTCP_H
#include "../PortInclude/cprotocolbase.h"
#include <QTcpServer>
#include <QTcpSocket>
#include "../PortInclude/CBaseFrame.h"
#include <QMutex>
#include <QTimerEvent>

#define MinFrameLength 12

class CPointBaseModbus;
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

#define  SEND_FRAME_MAX_LENGTH  280
/*!
 * \class  CProtocolDeliverModbusTcp
 * \brief  概述 ModbusTcp转发驱动类
 * \author zzy
 * \date   2015/5/25
 * 详细概述
 */
class CProtocolDeliverModbusTcp : public CProtocolBase
{
    Q_OBJECT
public:
    explicit CProtocolDeliverModbusTcp(QObject *parent);
public:///供运行系统调用的函数
    bool OnCreateChannel(const QString strChannelFileName_,CRTDBI *pDatabaseReturn);
    bool OnCreateChannel(const QDomElement &ChannelElement_,ChannelType nChannelType_, int nComOrCanNumber_,CRTDBI *pDatabaseReturn_);
    bool OnTagChange(CPointBase *pPointBase_,QVariant VarSet_);

public slots:
    void ProcessRespond(QByteArray data);
    void slotConnect(bool);

private:
    ModbusRtuQuery *m_pModbusRtuQuery;///< 下发的报文

    QMap<int,CPointBaseModbus *> RXFunctinMap;///< 遥信Map
    QMap<int,CPointBaseModbus *> RCFunctinMap;///< 遥测Map
    QMap<int,CPointBaseModbus *> RKFunctinMap;///< 遥控Map
    QMap<int,CPointBaseModbus *> RSFunctinMap;///< 遥设Map
    QString m_strLink;///< 记录遥控、遥设的Link名字,用于设置值得处理
    int m_nLastTime;
    int SendFrameListNumber;
//Data Member
//! **************************************** Modbus Tcp驱动部分
private:
    void SetFunctionMap(const QDomElement &DeviceListElement_);
    void CGI_SetFunctionMap(const QDomElement &DeviceListElement_);
    void InsertPoint(const QDomElement &PointElem_, const int &nPointType, const int &nDeviceAddress);

private:
    QByteArray m_frameQUery;
    bool MakeRespond(const QByteArray &Command, QByteArray &Respond);///合成帧数据入口函数
    bool MakeResponse(const QByteArray &FrameQuery_, QByteArray &Respond, void *pInfo, int nInfoSize, int nFunction);///合成TCP报文
    void SendFrame(const QByteArray SendFrame_);
    bool MakeStatusResponse(const QByteArray &Command,QByteArray &Respond);//1\2
    bool MakeRegisterResponse(const QByteArray &Command,QByteArray &Respond);//3\4
    bool ForceSinleCoil(const QByteArray &Command,QByteArray &Respond);//5
    bool PresetSinleRegister(const QByteArray &Command,QByteArray &Respond);//6
    bool PresetMultipleRegister(const QByteArray &Command,QByteArray &Respond);//16
    bool MakeErrorRespond(const QByteArray &Command,QByteArray &Respond,int nErrorCode);//错误
    enum
    {
        EXCEPTION_CODE_ILLEGAL_FUNCTION=1,  ///< 非法功能码
        EXCEPTION_CODE_ILLEGAL_DATA_ADDRESS,///< 非法数据地址
        EXCEPTION_CODE_ILLEGAL_DATA_VALUE,  ///< 非法数据值
        EXCEPTION_CODE_SLAVE_DEVICE_FAILURE,///< 从机失败
        EXCEPTION_CODE_SLAVE_DEVICE_BUSY    ///< 从机忙
    };

private:
    unsigned int GetUIntValue_Hl(const QByteArray &data_,int nOffset_,int nSize_);
    void ReverseByte(void *pInfo, int nInfoSize,int nGroupSize=2);
    unsigned int SetBit(unsigned int nData, unsigned int nBit);
    unsigned int ResetBit(unsigned int nData, unsigned int nBit);

public:
    void timerEvent(QTimerEvent *event);
private:
    void ProcessRespond();
    CBaseFrame m_bufferReceive;
    QMutex m_Mutex;
    int m_nTimerID;

    enum {BUFFER_SIZE=256};
    enum {MAX_FRAME = 1024*4};

    void ProcessRespond_Buffer(QByteArray data);
    int IsAFrame(BYTE *pInfo, int nInfoLength);
    unsigned int GetUInt(const BYTE *pBufferHead, const int nInfomationSize, const int nStartByte, const int nSize) const;
};

#endif // CDELIVERMODBUSTCP_H
