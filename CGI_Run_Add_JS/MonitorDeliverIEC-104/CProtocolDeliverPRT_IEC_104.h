/*!
 * \file
 * \brief   概述
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/3/17
 *
 * \b       修改记录：
 * \li      2015/3/17
 *  添加注释
 */

#ifndef CPROTOCOLDELIVERPRT_IEC_104_H
#define CPROTOCOLDELIVERPRT_IEC_104_H

#include "cprotocolbase.h"
#include "IEC_104.h"
#include "cpointiec_104.h"
#include "ciec104deliverquery.h"
#include "CWriteFrame.h"

#include <QTcpSocket>
#include <QTcpServer>
#include <QDomElement>
#include <QTimerEvent>
#include <QMutex>
#include <QMutexLocker>

///                 端口号
#define DBPort      9001
#define FramePort   9002

typedef struct _AAALinkLayerConfigInfo
{
    QString strTransmitCauseLen; //传送原因地址长度
    QString strCommAddrLen; //公共地址长度
    QString strInfoAddrLen; //信息体地址长度
    QString strTimeType;    //时标格式
    QString strTimingType;  //校时方式
    QString strCallingType; //总召唤方式
    QString strLinkTimeOut; //连接超时
    QString strSendFrameTimeOut; //发送或测试超时
    QString strNonDataTimeOut;  //无数据超时
    QString strSendSFrameTimeOut; //S帧发送超时
    QString strMaxFrameInx; //最大不同接收序号
    QString strSFrameConfirm; //最大S帧确认数
    QString strASDULen; //ASDU数据单元长度
    QString strKwhCjType; //累积量采集方式

    _AAALinkLayerConfigInfo()
    {

    }

    void SetEmpty()
    {
       strTransmitCauseLen = "";
       strCommAddrLen = "";
       strInfoAddrLen = "";
       strTimeType = "";
       strTimingType = "";
       strCallingType = "";
       strLinkTimeOut = "";
       strSendFrameTimeOut = "";
       strNonDataTimeOut = "";
       strSendSFrameTimeOut = "";
       strMaxFrameInx = "";
       strSFrameConfirm =  "";
       strASDULen = "";
       strKwhCjType = "";
    }

}AAALinkLayerConfigInfo;

/*!
 * \class
 * \brief  概述
 * \author zzy
 * \date   2015/3/17
 * 详细概述
 */
class CProtocolDeliverPRT_IEC_104 : public CProtocolBase
{
    Q_OBJECT
public:
    explicit CProtocolDeliverPRT_IEC_104(QObject *parent = 0);
//! 供运行系统调用的函数
    bool OnCreateChannel(const QString strMonitorIPAddress_,CRTDBI *pDatabaseReturn);//建立通道并返回通道自己的实时库  实时数据库
    bool OnStart(); //启动通道
    bool OnScan();  //扫描通道
    bool OnStop();  //停止通道
    bool OnTagChange(CPointBase *pPointBase_, QVariant VarSet_);
    void SetPRTMap(CPRTMapI *pPRTMapI_);
    qint64 Write(const char * data, qint64 maxSize);
    qint64 Write(const char * data);
    qint64 Write(const QByteArray  byteArray);
    int GetZTChannelNumber();
    QList<int> GetZTDeliversChannelNumberList();
    void WriteMessage(int type,QString message,int nChannelNumber);
    //! end
    void timerEvent(QTimerEvent *event);
    int m_nTimerID;
    QMutex m_Lock;
    QList<QByteArray > m_SendFrameList;
private:
    CWriteFrame *m_pWriteFrame;

    //应用参数
    QMap<int,CPointIEC_104 *>YX_PointMap;//遥信点map
    QMap<int,CPointIEC_104 *>YC_PointMap;//遥测点map
    QMap<int,CPointIEC_104 *>YK_PointMap;//遥控点map
    QMap<int,CPointIEC_104 *>YS_PointMap;//遥设点map
    QMap<int,CPointIEC_104 *>YM_PointMap;//遥脉点map
    typedef QMap<QString,CTagI *> TAG_MAP;
    void InitPointMap(TAG_MAP *pTagMap);
signals:
    void SignalSendFrame(int type,QString message,int nChannelNumber);

public slots:
private slots:
    void ProcessRespond();/// 此槽函数由串口的readyRead()信号触发 处理返回的帧 即解包函数
    void OnConnected();///成功连接服务器
    void displayError(QAbstractSocket::SocketError);
    void TCPConnect();
    void OnserverConnected();
    void SocketDeleteLater();
private:
    int m_nProtcolTimerId;/// 定时器
    QTcpServer *m_pTcpServerDB;     /// @note 9001端口
    QTcpServer *m_pTcpServerFrame;  /// @note 9002端口

    QTcpServer *m_tcpServer;///用于传输实时数据的服务端
    QTcpSocket *m_TcpSocket;///用于传输实时数据的客户端
    QString m_strTcpIp;
    bool bConnectOk;// 是否成功连接服务器 连接未成功前不走规约
    CIEC104DeliverQuery m_IEC104DeliverQuery;///接收到的报文帧
    QByteArray m_ByteArrayFrame;//合成发送的报文帧
    enum{
        BUFFER_SIZE = 256,
    };
private:
    void DecodeFrame(QByteArray &ReceiveData);
    void DecodeIFrame(CIEC104DeliverQuery &Query_);
    void DecodeSFrame(CIEC104DeliverQuery &Query_);
    void DecodeUFrame(CIEC104DeliverQuery &Query_);
    void DecodeStartFrame(CIEC104DeliverQuery &Query_);

    void ZFFrame(QByteArray datas);
private:/// 3.2 常用名词
    int m_nMaxSRValue;//收发最大值
    int m_nUnconfirmIFrame;///未认可的I帧




    int m_nK;//发送方未被确认的I格式的APDU数目 K=12  m_nMaxUnconfirmIFrame
    int m_nW;//接收方最多收到未被确认的I格式的APDU数目 W=8
    int m_nt0;//网络建立连接超时时间 t0<30s
    int m_nt1;//发送或测试APDU的超时时间 t1<20s
    int m_nt2;//接收方无数据报文时确认的超时时间 t2<15s
    int m_nt3;//通道长期空闲时发送确认帧的超时时间 t3<25s  t3<10  10秒钟发送一帧测试报文
    int n_mAPDULen; //APDU最大长度 最大长度253

    //内部控制参数
    int n_mLastTestFrameTime;   //测试帧最后帧接收时间
    int n_mLastFrameTime;       //最后帧接收时间
    int n_mSendIFrameFullTime;  //发送或测试APDU的超时时间
    unsigned char n_mReadFrameDateLen;  //帧第二个字节，即帧长度
    int n_mLastTxTime;         //最后发送帧时间

    int n_mLastReciveIFrameTime; //最后收到的I帧时间，T2比较，并发送S帧确认
    int n_mLastSendIFrameTime;   //最后的发送I帧时间，如果一直没被确认，则初始化链路

    int n_mLastCallDataTime; //最后的总召时间，用于周期总召
    int n_mLastTimingTime;  //最后的对时时间，用于周期校时

    //重启或链路复位标识
    int n_mStartDTWaitIFormatFlag;
    int n_mResetProcessWaitIFormatFlag;
    int n_mResetColseLinkFlag;

    //应用参数
    BYTE m_nCommonAddress; ///公共地址
    int n_mCommonAddrLen; //公共地址长度
    int n_mTransmitCauseLen; //传输原因长度
    int n_mInformationAddrLen; //信息体地址
    int n_mTimeType;           //时标格式  1：CP56 Time2a    0:CP24 Time2a

    //传输控制参数
    int n_mCurrState; //链路当前状态
    WORD m_nSendIdx;   //发送序号
    WORD m_nReciveIdx; //接收序号
    int n_mSub_SendIdx;   //从站发送序号  报文中的序号
    int n_mSub_ReciveIdx; //从站接收序号  报文中的序号
    int m_nAck; //发送S帧的标志

    //标识位参数
    int n_mSyncTimeFlag;
    int n_mCallAllDataFlag;
    int n_mCallKwhDataFalg;

    short int n_mSyncTimeMode;  //校时模式  1:启动或重连时对时     0：周期对时
    int n_mSyncTimeCycle; //校时周期
    short int n_mCallDataMode;  //总召模式  1:启动或重连时总召唤    0：周期总召唤
    int n_mCallDataCycle; //总召周期



    AAALinkLayerConfigInfo g_param;
    bool m_bConfigInitParamOk;
    bool m_bResetFlag; //重置标识
    unsigned char m_nYKProcStep;
    void InitPointMapValue();
    void init104Param();
    bool Send_U_Confirm(bool bStart_, bool bStop_, bool bTest_);
    bool SetData(const BYTE *pSource_, int nLength_);
    void InCrease(WORD &nValue);
    void ResetDataLink();
    void Send_S_Frame(WORD nReceiveNo);
    void ProcessASDU101Type_C_IC_NA_1(ASDU101 &asdu);
    bool Send_IntrogenCallActivationOK(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);
    bool MakeQuery_I(WORD &nSendNo, WORD nReceiveNo, BYTE *pAsdu, int nAsduSize);
    bool Send_I_YX(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);
    bool Send_I_YC(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);
    bool Send_I_YC_Float(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);
    bool Send_IntrogenCallActivationEnd(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);
    void ProcessASDU101Type_C_CI_NA_1(ASDU101 &asdu);
    bool Send_IntrogenCallDDLActivationOK(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);
    bool Send_I_YM(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);
    bool Send_IntrogenCallDDLActivationEnd(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);
    void ProcessASDU101Type_C_CS_NA_1(ASDU101 &asdu);
    bool Send_I_C_CS_NA_1(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);
};

#endif // CPROTOCOLDELIVERPRT_IEC_104_H
