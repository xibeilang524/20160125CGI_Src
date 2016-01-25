#ifndef CALLPOWERFUL_PROTOCOL_H
#define CALLPOWERFUL_PROTOCOL_H

#include "../PortInclude/cprotocolbase.h"
#include "CAllPowerful_Query.h"
#include "CAllPowerful_Response.h"
#include <QMutex>
#include <QTimerEvent>

class CAllPowerful_Protocol : public CProtocolBase
{
    Q_OBJECT
public:
    explicit CAllPowerful_Protocol(QObject *parent = 0);
    // 供运行系统调用的函数
    bool OnCreateChannel(const QString strChannelFileName_,CRTDBI *pDatabaseReturn);
    bool OnCreateChannel(const QDomElement &ChannelElement_,ChannelType nChannelType_, int nComOrCanNumber_,CRTDBI *pDatabaseReturn_);
    bool OnTagChange(CPointBase *pPointBase_, QVariant VarSet_);
public slots:
    void ProcessRespond(QByteArray Respond_);
    void slotConnect(bool);
    // end 供运行系统调用的函数

public:
    void timerEvent(QTimerEvent *event);
signals:

public slots:
private:
    int m_nTimerID;///< 定时器ID
    int m_nLastSendFrameTime;///< 最后发送时间
    QMutex m_mutexProcessData;
    enum {BUFFER_SIZE=256};
    enum {MAX_FRAME = 1024*4};
    void ProcessRespond(CAllPowerful_Response &response);
    CBaseFrame            m_bufferReceive; ///< 接收充电桩报文的缓冲区
    CAllPowerful_Query   *m_pCurrentQuery;
    CAllPowerful_Query    m_Query;         ///< 发送报文
    CAllPowerful_Response m_Response;      ///< 接收报文解析

    bool Add(const QByteArray &baSendFrmae_, CAllPowerful_Point *pPoint_);

    QMap<QByteArray, CAllPowerful_Query*> m_AllPowerful_QueryMAP;/// QByteArray is send frame.
    void InitQuery(const QDomElement &DeviceListElement_);
    void Polling();
    quint64 SendFrame(CAllPowerful_Query *pQuery_);

};

#endif // CALLPOWERFUL_PROTOCOL_H
