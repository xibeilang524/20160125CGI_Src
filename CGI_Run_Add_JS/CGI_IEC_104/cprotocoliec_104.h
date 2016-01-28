/*!
 * \file    cprotocoliec_104.h
 * \brief   概述 CProtocolIEC_104类的头文件
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

#ifndef CPROTOCOLIEC_104_H
#define CPROTOCOLIEC_104_H

/** *
 * 发送站和接收站在某个具体时间段内(超时时间t3)没有数据传输(或超时)会启动测试过程.每一帧的接收(I帧,S帧或U帧)会重新计时t3.
 * 发送站和接收站要独立地监视连接情况,一旦接收到对方发送过来的测试帧,就必须回答测试确认,而且本方就不需要再发送测试帧.
 *
 * 在链接建立后就进入检查通道空闲的超时时间,一旦空闲时间大于t3时就发出测试命令.
 * 接收方收到任何一条I格式、S格式、U格式报文都将使t3清零重新开始计时.
 * 控制站和被控站都可以发送测试确认命令.一旦其中一方已经发出了测试命令,另一方必须回答且不需要再发送测试命令,用于测试链路是否完好
 *
 *
 * */

#include <QTcpSocket>
#include <QTimerEvent>
#include <QDomElement>
#include <QMutex>

#include "../PortInclude/cprotocolbase.h"
#include "IEC_104.h"
#include "ciec104query.h"
#include "CIEC104Response_new.h"
#include "ciec104response.h"

#define ABS(a)  (((a)<0)?(-(a)):(a))
#define ABSDEC(a,b)	(((a)>=(b)) ? ((a)-(b)):((b)-(a)))

/*!
 * \struct _AAALinkLayerConfigInfo
 * \brief  概述 IEC-104协议参数结构体
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
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
class CPointIEC_104;

/*!
 * \class  CProtocolIEC_104
 * \brief  概述 IEC-104协议驱动类
 * \author zzy
 * \date   2015/5/25
 * 详细概述
 */
class CProtocolIEC_104 : public CProtocolBase
{
    Q_OBJECT
public:
    explicit CProtocolIEC_104(QObject *parent = 0);
//! 供运行系统调用的函数
    bool OnCreateChannel(const QString strChannelFileName_,CRTDBI *pDatabaseReturn);//建立通道并返回通道自己的实时库  实时数据库
    bool OnCreateChannel(const QDomElement &ChannelElement_,ChannelType nChannelType_, int nComOrCanNumber_,CRTDBI *pDatabaseReturn_);

    bool OnTagChange(CPointBase *pPointBase_, QVariant VarSet_);
public slots:
    virtual void ProcessRespond(QByteArray data);
    virtual void slotConnect(bool);
//! end
//!
signals:
    void signal_YS_Change(CPointBase *pPointBase_, QVariant VarSet_);
    void signal_YK_Change(CPointBase *pPointBase_, QVariant VarSet_);
private slots:
    void slot_YS_Change(CPointBase *pPointBase_, QVariant VarSet_);
    void slot_YK_Change(CPointBase *pPointBase_, QVariant VarSet_);

/** * IEC-104驱动部分
 * */
private://函数
    void CGI_InitPointMap(const QDomElement &DeviceListElement_);
    void InsertPoint(const QDomElement &PointElem_, int nPointType, int nDeviceAddress);
    void CGI_InsertPoint(const QDomElement &PointElem_, int nPointType, int nDeviceAddress);
    void ConnectLink();///发送启动U帧 连接链路
    void timerEvent(QTimerEvent *event);
    bool SendFrame(CIEC104Query &frameSend);
//    void DecodeFrame(QByteArray &ReceiveData);
    void DecodeFrame(CIEC104Response_new &response);
    int FindATrueFrame(BYTE *pInfo, int nInfoLength);
    void DecodeSFrame(CIEC104Response_new &response);
    void DecodeIFrame(CIEC104Response_new &response);
    void DecodeUFrame(CIEC104Response_new &response);
    void InCrease(WORD &nValue);
    void ResetDataLink();
    void InitIEC104();
    void Protocal_TX();
    void receiveTimeOut();
    bool ProtCallAllData();
    bool ProtCallKwhData();
    void SendOneSFrame();
    void CheckProcState();
    void init104Param();
    void setConfigProtocolInitParam(const AAALinkLayerConfigInfo &para);
private://解析I帧
    void ProcessASDU101_Type232(ASDU101 &asdu);
    void ProcessASDU101_Type021(ASDU101 &asdu);
    void ProcessASDU101_Type020(ASDU101 &asdu);
    void ProcessASDU101_Type019(ASDU101 &asdu);
    void ProcessASDU101_Type018(ASDU101 &asdu);
    void ProcessASDU101_Type017(ASDU101 &asdu);
    void ProcessASDU101_Type016(ASDU101 &asdu);
    void ProcessASDU101_Type015(ASDU101 &asdu);
    void ProcessASDU101_Type040(ASDU101 &asdu);
    void ProcessASDU101_Type039(ASDU101 &asdu);
    void ProcessASDU101_Type038(ASDU101 &asdu);
    void ProcessASDU101_Type037(ASDU101 &asdu);
    void ProcessASDU101_Type036(ASDU101 &asdu);
    void ProcessASDU101_Type035(ASDU101 &asdu);
    void ProcessASDU101_Type034(ASDU101 &asdu);
    void ProcessASDU101_Type033(ASDU101 &asdu);
    void ProcessASDU101_Type032(ASDU101 &asdu);
    void ProcessASDU101_Type031(ASDU101 &asdu);
    void ProcessASDU101_Type030(ASDU101 &asdu);
    void ProcessASDU101_Type014(ASDU101 &asdu);
    void ProcessASDU101_Type013(ASDU101 &asdu);
    void ProcessASDU101_Type012(ASDU101 &asdu);
    void ProcessASDU101_Type011(ASDU101 &asdu);
    void ProcessASDU101_Type010(ASDU101 &asdu);
    void ProcessASDU101_Type008(ASDU101 &asdu);
    void ProcessASDU101_Type007(ASDU101 &asdu);
    void ProcessASDU101_Type006(ASDU101 &asdu);
    void ProcessASDU101_Type005(ASDU101 &asdu);
    void ProcessASDU101_Type004(ASDU101 &asdu);
    void ProcessASDU101_Type003(ASDU101 &asdu);
    void ProcessASDU101_Type001(ASDU101 &asdu);
    void ProcessASDU101_Type002(ASDU101 &asdu);
    void ProcessASDU101_Type009(ASDU101 &asdu);


private://变量
    enum{MAX_FRAME=1024*10};
    QMutex m_mutexProcessData;
    CBaseFrame      m_bufferReceive;
    CIEC104Query    m_IEC104Query;//发送的报文
    CIEC104Response_new m_IEC104Response;//接收到的报文
    int m_nProtcolTimerId;
    bool m_bConnectOk;// 是否成功连接服务器 连接未成功前不走规约
    bool m_bTimerIsStart;
    bool m_bDataLink;//协议链路
    int m_nMaxSRValue;//收发最大值
    int m_nUnconfirmIFrame;///未认可的I帧
    QMap<int,CPointIEC_104 *>YX_PointMap;//遥信点map
    QMap<int,CPointIEC_104 *>YC_PointMap;//遥测点map
    QMap<int,CPointIEC_104 *>YK_PointMap;//遥控点map
    QMap<int,CPointIEC_104 *>YS_PointMap;//遥设点map
    QMap<int,CPointIEC_104 *>YM_PointMap;//遥脉点map

signals:

public slots:
private:/// 3.2 常用名词
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
    int n_mSub_SendIdx; //从站发送序号
    int n_mSub_ReciveIdx; //从站接收序号
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

    int m_nLinkCount;///<zzy 2015/1/14 修改:超过三次U启动帧无返回则重新链接链路

};

#endif // CPROTOCOLIEC_104_H
