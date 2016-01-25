/*!
 * \file    cprotocoldeliveriec_104.h
 * \brief   概述 CProtocolDeliverIEC_104类的头文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/26
 *
 * \b       修改记录：
 * \li      2015/5/26
 *  添加注释
 */

#ifndef CPROTOCOLDELIVERIEC_104_H
#define CPROTOCOLDELIVERIEC_104_H
#include "../PortInclude/cprotocolbase.h"
//#include "CIEC104DeliverQuery_new.h"
#include "cpointiec_104.h"
#include "IEC_104.h"
#include <QTcpServer>
#include <QDomElement>
#include <QMutex>
#include <QTimerEvent>

#include "CIEC104DeliverQuery_new.h"
#include "CGI_D_IEC_104_PointNode.h"


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

/*!
 * \class  CProtocolDeliverIEC_104
 * \brief  概述 IEC-104的转发驱动类
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
class CProtocolDeliverIEC_104 : public CProtocolBase
{
    Q_OBJECT
public:
    explicit CProtocolDeliverIEC_104(QObject *parent = 0);
    //! 供运行系统调用的函数
    bool OnCreateChannel(const QDomElement &ChannelElement_,ChannelType nChannelType_, int nComOrCanNumber_,CRTDBI *pDatabaseReturn_);
    bool OnTagChange(CPointBase *pPointBase_, QVariant VarSet_);
public slots:
    void ProcessRespond(QByteArray data);
    void slotConnect(bool isConnected_);
    //! end
public:
    enum{
        BUFFER_SIZE = 10240,///< 缓冲区最大字节数
    };
    void timerEvent(QTimerEvent *event);
private://函数
    void CGI_InitPointMap(const QDomElement &DeviceListElement_);
    void init104Param();
    void InitPointMapValue();
    int  FindATrueFrame(BYTE *pInfo, int nInfoLength);
    void DecodeFrame(CIEC104DeliverQuery_new &Query_);

    void DecodeIFrame(CIEC104DeliverQuery_new &Query_);
    void DecodeSFrame(CIEC104DeliverQuery_new &Query_);
    void DecodeUFrame(CIEC104DeliverQuery_new &Query_);
    bool Send_U_Confirm(bool bStart_, bool bStop_, bool bTest_);
    void InCrease(WORD &nValue);
    void ResetDataLink();
    void Send_S_Frame(WORD nReceiveNo);
    void ProcessASDU101Type_C_CI_NA_1(ASDU101 &asdu);
    void ProcessASDU101Type_C_CS_NA_1(ASDU101 &asdu);
    void ProcessASDU101Type_C_SC_NA_1(WORD nTransmissionCause, int nPointAddress, BYTE nSCO);
    void ProcessASDU101Type_C_SC_NA_1(ASDU101 &asdu);
    void ProcessASDU101Type_C_DC_NA_1(ASDU101 &asdu);
    void ProcessASDU101Type_C_DC_NA_1(WORD nTransmissionCause,int nPointAddress, BYTE nSCO);
    void ProcessASDU101Type_YS(ASDU101 &asdu);

    void ProcessASDU101Type_C_IC_NA_1(CIEC104DeliverQuery_new &Query_);
    bool Send_C_IC_NA_1(WORD &nSendNo,WORD nReceiveNo,BYTE nCommonAddress,BYTE nReason);
    bool Send_C_IC_NA_1_YX(WORD &nSendNo,WORD nReceiveNo,BYTE nCommonAddress,BYTE nReason,CGI_D_IEC_104_PublicAddress_Node *pPublicAddressPointNode_);
    bool Send_C_IC_NA_1_YC(WORD &nSendNo,WORD nReceiveNo,BYTE nCommonAddress,BYTE nReason,CGI_D_IEC_104_PublicAddress_Node *pPublicAddressPointNode_);
    bool Send_C_IC_NA_1_YX_M_SP_NA_1(WORD &nSendNo,WORD nReceiveNo,BYTE nCommonAddress,BYTE nReason,QMap<int,CGI_D_IEC_104_PointYX *> *pPointMap);
    bool Send_C_IC_NA_1_YX_M_DP_NA_1(WORD &nSendNo,WORD nReceiveNo,BYTE nCommonAddress,BYTE nReason,QMap<int,CGI_D_IEC_104_PointYX *> *pPointMap);
    bool Send_C_IC_NA_1_YC_M_ME_NA_1(WORD &nSendNo,WORD nReceiveNo,BYTE nCommonAddress,BYTE nReason,QMap<int,CGI_D_IEC_104_PointYC *> *pPointMap);
    bool Send_C_IC_NA_1_YC_M_ME_NB_1(WORD &nSendNo,WORD nReceiveNo,BYTE nCommonAddress,BYTE nReason,QMap<int,CGI_D_IEC_104_PointYC *> *pPointMap);
    bool Send_C_IC_NA_1_YC_M_ME_NC_1(WORD &nSendNo,WORD nReceiveNo,BYTE nCommonAddress,BYTE nReason,QMap<int,CGI_D_IEC_104_PointYC *> *pPointMap);
    bool Send_C_IC_NA_1_YC_M_ME_ND_1(WORD &nSendNo,WORD nReceiveNo,BYTE nCommonAddress,BYTE nReason,QMap<int,CGI_D_IEC_104_PointYC *> *pPointMap);

    bool Send_IntrogenCallDDLActivationOK(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);
    bool Send_IntrogenCallDDLActivationEnd(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);
    bool Send_I_C_CS_NA_1(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);

    bool MakeQuery_I(WORD &nSendNo, WORD nReceiveNo, BYTE *pAsdu, int nAsduSize);

    bool Send_I_YX (WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);
    bool Send_I_YC (WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);
    bool Send_I_YM (WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);

    bool SendFrame(const BYTE *pSource_, int nLength_);
    void SendFrame();

    void YC_Change(CPointBase *pPointBase_,QVariant VarSet_);
    void YX_Change(CPointBase *pPointBase_,QVariant VarSet_);
signals:
    void SignalMakeQuery_I_YX_ChangeYX(CPointBase *pPointBase_, QVariant VarSet_);
    void SignalMakeQuery_I_YC_ChangeYC(CPointBase *pPointBase_, QVariant VarSet_);
private slots:
    bool MakeQuery_I_YX_ChangeYX(CPointBase *pPointBase_, QVariant VarSet_);
    bool MakeQuery_I_YC_ChangeYC(CPointBase *pPointBase_, QVariant VarSet_);
private:
    bool Send_U_Command(bool bStart_, bool bStop_, bool bTest_);

private://变量
    CGI_D_IEC_104_PointNode m_PointNode;
    enum{MAX_FRAME=1024*10};
    QList<QByteArray>       m_SendFrameList;
    int                     m_nTimerID;
    QMutex                  m_mutexProcessData;
    QMutex                  m_mutexSend;
    CBaseFrame              m_bufferReceive;
    CIEC104DeliverQuery_new m_IEC104DeliverQuery;///< 接收到的报文帧
//    QByteArray              m_ByteArrayFrame;///< 合成发送的报文帧
//    CIEC104Query    m_IEC104Query;//发送的报文
//    CIEC104Response m_IEC104Response;//接收到的报文
    int m_nProtcolTimerId;
    bool bConnectOk;///< 是否成功连接服务器 连接未成功前不走规约
    bool m_bDataLink;///< 协议链路
    int m_nMaxSRValue;///< 收发最大值
    int m_nUnconfirmIFrame;///< 未认可的I帧
    QMap<int,CPointIEC_104 *>YX_PointMap;///< 遥信点map
    QMap<int,CPointIEC_104 *>YC_PointMap;///< 遥测点map
    QMap<int,CPointIEC_104 *>YK_PointMap;///< 遥控点map
    QMap<int,CPointIEC_104 *>YS_PointMap;///< 遥设点map
    QMap<int,CPointIEC_104 *>YM_PointMap;///< 遥脉点map

    //end Tcp IP 处理
private:/// 3.2 常用名词
    int m_nK;///< 发送方未被确认的I格式的APDU数目 K=12  m_nMaxUnconfirmIFrame
    int m_nW;///< 接收方最多收到未被确认的I格式的APDU数目 W=8
    int m_nt0;///< 网络建立连接超时时间 t0<30s
    int m_nt1;///< 发送或测试APDU的超时时间 t1<20s
    int m_nt2;///< 接收方无数据报文时确认的超时时间 t2<15s
    int m_nt3;///< 通道长期空闲时发送确认帧的超时时间 t3<25s  t3<10  10秒钟发送一帧测试报文
    int n_mAPDULen; ///< APDU最大长度 最大长度253

    //内部控制参数
    int n_mLastTestFrameTime;   ///< 测试帧最后帧接收时间
    int n_mLastFrameTime;       ///< 最后帧接收时间
    int n_mSendIFrameFullTime;  ///< 发送或测试APDU的超时时间
    unsigned char n_mReadFrameDateLen;  ///< 帧第二个字节，即帧长度
    int n_mLastTxTime;         ///< 最后发送帧时间

    int n_mLastReciveIFrameTime; ///< 最后收到的I帧时间，T2比较，并发送S帧确认
    int n_mLastSendIFrameTime;   ///< 最后的发送I帧时间，如果一直没被确认，则初始化链路

    int n_mLastCallDataTime; ///< 最后的总召时间，用于周期总召
    int n_mLastTimingTime;  ///< 最后的对时时间，用于周期校时

    //重启或链路复位标识
    int n_mStartDTWaitIFormatFlag;
    int n_mResetProcessWaitIFormatFlag;
    int n_mResetColseLinkFlag;

    //应用参数
    BYTE m_nCommonAddress; ///< 公共地址
    int n_mCommonAddrLen; ///< 公共地址长度
    int n_mTransmitCauseLen; ///< 传输原因长度
    int n_mInformationAddrLen; ///< 信息体地址
    int n_mTimeType;           ///< 时标格式  1：CP56 Time2a    0:CP24 Time2a

    //传输控制参数
    int n_mCurrState; ///< 链路当前状态
    WORD m_nSendIdx;   ///< 发送序号
    WORD m_nReciveIdx; ///< 接收序号
    int n_mSub_SendIdx;   ///< 从站发送序号  报文中的序号
    int n_mSub_ReciveIdx; ///< 从站接收序号  报文中的序号
    int m_nAck; ///< 发送S帧的标志

    //标识位参数
    int n_mSyncTimeFlag;
    int n_mCallAllDataFlag;
    int n_mCallKwhDataFalg;

    short int n_mSyncTimeMode;  ///< 校时模式  1:启动或重连时对时     0：周期对时
    int n_mSyncTimeCycle; ///< 校时周期
    short int n_mCallDataMode;  ///< 总召模式  1:启动或重连时总召唤    0：周期总召唤
    int n_mCallDataCycle; ///< 总召周期



    AAALinkLayerConfigInfo g_param;
    bool m_bConfigInitParamOk;
    bool m_bResetFlag; ///< 重置标识
    unsigned char m_nYKProcStep;
};

#endif // CPROTOCOLDELIVERIEC_104_H
