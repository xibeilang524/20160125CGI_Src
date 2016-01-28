#ifndef CPROTOCOLDEMO_H
#define CPROTOCOLDEMO_H

#include "../PortInclude/cprotocolbase.h"
#include "CPointNode.h"
#define PROC_STAT     "/proc/stat"
#define PROC_MEMINFO  "/proc/meminfo"

class CProtocolDemo : public CProtocolBase
{
    Q_OBJECT
public:
    explicit CProtocolDemo(QObject *parent = 0);
    // 供运行系统调用的函数
    bool OnCreateChannel(const QString strChannelFileName_,CRTDBI *pDatabaseReturn);
    bool OnCreateChannel(const QDomElement &ChannelElement_,ChannelType nChannelType_, int nComOrCanNumber_,CRTDBI *pDatabaseReturn_);
    bool OnTagChange(CPointBase *pPointBase_, QVariant VarSet_);
public slots:
    void ProcessRespond(QByteArray Respond_);
    void slotConnect(bool);
    // end 供运行系统调用的函数

    void timerEvent(QTimerEvent *event_);
signals:

public slots:
private:
    void CGI_InitPointNode(const QDomElement &DeviceListElement_);
    bool ChangeValue(CPointDemoBase *pPoint);
    int m_nTimerID;
    void Polling();
    CPointNode m_PointNode;
//    typedef QMap<QString, Point> PointMap;
//    QMap<int > m_;

    void GetRand(CPointDemoBase *pPoint_);
    void GetTimeInfo(CPointDemoBase *pPoint_);
    void GetCPUInfo(CPointDemoBase *pPoint_);
    void GetMemoryInfo(CPointDemoBase *pPoint_);

};

#endif // CPROTOCOLDEMO_H
