#ifndef DLT645_PROTOCOL_97_H
#define DLT645_PROTOCOL_97_H

#include <QMutex>
#include <QMap>
#include <QVariant>
#include <QTimerEvent>
#include "../../PortInclude/cprotocolbase.h"
#include "../../PortInclude/CBaseFrame.h"
#include "dlt645_point.h"
#include "dlt645_query.h"
#include "dlt645_97_response.h"
#include <QTimer>
#define MinFrameLength 12

class CSerialPort;
class CValueI;
class CRTDBI;
class CPointBase;
//class QTimerEvent;
class QDomElement;
class DLT645_Query;


class DLT645_Protocol_97 : public CProtocolBase
{
    Q_OBJECT
public:
    explicit DLT645_Protocol_97(QObject *parent);
public:
    bool OnCreateChannel(const QString strChannelFile,CRTDBI *pDataBaseReturn);
    bool OnCreateChannel(const QDomElement &ChannelElement_,ChannelType nChannelType_, int nComOrCanNumber_,CRTDBI *pDatabaseReturn_);
    bool OnTagChange(CPointBase *pPointBase_,QVariant varSet_);

public slots:
    void slotConnect(bool result);
    void ProcessRespond(QByteArray data);

public:

    DLT645_Query *passQuery;

    typedef QMap<int,DLT645_Point *> Function;
    QMap<int,DLT645_Point*> Function_YC_Map;
    QList<DLT645_Query *> Send_Framlist;
//    QMap<int, DLT645_Point *> Function_RC_Map;
    QMap<QString,Function > Device_YC_Map;
//    QMap<int,Function *> Device_Map_No2;
//    QMap<int,Function *> Device_Map_No3;

    QString mp_strLink;
    enum {MAX_FRAME = 1024*4};
    int m_nLastTime;  ///最后一次发送时间
    int SendFramListNumber;
    int m_nTimerID;
    QMutex LockData;
    QTimer *pTimer;
    QByteArray nowQuery;
    QByteArray m_responseBuffer;
    CBaseFrame m_arrayRespondBuffer;///< 接收到报文缓冲区
    QByteArray m_arrayCurrentAddress;///< 当前发送的设备地址的报文
    DLT645_97_Response m_response;

    void CGI_SetFunctionMap(const QDomElement &DeviceListElement);
//    void SetInitSend_YC_Framlise();
    void timerEvent(QTimerEvent *event);
    void DecodeReceive(const QByteArray data);
   // void DecodeReceive(const QByteArray data);
    void ProcessRespond(DLT645_97_Response &response);
    int ReceiveIsTrue(const BYTE *pBufferHead_,const int nInfomationSize_);
    unsigned int GetUInt(const BYTE *pBufferHead, const int nInfomationSize, const int nStartByte, const int nSize ) const;

    DWORD MakeSum_(const void *pInfo_, size_t nSize_);
 //   void OnTimeOut();
    void Polling();
};
#endif // DLT645_PROTOCOL_97_H
