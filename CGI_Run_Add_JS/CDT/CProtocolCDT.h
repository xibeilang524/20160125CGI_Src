#ifndef CPROTOCOLCDT_H
#define CPROTOCOLCDT_H

#include "../PortInclude/cprotocolbase.h"
#include "../PortInclude/CBufferPool.h"
#include <QMutex>
#include <QTimerEvent>
#include <QDebug>

#include "CCDTQuery.h"
#include "CCDTResponse.h"
#include "CPointNodeCDT.h"


#define MinFrameLength 12

/*!
 \brief CDT协议管理类

 \class CProtocolCDT CProtocolCDT.h "CProtocolCDT.h"
*/
class CProtocolCDT : public CProtocolBase
{
    Q_OBJECT
public:
    explicit CProtocolCDT(QObject *parent = 0);
public:
//! 供运行系统调用的函数
    bool OnCreateChannel(const QString strChannelFileName_, CRTDBI *pDatabaseReturn);//建立通道并返回通道自己的实时库  实时数据库
    bool OnCreateChannel(const QDomElement &ChannelElement_,ChannelType nChannelType_, int nComOrCanNumber_,CRTDBI *pDatabaseReturn_);
    bool OnTagChange(CPointBase *pPointBase_, QVariant VarSet_);
public slots:
    void ProcessRespond(QByteArray data_);
    void slotConnect(bool isConnected_);
//! end 供运行系统调用的函数
//!
//!
signals:

public slots:
private:
    enum {BUFFER_SIZE=256};
    enum {MAX_FRAME = 1024*4};
    QMutex m_Mutex;                     ///< 解析报文的锁
    CBaseFrame      m_bufferReceive;    ///< 接收充电桩报文的缓冲区
    CCDTQuery       m_CDTQuery;         ///< 发送报文
    CCDTResponse    m_CDTResponse;      ///< 接收报文解析
    int             m_TS2;              ///< 子站时钟
    int             m_T0;               ///<等待时间
    int m_nTargetStationAddress;        ///< 目的站地址
    CPointNodeCDT  m_PointNodeCDT;      ///< 点的管理类

    int IsAFrame(BYTE *pInfo, int nInfoLength);

    bool IsValidInfoWord(const void *pInfo);
    void ProcessResponse(CCDTResponse &response_);
    void ProcessFrameExtend(CCDTResponse &response_);
    void ProcessRMInfoWord(CBaseFrame &frameInfoWord, int n_SrcStationAddress_);
    void ProcessTMInfoWord(CBaseFrame &frameInfoWord, int n_SrcStationAddress_);
    void ProcessRSInfoWord(CBaseFrame &frameInfoWord, int n_SrcStationAddress_);
    void ProcessRPInfoWord(CBaseFrame &frameInfoWord, int n_SrcStationAddress_);
    void ProcessWLInfoWord(CBaseFrame &frameInfoWord, int n_SrcStationAddress_);
    void ProcessSoeInfoWord(CBaseFrame &frameInfoWord, int n_SrcStationAddress_);
    void ProcessTimeBackInfoWord(CBaseFrame &frameInfoWord, int n_SrcStationAddress_);
    void ProcessSTInfoWord(CBaseFrame &frameInfoWord, int n_SrcStationAddress_);
    void ProcessRRInfoWord(CBaseFrame &frameInfoWord, int n_SrcStationAddress_);
    void ProcessFrequencyInfoWord(CBaseFrame &frameInfoWord, int n_SrcStationAddress_);
    void InitPointMap(const QDomElement &DeviceListElement_);
    void CGI_InitPointMap(const QDomElement &DeviceListElement_);
    void InitPointMessage(const QDomElement &PointElem_, TAG_TYPE nTagType_, int nSrcStationAddress_);
    CPointCDT *FindPoint(int nSrcStationAdd_, int nStart_, int nPoint);
    unsigned int BCD2Int(const void *pInfo, size_t nSize);
};

#endif // CPROTOCOLCDT_H
