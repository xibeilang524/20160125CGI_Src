#ifndef CCHANNELTHREAD_CGI_H
#define CCHANNELTHREAD_CGI_H

#include <QThread>
#include <QDomElement>
#include <QLibrary>
#include "Src/crtdbbase.h"

#ifndef ChannelType_H
#define ChannelType_H
enum ChannelType{
    ChannelType_COM = 0,
    ChannelType_NET,
    ChannelType_CAN,
    ChannelType_Monitor104
};
#endif

class CProtocolI;

class CChannelThread_CGI : public QThread
{
    Q_OBJECT
public:
    explicit CChannelThread_CGI(CRTDBBase *pRealTimeDB_,int nChannelNumber_,ChannelType nChannelType_,QObject *parent = 0);
    explicit CChannelThread_CGI(CRTDBBase *pRealTimeDB_,int nChannelNumber_,ChannelType nChannelType_,int nComOrCanNumber_,QObject *parent = 0);
    void SetMonitorIPAddress(QString strMonitorIPAddress_);
    void SetDomElement(QDomElement ChannelElement_);
    void InitChannel();
    QDomElement m_ChannelElement;
private:
    void run() Q_DECL_OVERRIDE;

signals:

public slots:
private:
    CProtocolI *m_pProtocolI;///< 库中导出的类
    QString m_LibName;///< 库文件的路径
    QLibrary mylib;///< 库文件
    QString m_strMonitorIPAddress;///< Monitor的IP地址
    CRTDBBase *m_pRealTimeDB;
    int m_nChannelNumber;///< 通道编号，按顺序分配的
    int m_nChannelType;
    int m_nComOrCanNumber;///< 串口号或者Can口号，只有串口和can类型才有串口或can号 net类型没有，这一点由工程文件决定
};

#endif // CCHANNELTHREAD_CGI_H
