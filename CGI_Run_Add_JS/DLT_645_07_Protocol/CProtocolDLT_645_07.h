/*!
 * \file    CProtocolDLT_645_07.h
 * \brief   概述 CProtocolDLT_645_07类的头文件
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

#ifndef CPROTOCOLEMPTY_H
#define CPROTOCOLEMPTY_H

#include "../PortInclude/cprotocolbase.h"
#include "CDLT_645_07Query.h"
#include "CDLT_645_07Response.h"
#include <QTimerEvent>
#include <QMutex>
#include "CPointNode.h"
#define MinFrameLength 12

class CPointDLT_645_07;

/*!
 * \class  CProtocolDLT_645_07
 * \brief  概述 DLT/645-07协议驱动
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CProtocolDLT_645_07 : public CProtocolBase
{
    Q_OBJECT
public:
    explicit CProtocolDLT_645_07(QObject *parent = 0);

    // 供运行系统调用的函数
    bool OnCreateChannel(const QString strChannelFileName_,CRTDBI *pDatabaseReturn);
    bool OnCreateChannel(const QDomElement &ChannelElement_,ChannelType nChannelType_, int nComOrCanNumber_,CRTDBI *pDatabaseReturn_);
    bool OnTagChange(CPointBase *pPointBase_, QVariant VarSet_);
    int IsAFrame(BYTE *pInfo, int nInfoLength);
public slots:
    void ProcessRespond(QByteArray Respond_);
    void slotConnect(bool);
    // end 供运行系统调用的函数

public:
    void SetDataTypeMap(const QDomElement &DeviceListElement_);
    void CGI_SetDataTypeMap(const QDomElement &DeviceListElement_);
    typedef QMap<unsigned int,CPointDLT_645_07 *> DLT_645_07PointMap_2;///< 点MAP
    QMap<QByteArray,DLT_645_07PointMap_2 *> Device_Read_Map;///<e 读点

    QList<CDLT_645_07Query *> DLT_645_07QueryList;///< 报文列表
    QByteArray m_arrayCurrentAddress;///< 当前发送的设备地址的报文
    int m_nSendFrameListNumber;
    void timerEvent(QTimerEvent *event);
    int m_nTimerID;
    QMutex m_mutexProcessData;
    void Polling();
    void DecodeReceive(const QByteArray data);
    QByteArray m_arrayRespondBuffer;///< 接收到报文缓冲区
    int ReceiveIsTrue(const BYTE *pBufferHead, const int nInfomationSize);
    unsigned int GetUInt(const BYTE *pBufferHead, const int nInfomationSize, const int nStartByte, const int nSize) const;
    DWORD MakeSum(const void *pInfo_, size_t nSize_);

    enum {BUFFER_SIZE=256};
    enum {MAX_FRAME = 1024*4};
    void ProcessRespond(CDLT_645_07Response &response);
    CBaseFrame          m_bufferReceive; ///< 接收充电桩报文的缓冲区
    CDLT_645_07Query    m_Query;         ///< 发送报文
    CDLT_645_07Response m_Response;      ///< 接收报文解析
    CPointNode m_PointNode;
};

#endif // CPROTOCOLEMPTY_H
