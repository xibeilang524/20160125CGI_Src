/*!
 * \file    cprotocoldeliveriec_104.cpp
 * \brief   概述 CProtocolDeliverIEC_104类的源文件
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

#include "cprotocoldeliveriec_104.h"
#include <QTimer>
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QDateTime>
#include <QTcpSocket>
#include <QTcpSocket>
#include <QDir>

#include "../PortInclude/CRTDBI.h"
#include "../PortInclude/CTagI.h"
#include "../PortInclude/cvaluebase.h"
#include "cpointiec_104.h"

/*!
 * \brief 功能概述 库文件导出接口函数
 * \param 参数描述 parent是QObject对象指针
 * \return 返回值描述 返回创建的驱动指针
 * \author zzy
 * \date 2015/5/26
 */
CProtocolI * CreateDriver(QObject *parent)
{
    ///库文件导出函数
    CProtocolDeliverIEC_104 *testProtocol = new CProtocolDeliverIEC_104(parent);
    return  testProtocol;
}

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 parent父对象的指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
CProtocolDeliverIEC_104::CProtocolDeliverIEC_104(QObject */*parent*/)
{

    m_nMaxSRValue=32767;//收发最大值
    m_nCommonAddress = 1;
    m_nW = 8;
    m_nTimerID = 0;

    connect(this,SIGNAL(SignalMakeQuery_I_YC_ChangeYC(CPointBase*,QVariant)),this,SLOT(MakeQuery_I_YC_ChangeYC(CPointBase*,QVariant)),Qt::QueuedConnection);
    connect(this,SIGNAL(SignalMakeQuery_I_YX_ChangeYX(CPointBase*,QVariant)),this,SLOT(MakeQuery_I_YX_ChangeYX(CPointBase*,QVariant)),Qt::QueuedConnection);
    connect(this,SIGNAL(SignalMakeQuery_I_YM_ChangeYM(CPointBase*,QVariant)),this,SLOT(MakeQuery_I_YM_ChangeYM(CPointBase*,QVariant)),Qt::QueuedConnection);
}

bool CProtocolDeliverIEC_104::OnCreateChannel(const QDomElement &ChannelElement_, ChannelType nChannelType_, int nComOrCanNumber_, CRTDBI *pDatabaseReturn_)
{
    SetPROTOCOL_TYPE(Transform_Type);
    if (!CProtocolBase::OnCreateChannel(ChannelElement_,nChannelType_,nComOrCanNumber_,pDatabaseReturn_))
        return false;

    if (ChannelElement_.elementsByTagName("Protocol").isEmpty())
        return false;
    if (ChannelElement_.elementsByTagName("DeviceList").isEmpty())
        return false;
    QDomElement Protocol   = ChannelElement_.elementsByTagName("Protocol").at(0).toElement();
    QDomElement DeviceList = ChannelElement_.elementsByTagName("DeviceList").at(0).toElement();


    g_param.strTransmitCauseLen = Protocol.attribute("TransmitCauseLen"); //传送原因地址长度
    g_param.strCommAddrLen = Protocol.attribute("CommAddrLen"); //公共地址长度
    g_param.strInfoAddrLen = Protocol.attribute("InfoAddrLen"); //信息体地址长度
    g_param.strTimeType = Protocol.attribute("TimeType");    //时标格式
    g_param.strTimingType = Protocol.attribute("TimingType");  //校时方式
    g_param.strCallingType = Protocol.attribute("CallingType"); //总召唤方式
    g_param.strLinkTimeOut = Protocol.attribute("t0");///<zzy 2015/1/14 修改: //连接超时
    g_param.strSendFrameTimeOut = Protocol.attribute("t1"); //发送或测试超时
    g_param.strNonDataTimeOut = Protocol.attribute("t2");///<zzy 2015/1/14 修改:  //无数据超时
    g_param.strSendSFrameTimeOut = Protocol.attribute("t3");///<zzy 2015/1/14 修改: //S帧发送超时
    g_param.strMaxFrameInx = Protocol.attribute("K");///<zzy 2015/1/14 修改: //最大不同接收序号
    g_param.strSFrameConfirm = Protocol.attribute("W");///<zzy 2015/1/14 修改: //最大S帧确认数
    g_param.strASDULen = Protocol.attribute("ASDULen"); //ASDU数据单元长度
    g_param.strKwhCjType = Protocol.attribute("KwhCjType"); //累积量采集方式

    CGI_InitPointMap(DeviceList);
    return true;
}

/*!
 * \brief  功能概述 响应readyRead(QByteArray)信号，解析收到的报文函数
 * \param  参数描述 data是收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolDeliverIEC_104::ProcessRespond(QByteArray data)
{
    ///接收包处理函数
    if (data.isEmpty())
        return;
    m_bufferReceive.AppendData(data.data(),data.count());
    QMutexLocker locker(&m_mutexProcessData);
    for(int nStart=0;nStart<m_bufferReceive.GetInfoSize()-5;nStart++)
    {
        int nFrameLength = FindATrueFrame(m_bufferReceive.GetBuffer(nStart),m_bufferReceive.GetInfoSize()-nStart);

        if(nFrameLength>0)
        {
            m_IEC104DeliverQuery.SetData((char *)m_bufferReceive.GetBuffer(nStart),nFrameLength);
            if (nStart == 0)
            {

            }else
            {
                ShowMessage(SHOWMESSAGE_TYPE_DEBUG,m_IEC104DeliverQuery.GetToHexFrame());
//                qDebug()<<"Error Frame:"<<QByteArray((char *)m_bufferReceive.GetBuffer(),nStart).toHex().toUpper();
            }
            m_bufferReceive.Fifo(nStart+nFrameLength);
            DecodeFrame(m_IEC104DeliverQuery);
//            bReadFrame = true;
            nStart=-1;//重新再来
        }
    }
    if(m_bufferReceive.GetInfoSize()>MAX_FRAME)
    {
        qDebug()<<"Error Frame:"<<QByteArray((char *)m_bufferReceive.GetBuffer(),m_bufferReceive.GetInfoSize()).toHex().toUpper();
        ShowMessage(SHOWMESSAGE_TYPE_DEBUG,QByteArray((char *)m_bufferReceive.GetBuffer(),m_bufferReceive.GetInfoSize()).toHex().toUpper());
        m_bufferReceive.SetInfoSize(0);
    }
}

/*!
 \brief 在接收数据缓冲区中找一帧正确的报文

 \fn ClientSocket::FindATrueFrame
 \param pInfo 开始地址
 \param nInfoLength 长度
 \return int 如果没有正确的帧则返回0，若有正确帧则返回正确帧的长度
*/
int CProtocolDeliverIEC_104::FindATrueFrame(BYTE *pInfo,int nInfoLength)
{
    if(nInfoLength<6)
        return 0;
    int nTrueFrameLength = 0;
    if (pInfo[0] != 0x68)
        return 0;
    int nFrameLength = pInfo[1];
    if (nFrameLength+2 > nInfoLength)
        return 0;
    nTrueFrameLength = nFrameLength + 2;
    return nTrueFrameLength;
}

void CProtocolDeliverIEC_104::DecodeFrame(CIEC104DeliverQuery_new &Query_)
{
    ShowMessage(SHOWMESSAGE_TYPE_RECEIVE,Query_.GetToHexFrame());
    switch (Query_.GetFrameType()) {
    case IEC104_I_TYPE:
        ///解析I帧
        if (Query_.GetInfoSize() <= 6)
        {
            ShowMessage(SHOWMESSAGE_TYPE_DEBUG,Query_.GetToHexFrame());
        }else
        {
            DecodeIFrame(Query_);
        }
        break;
    case IEC104_S_TYPE:
        ///解析S帧
        if (Query_.GetInfoSize() != 6)
        {
            ShowMessage(SHOWMESSAGE_TYPE_DEBUG,Query_.GetToHexFrame());
        }else
        {
            DecodeSFrame(Query_);
        }
        break;
    case IEC104_U_TYPE:
        ///解析U帧
        if (Query_.GetInfoSize() != 6)
        {
            ShowMessage(SHOWMESSAGE_TYPE_DEBUG,Query_.GetToHexFrame());
        }else
        {
            DecodeUFrame(Query_);
        }
        break;
    default:
        break;
    }
}

/*!
 * \brief  功能概述 处理 I帧
 * \param  参数描述 response是接收到的报文对象
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolDeliverIEC_104::DecodeIFrame(CIEC104DeliverQuery_new &Query_)
{
    Q_ASSERT(Query_.GetFrameType() == IEC104_I_TYPE);
    WORD nAckNumber = Query_.GetReceiveFrameNo();
    WORD nReceive = Query_.GetSendFrameNo();
    {
        if(nReceive == m_nReciveIdx)
        {
//            qDebug()<<"nReceive = m_nReceiveNo";
            InCrease(m_nReciveIdx);
        }else
        {
            qDebug()<<"nReceive != m_nReceiveNo"<<nReceive<<m_nReciveIdx;
            ResetDataLink();
            return;
        }
        m_nUnconfirmIFrame++;
        if(m_nUnconfirmIFrame>=1)//还是每帧都确认保险 //m_nMaxUnconfirmIFrame)//每m_nMaxUnconfirmIFrame 确认一次
        {
            Send_S_Frame(Query_.GetSendFrameNo());
//            m_query.MakeQuery_S(Query_.GetSendFrameNo()+1);
//            SendFrame(m_query,IEC104_S_TYPE);
            m_nUnconfirmIFrame =0;
        }
        if(m_nSendIdx-nAckNumber>m_nW)
        {
            //已经很多帧没有认可了
            ResetDataLink();
            return;
        }
    }
    ASDU101 *pAsdu = Query_.GetAsdu();
    if(pAsdu == NULL)
        return ;
    switch (pAsdu->m_nType)
    {
    case C_SC_NA_1:	// 45 单命令

        return;
    case C_DC_NA_1: // 46 双点遥控命令
//        ProcessASDU101Type_C_DC_NA_1(response.GetTransmissionCause(),response.GetPointAddress(),response.GetUInt(15,1));
//        ProcessASDU101Type_C_DC_NA_1(*pAsdu);
        qDebug()<<"45、46 双点遥控命令";
        return;
        break;
    case C_RC_NA_1:		// 47 升降命令
        qDebug()<<"47 升降命令";
//        return;
        break;
    case C_SE_NA_1:// 48 设定值命令，归一化值
    case C_SE_NB_1:// 49 设定值命令，标度化值
    case C_SE_NC_1:// 50 设定值命令，短浮点数
//        ProcessASDU101Type_YS(*pAsdu);
        break;
    case C_BO_NA_1:		// 51 32位的位串
        qDebug()<<"51 32位的位串";
//        return;
        break;
    case C_IC_NA_1: // 100 总召唤命令
//        qDebug()<<"100 总召唤命令";
        ProcessASDU101Type_C_IC_NA_1(Query_);
        return;
        break;
    case C_CI_NA_1:		// 101 电能脉冲召唤命令
//        qDebug()<<"101 电能脉冲召唤命令";
        ProcessASDU101Type_C_CI_NA_1(Query_);
        return;
        break;
    case C_RD_NA_1:		// 102 读数据命令
        qDebug()<<"102 读数据命令";
//        return;
        break;
    case C_CS_NA_1:		// 103 时钟同步命令
        ProcessASDU101Type_C_CS_NA_1(*pAsdu);
//        qDebug()<<"103 时钟同步命令";
        return;
        break;
    case C_TS_NA_1:		// 104 测试命令
        qDebug()<<"104 测试命令";
//        return;
        break;
    case C_RP_NA_1:		// 105 复位进程命令
        qDebug()<<"105 复位进程命令";
//        return;
        break;
    case C_CD_NA_1:		// 106 延时获得命令
        qDebug()<<"106 延时获得命令";
//        return;
        break;
//    case C_TS_TA_1:		// 107 带时标CP56Time2a的测试命令
        qDebug()<<"107 带时标CP56Time2a的测试命令";
//        return;
//        break;
    default:
        CIEC104DeliverQuery_new  query;
        query.SetData(Query_.GetBuffer(),Query_.GetInfoSize());
        ASDU101 *pAsdu_response = query.GetAsdu();
        WORD dwReason = ASDU101_TR_UNKNOW_ASDU_TYPE;///< 未知的类型标识
        pAsdu_response->REASON = dwReason;
        SendFrame(query.GetBuffer(),query.GetInfoSize());
//        InsertSendFrame(query);
        break;
    }
}

/*!
 * \brief  功能概述 处理 S帧
 * \param  参数描述 response是接收到的报文对象
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolDeliverIEC_104::DecodeSFrame(CIEC104DeliverQuery_new &Query_)
{
    Q_ASSERT(Query_.GetFrameType() == IEC104_S_TYPE);
    //由于我们没有下发缓冲 所以不用

    WORD nAckNumber = Query_.GetReceiveFrameNo();

//    m_nSendIdx
//    m_nReciveIdx
    if(m_nSendIdx-nAckNumber>m_nW)
    {
        qDebug()<<"// 已经很多帧没有认可了";
    }

    if(nAckNumber>m_nSendIdx || m_nSendIdx-nAckNumber>m_nW)
    {
        qDebug()<<"(nAckNumber>m_nSendNo || m_nSendNo-nAckNumber>m_nMaxUnconfirmIFrame)";
    }
}

/*!
 * \brief  功能概述 解析U帧
 * \param  参数描述 response是接收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolDeliverIEC_104::DecodeUFrame(CIEC104DeliverQuery_new &Query_)
{
    Q_ASSERT(Query_.GetFrameType() == IEC104_U_TYPE);
    if (Query_.IsStartFrame())//
    {
        init104Param();
        Send_U_Confirm(true,false,false);
    }else if (Query_.IsStopFrame())
    {
        init104Param();
        Send_U_Confirm(false,true,false);
   }else if (Query_.IsTestFrame())
    {
        Send_U_Confirm(false,false,true);
    }
}

/*!
 * \brief  功能概述 合成并发送U帧
 * \param  参数描述 bStart是开始标志，bStop是停止标志，bTest是测试标志
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolDeliverIEC_104::Send_U_Confirm(bool bStart_, bool bStop_, bool bTest_)
{
    //功能：合成U帧  确认帧
    //参数：bStart 开始标志 bStop 停止标志 bTest 测试标志
    //建立日期：2007-09-14

    Q_ASSERT(bStart_+bStop_+bTest_ == 1);
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;

    *pCur++ = 0x68;
    *pCur++ = 4;
    *pCur = 3;
    if(bStart_)
        *pCur |= 8;
    if(bStop_)
        *pCur |= 0x20;
    if(bTest_)
        *pCur |= 0x80;
    pCur++;

    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return SendFrame(szSend,pCur-szSend);
}

/*!
 * \brief  功能概述 判断接收序号是否大于最大值
 * \param  参数描述 nValue接收序号
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolDeliverIEC_104::InCrease(WORD &nValue)
{
    nValue++;
    if (nValue>m_nMaxSRValue)
        nValue=0;
}

/*!
 * \brief  功能概述 复位链路
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolDeliverIEC_104::ResetDataLink()
{
    init104Param();
}

/*!
 * \brief  功能概述 合成并发送S帧
 * \param  参数描述 nReceiveNo是接收序号
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolDeliverIEC_104::Send_S_Frame(WORD nReceiveNo)
{
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    // TODO: Add extra operator here
    *pCur++ = 0x68;
    *pCur++ = 4;
    *pCur++ = 1;
    *pCur++ = 0;
    WORD nTemp = (nReceiveNo << 1);
    *pCur++ = LOBYTE(nTemp);
    *pCur++ = HIBYTE(nTemp);


    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    SendFrame(szSend,pCur-szSend);
}

/*!
 * \brief  功能概述 处理总召唤
 * \param  参数描述 asdu是接收到的报文中的ASDU部分的指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolDeliverIEC_104::ProcessASDU101Type_C_IC_NA_1(CIEC104DeliverQuery_new &Query_)
{
    ASDU101 *pASDU = Query_.GetAsdu();
    BYTE dwReason = pASDU->REASON;
    BYTE nCommandAddress = pASDU->GetCommonAddress();
    if (nCommandAddress == 0xFF)
    {
        switch (dwReason) {
        case ASDU101_TR_ACT:
        {
            dwReason = ASDU101_TR_ACTCON;///< 激活确认
            Send_C_IC_NA_1(m_nSendIdx,m_nReciveIdx,0xFF,dwReason);

            QList<int> nCommandAddressList = m_PointNode.GetCommandAddress_PointNode()->keys();
            dwReason = ASDU101_TR_INTROGEN;
            for (int i = 0; i < nCommandAddressList.count(); ++i)
            {
                nCommandAddress = nCommandAddressList.at(i);
                CGI_D_IEC_104_PublicAddress_Node *pPublicAddressPointNode = m_PointNode.GetCommandAddress_PointNode()->value(nCommandAddress);
                if (pPublicAddressPointNode != NULL)
                {
                    Send_C_IC_NA_1_YX(m_nSendIdx,m_nReciveIdx,nCommandAddress,dwReason,pPublicAddressPointNode);
                    Send_C_IC_NA_1_YC(m_nSendIdx,m_nReciveIdx,nCommandAddress,dwReason,pPublicAddressPointNode);
                }
            }

            dwReason = ASDU101_TR_ACTTERM;///< 激活终止
            Send_C_IC_NA_1(m_nSendIdx,m_nReciveIdx, 0xFF,dwReason);
        }
            break;
        case ASDU101_TR_DEACT:
            dwReason = ASDU101_TR_DEACTCON;///< 停止激活确认
            Send_C_IC_NA_1(m_nSendIdx,m_nReciveIdx,0xFF,dwReason);
            break;
        default:
            dwReason = ASDU101_TR_UNKNOW_REASON_TYPE;
            pASDU->REASON = dwReason;
            SendFrame(Query_.GetBuffer(),Query_.GetInfoSize());
            break;
        }
    }else
    {
        QList<int> nCommandAddressList = m_PointNode.GetCommandAddress_PointNode()->keys();
        if (nCommandAddressList.contains(nCommandAddress))
        {
            switch (dwReason) {
            case ASDU101_TR_ACT:
            {
                dwReason = ASDU101_TR_ACTCON;///< 激活确认
                Send_C_IC_NA_1(m_nSendIdx,m_nReciveIdx,nCommandAddress,dwReason);

                dwReason = ASDU101_TR_INTROGEN;
                CGI_D_IEC_104_PublicAddress_Node *pPublicAddressPointNode = m_PointNode.GetCommandAddress_PointNode()->value(nCommandAddress);
                if (pPublicAddressPointNode != NULL)
                {
                    Send_C_IC_NA_1_YX(m_nSendIdx,m_nReciveIdx,nCommandAddress,dwReason,pPublicAddressPointNode);
                    Send_C_IC_NA_1_YC(m_nSendIdx,m_nReciveIdx,nCommandAddress,dwReason,pPublicAddressPointNode);
                }

                dwReason = ASDU101_TR_ACTTERM;///< 激活终止
                Send_C_IC_NA_1(m_nSendIdx,m_nReciveIdx,nCommandAddress,dwReason);
            }
                break;
            case ASDU101_TR_DEACT:
                dwReason = ASDU101_TR_DEACTCON;///< 停止激活确认
                Send_C_IC_NA_1(m_nSendIdx,m_nReciveIdx,nCommandAddress,dwReason);
                break;
            default:
                dwReason = ASDU101_TR_UNKNOW_REASON_TYPE;
                pASDU->REASON = dwReason;
                SendFrame(Query_.GetBuffer(),Query_.GetInfoSize());
                break;
            }
        }else
        {
            dwReason = ASDU101_TR_UNKNOW_PUBLIC_ADDR;
            pASDU->REASON = dwReason;
            SendFrame(Query_.GetBuffer(),Query_.GetInfoSize());
        }
    }
}



/*!
 * \brief  功能概述
 * \param  参数描述
 * \return 返回值描述
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolDeliverIEC_104::ProcessASDU101Type_C_CS_NA_1(ASDU101 &/*asdu*/)
{

    //////////处理校时
    Send_I_C_CS_NA_1(m_nSendIdx, m_nReciveIdx, m_nCommonAddress);
}

bool CProtocolDeliverIEC_104::Send_C_IC_NA_1(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress, BYTE nReason)
{
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    *pCur++ = C_IC_NA_1;//0x64;//100 类型标识
    *pCur++ = 1;        //结构限定词
    *pCur++ = nReason;  //传输原因
    *pCur++ = 0;        //传输原因高位

    *pCur++ = nCommonAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0;        //公共地址高位

    *pCur++ = 0; //信息体地址低位
    *pCur++ = 0; //信息体地址高位
    *pCur++ = 0; //信息体地址高位

    *pCur++ = 0x14;//总召唤限定词(Qoi)

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
}

bool CProtocolDeliverIEC_104::Send_C_IC_NA_1_YX(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress, BYTE nReason, CGI_D_IEC_104_PublicAddress_Node *pPublicAddressPointNode_)
{
    QMap<int,CGI_D_IEC_104_PointYX *> *pPointMap = pPublicAddressPointNode_->Get_YX_PointMap(M_SP_NA_1);
    if (pPointMap != NULL)
    {
        Send_C_IC_NA_1_YX_M_SP_NA_1(nSendNo,nReceiveNo,nCommonAddress,nReason,pPointMap);
    }
    pPointMap = pPublicAddressPointNode_->Get_YX_PointMap(M_DP_NA_1);
    if (pPointMap != NULL)
    {
        Send_C_IC_NA_1_YX_M_DP_NA_1(nSendNo,nReceiveNo,nCommonAddress,nReason,pPointMap);
    }

    return true;
}

bool CProtocolDeliverIEC_104::Send_C_IC_NA_1_YC(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress, BYTE nReason, CGI_D_IEC_104_PublicAddress_Node *pPublicAddressPointNode_)
{
    QMap<int,CGI_D_IEC_104_PointYC *> *pPointMap = pPublicAddressPointNode_->Get_YC_PointMap(M_ME_NA_1);
    if (pPointMap != NULL)
    {
        Send_C_IC_NA_1_YC_M_ME_NA_1(nSendNo,nReceiveNo,nCommonAddress,nReason,pPointMap);
    }
    pPointMap = pPublicAddressPointNode_->Get_YC_PointMap(M_ME_NB_1);
    if (pPointMap != NULL)
    {
        Send_C_IC_NA_1_YC_M_ME_NB_1(nSendNo,nReceiveNo,nCommonAddress,nReason,pPointMap);
    }
    pPointMap = pPublicAddressPointNode_->Get_YC_PointMap(M_ME_NC_1);
    if (pPointMap != NULL)
    {
        Send_C_IC_NA_1_YC_M_ME_NC_1(nSendNo,nReceiveNo,nCommonAddress,nReason,pPointMap);
    }
    pPointMap = pPublicAddressPointNode_->Get_YC_PointMap(M_ME_ND_1);
    if (pPointMap != NULL)
    {
        Send_C_IC_NA_1_YC_M_ME_ND_1(nSendNo,nReceiveNo,nCommonAddress,nReason,pPointMap);
    }
    return true;
}

bool CProtocolDeliverIEC_104::Send_C_IC_NA_1_YX_M_SP_NA_1(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress, BYTE nReason, QMap<int,CGI_D_IEC_104_PointYX *> *pPointMap)
{
    if (pPointMap->isEmpty())
        return false;

    QMap<int,CGI_D_IEC_104_PointYX *>::iterator Iterator;
    Iterator = pPointMap->begin();
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    BYTE *nASDUVarSQ;//可变结构限定词
    BYTE *pAddress;

    int number = 0;///计数
    int maximumTag = 0;///最大信息体数

    *pCur++ = M_SP_NA_1;//类型标识  0x01 单点信息

    nASDUVarSQ = pCur;//可变结构限定词
    *pCur++ = 0x80;   //可变结构限定词  0x80代表顺序的

    *pCur++ = nReason;//传送原因低位  响应总召唤
    *pCur++ = 0;   //传输原因高位

    *pCur++ = nCommonAddress;//ASDU公共地址 (一般为子站地址)
    *pCur++ = 0;             //公共地址高位

    pAddress = pCur;

    int nPointNumber = Iterator.key();
    *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
    *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
    *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位

    ASDU101_SIQ siq; ///带品质描述词的单点信息
    if (maximumTag == 0)
        maximumTag = 240/(siq.GetSize());
    if (maximumTag > 127)
        maximumTag = 127;
    for (;;)
    {
        CGI_D_IEC_104_PointYX *pPoint = Iterator.value();
        CTagI *tag = pPoint->GetTag();
        if (tag == NULL)
        {
            siq.IV = 1;
            siq.SPI = 0;
            siq.RES = 0;
            siq.BL = 0;
            siq.SB = 0;
            siq.NT = 0;
        }else
        {
            if (tag->GetPointCollection() == NULL)
            {
                siq.IV = 1;
                siq.SPI = 0;
                siq.RES = 0;
                siq.BL = 0;
                siq.SB = 0;
                siq.NT = 0;
            }else
            {
                siq.IV = 0;
                unsigned int bValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
                pPoint->GetValue()->SetVarValue(tag->GetPointCollection()->GetValue()->GetVarValue());
                if (bValue)
                    siq.SPI = 1;
                else
                    siq.SPI = 0;
                siq.RES = 0;
                siq.BL = 0;
                siq.SB = 0;
                siq.NT = 0;
            }
        }
        memcpy(pCur,(BYTE *)&siq,siq.GetSize());
        pCur += siq.GetSize();

        if (++Iterator == pPointMap->end())
        {
            *nASDUVarSQ |= ++number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            return true;
        }
        ++nPointNumber;
        if ( (++number>=maximumTag) || (nPointNumber!=Iterator.key()))
        {
            *nASDUVarSQ |= number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            nPointNumber = Iterator.key();
            pCur = pAddress;
            *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
            *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
            *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
            number = 0;
        }
    }
}

bool CProtocolDeliverIEC_104::Send_C_IC_NA_1_YX_M_DP_NA_1(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress, BYTE nReason, QMap<int, CGI_D_IEC_104_PointYX *> *pPointMap)
{
    if (pPointMap->isEmpty())
        return true;
    QMap<int,CGI_D_IEC_104_PointYX *>::iterator Iterator;
    Iterator = pPointMap->begin();
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    BYTE *nASDUVarSQ;//可变结构限定词
    BYTE *pAddress;

    int number = 0;///计数
    int maximumTag = 0;///最大信息体数

    *pCur++ = M_DP_NA_1;//类型标识  0x03 双点信息

    nASDUVarSQ = pCur;//可变结构限定词
    *pCur++ = 0x00;   //可变结构限定词  0x80代表顺序的

    *pCur++ = nReason;//传送原因低位  响应总召唤
    *pCur++ = 0;   //传输原因高位

    *pCur++ = nCommonAddress;//ASDU公共地址 (一般为子站地址)
    *pCur++ = 0;             //公共地址高位

    pAddress = pCur;

    int nPointNumber = Iterator.key();
    *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
    *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
    *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位

    ASDU101_DIQ diq; ///带品质描述词的单点信息
    if (maximumTag == 0)
        maximumTag = 240/(diq.GetSize());
    if (maximumTag > 127)
        maximumTag = 127;
    for (;;)
    {
        CGI_D_IEC_104_PointYX *pPoint = Iterator.value();
        CTagI *tag = pPoint->GetTag();
        if (tag == NULL)
        {
            diq.IV = 1;
            diq.DPI = 0;
            diq.RES = 0;
            diq.BL = 0;
            diq.SB = 0;
            diq.NT = 0;
        }else
        {
            if (tag->GetPointCollection() == NULL)
            {
                diq.IV = 1;
                diq.DPI = 0;
                diq.RES = 0;
                diq.BL = 0;
                diq.SB = 0;
                diq.NT = 0;
            }else
            {
                diq.IV = 0;
                unsigned int bValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
                pPoint->GetValue()->SetVarValue(tag->GetPointCollection()->GetValue()->GetVarValue());
                if (bValue>3)
                {
                    diq.DPI = 3;
                }else
                {
                    diq.DPI = bValue;
                }
                diq.RES = 0;
                diq.BL = 0;
                diq.SB = 0;
                diq.NT = 0;
            }
        }
        memcpy(pCur,(BYTE *)&diq,diq.GetSize());
        pCur += diq.GetSize();

        if (++Iterator == pPointMap->end())
        {
            *nASDUVarSQ |= ++number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            return true;
        }
        ++nPointNumber;
        if ( (++number>=maximumTag) || (nPointNumber!=Iterator.key()))
        {
            *nASDUVarSQ |= number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            nPointNumber = Iterator.key();
            pCur = pAddress;
            *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
            *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
            *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
            number = 0;
        }
    }
}

bool CProtocolDeliverIEC_104::Send_C_IC_NA_1_YC_M_ME_NA_1(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress, BYTE nReason, QMap<int, CGI_D_IEC_104_PointYC *> *pPointMap)
{
    if (pPointMap->isEmpty())
        return false;

    QMap<int,CGI_D_IEC_104_PointYC *>::iterator pointIterator;
    pointIterator = pPointMap->begin();

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    BYTE *nASDUVarSQ;//可变结构限定词
    BYTE *pAddress;

    int number = 0;///计数
    int maximumTag = 0;///最大信息体数
    *pCur++ = M_ME_NA_1;//类型标识

    nASDUVarSQ = pCur;//可变结构限定词
    *pCur++ = 0x80;   //可变结构限定词  0x80代表顺序的

    *pCur++ = nReason;//传送原因低位  响应总召唤
    *pCur++ = 0;   //传输原因高位

    *pCur++ = nCommonAddress;//ASDU公共地址 (一般为子站地址)
    *pCur++ = 0;             //公共地址高位

    pAddress = pCur;
    int nPointNumber = pointIterator.key();

    *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
    *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
    *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
    ASDU101_NVA nva;///归一化值
    ASDU101_QDS qds;
    if (maximumTag == 0)
        maximumTag = 240/(nva.GetSize()+qds.GetSize());
    if (maximumTag > 127)
        maximumTag = 127;
    for (;;)
    {
        CGI_D_IEC_104_PointYC *pPoint = pointIterator.value();
        CTagI *tag = pPoint->GetTag();
        nva.nValue = 0;//每次将原值此初始化
        if (tag == NULL)
        {
            qds.OV  = 0;
            qds.RES = 0;
            qds.BL  = 0;
            qds.SB  = 0;
            qds.NT  = 0;
            qds.IV  = 1;//无效
        }else
        {
            if (tag->GetPointCollection() == NULL)
            {
                qds.OV  = 0;
                qds.RES = 0;
                qds.BL  = 0;
                qds.SB  = 0;
                qds.NT  = 0;
                qds.IV  = 1;//无效
            }else
            {
                QVariant Value = tag->GetPointCollection()->GetValue()->GetVarValue();
                pPoint->GetValue()->SetVarValue(Value);
                nva.nValue = Value.toFloat()/pPoint->m_fKValue-pPoint->m_fBaseValue;
                qds.OV  = 0;
                qds.RES = 0;
                qds.BL  = 0;
                qds.SB  = 0;
                qds.NT = 0;
                qds.IV  = 0;//有效
            }
        }
        memcpy(pCur,(BYTE *)&nva,nva.GetSize());
        pCur += nva.GetSize();
        memcpy(pCur,(BYTE *)&qds,qds.GetSize());
        pCur += qds.GetSize();

        if (++pointIterator == pPointMap->end())
        {
            *nASDUVarSQ |= ++number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            return true;
        }
        ++nPointNumber;
        if ( (++number>=maximumTag) || (nPointNumber!=pointIterator.key()))
        {
            *nASDUVarSQ |= number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            nPointNumber = pointIterator.key();
            pCur = pAddress;
            *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
            *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
            *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
            number = 0;
        }
    }
}

bool CProtocolDeliverIEC_104::Send_C_IC_NA_1_YC_M_ME_NB_1(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress, BYTE nReason, QMap<int, CGI_D_IEC_104_PointYC *> *pPointMap)
{
    if (pPointMap->isEmpty())
        return true;

    QMap<int,CGI_D_IEC_104_PointYC *>::iterator pointIterator;
    pointIterator = pPointMap->begin();

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    BYTE *nASDUVarSQ;//可变结构限定词
    BYTE *pAddress;

    int number = 0;///计数
    int maximumTag = 0;///最大信息体数
    *pCur++ = M_ME_NB_1;//类型标识

    nASDUVarSQ = pCur;//可变结构限定词
    *pCur++ = 0x80;   //可变结构限定词  0x80代表顺序的

    *pCur++ = nReason;//传送原因低位  响应总召唤
    *pCur++ = 0;   //传输原因高位

    *pCur++ = nCommonAddress;//ASDU公共地址 (一般为子站地址)
    *pCur++ = 0;             //公共地址高位

    pAddress = pCur;
    int nPointNumber = pointIterator.key();

    *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
    *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
    *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
    ASDU101_SVA sva;///标度化值
    ASDU101_QDS qds;
    if (maximumTag == 0)
        maximumTag = 240/(sva.GetSize()+qds.GetSize());
    if (maximumTag > 127)
        maximumTag = 127;
    for (;;)
    {
        CGI_D_IEC_104_PointYC *pPoint = pointIterator.value();
        CTagI *tag = pPoint->GetTag();
        sva.m_nValue = 0;//每次将原值此初始化
        if (pPoint == NULL)
        {
            qds.OV  = 0;
            qds.RES = 0;
            qds.BL  = 0;
            qds.SB  = 0;
            qds.NT  = 0;
            qds.IV  = 1;//无效
        }else
        {
            if (tag->GetPointCollection() == NULL)
            {
                qds.OV  = 0;
                qds.RES = 0;
                qds.BL  = 0;
                qds.SB  = 0;
                qds.NT  = 0;
                qds.IV  = 1;//无效
            }else
            {
                sva.m_nValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
                pPoint->GetValue()->SetVarValue(tag->GetPointCollection()->GetValue()->GetVarValue());
                qds.OV  = 0;
                qds.RES = 0;
                qds.BL  = 0;
                qds.SB  = 0;
                qds.NT = 0;
                qds.IV  = 0;//有效
            }
        }
        memcpy(pCur,(BYTE *)&sva,sva.GetSize());
        pCur += sva.GetSize();
        memcpy(pCur,(BYTE *)&qds,qds.GetSize());
        pCur += qds.GetSize();

        if (++pointIterator == pPointMap->end())
        {
            *nASDUVarSQ |= ++number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            return true;
        }
        ++nPointNumber;
        if ( (++number>=maximumTag) || (nPointNumber!=pointIterator.key()))
        {
            *nASDUVarSQ |= number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            nPointNumber = pointIterator.key();
            pCur = pAddress;
            *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
            *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
            *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
            number = 0;
        }
    }
}

bool CProtocolDeliverIEC_104::Send_C_IC_NA_1_YC_M_ME_NC_1(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress, BYTE nReason, QMap<int, CGI_D_IEC_104_PointYC *> *pPointMap)
{
    if (pPointMap->isEmpty())
        return true;

    QMap<int,CGI_D_IEC_104_PointYC *>::iterator pointIterator;
    pointIterator = pPointMap->begin();

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    BYTE *nASDUVarSQ;//可变结构限定词
    BYTE *pAddress;

    int number = 0;///计数
    int maximumTag = 0;///最大信息体数
    *pCur++ = M_ME_NC_1;//类型标识

    nASDUVarSQ = pCur;//可变结构限定词
    *pCur++ = 0x80;   //可变结构限定词  0x80代表顺序的

    *pCur++ = nReason;//传送原因低位  响应总召唤
    *pCur++ = 0;   //传输原因高位

    *pCur++ = nCommonAddress;//ASDU公共地址 (一般为子站地址)
    *pCur++ = 0;             //公共地址高位

    pAddress = pCur;
    int nPointNumber = pointIterator.key();

    *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
    *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
    *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
    ASDU101_R32 r32;///短浮点数
    ASDU101_QDS qds;
    if (maximumTag == 0)
        maximumTag = 240/(r32.GetSize()+qds.GetSize());
    if (maximumTag > 127)
        maximumTag = 127;
    for (;;)
    {
        CGI_D_IEC_104_PointYC *pPoint = pointIterator.value();
        CTagI *tag = pPoint->GetTag();
        r32.m_nValue = 0;//每次将原值此初始化
        if (tag == NULL)
        {
            qds.OV  = 0;
            qds.RES = 0;
            qds.BL  = 0;
            qds.SB  = 0;
            qds.NT  = 0;
            qds.IV  = 1;//无效
        }else
        {
            if (tag->GetPointCollection() == NULL)
            {
                qds.OV  = 0;
                qds.RES = 0;
                qds.BL  = 0;
                qds.SB  = 0;
                qds.NT  = 0;
                qds.IV  = 1;//无效
            }else
            {
                r32.m_nValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
                pPoint->GetValue()->SetVarValue(tag->GetPointCollection()->GetValue()->GetVarValue());
                qds.OV  = 0;
                qds.RES = 0;
                qds.BL  = 0;
                qds.SB  = 0;
                qds.NT = 0;
                qds.IV  = 0;//有效
            }
        }
        memcpy(pCur,(BYTE *)&r32,r32.GetSize());
        pCur += r32.GetSize();
        memcpy(pCur,(BYTE *)&qds,qds.GetSize());
        pCur += qds.GetSize();

        if (++pointIterator == pPointMap->end())
        {
            *nASDUVarSQ |= ++number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            return true;
        }
        ++nPointNumber;
        if ( (++number>=maximumTag) || (nPointNumber!=pointIterator.key()))
        {
            *nASDUVarSQ |= number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            nPointNumber = pointIterator.key();
            pCur = pAddress;
            *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
            *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
            *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
            number = 0;
        }
    }
}

bool CProtocolDeliverIEC_104::Send_C_IC_NA_1_YC_M_ME_ND_1(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress, BYTE nReason, QMap<int, CGI_D_IEC_104_PointYC *> *pPointMap)
{
    if (pPointMap->isEmpty())
        return true;

    QMap<int,CGI_D_IEC_104_PointYC *>::iterator pointIterator;
    pointIterator = pPointMap->begin();

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    BYTE *nASDUVarSQ;//可变结构限定词
    BYTE *pAddress;

    int number = 0;///计数
    int maximumTag = 0;///最大信息体数
    *pCur++ = M_ME_ND_1;//类型标识

    nASDUVarSQ = pCur;//可变结构限定词
    *pCur++ = 0x80;   //可变结构限定词  0x80代表顺序的

    *pCur++ = nReason;//传送原因低位  响应总召唤
    *pCur++ = 0;   //传输原因高位

    *pCur++ = nCommonAddress;//ASDU公共地址 (一般为子站地址)
    *pCur++ = 0;             //公共地址高位

    pAddress = pCur;
    int nPointNumber = pointIterator.key();

    *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
    *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
    *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
    ASDU101_NVA nva;///归一化值
    if (maximumTag == 0)
        maximumTag = 240/(nva.GetSize());
    if (maximumTag > 127)
        maximumTag = 127;
    for (;;)
    {
        CGI_D_IEC_104_PointYC *pPoint = pointIterator.value();
        CTagI *tag = pPoint->GetTag();
        nva.nValue = 0;//每次将原值此初始化
        if (tag == NULL)
        {
        }else
        {
            if (tag->GetPointCollection() == NULL)
            {
            }else
            {
                nva.nValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
                pPoint->GetValue()->SetVarValue(tag->GetPointCollection()->GetValue()->GetVarValue());
            }
        }
        memcpy(pCur,(BYTE *)&nva,nva.GetSize());
        pCur += nva.GetSize();

        if (++pointIterator == pPointMap->end())
        {
            *nASDUVarSQ |= ++number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            return true;
        }
        ++nPointNumber;
        if ( (++number>=maximumTag) || (nPointNumber!=pointIterator.key()))
        {
            *nASDUVarSQ |= number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            nPointNumber = pointIterator.key();
            pCur = pAddress;
            *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
            *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
            *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
            number = 0;
        }
    }
}

/*!
 \brief 电度量总召唤

 \fn CProtocolDeliverIEC_104::ProcessASDU101Type_C_CI_NA_1
 \param Query_
*/
void CProtocolDeliverIEC_104::ProcessASDU101Type_C_CI_NA_1(CIEC104DeliverQuery_new &Query_)
{
    ASDU101 *pASDU = Query_.GetAsdu();
    BYTE dwReason = pASDU->REASON;
    BYTE nCommandAddress = pASDU->GetCommonAddress();
    if (nCommandAddress == 0xFF)
    {
        switch (dwReason) {
        case ASDU101_TR_ACT:
        {
            dwReason = ASDU101_TR_ACTCON;///< 激活确认
            Send_C_CI_NA_1(m_nSendIdx,m_nReciveIdx,0xFF,dwReason);

            QList<int> nCommandAddressList = m_PointNode.GetCommandAddress_PointNode()->keys();
            dwReason = ASDU101_TR_INTROGEN;
            for (int i = 0; i < nCommandAddressList.count(); ++i)
            {
                nCommandAddress = nCommandAddressList.at(i);
                CGI_D_IEC_104_PublicAddress_Node *pPublicAddressPointNode = m_PointNode.GetCommandAddress_PointNode()->value(nCommandAddress);
                if (pPublicAddressPointNode != NULL)
                {
                    Send_C_CI_NA_1_YM(m_nSendIdx,m_nReciveIdx,nCommandAddress,dwReason,pPublicAddressPointNode);
                }
            }

            dwReason = ASDU101_TR_ACTTERM;///< 激活终止
            Send_C_CI_NA_1(m_nSendIdx,m_nReciveIdx, 0xFF,dwReason);
        }
            break;
        case ASDU101_TR_DEACT:
            dwReason = ASDU101_TR_DEACTCON;///< 停止激活确认
            Send_C_CI_NA_1(m_nSendIdx,m_nReciveIdx,0xFF,dwReason);
            break;
        default:
            dwReason = ASDU101_TR_UNKNOW_REASON_TYPE;
            pASDU->REASON = dwReason;
            SendFrame(Query_.GetBuffer(),Query_.GetInfoSize());
            break;
        }
    }else
    {
        QList<int> nCommandAddressList = m_PointNode.GetCommandAddress_PointNode()->keys();
        if (nCommandAddressList.contains(nCommandAddress))
        {
            switch (dwReason) {
            case ASDU101_TR_ACT:
            {
                dwReason = ASDU101_TR_ACTCON;///< 激活确认
                Send_C_CI_NA_1(m_nSendIdx,m_nReciveIdx,nCommandAddress,dwReason);

                dwReason = ASDU101_TR_INTROGEN;
                CGI_D_IEC_104_PublicAddress_Node *pPublicAddressPointNode = m_PointNode.GetCommandAddress_PointNode()->value(nCommandAddress);
                if (pPublicAddressPointNode != NULL)
                {
                    Send_C_CI_NA_1_YM(m_nSendIdx,m_nReciveIdx,nCommandAddress,dwReason,pPublicAddressPointNode);
                }

                dwReason = ASDU101_TR_ACTTERM;///< 激活终止
                Send_C_CI_NA_1(m_nSendIdx,m_nReciveIdx,nCommandAddress,dwReason);
            }
                break;
            case ASDU101_TR_DEACT:
                dwReason = ASDU101_TR_DEACTCON;///< 停止激活确认
                Send_C_CI_NA_1(m_nSendIdx,m_nReciveIdx,nCommandAddress,dwReason);
                break;
            default:
                dwReason = ASDU101_TR_UNKNOW_REASON_TYPE;
                pASDU->REASON = dwReason;
                SendFrame(Query_.GetBuffer(),Query_.GetInfoSize());
                break;
            }
        }else
        {
            dwReason = ASDU101_TR_UNKNOW_PUBLIC_ADDR;
            pASDU->REASON = dwReason;
            SendFrame(Query_.GetBuffer(),Query_.GetInfoSize());
        }
    }
}

bool CProtocolDeliverIEC_104::Send_C_CI_NA_1(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress, BYTE nReason)
{
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    *pCur++ = C_CI_NA_1;//0x65;//101 累计量召唤命令
    *pCur++ = 1;        //结构限定词
    *pCur++ = nReason;  //传输原因
    *pCur++ = 0;        //传输原因高位

    *pCur++ = nCommonAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0;        //公共地址高位

    *pCur++ = 0; //信息体地址低位
    *pCur++ = 0; //信息体地址高位
    *pCur++ = 0; //信息体地址高位

    *pCur++ = 0x14;//总召唤限定词(Qoi)

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
}

bool CProtocolDeliverIEC_104::Send_C_CI_NA_1_YM(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress, BYTE nReason, CGI_D_IEC_104_PublicAddress_Node *pPublicAddressPointNode_)
{
    QMap<int,CGI_D_IEC_104_PointYM *> *pPointMap = pPublicAddressPointNode_->Get_YM_PointMap(M_IT_NA_1);
    if (pPointMap != NULL)
    {
        Send_C_CI_NA_1_YM_M_IT_NA_1(nSendNo,nReceiveNo,nCommonAddress,nReason,pPointMap);
    }
}

bool CProtocolDeliverIEC_104::Send_C_CI_NA_1_YM_M_IT_NA_1(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress, BYTE nReason, QMap<int, CGI_D_IEC_104_PointYM *> *pPointMap)
{
    if (pPointMap->isEmpty())
        return false;

    QMap<int,CGI_D_IEC_104_PointYM *>::iterator pointIterator;
    pointIterator = pPointMap->begin();

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    BYTE *nASDUVarSQ;//可变结构限定词
    BYTE *pAddress;

    int number = 0;///计数
    int maximumTag = 0;///最大信息体数
    *pCur++ = M_IT_NA_1;//类型标识 累计量

    nASDUVarSQ = pCur;//可变结构限定词
    *pCur++ = 0x80;   //可变结构限定词  0x80代表顺序的

    *pCur++ = nReason;//传送原因低位  响应总召唤
    *pCur++ = 0;   //传输原因高位

    *pCur++ = nCommonAddress;//ASDU公共地址 (一般为子站地址)
    *pCur++ = 0;             //公共地址高位

    pAddress = pCur;
    int nPointNumber = pointIterator.key();

    *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
    *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
    *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
    ASDU101_BCR bcr;
    if (maximumTag == 0)
        maximumTag = 240/(bcr.GetSize());
    if (maximumTag > 127)
        maximumTag = 127;
    for (;;)
    {
        CGI_D_IEC_104_PointYM *pPoint = pointIterator.value();
        CTagI *tag = pPoint->GetTag();
        bcr.VALUE = 0;//每次将原值此初始化
        if (tag == NULL)
        {
            bcr.S  = 0;
            bcr.SQ = 0;
            bcr.CY = 0;
            bcr.CV = 0;
            bcr.IV = 1;//无效
        }else
        {
            if (tag->GetPointCollection() == NULL)
            {
                bcr.S  = 0;
                bcr.SQ = 0;
                bcr.CY = 0;
                bcr.CV = 0;
                bcr.IV = 1;//无效
            }else
            {
                bcr.VALUE = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
                pPoint->GetValue()->SetVarValue(tag->GetPointCollection()->GetValue()->GetVarValue());

                bcr.S  = 0;
                bcr.SQ = 0;
                bcr.CY = 0;
                bcr.CV = 0;
                bcr.IV = 0;//有效
            }
        }
        memcpy(pCur,(BYTE *)&bcr,bcr.GetSize());
        pCur += bcr.GetSize();

        if (++pointIterator == pPointMap->end())
        {
            *nASDUVarSQ |= ++number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            return true;
        }
        ++nPointNumber;
        if ( (++number>=maximumTag) || (nPointNumber!=pointIterator.key()))
        {
            *nASDUVarSQ |= number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            nPointNumber = pointIterator.key();
            pCur = pAddress;
            *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
            *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
            *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
            number = 0;
        }
    }
}

/*!
 * \brief  功能概述 合成并发送电度总召唤激活确认帧
 * \param  参数描述 nSendNo是发送序号，nReceiveNo是接收序号，nCommonAddress是公共地址
 * \return 返回值描述 如果成功则返回true，否则返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolDeliverIEC_104::Send_IntrogenCallDDLActivationOK(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    //C_CI_NA_1 ACT
    //说明：格式为：

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    *pCur++ = C_CI_NA_1;//0x65;// 101 电能脉冲召唤命令
    *pCur++ = 1;   //结构限定词
    *pCur++ = 0x07;   //传输原因 0x07=激活确认
    *pCur++ = 0; //传输原因高位

    *pCur++ = nCommonAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    *pCur++ = 0; //信息体地址低位
    *pCur++ = 0; //信息体地址高位
    *pCur++ = 0; //信息体地址高位

    *pCur++ = 0x05;//总召唤限定词(Qcc)

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
}

/*!
 * \brief  功能概述 合成并发送电度总召唤激活结束帧
 * \param  参数描述 nSendNo是发送序号，nReceiveNo是接收序号，nCommonAddress是公共地址
 * \return 返回值描述 如果成功则返回true，否则返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolDeliverIEC_104::Send_IntrogenCallDDLActivationEnd(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    //C_CI_NA_1 ACT
    //说明：格式为：

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    *pCur++ = C_CI_NA_1;//0x65;// 101 电能脉冲召唤命令
    *pCur++ = 1;   //结构限定词
    *pCur++ = 0x0A;   //传输原因 0x0A=激活结束
    *pCur++ = 0; //传输原因高位

    *pCur++ = nCommonAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    *pCur++ = 0; //信息体地址低位
    *pCur++ = 0; //信息体地址高位
    *pCur++ = 0; //信息体地址高位

    *pCur++ = 0x05;//总召唤限定词(Qcc)

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
}

/*!
 * \brief  功能概述 合成并发送对时报文帧
 * \param  参数描述 nSendNo是发送序号，nReceiveNo是接收序号，nCommonAddress是公共地址
 * \return 返回值描述 如果成功则返回true，否则返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolDeliverIEC_104::Send_I_C_CS_NA_1(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    //C_CS_NA_1 ACT
    //说明：格式为：
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    *pCur++ = C_CS_NA_1;//0x67;//103 类型标识
    *pCur++ = 1;   //结构限定词
    *pCur++ = 6;   //传输原因 6=激活
    *pCur++ = 0; //传输原因高位

    *pCur++ = nCommonAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    *pCur++ = 0; //信息体地址低位
    *pCur++ = 0; //信息体地址高位
    *pCur++ = 0; //信息体地址高位

    ASDU101_CP56Time2a synTime;
    synTime.SetCurretTime();
    memcpy(pCur, &synTime, synTime.GetSize());
    pCur += synTime.GetSize();
    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
}

/*!
 * \brief  功能概述 合成并发送可变帧报文
 * \param  参数描述 nSendNo是发送序号，nReceiveNo是接收序号，pAsdu是asdu指针，nAsduSize是ASDU大小
 * \return 返回值描述 如果成功则返回true，否则返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolDeliverIEC_104::MakeQuery_I(WORD &nSendNo, WORD nReceiveNo, BYTE *pAsdu, int nAsduSize)
{
    Q_ASSERT(pAsdu != NULL);
    Q_ASSERT(nAsduSize>=0);
    Q_ASSERT(nAsduSize<=249);

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    *pCur++ = 0x68;
    *pCur++ = nAsduSize+4;

    WORD nTemp = (nSendNo++ <<1);
    *pCur++ = LOBYTE(nTemp);
    *pCur++ = HIBYTE(nTemp);

    nTemp = (nReceiveNo <<1);
    *pCur++ = LOBYTE(nTemp);
    *pCur++ = HIBYTE(nTemp);

    memcpy(pCur,pAsdu,nAsduSize);
    pCur += nAsduSize;


    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    SendFrame(szSend, pCur-szSend);
    return true;
}


#if 0
bool CProtocolDeliverIEC_104::Send_I_YC(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    // 归一化遥测值    带品质描述、不带时标
    ///功能：总召唤遥测帧返回
    ///参数：nSendNo 发送序号 nReceiveNo 设备地址 nCommonAddress公共地址
    /// ADSUType ASDU类型
    ///
    /// 返回值：true
    /// 建立日期: 2014-09-12
    if (YC_PointMap.isEmpty())
        return false;

    QMap<int,CPointIEC_104 *>::iterator tagIterator;
    tagIterator = YC_PointMap.begin();

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    BYTE *nASDUVarSQ;//可变结构限定词
    BYTE *pAddress;

    int number = 0;///计数
    int maximumTag = 0;///最大信息体数
    *pCur++ = M_ME_NA_1;//类型标识  0x09 总召唤遥测帧返回

    nASDUVarSQ = pCur;//可变结构限定词
    *pCur++ = 0x80;   //可变结构限定词  0x80代表顺序的

    *pCur++ = 0x14;//传送原因低位  响应总召唤
    *pCur++ = 0;   //传输原因高位

    *pCur++ = nCommonAddress;//ASDU公共地址 (一般为子站地址)
    *pCur++ = 0;             //公共地址高位

    pAddress = pCur;
    int nPointNumber = tagIterator.key();

    *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
    *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
    *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
    ASDU101_NVA nva;///归一化值
    ASDU101_QDS qds;
    if (maximumTag == 0)
        maximumTag = 240/(nva.GetSize()+qds.GetSize());
    if (maximumTag > 127)
        maximumTag = 127;
    for (;;)
    {
        CTagI *tag = m_pRTDB->FindTag(tagIterator.value()->m_strRTDBName);
        nva.nValue = 0;//每次将原值此初始化
        if (tag == NULL)
        {
            qds.OV  = 0;
            qds.RES = 0;
            qds.BL  = 0;
            qds.SB  = 0;
            qds.NT  = 0;
            qds.IV  = 1;//无效
        }else
        {
            if (tag->GetPointCollection() == NULL)
            {
                qds.OV  = 0;
                qds.RES = 0;
                qds.BL  = 0;
                qds.SB  = 0;
                qds.NT  = 0;
                qds.IV  = 1;//无效
            }else
            {
                nva.nValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
                qds.OV  = 0;
                qds.RES = 0;
                qds.BL  = 0;
                qds.SB  = 0;
                qds.NT  = 0;
                qds.IV  = 0;//有效
            }
        }
        memcpy(pCur,(BYTE *)&nva,nva.GetSize());
        pCur += nva.GetSize();
        memcpy(pCur,(BYTE *)&qds,qds.GetSize());
        pCur += qds.GetSize();

        if (++tagIterator == YC_PointMap.end())
        {
            *nASDUVarSQ |= ++number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            return true;
        }
        ++nPointNumber;
        if ( (++number>=maximumTag) || (nPointNumber!=tagIterator.key()))
        {
            *nASDUVarSQ |= number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            nPointNumber = tagIterator.key();
            pCur = pAddress;
            *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
            *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
            *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
            number = 0;
        }
    }
}
#endif

#if 0
bool CProtocolDeliverIEC_104::Send_I_YX(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    ///功能：总召唤遥信返回
    ///参数：nSendNo 发送序号 nReceiveNo 设备地址 nCommonAddress公共地址
    /// ADSUType ASDU类型
    /// M_SP_NA_1  单点信息
    /// M_DP_NA_1  双点信息
    ///   二进制状态信息
    ///  子站状态信息
    /// 返回值：true
    /// 建立日期: 2014-09-12
    if (YX_PointMap.isEmpty())
        return false;

    QMap<int,CPointIEC_104 *>::iterator tagIterator;
    tagIterator = YX_PointMap.begin();

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    BYTE *nASDUVarSQ;//可变结构限定词
    BYTE *pAddress;

    int number = 0;///计数
    int maximumTag = 0;///最大信息体数

    *pCur++ = M_SP_NA_1;//类型标识  0x01 单点信息

    nASDUVarSQ = pCur;//可变结构限定词
    *pCur++ = 0x80;   //可变结构限定词  0x80代表顺序的

    *pCur++ = 0x14;//传送原因低位  响应总召唤
    *pCur++ = 0;   //传输原因高位

    *pCur++ = nCommonAddress;//ASDU公共地址 (一般为子站地址)
    *pCur++ = 0;             //公共地址高位

    int nPointNumber = tagIterator.key();
    pAddress = pCur;
    *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
    *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
    *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
    ASDU101_SIQ siq; ///带品质描述词的单点信息
    if (maximumTag == 0)
        maximumTag = 240/siq.GetSize();
    if (maximumTag > 127)
        maximumTag = 127;
    for (;;)
    {
//        qDebug() << tagIterator.key() << ": " << tagIterator.value()->m_strRTDBName;
        CTagI *tag = m_pRTDB->FindTag(tagIterator.value()->m_strRTDBName);
        if (tag == NULL)
        {
            siq.IV = 1;
            siq.SPI = 0;
            siq.RES = 0;
            siq.BL = 0;
            siq.SB = 0;
            siq.NT = 0;
        }else
        {
            if (tag->GetPointCollection() == NULL)
            {
                siq.IV = 1;
                siq.SPI = 0;
                siq.RES = 0;
                siq.BL = 0;
                siq.SB = 0;
                siq.NT = 0;
            }else
            {
                siq.IV = 0;
                unsigned int bValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
                if (bValue)
                    siq.SPI = 1;
                else
                    siq.SPI = 0;
                siq.RES = 0;
                siq.BL = 0;
                siq.SB = 0;
                siq.NT = 0;
            }
        }

        memcpy(pCur,(BYTE *)&siq,siq.GetSize());
        pCur += siq.GetSize();

        if (++tagIterator == YX_PointMap.end())
        {
            *nASDUVarSQ |= ++number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            return true;
        }
        if (++number >= maximumTag)
        {
            *nASDUVarSQ |= number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            nPointNumber = tagIterator.key();
            pCur = pAddress;
            *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
            *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
            *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
            number = 0;
        }
    }
}
bool CProtocolDeliverIEC_104::Send_I_YC(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    ///功能：总召唤遥测帧返回
    ///参数：nSendNo 发送序号 nReceiveNo 设备地址 nCommonAddress公共地址
    /// ADSUType ASDU类型
    ///
    /// 返回值：true
    /// 建立日期: 2014-09-12
    if (YC_PointMap.isEmpty())
        return false;

    QMap<int,CPointIEC_104 *>::iterator tagIterator;
    tagIterator = YC_PointMap.begin();

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    BYTE *nASDUVarSQ;//可变结构限定词
    BYTE *pAddress;

    int number = 0;///计数
    int maximumTag = 0;///最大信息体数
    *pCur++ = M_ME_NA_1;//类型标识  0x09 总召唤遥测帧返回

    nASDUVarSQ = pCur;//可变结构限定词
    *pCur++ = 0x80;   //可变结构限定词  0x80代表顺序的

    *pCur++ = 0x14;//传送原因低位  响应总召唤
    *pCur++ = 0;   //传输原因高位

    *pCur++ = nCommonAddress;//ASDU公共地址 (一般为子站地址)
    *pCur++ = 0;             //公共地址高位

    pAddress = pCur;
    int nPointNumber = tagIterator.key();

    *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
    *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
    *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
    ASDU101_NVA nva;///归一化值
    ASDU101_QDS qds;
    if (maximumTag == 0)
        maximumTag = 240/(nva.GetSize()+qds.GetSize());
    if (maximumTag > 127)
        maximumTag = 127;
    for (;;)
    {
        CTagI *tag = m_pRTDB->FindTag(tagIterator.value()->m_strRTDBName);
        nva.nValue = 0;//每次将原值此初始化
        if (tag == NULL)
        {
            qds.OV  = 0;
            qds.RES = 0;
            qds.BL  = 0;
            qds.SB  = 0;
            qds.NT  = 0;
            qds.IV  = 1;//无效
        }else
        {
            if (tag->GetPointCollection() == NULL)
            {
                qds.OV  = 0;
                qds.RES = 0;
                qds.BL  = 0;
                qds.SB  = 0;
                qds.NT  = 0;
                qds.IV  = 1;//无效
            }else
            {
                nva.nValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
                qds.OV  = 0;
                qds.RES = 0;
                qds.BL  = 0;
                qds.SB  = 0;
                qds.NT  = 0;
                qds.IV  = 0;//有效
            }
        }
        memcpy(pCur,(BYTE *)&nva,nva.GetSize());
        pCur += nva.GetSize();
        memcpy(pCur,(BYTE *)&qds,qds.GetSize());
        pCur += qds.GetSize();

        if (++tagIterator == YC_PointMap.end())
        {
            *nASDUVarSQ |= ++number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            return true;
        }
        if (++number >= maximumTag)
        {
            *nASDUVarSQ |= number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            nPointNumber = tagIterator.key();
            pCur = pAddress;
            *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
            *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
            *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
            number = 0;
        }
    }
}

bool CProtocolDeliverIEC_104::Send_I_YM(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    /// 功能：总召唤电度量返回
    /// 参数：nSendNo 发送序号 nReceiveNo 设备地址 nCommonAddress公共地址
    /// ADSUType ASDU类型
    ///
    /// 返回值：true
    /// 建立日期: 2014-09-12
    if (YM_PointMap.isEmpty())
        return false;

    QMap<int,CPointIEC_104 *>::iterator tagIterator;
    tagIterator = YM_PointMap.begin();

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    BYTE *nASDUVarSQ;//可变结构限定词
    BYTE *pAddress;

    int number = 0;///计数
    int maximumTag = 0;///最大信息体数

    *pCur++ = M_IT_NA_1;//类型标识  0x09 总召唤遥测帧返回

    nASDUVarSQ = pCur;//可变结构限定词
    *pCur++ = 0x80;   //可变结构限定词  0x80代表顺序的

    *pCur++ = 0x05;//传送原因低位  响应总召唤
    *pCur++ = 0;   //传输原因高位

    *pCur++ = nCommonAddress;//ASDU公共地址 (一般为子站地址)
    *pCur++ = 0;             //公共地址高位

    pAddress = pCur;
    int nPointNumber = tagIterator.key();

    *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
    *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
    *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
    ASDU101_BCR bcr;
    if (maximumTag == 0)
        maximumTag = 240/(bcr.GetSize());
    if (maximumTag > 127)
        maximumTag = 127;
    for (;;)
    {
        CTagI *tag = m_pRTDB->FindTag(tagIterator.value()->m_strRTDBName);
        bcr.VALUE = 0;//每次将原值此初始化
        if (tag == NULL)
        {
            bcr.S  = 0;
            bcr.SQ = 0;
            bcr.CY = 0;
            bcr.CV = 0;
            bcr.IV = 1;//无效
        }else
        {
            if (tag->GetPointCollection() == NULL)
            {
                bcr.S  = 0;
                bcr.SQ = 0;
                bcr.CY = 0;
                bcr.CV = 0;
                bcr.IV = 1;//无效
            }else
            {
                bcr.VALUE = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();

                bcr.S  = 0;
                bcr.SQ = 0;
                bcr.CY = 0;
                bcr.CV = 0;
                bcr.IV = 0;//有效
            }
        }
        memcpy(pCur,(BYTE *)&bcr,bcr.GetSize());
        pCur += bcr.GetSize();

        if (++tagIterator == YM_PointMap.end())
        {
            *nASDUVarSQ |= ++number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            return true;
        }
        if (++number >= maximumTag)
        {
            *nASDUVarSQ |= number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            nPointNumber = tagIterator.key();
            pCur = pAddress;
            *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
            *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
            *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
            number = 0;
        }
    }
}
#endif

/*!
 * \brief  功能概述 发送数据
 * \param  参数描述 pSource_是发送报文的头指针，nLength_是发送报文的长度
 * \return 返回值描述 发送成功返回true，失败则返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolDeliverIEC_104::SendFrame(const BYTE *pSource_, int nLength_)
{
    QMutexLocker lock(&m_mutexSend);
    m_SendFrameList.append(QByteArray((const char*)pSource_,nLength_));
    return true;
//    m_ByteArrayFrame = QByteArray((const char*)pSource_,nLength_);

//    if (m_pLink->write(m_ByteArrayFrame))
//    {
//        ShowMessage(SHOWMESSAGE_TYPE_SEND,m_ByteArrayFrame.toHex().toUpper());
//        return true;
//    }else
//    {
//        return false;
//    }
}
void CProtocolDeliverIEC_104::SendFrame()
{
    QMutexLocker lock(&m_mutexSend);
    if (m_SendFrameList.isEmpty())
    {
        return ;
    }else
    {
        if (m_pLink->write(m_SendFrameList.at(0)))
        {
            ShowMessage(SHOWMESSAGE_TYPE_SEND,m_SendFrameList.at(0).toHex().toUpper());
            m_SendFrameList.removeAt(0);
        }else
        {
            ShowMessage(SHOWMESSAGE_TYPE_DEBUG,m_SendFrameList.at(0).toHex().toUpper());
        }
    }
}

/*!
 * \brief  功能概述 遥测变位函数
 * \param  参数描述 pPointBase_是发生变化的遥信点，VarSet_是发生变化的值
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolDeliverIEC_104::YC_Change(CPointBase *pPointBase_, QVariant VarSet_)
{
    qDebug()<<__func__<<__LINE__<<pPointBase_->m_strPointName<<pPointBase_->GetValue()->GetVarValue()<<VarSet_;
    emit SignalMakeQuery_I_YC_ChangeYC(pPointBase_, VarSet_);
    qDebug()<<__func__<<__LINE__<<pPointBase_->m_strPointName<<pPointBase_->GetValue()->GetVarValue()<<VarSet_;
}
#if 0
/*!
 * \brief  功能概述 合成并发送I帧，遥测变位帧
 * \param  参数描述 pPointBase_是发生变化的遥信点，VarSet_是发生变化的值
 * \return 返回值描述 若成功则返回true，失败则返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolDeliverIEC_104::MakeQuery_I_YC_ChangeYC(CPointBase *pPointBase_, QVariant VarSet_)
{
    BYTE szSend[BUFFER_SIZE] = {0};
    int nPointAddress = 0;
    CTagI *tag;
    QMap<int,CPointIEC_104 *>::iterator PointIterator;
    BYTE *pCur = szSend;

    *pCur++ = M_ME_TD_1;
    *pCur++ = 1;   //一个信息
    *pCur++ = 3;   //传输原因 3=突发事件
    *pCur++ = 0; //传输原因高位

    *pCur++ = m_nCommonAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    ASDU101_NVA nva;///归一化值
    ASDU101_QDS qds;

    tag = pPointBase_->GetTag();
    if (tag == NULL)
        return false;
    nva.nValue = 0;//每次将原值此初始化
    if (tag == NULL)
    {
        qds.OV  = 0;
        qds.RES = 0;
        qds.BL  = 0;
        qds.SB  = 0;
        qds.NT  = 0;
        qds.IV  = 1;//无效
    }else
    {
        if (tag->GetPointCollection() == NULL)
        {
            qds.OV  = 0;
            qds.RES = 0;
            qds.BL  = 0;
            qds.SB  = 0;
            qds.NT  = 0;
            qds.IV  = 1;//无效
        }else
        {
            nva.nValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
            qds.OV  = 0;
            qds.RES = 0;
            qds.BL  = 0;
            qds.SB  = 0;
            qds.NT  = 0;
            qds.IV  = 0;//有效
        }
    }
    for (PointIterator = YC_PointMap.begin(); PointIterator != YC_PointMap.end(); ++PointIterator)
    {
        if(PointIterator.value()->m_strRTDBName == pPointBase_->m_strRTDBName)
        {
            float SrcValue =PointIterator.value()->GetValue()->GetVarValue().toFloat();
            if (SrcValue < 0)
                SrcValue = -SrcValue;

            float NewValue = VarSet_.toFloat();
            if (NewValue < 0)
                NewValue = -NewValue;
            float ChangePercentage = PointIterator.value()->m_fChangePercentage;
            if (ChangePercentage < 0)
                ChangePercentage = -ChangePercentage;

            if ( ((SrcValue>NewValue)?(SrcValue-NewValue):(NewValue-SrcValue)) > (SrcValue*ChangePercentage))
            {
                PointIterator.value()->GetValue()->SetVarValue(VarSet_);
            }else
            {
                return false;
            }
            nPointAddress = PointIterator.key();
        }
    }
    *pCur++ = nPointAddress & 0x000000ff;        //信息体地址低位
    *pCur++ = (nPointAddress & 0x0000ff00)>>8;   //信息体地址高位
    *pCur++ = (nPointAddress & 0x00ff0000)>>16;  //信息体地址高位
    memcpy(pCur,(BYTE *)&nva,nva.GetSize());
    pCur += nva.GetSize();
    memcpy(pCur,(BYTE *)&qds,qds.GetSize());
    pCur += qds.GetSize();

    ASDU101_CP56Time2a synTime;
    synTime.SetCurretTime();
    memcpy(pCur, &synTime, synTime.GetSize());
    pCur += synTime.GetSize();

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(m_nSendIdx,m_nReciveIdx,szSend, pCur-szSend);
}
#else
/*!
 * \brief  功能概述 合成并发送I帧，遥测变位帧
 * \param  参数描述 pPointBase_是发生变化的遥信点，VarSet_是发生变化的值
 * \return 返回值描述 若成功则返回true，失败则返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolDeliverIEC_104::MakeQuery_I_YC_ChangeYC(CPointBase *pPointBase_, QVariant VarSet_)
{
    qDebug()<<__func__<<__LINE__<<pPointBase_->m_strPointName<<pPointBase_->GetValue()->GetVarValue()<<VarSet_;
    qDebug()<<__func__<<__LINE__<<pPointBase_->m_strPointName<<pPointBase_->GetValue()->GetVarValue()<<VarSet_;
    CGI_D_IEC_104_PointYC *pPointYC = (CGI_D_IEC_104_PointYC *)pPointBase_;
    float SrcValue = pPointYC->GetValue()->GetVarValue().toFloat();
    if (SrcValue < 0)
        SrcValue = -SrcValue;

    float NewValue = VarSet_.toFloat();
    if (NewValue < 0)
        NewValue = -NewValue;
    float ChangePercentage = 0.05;
    if (ChangePercentage < 0)
        ChangePercentage = -ChangePercentage;

    float chazhi = ((SrcValue>NewValue)?(SrcValue-NewValue):(NewValue-SrcValue));
    qDebug()<<__func__<<__LINE__<<"chazhi"<<chazhi<<"(SrcValue*ChangePercentage)"<<(SrcValue*ChangePercentage)<<pPointBase_->m_strRTDBName<<"SrcValue"<<SrcValue<<"NewValue"<<NewValue<<"ChangePercentage"<<ChangePercentage<<VarSet_;

    if ( chazhi > (SrcValue*ChangePercentage))
    {
        qDebug()<<__func__<<__LINE__<<pPointBase_->m_strRTDBName<<"SrcValue"<<SrcValue<<"NewValue"<<NewValue<<"ChangePercentage"<<ChangePercentage<<VarSet_;
        pPointYC->GetValue()->SetVarValue(VarSet_);
        qDebug()<<__func__<<__LINE__<<pPointBase_->m_strRTDBName<<"SrcValue"<<SrcValue<<"NewValue"<<NewValue<<"ChangePercentage"<<ChangePercentage<<VarSet_;
    }else
    {
        qDebug()<<__func__<<__LINE__<<pPointBase_->m_strRTDBName<<"SrcValue"<<SrcValue<<"NewValue"<<NewValue<<"ChangePercentage"<<ChangePercentage<<VarSet_;
        return false;
    }
//    qDebug()<<
    qDebug()<<__func__<<__LINE__<<pPointBase_->m_strRTDBName<<"SrcValue"<<SrcValue<<"NewValue"<<NewValue<<"ChangePercentage"<<ChangePercentage<<VarSet_;
    switch (pPointYC->Get104Type()) {
    case M_ME_NA_1:
    case M_ME_ND_1:
        Make_I_YC_M_ME_TD_1(pPointYC);
        break;
    case M_ME_NB_1:
        Make_I_YC_M_ME_TE_1(pPointYC);
        break;
    case M_ME_NC_1:
        Make_I_YC_M_ME_TF_1(pPointYC);
        break;
    default:
        break;
    }
    return true;
}

bool CProtocolDeliverIEC_104::MakeQuery_I_YM_ChangeYM(CPointBase *pPointBase_, QVariant VarSet_)
{
    CGI_D_IEC_104_PointYM *pPointYM = (CGI_D_IEC_104_PointYM *)pPointBase_;
    float SrcValue = pPointYM->GetValue()->GetVarValue().toFloat();
    if (SrcValue < 0)
        SrcValue = -SrcValue;

    float NewValue = VarSet_.toFloat();
    if (NewValue < 0)
        NewValue = -NewValue;
    float ChangePercentage = 0.05;
    if (ChangePercentage < 0)
        ChangePercentage = -ChangePercentage;

    float chazhi = ((SrcValue>NewValue)?(SrcValue-NewValue):(NewValue-SrcValue));
    qDebug()<<__func__<<__LINE__<<"chazhi"<<chazhi<<"(SrcValue*ChangePercentage)"<<(SrcValue*ChangePercentage)<<pPointBase_->m_strRTDBName<<"SrcValue"<<SrcValue<<"NewValue"<<NewValue<<"ChangePercentage"<<ChangePercentage<<VarSet_;

    if ( chazhi > (SrcValue*ChangePercentage))
    {
        qDebug()<<__func__<<__LINE__<<pPointBase_->m_strRTDBName<<"SrcValue"<<SrcValue<<"NewValue"<<NewValue<<"ChangePercentage"<<ChangePercentage<<VarSet_;
        pPointYM->GetValue()->SetVarValue(VarSet_);
        qDebug()<<__func__<<__LINE__<<pPointBase_->m_strRTDBName<<"SrcValue"<<SrcValue<<"NewValue"<<NewValue<<"ChangePercentage"<<ChangePercentage<<VarSet_;
    }else
    {
        qDebug()<<__func__<<__LINE__<<pPointBase_->m_strRTDBName<<"SrcValue"<<SrcValue<<"NewValue"<<NewValue<<"ChangePercentage"<<ChangePercentage<<VarSet_;
        return false;
    }
//    qDebug()<<
    qDebug()<<__func__<<__LINE__<<pPointBase_->m_strRTDBName<<"SrcValue"<<SrcValue<<"NewValue"<<NewValue<<"ChangePercentage"<<ChangePercentage<<VarSet_;
    Make_I_YM_M_IT_TB_1(pPointYM);
    return true;
}
#endif


/*!
 * \brief  功能概述 遥信变位函数
 * \param  参数描述 pPointBase_是发生变化的遥测点，VarSet_是发生变化的值
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolDeliverIEC_104::YX_Change(CPointBase *pPointBase_, QVariant VarSet_)
{
    emit SignalMakeQuery_I_YX_ChangeYX(pPointBase_, VarSet_);
}

void CProtocolDeliverIEC_104::YM_Change(CPointBase *pPointBase_, QVariant VarSet_)
{
    emit SignalMakeQuery_I_YM_ChangeYM(pPointBase_,VarSet_);
}

/*!
 \brief 单点遥控处理函数

 \fn CProtocolDeliverIEC_104::ProcessASDU101Type_C_SC_NA_1
 \param Query_
*/
bool CProtocolDeliverIEC_104::ProcessASDU101Type_C_SC_NA_1(CIEC104DeliverQuery_new &Query_)
{
    ASDU101 *pAsdu = Query_.GetAsdu();
    ASDU101_SCO Sco;
    memcpy((BYTE *)&Sco,Query_.GetBuffer(15),Sco.GetSize());
    WORD dwReason = pAsdu->REASON;
    int nPointNumber = pAsdu->GetPoint(pAsdu->m_pInfo);
    qDebug()<<__func__<<__LINE__;

    WORD nCommonAddress = pAsdu->m_nCommonAddress;

    CGI_D_IEC_104_PublicAddress_Node *pPublicAddressPointNode = m_PointNode.GetCommandAddress_PointNode()->value(nCommonAddress);
    if (pPublicAddressPointNode == NULL)
    {
        qDebug()<<__func__<<__LINE__<<"没有此公共地址的点"<<"nCommonAddress"<<nCommonAddress;
        dwReason = ASDU101_TR_UNKNOW_MESSAGE_ADDR;
    }else
    {
        CGI_D_IEC_104_PointYK *pPoint = pPublicAddressPointNode->Get_YK_PointMap()->value(nPointNumber);
        if (pPoint == NULL)
        {
            qDebug()<<__func__<<__LINE__<<"遥控点中没有此点号的点"<<"nPointNumber"<<nPointNumber;
            dwReason = ASDU101_TR_UNKNOW_MESSAGE_ADDR;
        }else
        {
            CTagI *pTag = pPoint->GetTag();
            if (pTag == NULL)
            {
                qDebug()<<__func__<<__LINE__<<"在实时库中没有此点"<<"pPoint->m_strRTDBName"<<pPoint->m_strRTDBName;
                dwReason = ASDU101_TR_UNKNOW_MESSAGE_ADDR;
            }else
            {

                qDebug()<<__func__<<__LINE__<<nPointNumber;
                if (dwReason == ASDU101_TR_ACT)
                {
                    qDebug()<<__func__<<__LINE__<<Sco.S_E<<pPoint->m_nSE ;
                    if (Sco.S_E)
                    {/// S_E=1：遥控选择命令
                        pPoint->m_nSE = CGI_D_IEC_104_PointYK::Point_YK_Select;
                        qDebug()<<__func__<<__LINE__<<pPoint->m_nSE ;
                        dwReason = ASDU101_TR_ACTCON;
                    }else
                    {/// S_E=0：遥控执行命令
                        if (pPoint->m_nSE == CGI_D_IEC_104_PointYK::Point_YK_Select)
                        {
                            qDebug()<<__func__<<__LINE__<<pPoint->m_nSE<<"已经选择" ;
                            if (Sco.SCS)
                            {
                                QVariant value(1);
                                qDebug()<<__func__<<__LINE__<<"value"<<value;
                                pPoint->GetTag()->SetValue(pPoint,value,value);
                                dwReason = ASDU101_TR_ACTCON;
                                pPoint->m_nSE = CGI_D_IEC_104_PointYK::Point_YK_NoSelect;
                            }else
                            {
                                QVariant value(0);
                                qDebug()<<__func__<<__LINE__<<"value"<<value;
                                pPoint->GetTag()->SetValue(pPoint,value,value);
                                dwReason = ASDU101_TR_ACTCON;
                                pPoint->m_nSE = CGI_D_IEC_104_PointYK::Point_YK_NoSelect;
                            }
                        }else
                        {
                            qDebug()<<__func__<<__LINE__<<pPoint->m_nSE<<"未选择" ;
                            dwReason = ASDU101_TR_ACTTERM;
                        }

                    }
                }else if (dwReason == ASDU101_TR_DEACT)
                {
                    qDebug()<<__func__<<__LINE__<<"停止激活";
                    pPoint->m_nSE = CGI_D_IEC_104_PointYK::Point_YK_NoSelect;
                    dwReason = ASDU101_TR_DEACTCON;
                }else
                {
                    qDebug()<<__func__<<__LINE__<<"未知的传送原因"<<dwReason;
                    dwReason = ASDU101_TR_UNKNOW_REASON_TYPE;
                }
            }
        }
    }

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;

    *pCur++ = C_SC_NA_1;//类型标识

    *pCur++ = 0x01;   //可变结构限定词  0x80代表顺序的

    *pCur++ = dwReason;//传送原因低位  响应总召唤
    *pCur++ = 0;   //传输原因高位

    *pCur++ = nCommonAddress;//ASDU公共地址 (一般为子站地址)
    *pCur++ = 0;             //公共地址高位

    *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
    *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
    *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
    memcpy(pCur,(BYTE *)&Sco,Sco.GetSize());
    pCur += Sco.GetSize();


    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(m_nSendIdx,m_nReciveIdx,szSend, pCur-szSend);
}

/*!
 \brief 双点遥控处理函数

 \fn CProtocolDeliverIEC_104::ProcessASDU101Type_C_DC_NA_1
 \param Query_
*/
bool CProtocolDeliverIEC_104::ProcessASDU101Type_C_DC_NA_1(CIEC104DeliverQuery_new &Query_)
{
    ASDU101 *pAsdu = Query_.GetAsdu();
    ASDU101_DCO Dco;
    memcpy((BYTE *)&Dco,Query_.GetBuffer(15),Dco.GetSize());
    WORD dwReason = pAsdu->REASON;
    int nPointNumber = pAsdu->GetPoint(pAsdu->m_pInfo);
    qDebug()<<__func__<<__LINE__;

    WORD nCommonAddress = pAsdu->m_nCommonAddress;

    CGI_D_IEC_104_PublicAddress_Node *pPublicAddressPointNode = m_PointNode.GetCommandAddress_PointNode()->value(nCommonAddress);
    if (pPublicAddressPointNode == NULL)
    {
        qDebug()<<__func__<<__LINE__<<"没有此公共地址的点"<<"nCommonAddress"<<nCommonAddress;
        dwReason = ASDU101_TR_UNKNOW_MESSAGE_ADDR;
    }else
    {
        CGI_D_IEC_104_PointYK *pPoint = pPublicAddressPointNode->Get_YK_PointMap()->value(nPointNumber);
        if (pPoint == NULL)
        {
            qDebug()<<__func__<<__LINE__<<"遥控点中没有此点号的点"<<"nPointNumber"<<nPointNumber;
            dwReason = ASDU101_TR_UNKNOW_MESSAGE_ADDR;
        }else
        {
            CTagI *pTag = pPoint->GetTag();
            if (pTag == NULL)
            {
                qDebug()<<__func__<<__LINE__<<"在实时库中没有此点"<<"pPoint->m_strRTDBName"<<pPoint->m_strRTDBName;
                dwReason = ASDU101_TR_UNKNOW_MESSAGE_ADDR;
            }else
            {

                qDebug()<<__func__<<__LINE__<<nPointNumber;
                if (dwReason == ASDU101_TR_ACT)
                {
                    qDebug()<<__func__<<__LINE__<<Dco.S_E<<pPoint->m_nSE ;
                    if (Dco.S_E)
                    {/// S_E=1：遥控选择命令
                        pPoint->m_nSE = CGI_D_IEC_104_PointYK::Point_YK_Select;
                        qDebug()<<__func__<<__LINE__<<pPoint->m_nSE ;
                        dwReason = ASDU101_TR_ACTCON;
                    }else
                    {/// S_E=0：遥控执行命令
                        if (pPoint->m_nSE == CGI_D_IEC_104_PointYK::Point_YK_Select)
                        {
                            qDebug()<<__func__<<__LINE__<<pPoint->m_nSE<<"已经选择" ;
                            QVariant value(Dco.DCS);
                            switch (Dco.DCS) {
                            case 1:
                            case 2:
                                pPoint->GetTag()->SetValue(pPoint,value,value);
                                dwReason = ASDU101_TR_ACTCON;
                                pPoint->m_nSE = CGI_D_IEC_104_PointYK::Point_YK_NoSelect;
                                break;
                            default:
                                dwReason = ASDU101_TR_ACTTERM;
                                break;
                            }
                        }else
                        {
                            qDebug()<<__func__<<__LINE__<<pPoint->m_nSE<<"未选择" ;
                            dwReason = ASDU101_TR_ACTTERM;
                        }
                    }
                }else if (dwReason == ASDU101_TR_DEACT)
                {
                    qDebug()<<__func__<<__LINE__<<"停止激活";
                    pPoint->m_nSE = CGI_D_IEC_104_PointYK::Point_YK_NoSelect;
                    dwReason = ASDU101_TR_DEACTCON;
                }else
                {
                    qDebug()<<__func__<<__LINE__<<"未知的传送原因"<<dwReason;
                    dwReason = ASDU101_TR_UNKNOW_REASON_TYPE;
                }
            }
        }
    }

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;

    *pCur++ = C_DC_NA_1;//类型标识

    *pCur++ = 0x01;   //可变结构限定词  0x80代表顺序的

    *pCur++ = dwReason;//传送原因低位  响应总召唤
    *pCur++ = 0;   //传输原因高位

    *pCur++ = nCommonAddress;//ASDU公共地址 (一般为子站地址)
    *pCur++ = 0;             //公共地址高位

    *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
    *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
    *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
    memcpy(pCur,(BYTE *)&Dco,Dco.GetSize());
    pCur += Dco.GetSize();


    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(m_nSendIdx,m_nReciveIdx,szSend, pCur-szSend);
}

/*!
 * \brief  功能概述 合成并发送I帧，遥信变位帧
 * \param  参数描述 pPointBase_是发生变化的遥测点，VarSet_是发生变化的值
 * \return 返回值描述 若成功则返回true，失败则返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolDeliverIEC_104::MakeQuery_I_YX_ChangeYX(CPointBase *pPointBase_, QVariant VarSet_)
{
    CGI_D_IEC_104_PointYX *pPointYX = (CGI_D_IEC_104_PointYX *)pPointBase_;
    if (pPointYX->GetValue()->GetVarValue() == VarSet_)
    {

        return false;
    }else
    {
        pPointYX->GetValue()->SetVarValue(VarSet_);
    }
    switch (pPointYX->Get104Type()) {
    case M_SP_NA_1:
        Make_I_YX_M_SP_TB_1(pPointYX);
        break;
    case M_DP_NA_1:
        Make_I_YX_M_DP_TB_1(pPointYX);
        break;
    default:
        break;
    }
    return true;
}

/*!
 * \brief  功能概述 合成并发送U帧  命令帧
 * \param  参数描述 bStart_是开始标志，bStop_是停止标志， bTest_是测试标志
 * \return 返回值描述 若发送成功则返回true，失败则返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolDeliverIEC_104::Send_U_Command(bool bStart_, bool bStop_, bool bTest_)
{
    Q_ASSERT(bStart_+bStop_+bTest_ == 1);

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;

    *pCur++ = 0x68;
    *pCur++ = 4;
    *pCur = 3;
    if(bStart_)
        *pCur |= 4;
    if(bStop_)
        *pCur |= 0x10;
    if(bTest_)
        *pCur |= 0x40;
    pCur++;

    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return SendFrame(szSend,pCur-szSend);
}

/*!
 \brief 合成单点遥信变化报文

 \fn CProtocolDeliverIEC_104::Make_I_YX_M_SP_TB_1
 \param pPoint_
*/
bool CProtocolDeliverIEC_104::Make_I_YX_M_SP_TB_1(CGI_D_IEC_104_PointYX *pPoint_)
{
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur;
    int nPointAddress = pPoint_->m_nPointNumber;
    CTagI *tag;
    ASDU101_SIQ siq; ///带品质描述词的单点信息

////////不带时标
    pCur = szSend;

    *pCur++ = M_SP_NA_1;
    *pCur++ = 1;   //一个变位遥信
    *pCur++ = 3;   //传输原因 3=突发事件
    *pCur++ = 0; //传输原因高位

    *pCur++ = pPoint_->m_nPublicAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位
    tag = pPoint_->GetTag();
    if (tag == NULL)
        return false;
    if (tag == NULL)
    {
        return false;
        siq.IV = 1;
        siq.SPI = 0;
        siq.RES = 0;
        siq.BL = 0;
        siq.SB = 0;
        siq.NT = 0;
    }else
    {
        if (tag->GetPointCollection() == NULL)
        {
            return false;
            siq.IV = 1;
            siq.SPI = 0;
            siq.RES = 0;
            siq.BL = 0;
            siq.SB = 0;
            siq.NT = 0;
        }else
        {
            siq.IV = 0;
            unsigned int bValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
            pPoint_->GetValue()->SetVarValue(tag->GetPointCollection()->GetValue()->GetVarValue());
            if (bValue)
                siq.SPI = 1;
            else
                siq.SPI = 0;
            siq.RES = 0;
            siq.BL = 0;
            siq.SB = 0;
            siq.NT = 0;
        }
    }
    *pCur++ = nPointAddress & 0x000000ff;        //信息体地址低位
    *pCur++ = (nPointAddress & 0x0000ff00)>>8;   //信息体地址高位
    *pCur++ = (nPointAddress & 0x00ff0000)>>16;  //信息体地址高位
    memcpy(pCur,(BYTE *)&siq,siq.GetSize());
    pCur += siq.GetSize();
    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    MakeQuery_I(m_nSendIdx,m_nReciveIdx,szSend, pCur-szSend);

///////带绝对时标
    pCur = szSend;
    *pCur++ = M_SP_TB_1;
    *pCur++ = 1;   //一个变位遥信
    *pCur++ = 3;   //传输原因 3=突发事件
    *pCur++ = 0; //传输原因高位

    *pCur++ = pPoint_->m_nPublicAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    tag = pPoint_->GetTag();
    if (tag == NULL)
    {
        return false;
        siq.IV = 1;
        siq.SPI = 0;
        siq.RES = 0;
        siq.BL = 0;
        siq.SB = 0;
        siq.NT = 0;
    }else
    {
        if (tag->GetPointCollection() == NULL)
        {
            return false;
            siq.IV = 1;
            siq.SPI = 0;
            siq.RES = 0;
            siq.BL = 0;
            siq.SB = 0;
            siq.NT = 0;
        }else
        {
            siq.IV = 0;
            unsigned int bValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
            pPoint_->GetValue()->SetVarValue(tag->GetPointCollection()->GetValue()->GetVarValue());
            if (bValue)
                siq.SPI = 1;
            else
                siq.SPI = 0;
            siq.RES = 0;
            siq.BL = 0;
            siq.SB = 0;
            siq.NT = 0;
        }
    }
    *pCur++ = nPointAddress & 0x000000ff;        //信息体地址低位
    *pCur++ = (nPointAddress & 0x0000ff00)>>8;   //信息体地址高位
    *pCur++ = (nPointAddress & 0x00ff0000)>>16;  //信息体地址高位
    memcpy(pCur,(BYTE *)&siq,siq.GetSize());
    pCur += siq.GetSize();

    ASDU101_CP56Time2a synTime;
    synTime.SetCurretTime();
    memcpy(pCur, &synTime, synTime.GetSize());
    pCur += synTime.GetSize();

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    MakeQuery_I(m_nSendIdx,m_nReciveIdx,szSend, pCur-szSend);
    return true;
}

/*!
 \brief 合成双点遥信变化报文

 \fn CProtocolDeliverIEC_104::Make_I_YX_M_DP_TB_1
 \param pPoint_
*/
bool CProtocolDeliverIEC_104::Make_I_YX_M_DP_TB_1(CGI_D_IEC_104_PointYX *pPoint_)
{
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur;
    int nPointAddress = pPoint_->m_nPointNumber;
    CTagI *tag;
    ASDU101_DIQ diq; ///带品质描述词的单点信息

////////不带时标
    pCur = szSend;

    *pCur++ = M_SP_NA_1;
    *pCur++ = 1;   //一个变位遥信
    *pCur++ = 3;   //传输原因 3=突发事件
    *pCur++ = 0; //传输原因高位

    *pCur++ = pPoint_->m_nPublicAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位
    tag = pPoint_->GetTag();
    if (tag == NULL)
        return false;
    if (tag == NULL)
    {
        return false;
        diq.IV = 1;
        diq.DPI = 0;
        diq.RES = 0;
        diq.BL = 0;
        diq.SB = 0;
        diq.NT = 0;
    }else
    {
        if (tag->GetPointCollection() == NULL)
        {
            return false;
            diq.IV = 1;
            diq.DPI = 0;
            diq.RES = 0;
            diq.BL = 0;
            diq.SB = 0;
            diq.NT = 0;
        }else
        {
            diq.IV = 0;
            unsigned int bValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
            pPoint_->GetValue()->SetVarValue(tag->GetPointCollection()->GetValue()->GetVarValue());
            if (bValue > 3)
            {
                diq.DPI = 3;
            }else
            {
                diq.DPI = bValue;
            }

            diq.RES = 0;
            diq.BL = 0;
            diq.SB = 0;
            diq.NT = 0;
        }
    }
    *pCur++ = nPointAddress & 0x000000ff;        //信息体地址低位
    *pCur++ = (nPointAddress & 0x0000ff00)>>8;   //信息体地址高位
    *pCur++ = (nPointAddress & 0x00ff0000)>>16;  //信息体地址高位
    memcpy(pCur,(BYTE *)&diq,diq.GetSize());
    pCur += diq.GetSize();
    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    MakeQuery_I(m_nSendIdx,m_nReciveIdx,szSend, pCur-szSend);

///////带绝对时标
    pCur = szSend;
    *pCur++ = M_SP_TB_1;
    *pCur++ = 1;   //一个变位遥信
    *pCur++ = 3;   //传输原因 3=突发事件
    *pCur++ = 0; //传输原因高位

    *pCur++ = pPoint_->m_nPublicAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    tag = pPoint_->GetTag();
    if (tag == NULL)
    {
        return false;
        diq.IV = 1;
        diq.DPI = 0;
        diq.RES = 0;
        diq.BL = 0;
        diq.SB = 0;
        diq.NT = 0;
    }else
    {
        if (tag->GetPointCollection() == NULL)
        {
            return false;
            diq.IV = 1;
            diq.DPI = 0;
            diq.RES = 0;
            diq.BL = 0;
            diq.SB = 0;
            diq.NT = 0;
        }else
        {
            diq.IV = 0;
            unsigned int bValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
            pPoint_->GetValue()->SetVarValue(tag->GetPointCollection()->GetValue()->GetVarValue());
            if (bValue > 3)
            {
                diq.DPI = 3;
            }else
            {
                diq.DPI = bValue;
            }
            diq.RES = 0;
            diq.BL = 0;
            diq.SB = 0;
            diq.NT = 0;
        }
    }
    *pCur++ = nPointAddress & 0x000000ff;        //信息体地址低位
    *pCur++ = (nPointAddress & 0x0000ff00)>>8;   //信息体地址高位
    *pCur++ = (nPointAddress & 0x00ff0000)>>16;  //信息体地址高位
    memcpy(pCur,(BYTE *)&diq,diq.GetSize());
    pCur += diq.GetSize();

    ASDU101_CP56Time2a synTime;
    synTime.SetCurretTime();
    memcpy(pCur, &synTime, synTime.GetSize());
    pCur += synTime.GetSize();

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    MakeQuery_I(m_nSendIdx,m_nReciveIdx,szSend, pCur-szSend);
    return true;
}

/*!
 \brief 合成归一化遥测值变化上传报文

 \fn CProtocolDeliverIEC_104::Make_I_YC_M_ME_TD_1
 \param pPoint_
 \return bool
*/
bool CProtocolDeliverIEC_104::Make_I_YC_M_ME_TD_1(CGI_D_IEC_104_PointYC *pPoint_)
{
    BYTE szSend[BUFFER_SIZE] = {0};
    int nPointAddress = pPoint_->m_nPointNumber;
    CTagI *tag;
    BYTE *pCur = szSend;

    *pCur++ = M_ME_TD_1;
    *pCur++ = 1;   //一个信息
    *pCur++ = 3;   //传输原因 3=突发事件
    *pCur++ = 0; //传输原因高位

    *pCur++ = pPoint_->m_nPublicAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    ASDU101_NVA nva;///归一化值
    ASDU101_QDS qds;

    tag = pPoint_->GetTag();
    if (tag == NULL)
        return false;
    nva.nValue = 0;//每次将原值此初始化
    if (tag == NULL)
    {
        qds.OV  = 0;
        qds.RES = 0;
        qds.BL  = 0;
        qds.SB  = 0;
        qds.NT  = 0;
        qds.IV  = 1;//无效
    }else
    {
        if (tag->GetPointCollection() == NULL)
        {
            qds.OV  = 0;
            qds.RES = 0;
            qds.BL  = 0;
            qds.SB  = 0;
            qds.NT  = 0;
            qds.IV  = 1;//无效
        }else
        {
            nva.nValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
            qds.OV  = 0;
            qds.RES = 0;
            qds.BL  = 0;
            qds.SB  = 0;
            qds.NT  = 0;
            qds.IV  = 0;//有效
        }
    }
    *pCur++ = nPointAddress & 0x000000ff;        //信息体地址低位
    *pCur++ = (nPointAddress & 0x0000ff00)>>8;   //信息体地址高位
    *pCur++ = (nPointAddress & 0x00ff0000)>>16;  //信息体地址高位
    memcpy(pCur,(BYTE *)&nva,nva.GetSize());
    pCur += nva.GetSize();
    memcpy(pCur,(BYTE *)&qds,qds.GetSize());
    pCur += qds.GetSize();

    ASDU101_CP56Time2a synTime;
    synTime.SetCurretTime();
    memcpy(pCur, &synTime, synTime.GetSize());
    pCur += synTime.GetSize();
    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(m_nSendIdx,m_nReciveIdx,szSend, pCur-szSend);
}

/*!
 \brief 合成标度化遥测值上传报文

 \fn CProtocolDeliverIEC_104::Make_I_YC_M_ME_TE_1
 \param pPoint_
 \return bool
*/
bool CProtocolDeliverIEC_104::Make_I_YC_M_ME_TE_1(CGI_D_IEC_104_PointYC *pPoint_)
{
    BYTE szSend[BUFFER_SIZE] = {0};
    int nPointAddress = pPoint_->m_nPointNumber;
    CTagI *tag;
    BYTE *pCur = szSend;

    *pCur++ = M_ME_TE_1;
    *pCur++ = 1;   //一个信息
    *pCur++ = 3;   //传输原因 3=突发事件
    *pCur++ = 0; //传输原因高位

    *pCur++ = pPoint_->m_nPublicAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    ASDU101_SVA sva;///标度化值
    ASDU101_QDS qds;

    tag = pPoint_->GetTag();
    if (tag == NULL)
        return false;
    sva.m_nValue = 0;//每次将原值此初始化
    if (tag == NULL)
    {
        qds.OV  = 0;
        qds.RES = 0;
        qds.BL  = 0;
        qds.SB  = 0;
        qds.NT  = 0;
        qds.IV  = 1;//无效
    }else
    {
        if (tag->GetPointCollection() == NULL)
        {
            qds.OV  = 0;
            qds.RES = 0;
            qds.BL  = 0;
            qds.SB  = 0;
            qds.NT  = 0;
            qds.IV  = 1;//无效
        }else
        {
            sva.m_nValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
//            pPoint_->GetValue()->SetVarValue(tag->GetPointCollection()->GetValue()->GetVarValue());
            qds.OV  = 0;
            qds.RES = 0;
            qds.BL  = 0;
            qds.SB  = 0;
            qds.NT  = 0;
            qds.IV  = 0;//有效
        }
    }
    *pCur++ = nPointAddress & 0x000000ff;        //信息体地址低位
    *pCur++ = (nPointAddress & 0x0000ff00)>>8;   //信息体地址高位
    *pCur++ = (nPointAddress & 0x00ff0000)>>16;  //信息体地址高位
    memcpy(pCur,(BYTE *)&sva,sva.GetSize());
    pCur += sva.GetSize();
    memcpy(pCur,(BYTE *)&qds,qds.GetSize());
    pCur += qds.GetSize();

    ASDU101_CP56Time2a synTime;
    synTime.SetCurretTime();
    memcpy(pCur, &synTime, synTime.GetSize());
    pCur += synTime.GetSize();

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(m_nSendIdx,m_nReciveIdx,szSend, pCur-szSend);
}

/*!
 \brief 合成短浮点遥测值变化报文

 \fn CProtocolDeliverIEC_104::Make_I_YC_M_ME_TF_1
 \param pPoint_
 \return bool
*/
bool CProtocolDeliverIEC_104::Make_I_YC_M_ME_TF_1(CGI_D_IEC_104_PointYC *pPoint_)
{
    qDebug()<<__func__<<__LINE__<<pPoint_->m_strRTDBName;///<<"SrcValue"<<SrcValue<<"NewValue"<<NewValue<<"ChangePercentage"<<ChangePercentage<<VarSet_;
    BYTE szSend[BUFFER_SIZE] = {0};
    int nPointAddress = pPoint_->m_nPointNumber;
    CTagI *tag;
    BYTE *pCur = szSend;

    *pCur++ = M_ME_TF_1;
    *pCur++ = 1;   //一个信息
    *pCur++ = 3;   //传输原因 3=突发事件
    *pCur++ = 0; //传输原因高位

    *pCur++ = pPoint_->m_nPublicAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    ASDU101_R32 r32;///短浮点数
    ASDU101_QDS qds;
    qDebug()<<__func__<<__LINE__<<pPoint_->m_strRTDBName;
    tag = pPoint_->GetTag();
    qDebug()<<__func__<<__LINE__<<pPoint_->m_strRTDBName;
    if (tag == NULL)
        return false;
    r32.m_nValue = 0;//每次将原值此初始化
    if (tag == NULL)
    {
        qds.OV  = 0;
        qds.RES = 0;
        qds.BL  = 0;
        qds.SB  = 0;
        qds.NT  = 0;
        qds.IV  = 1;//无效
    }else
    {
        qDebug()<<__func__<<__LINE__<<pPoint_->m_strRTDBName;
        if (tag->GetPointCollection() == NULL)
        {
            qDebug()<<__func__<<__LINE__<<pPoint_->m_strRTDBName;
            qds.OV  = 0;
            qds.RES = 0;
            qds.BL  = 0;
            qds.SB  = 0;
            qds.NT  = 0;
            qds.IV  = 1;//无效
        }else
        {
            qDebug()<<__func__<<__LINE__<<pPoint_->m_strRTDBName<<pPoint_->m_fKValue<<pPoint_->m_fBaseValue;
            qDebug()<<tag->GetPointCollection()->GetValue()->GetVarValue();
            qDebug()<<__func__<<__LINE__;
            r32.m_nValue = tag->GetPointCollection()->GetValue()->GetVarValue().toFloat()/pPoint_->m_fKValue-pPoint_->m_fBaseValue;
            qDebug()<<__func__<<__LINE__<<pPoint_->m_strRTDBName;
//            pPoint_->GetValue()->SetVarValue(tag->GetPointCollection()->GetValue()->GetVarValue());
            qds.OV  = 0;
            qds.RES = 0;
            qds.BL  = 0;
            qds.SB  = 0;
            qds.NT  = 0;
            qds.IV  = 0;//有效
        }
        qDebug()<<__func__<<__LINE__<<pPoint_->m_strRTDBName;
    }
    qDebug()<<__func__<<__LINE__<<pPoint_->m_strRTDBName;
    *pCur++ = nPointAddress & 0x000000ff;        //信息体地址低位
    *pCur++ = (nPointAddress & 0x0000ff00)>>8;   //信息体地址高位
    *pCur++ = (nPointAddress & 0x00ff0000)>>16;  //信息体地址高位
    memcpy(pCur,(BYTE *)&r32,r32.GetSize());
    pCur += r32.GetSize();
    memcpy(pCur,(BYTE *)&qds,qds.GetSize());
    pCur += qds.GetSize();

    ASDU101_CP56Time2a synTime;
    synTime.SetCurretTime();
    memcpy(pCur, &synTime, synTime.GetSize());
    pCur += synTime.GetSize();

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(m_nSendIdx,m_nReciveIdx,szSend, pCur-szSend);
}

bool CProtocolDeliverIEC_104::Make_I_YM_M_IT_TB_1(CGI_D_IEC_104_PointYM *pPoint_)
{
    BYTE szSend[BUFFER_SIZE] = {0};
    int nPointAddress = pPoint_->m_nPointNumber;
    BYTE *pCur = szSend;

    *pCur++ = M_IT_TB_1;
    *pCur++ = 1;   //一个信息
    *pCur++ = ASDU101_TR_SPONT;   //传输原因 3=突发事件
    *pCur++ = 0; //传输原因高位

    *pCur++ = pPoint_->m_nPublicAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    ASDU101_BCR bcr;
    memset(&bcr,0,bcr.GetSize());

    if (pPoint_->GetTag() == NULL)
    {
        bcr.IV = 1;//无效
    }else
    {
        bcr.VALUE = pPoint_->GetValue()->GetVarValue().toFloat();
        bcr.CV=1;
        bcr.IV = 0;//有效
    }

    nPointAddress = pPoint_->m_nPointNumber;

    *pCur++ = nPointAddress & 0x000000ff;        //信息体地址低位
    *pCur++ = (nPointAddress & 0x0000ff00)>>8;   //信息体地址高位
    *pCur++ = (nPointAddress & 0x00ff0000)>>16;  //信息体地址高位
    memcpy(pCur,(BYTE *)&bcr,bcr.GetSize());
    pCur += bcr.GetSize();

    ASDU101_CP56Time2a synTime;
    synTime.SetCurretTime();
    memcpy(pCur, &synTime, synTime.GetSize());
    pCur += synTime.GetSize();

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(m_nSendIdx,m_nReciveIdx,szSend,pCur-szSend);
}

/*!
 * \brief  功能概述 用于响应点的值发生变化的函数
 * \param  参数描述 pPointBase_是点的指针，VarSet_是点的值
 * \return 返回值描述 返回true
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolDeliverIEC_104::OnTagChange(CPointBase *pPointBase_, QVariant VarSet_)
{
/// 转发遥控、遥设  采集驱动中下发遥控、遥设报文
/// 生成遥控、遥设报文
/// 转发驱动中变位处理
    qDebug()<<__func__<<__LINE__<<pPointBase_->m_strPointName<<pPointBase_->GetValue()->GetVarValue()<<VarSet_;
    Q_ASSERT(pPointBase_ != NULL);
    switch (pPointBase_->m_nTagType) {
    case TAG_TYPE_AI://遥测
        YC_Change(pPointBase_,VarSet_);
        break;
    case TAG_TYPE_AO://遥设
        break;
    case TAG_TYPE_DI://遥信
        YX_Change(pPointBase_,VarSet_);
        break;
    case TAG_TYPE_DO://遥控
        break;
    case TAG_TYPE_MEM:
        YM_Change(pPointBase_,VarSet_);
        break;
    default:
        break;
    }
    return true;
}


/*!
 * \brief 功能概述 连接成功
 * \param 参数描述 isConnected_是连接标志
 * \return 返回值描述 无
 * \author zzy
 * \date 2015/5/26
 */
void CProtocolDeliverIEC_104::slotConnect(bool isConnected_)
{
    if (isConnected_)
    {
        qDebug() << "1111111111111111111111connect success"<<"成功";
        init104Param();//连接成功之后初始化发送和接收序号为0
        m_nTimerID = startTimer(10);
        while (m_nTimerID == 0) {
            m_nTimerID = startTimer(10);
        }
    }else
    {
        if (m_nTimerID != 0)
        {
            killTimer(m_nTimerID);
            m_nTimerID = 0;
        }
    }
}

void CProtocolDeliverIEC_104::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nTimerID)
    {
        SendFrame();
    }
}

#if 0
/*!
 \brief CGI 使用

 \fn CProtocolDeliverIEC_104::CGI_InitPointMap
 \param DeviceListElement_
*/
void CProtocolDeliverIEC_104::CGI_InitPointMap(const QDomElement &DeviceListElement_)
{
    QDomNodeList DeviceList = DeviceListElement_.childNodes();
    for (int i = 0; i <DeviceList.count(); ++i)
    {
        QDomElement DeviceElement = DeviceList.at(i).toElement();

        if (DeviceElement.elementsByTagName("Attribute").count() < 1)
        {
            Q_ASSERT(false);
        }
        int nDeviceAddress = DeviceElement.elementsByTagName("Attribute").at(0).toElement().attribute("Device_Address").toInt();
        m_nCommonAddress = nDeviceAddress;
        QDomNodeList TypeNodeList = DeviceElement.elementsByTagName("Type");
        for (int nType = 0; nType < TypeNodeList.count(); ++nType)
        {
            QDomElement TypeElem = TypeNodeList.at(nType).toElement();
            if (TypeElem.attribute("Name") == "YX"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_DI,nDeviceAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointIEC_104 *pPoint = new CPointIEC_104;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YX_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YX_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YX_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YX_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YX_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YX_KValue_Strings").toFloat();
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YX_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    pPoint->m_nDeviceAddr = nDeviceAddress;
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }else
                    {
                        delete pPoint;
                        continue;
                    }
                    int nPointNumber = PointElem_.attribute("Device_YX_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YX_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_DI;
                    qDebug()<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    if (!YX_PointMap.contains(nPointNumber))
                    {
                        YX_PointMap.insert(nPointNumber,pPoint);
                    }
                }
            }else if (TypeElem.attribute("Name") == "YC"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_AI,nDeviceAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointIEC_104 *pPoint = new CPointIEC_104;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YC_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YC_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YC_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YC_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YC_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YC_KValue_Strings").toFloat();
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YC_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    pPoint->m_nDeviceAddr = nDeviceAddress;
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }else
                    {
                        delete pPoint;
                        continue;
                    }
                    int nPointNumber = PointElem_.attribute("Device_YC_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YC_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_AI;
                    qDebug()<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    if (!YC_PointMap.contains(nPointNumber))
                    {
                        YC_PointMap.insert(nPointNumber,pPoint);
                    }
                }
            }else if (TypeElem.attribute("Name") == "YM"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_MEM,nDeviceAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointIEC_104 *pPoint = new CPointIEC_104;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YM_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YM_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YM_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YM_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YM_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YM_KValue_Strings").toFloat();
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YM_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    pPoint->m_nDeviceAddr = nDeviceAddress;
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }else
                    {
                        delete pPoint;
                        continue;
                    }
                    int nPointNumber = PointElem_.attribute("Device_YM_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YM_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_MEM;
                    qDebug()<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    if (!YM_PointMap.contains(nPointNumber))
                    {
                        YM_PointMap.insert(nPointNumber,pPoint);
                    }
                }
            }else if (TypeElem.attribute("Name") == "YK"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_DO,nDeviceAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointIEC_104 *pPoint = new CPointIEC_104;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YK_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YK_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YK_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YK_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YK_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YK_KValue_Strings").toFloat();
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YK_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    pPoint->m_nDeviceAddr = nDeviceAddress;
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }else
                    {
                        delete pPoint;
                        continue;
                    }
                    int nPointNumber = PointElem_.attribute("Device_YK_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YK_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_DO;
                    QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
                    qDebug()<<debug<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    if (!YK_PointMap.contains(nPointNumber))
                    {
                        YK_PointMap.insert(nPointNumber,pPoint);
                    }
                }
            }else if (TypeElem.attribute("Name") == "YS"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_AO,nDeviceAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointIEC_104 *pPoint = new CPointIEC_104;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YS_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YS_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YS_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YS_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YS_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YS_KValue_Strings").toFloat();
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YS_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    pPoint->m_nDeviceAddr = nDeviceAddress;
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }else
                    {
                        delete pPoint;
                        continue;
                    }
                    int nPointNumber = PointElem_.attribute("Device_YS_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YS_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_AO;
                    QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
                    qDebug()<<debug<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    if (!YS_PointMap.contains(nPointNumber))
                    {
                        YS_PointMap.insert(nPointNumber,pPoint);
                    }
                }
            }else{
                Q_ASSERT(false);
            }
        }
    }
}
#else
/*!
 \brief CGI 使用

 \fn CProtocolDeliverIEC_104::CGI_InitPointMap
 \param DeviceListElement_
*/
void CProtocolDeliverIEC_104::CGI_InitPointMap(const QDomElement &DeviceListElement_)
{
    QDomNodeList DeviceList = DeviceListElement_.childNodes();
    for (int i = 0; i <DeviceList.count(); ++i)
    {
        QDomElement DeviceElement = DeviceList.at(i).toElement();

        if (DeviceElement.elementsByTagName("Attribute").count() < 1 )
        {
            qFatal("No Attribute element!!!");
            Q_ASSERT(false);
        }else
        {
            m_nCommonAddress = DeviceElement.elementsByTagName("Attribute").at(0).toElement().attribute("Device_Address").toUInt();
        }
#if 1
        QDomNodeList TypeNodeList = DeviceElement.elementsByTagName("Type");
        for (int nType = 0; nType < TypeNodeList.count(); ++nType)
        {
            QDomElement TypeElem = TypeNodeList.at(nType).toElement();
            if (TypeElem.attribute("Name") == "YX")
            {
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CGI_D_IEC_104_PointYX *pPoint = new CGI_D_IEC_104_PointYX;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YX_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YX_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YX_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YX_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YX_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YX_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YX_BaseValue_Strings").toFloat();
                    QString str104type = PointElem_.attribute("Device_YX_104type_Strings");
                    if (str104type == "单点信息")
                    {
                        pPoint->m_n_IEC_104_YX_PointType = M_SP_NA_1;
                    }else if (str104type == "双点信息")
                    {
                        pPoint->m_n_IEC_104_YX_PointType = M_DP_NA_1;
                    }else
                    {
                        pPoint->m_n_IEC_104_YX_PointType = M_SP_NA_1;
                    }
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetNotify(pPoint);///设置采集点
                    }else
                    {
                        delete pPoint;
                        continue;
                    }
                    int nPointNumber = PointElem_.attribute("Device_YX_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YX_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_DI;

                    qDebug()<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    if (!m_PointNode.AddPoint(pPoint))
                    {
                        delete pPoint;
                    }
                }
            }else if (TypeElem.attribute("Name") == "YC"){
                qDebug()<<__func__<<__LINE__;
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    qDebug()<<__func__<<__LINE__;
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CGI_D_IEC_104_PointYC *pPoint = new CGI_D_IEC_104_PointYC;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YC_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YC_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YC_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YC_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YC_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YC_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YC_BaseValue_Strings").toFloat();
                    QString str104type = PointElem_.attribute("Device_YC_104type_Strings");
                    if (str104type == "归一化遥测值时标")
                    {
                        pPoint->m_n_IEC_104_YC_PointType = M_ME_NA_1;
                    }else if (str104type == "标度化遥测值")
                    {
                        pPoint->m_n_IEC_104_YC_PointType = M_ME_NB_1;
                    }else if (str104type == "短浮点遥测值")
                    {
                        pPoint->m_n_IEC_104_YC_PointType = M_ME_NC_1;
                    }else if (str104type == "归一化遥测值")
                    {
                        pPoint->m_n_IEC_104_YC_PointType = M_ME_ND_1;
                    }else
                    {
                        pPoint->m_n_IEC_104_YC_PointType = M_ME_NC_1;
                    }
                    qDebug()<<__func__<<__LINE__;
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    qDebug()<<__func__<<__LINE__;
                    if (pPoint->m_pTag)
                    {
                        qDebug()<<__func__<<__LINE__;
                        pPoint->m_pTag->SetNotify(pPoint);///设置采集点
                    }else
                    {
                        qDebug()<<__func__<<__LINE__;
                        delete pPoint;
                        continue;
                        qDebug()<<__func__<<__LINE__;
                    }
                    qDebug()<<__func__<<__LINE__;
                    int nPointNumber = PointElem_.attribute("Device_YC_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YC_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_AI;
                    qDebug()<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    qDebug()<<__func__<<__LINE__;
                    if (!m_PointNode.AddPoint(pPoint))
                    {
                        qDebug()<<__func__<<__LINE__;
                        delete pPoint;
                    }
                    qDebug()<<__func__<<__LINE__;
                }
            }else if (TypeElem.attribute("Name") == "YM"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CGI_D_IEC_104_PointYM *pPoint = new CGI_D_IEC_104_PointYM;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YM_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YM_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YM_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YM_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YM_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YM_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YM_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
//                    pPoint->m_nDeviceAddr = nDeviceAddress;
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetNotify(pPoint);///设置采集点
                    }else
                    {
                        delete pPoint;
                        continue;
                    }
                    int nPointNumber = PointElem_.attribute("Device_YM_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YM_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_MEM;
                    qDebug()<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    if (!m_PointNode.AddPoint(pPoint))
                    {
                        delete pPoint;
                    }
                }
            }else if (TypeElem.attribute("Name") == "YK"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CGI_D_IEC_104_PointYK *pPoint = new CGI_D_IEC_104_PointYK;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YK_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YK_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YK_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YK_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YK_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YK_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YK_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
//                    pPoint->m_nDeviceAddr = nDeviceAddress;
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetNotify(pPoint);///设置采集点
                    }else
                    {
                        delete pPoint;
                        continue;
                    }
                    int nPointNumber = PointElem_.attribute("Device_YK_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YK_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_DO;
                    QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
                    qDebug()<<debug<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    if (!m_PointNode.AddPoint(pPoint))
                    {
                        delete pPoint;
                    }
                }
            }else if (TypeElem.attribute("Name") == "YS"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CGI_D_IEC_104_PointYS *pPoint = new CGI_D_IEC_104_PointYS;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YS_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YS_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YS_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YS_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YS_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YS_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YS_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
//                    pPoint->m_nDeviceAddr = nDeviceAddress;
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetNotify(pPoint);///设置采集点
                    }else
                    {
                        delete pPoint;
                        continue;
                    }
                    int nPointNumber = PointElem_.attribute("Device_YS_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YS_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_AO;
                    QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
                    qDebug()<<debug<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    if (!m_PointNode.AddPoint(pPoint))
                    {
                        delete pPoint;
                    }
                }
            }else{
                qFatal("No Tag Name Type!!!");
                Q_ASSERT(false);
            }
        }
#endif
    }
}
#endif
/*!
 * \brief  功能概述 初始化值，为了当值发生变化是触发信号
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolDeliverIEC_104::InitPointMapValue()
{
    QMap<int,CPointIEC_104 *>::iterator PointIterator;
    QVariant variant("--");

    for (PointIterator = YX_PointMap.begin(); PointIterator != YX_PointMap.end();++PointIterator)
    {
        PointIterator.value()->GetValue()->SetVarValue(variant);
    }

    for (PointIterator = YC_PointMap.begin(); PointIterator != YC_PointMap.end();++PointIterator)
    {
        PointIterator.value()->GetValue()->SetVarValue(variant);
    }

    for (PointIterator = YS_PointMap.begin(); PointIterator != YS_PointMap.end();++PointIterator)
    {
        PointIterator.value()->GetValue()->SetVarValue(variant);
    }

    for (PointIterator = YK_PointMap.begin(); PointIterator != YK_PointMap.end();++PointIterator)
    {
        PointIterator.value()->GetValue()->SetVarValue(variant);
    }

    for (PointIterator = YM_PointMap.begin(); PointIterator != YM_PointMap.end();++PointIterator)
    {
        PointIterator.value()->GetValue()->SetVarValue(variant);
    }
}

/*!
 * \brief  功能概述 初始化IEC-104驱动标志位
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolDeliverIEC_104::init104Param()
{
    m_nSendIdx = 0;
    m_nReciveIdx = 0;
    InitPointMapValue();
    m_SendFrameList.clear();
}
