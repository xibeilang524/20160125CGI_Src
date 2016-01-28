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

#include "CProtocolDeliverPRT_IEC_104.h"
#include "../PortInclude/CRTDBI.h"
#include "../PortInclude/CTagI.h"
#include "../PortInclude/cvaluebase.h"
#include "../PortInclude/CPRTI.h"
#include "../PortInclude/CPRTMapI.h"
#include "cpointiec_104.h"

#include <QTimer>
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QDateTime>
#include <QDir>

//! 供运行程序调用的函数
CProtocolI * CreateDriver(QObject *parent)
{
    /*!
    * \brief  功能概述 库文件导出函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    CProtocolDeliverPRT_IEC_104 *testProtocol = new CProtocolDeliverPRT_IEC_104(parent);
    return  testProtocol;
}

CProtocolDeliverPRT_IEC_104::CProtocolDeliverPRT_IEC_104(QObject */*parent*/)
{
    /*!
    * \brief  功能概述 构造函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    m_tcpServer = NULL;
    m_TcpSocket = NULL;
    m_nMaxSRValue=32767;//收发最大值
    m_nW = 8;
    m_pWriteFrame = NULL;


    m_nCommonAddress = 0x01;
    bConnectOk = false;
    m_nTimerID = 0;
    m_nTimerID = startTimer(50);
}

void CProtocolDeliverPRT_IEC_104::TCPConnect()
{
    if (m_tcpServer == NULL)
    {
        m_tcpServer = new QTcpServer(this);
        connect(m_tcpServer,SIGNAL(newConnection()),
                this,SLOT(OnserverConnected()));
    }
    if (m_tcpServer->listen(QHostAddress(m_strTcpIp),DBPort))
    {
        qDebug()<<"open succeed-----------------------------------"<<m_strTcpIp<<DBPort;
        return;
    }else
    {
        qDebug()<<"open faild--------------------------------------"<<m_strTcpIp<<DBPort;
        QTimer::singleShot(5000,this,SLOT(TCPConnect()));
    }
}
void CProtocolDeliverPRT_IEC_104::OnserverConnected()
{
    m_TcpSocket = m_tcpServer->nextPendingConnection();
//    connect(m_TcpSocket,SIGNAL(disconnected()),m_TcpSocket,SLOT(deleteLater()));
    connect(m_TcpSocket,SIGNAL(disconnected()), this,SLOT(SocketDeleteLater()));
    connect(m_TcpSocket,SIGNAL(readyRead()),this,SLOT(ProcessRespond()));
     //readyRead()表示服务端发送数据过来即发动信号，接着revData()进行处理。
    connect(m_TcpSocket,SIGNAL(connected()),this,SLOT(OnConnected()));
    //连接成功时，返回相应信息
    connect(m_TcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));
    //当出现错误时返回错误信息
}
void CProtocolDeliverPRT_IEC_104::SocketDeleteLater()
{
    qDebug()<<"m_TcpSocekt 前"<<m_TcpSocket;
    m_TcpSocket->deleteLater();
    qDebug()<<"m_TcpSocekt 后"<<m_TcpSocket;
    m_TcpSocket = NULL;
}
bool CProtocolDeliverPRT_IEC_104::OnCreateChannel(const QString strMonitorIPAddress_, CRTDBI *pDatabaseReturn)
{
    qDebug()<<"MonitorIEC-104 ip address-------------------------------------------------"<<strMonitorIPAddress_;
//    m_pPRTMapI->FindChannelNumber(m_nChannelNumber)->SetDriverType(Transform_Type);/// @note 设置驱动的类型 该驱动为转发驱动
    m_pRTDB = pDatabaseReturn;

    m_strTcpIp = strMonitorIPAddress_;
//    m_nPort = MainPort.attribute("Port").toInt();

    InitPointMap(m_pRTDB->GetTAG_MAP());
//    init104Param();
//    OnOpenSocket();
//    TCPConnect();
    QTimer::singleShot(1000,this,SLOT(TCPConnect()));
    m_pWriteFrame = new CWriteFrame(m_strTcpIp);
    connect(this,SIGNAL(SignalSendFrame(int,QString,int)),m_pWriteFrame,SLOT(WriteMessage(int,QString,int)));
    QTimer::singleShot(1000,m_pWriteFrame,SLOT(TCPConnect()));
    return true;
}

bool CProtocolDeliverPRT_IEC_104::OnStart()
{
    /*!
    * \brief  功能概述 无
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    return false;
}

bool CProtocolDeliverPRT_IEC_104::OnScan()
{
    /*!
    * \brief  功能概述 无
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    return false;
}

bool CProtocolDeliverPRT_IEC_104::OnStop()
{
    /*!
    * \brief  功能概述 无
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    return false;
}

bool CProtocolDeliverPRT_IEC_104::OnTagChange(CPointBase */*pPointBase_*/, QVariant /*VarSet_*/)
{
    /*!
    * \brief  功能概述 值发生变化
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    return true;

}

void CProtocolDeliverPRT_IEC_104::SetPRTMap(CPRTMapI *pPRTMapI_)
{
    /*!
    * \brief  功能概述 设置驱动导出类指针和驱动的信息map的指针
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/24
    */
    m_pPRTMapI = pPRTMapI_;
}

qint64 CProtocolDeliverPRT_IEC_104::Write(const char *data, qint64 maxSize)
{
    /*!
    * \brief  功能概述 供其他驱动调用的write函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/24
    */
    QByteArray array(data,maxSize);
    qDebug()<<"DeliverPRT-IEC-104: Channel Number="<<m_nChannelNumber<<array.toHex();

    //作用:发送并显示所有报文
    return 0;
}

qint64 CProtocolDeliverPRT_IEC_104::Write(const char *data)
{
    /*!
    * \brief  功能概述 供其他驱动调用的write函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/24
    */
    QByteArray array(data);
    qDebug()<<"DeliverPRT-IEC-104: Channel Number="<<m_nChannelNumber<<array.toHex();

    //作用:发送并显示所有报文
    return 0;
}

qint64 CProtocolDeliverPRT_IEC_104::Write(const QByteArray byteArray)
{
    /*!
    * \brief  功能概述 供其他驱动调用的write函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/24
    */
    qDebug()<<"DeliverPRT-IEC-104: Channel Number="<<m_nChannelNumber<<byteArray.toHex();

    //作用:发送并显示所有报文
    return 0;
}

int CProtocolDeliverPRT_IEC_104::GetZTChannelNumber()
{
    /*!
    * \brief  功能概述 获取组态时设置的此通道的通道号
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/20
    */
    return 0;
}

QList<int> CProtocolDeliverPRT_IEC_104::GetZTDeliversChannelNumberList()
{
    /*!
    * \brief  功能概述 获取组态时设置的转发的通道号所在的list
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/20
    */
    //    return m_ZTDeliversChannelNumberList;
    return QList<int>();
}

void CProtocolDeliverPRT_IEC_104::WriteMessage(int type, QString message, int nChannelNumber)
{
//    qDebug()<<"run3:="<<type<<message<<nChannelNumber;
    if (m_pWriteFrame != NULL)
    {
//        qDebug()<<"run4:="<<type<<message<<nChannelNumber;
        emit SignalSendFrame(type,message,nChannelNumber);
    }
//    qDebug()<<"run5:="<<type<<message<<nChannelNumber;
}

void CProtocolDeliverPRT_IEC_104::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nTimerID)
    {
        if (m_TcpSocket == NULL)
        {
    //        QTimer::singleShot(1000,this,SLOT(OnOpenSocket()));
            return;
        }
        if (m_TcpSocket->isOpen())
        {
            if (!m_SendFrameList.isEmpty())
            {
                m_Lock.lock();
                if (m_TcpSocket->write(m_SendFrameList.at(0)))
                {
                    ShowMessage(0,m_ByteArrayFrame.toHex());
                    m_SendFrameList.removeAt(0);
                }
                m_Lock.unlock();
            }
        }
    }
}

void CProtocolDeliverPRT_IEC_104::InitPointMap(TAG_MAP *pTagMap)
{
    TAG_MAP::iterator iterator;
    for (iterator = pTagMap->begin(); iterator != pTagMap->end(); ++iterator)
    {
        CPointIEC_104 *pPoint = new CPointIEC_104;
        pPoint->m_pProtocol = this;
        pPoint->m_strRTDBName = iterator.key();
        pPoint->m_pTag = iterator.value();
        pPoint->m_nDeviceAddr = m_nCommonAddress;
        int nPointNumber = iterator.value()->GetTagID();
        pPoint->m_nPointNumber = nPointNumber;
        if (pPoint->m_pTag)
        {
            pPoint->m_pTag->SetNotify(pPoint);///设置转发点
        }
        switch (iterator.value()->GetType()) {
        case TAG_TYPE_DI:///遥信
            pPoint->m_nTagType = TAG_TYPE_DI;
            YX_PointMap.insert(nPointNumber,pPoint);
            break;
        case TAG_TYPE_AI:///遥测
            pPoint->m_nTagType = TAG_TYPE_AI;
            YC_PointMap.insert(nPointNumber,pPoint);
            break;
        case TAG_TYPE_MEM:///遥脉
            pPoint->m_nTagType = TAG_TYPE_MEM;
            YM_PointMap.insert(nPointNumber,pPoint);
            break;
        case TAG_TYPE_DO:///遥控
            pPoint->m_nTagType = TAG_TYPE_DO;
            YK_PointMap.insert(nPointNumber,pPoint);
            break;
        case TAG_TYPE_AO:///遥设
            pPoint->m_nTagType = TAG_TYPE_AO;
            YS_PointMap.insert(nPointNumber,pPoint);
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
}
void CProtocolDeliverPRT_IEC_104::ZFFrame(QByteArray /*datas*/)
{
    /*!
    * \brief  功能概述 透传函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/24
    */
#if 0
    QList<int> list = m_pPRTMapI->FindAllZTChannelNumber();
    for (int i = 0; i < list.count(); ++i)
    {
        for (int j = 0; j < m_ZTDeliversChannelNumberList.count(); ++j)
        {
            int nChannelNumber = list.at(i);
            qDebug()<<list.at(i)<<m_ZTDeliversChannelNumberList.at(j)<<"--------";
            if (m_ZTDeliversChannelNumberList.at(j) == nChannelNumber)
            {
                CPRTI *pPRTI = m_pPRTMapI->FindZTChannelNumber(nChannelNumber);
                if (pPRTI != NULL)///判断此通道号的转发驱动所在类是否为空
                {
                    CProtocolI *pProtocol = pPRTI->GetProcotolI();
                    if (pProtocol != NULL)///判断转发驱动是否为空
                    {
                        pProtocol->Write(datas);
                    }else
                    {
                        qDebug()<<"PRT-IEC-104:"<<"转发驱动为NULL";
                    }
                }else
                {
                    qDebug()<<"PRT-IEC-104:"<<"转发驱动所在类为NULL";
                }
            }
        }
    }
#endif
}
void CProtocolDeliverPRT_IEC_104::ProcessRespond()
{
    /*!
    * \brief  功能概述 解包函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    QByteArray datas = m_TcpSocket->readAll();
    if (datas.isEmpty())
        return;
//    qDebug()<<datas.toHex()<<"===================";
//    ZFFrame(datas);

    if (datas.count() < 6)
        return;
    if ((BYTE)datas.at(0) == 0x68)
    {
        while(!datas.isEmpty())
        {
            int nReceiveDataCount = datas.count();
            if (nReceiveDataCount < 6)
                return;
            int nReadFrameDateLen = (BYTE)datas.at(1);
            if (nReadFrameDateLen+2 > nReceiveDataCount)
                return;
            QByteArray array = datas.left(nReadFrameDateLen+2);
            datas.remove(0,nReadFrameDateLen+2);
            DecodeFrame(array);
        }
    }
}

void CProtocolDeliverPRT_IEC_104::OnConnected()
{
    /*!
    * \brief  功能概述 链接成功
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    qDebug() << "1111111111111111111111connect success"<<"成功";
    init104Param();//连接成功之后初始化发送和接收序号为0
}

void CProtocolDeliverPRT_IEC_104::displayError(QAbstractSocket::SocketError)
{
    /*!
    * \brief  功能概述 未连接服务器,重新连接
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    qDebug()<<m_TcpSocket->errorString()<<"转发驱动 链接失败";
//    m_TcpSocket->close();
}

void CProtocolDeliverPRT_IEC_104::DecodeFrame(QByteArray &ReceiveData)
{
    /*!
    * \brief  功能概述 解包函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
//    m_nLinkCount = 0;///<zzy 2015/1/14 修改:
    ShowMessage(SHOWMESSAGE_TYPE_RECEIVE,ReceiveData.toHex());
    if (ReceiveData.count() < 6)
        return;
    int nReadFrameDateLen = 0;
    nReadFrameDateLen = (BYTE)ReceiveData.at(1);
    if ( (nReadFrameDateLen<4) ||(nReadFrameDateLen > 253))
        return;
    if((nReadFrameDateLen+2) != ReceiveData.count())
        return ; //读取到的实际长度比帧内容不符合

    m_IEC104DeliverQuery.SetData(ReceiveData);
    if (!m_IEC104DeliverQuery.Is104Frame())
        return;
    switch (m_IEC104DeliverQuery.GetFrameType()) {
    case IEC104_I_TYPE:
        ///解析I帧
        DecodeIFrame(m_IEC104DeliverQuery);
        break;
    case IEC104_S_TYPE:
        ///解析S帧
        DecodeSFrame(m_IEC104DeliverQuery);
        break;
    case IEC104_U_TYPE:
        ///解析U帧
        DecodeUFrame(m_IEC104DeliverQuery);
        break;
    case IEC104_Start_TYPE:
        DecodeStartFrame(m_IEC104DeliverQuery);
        break;
    default:
        break;
    }
}

void CProtocolDeliverPRT_IEC_104::DecodeIFrame(CIEC104DeliverQuery &Query_)
{
    /*!
    * \brief  功能概述 解析I帧
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
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
//        ProcessASDU101Type_C_SC_NA_1(response.GetTransmissionCause(),response.GetPointAddress(),response.GetUInt(15,1));
//        ProcessASDU101Type_C_SC_NA_1(*pAsdu);
        qDebug()<<"C_SC_NA_1//45 单命令";
        return;
        break;
    case C_DC_NA_1: // 46 双点遥控命令
//        ProcessASDU101Type_C_DC_NA_1(response.GetTransmissionCause(),response.GetPointAddress(),response.GetUInt(15,1));
//        ProcessASDU101Type_C_DC_NA_1(*pAsdu);
        qDebug()<<"46 双点遥控命令";
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
        ProcessASDU101Type_C_IC_NA_1(*pAsdu);
        return;
        break;
    case C_CI_NA_1:		// 101 电能脉冲召唤命令
//        qDebug()<<"101 电能脉冲召唤命令";
        ProcessASDU101Type_C_CI_NA_1(*pAsdu);
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
        break;
    }
}
void CProtocolDeliverPRT_IEC_104::ProcessASDU101Type_C_CS_NA_1(ASDU101 &asdu)
{///68 14 0200 0000 67 01 0600 ff00 000000  370d2f0e68040f
    Q_ASSERT(asdu.m_nType == C_CS_NA_1);
    BYTE *pFrameStart=asdu.GetInfoStart();
    pFrameStart += 3;


    ASDU101_CP56Time2a *pTime=(ASDU101_CP56Time2a *)pFrameStart;
    SYSTEMTIME SYSTEMTIMEdatetime = pTime->GetTime();
    QDate date(SYSTEMTIMEdatetime.wYear,SYSTEMTIMEdatetime.wMonth,SYSTEMTIMEdatetime.wDay);
    QTime time(SYSTEMTIMEdatetime.wHour,SYSTEMTIMEdatetime.wMinute,SYSTEMTIMEdatetime.wSecond);
    QDateTime dateTime(date,time);
    QString str = QString("date -s \"%1\"").arg(dateTime.toString("yyyy/MM/dd HH:mm:ss"));
    qDebug()<<str.toLatin1().data();
    system(str.toLatin1().data());
    //////////处理校时
    Send_I_C_CS_NA_1(m_nSendIdx, m_nReciveIdx, m_nCommonAddress);
}
bool CProtocolDeliverPRT_IEC_104::Send_I_C_CS_NA_1(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    //功能：对时报文
    //参数：nAddress 子站地址 nFcb  fcb状态
    //返回值：true
    //建立日期: 2007-08-29
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
void CProtocolDeliverPRT_IEC_104::InCrease(WORD &nValue)
{
    nValue++;
    if (nValue>m_nMaxSRValue)
        nValue=0;
}
void CProtocolDeliverPRT_IEC_104::ResetDataLink()
{
    init104Param();
}
void CProtocolDeliverPRT_IEC_104::Send_S_Frame(WORD nReceiveNo)
{
    //功能：合成S帧
    //参数：nReceiveNo 接收序号
    //建立日期：2007-09-14

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
    SetData(szSend,pCur-szSend);
}

void CProtocolDeliverPRT_IEC_104::ProcessASDU101Type_C_IC_NA_1(ASDU101 &/*asdu*/)
{
    ////处理总召唤
    Send_IntrogenCallActivationOK(m_nSendIdx,m_nReciveIdx,m_nCommonAddress);

    Send_I_YX(m_nSendIdx, m_nReciveIdx, m_nCommonAddress);///单点遥信
//    Send_I_YC(m_nSendIdx, m_nReciveIdx, m_nCommonAddress);///带品质描述不带时标的遥测数据(测量值，规一化值)
    Send_I_YC_Float(m_nSendIdx, m_nReciveIdx, m_nCommonAddress);

    Send_IntrogenCallActivationEnd(m_nSendIdx,m_nReciveIdx, m_nCommonAddress);
}
bool CProtocolDeliverPRT_IEC_104::Send_IntrogenCallActivationOK(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    //功能：总召唤激活确认
    //参数：nSendNo 发送序号
    //      nReceiveNo  接收序号 nAddress 设备地址 nCommonAddress公共地址
    //返回值：true
    //建立日期: 2007-09-14
    //C_IC_NA_1 ACT
    //说明：格式为：

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    *pCur++ = C_IC_NA_1;//0x64;//100 类型标识
    *pCur++ = 1;   //结构限定词
    *pCur++ = 0x07;   //传输原因 0x0A=激活确认
    *pCur++ = 0; //传输原因高位

    *pCur++ = nCommonAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    *pCur++ = 0; //信息体地址低位
    *pCur++ = 0; //信息体地址高位
    *pCur++ = 0; //信息体地址高位

    *pCur++ = 0x14;//总召唤限定词(Qoi)
//	m_strFrameName = "总召唤";
//    m_strFrameName.Format("(总召唤:Send:%d  Receive:%d)",nSendNo,nReceiveNo);

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
}
bool CProtocolDeliverPRT_IEC_104::Send_I_YX(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
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
    qDebug()<<"遥信--Count"<<YX_PointMap.count();

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
//    qDebug()<<"===="<<maximumTag<<nPointNumber<<tagIterator.key()<<YX_PointMap.count();
    for (;;)
    {
        CTagI *tag = m_pRTDB->FindTag(tagIterator.value()->m_strRTDBName);
        qDebug()<<"遥信--"<<tagIterator.value()->m_strRTDBName;
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
//            qDebug()<<"YX_PointMap.end() End";
            *nASDUVarSQ |= ++number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            return true;
        }
        qDebug() <<"遥信tagIterator .key()="<<tagIterator.key()<<"nPointnumber="<<nPointNumber<<tagIterator.value()->m_strRTDBName;
        ++nPointNumber;
        if ( (++number>=maximumTag) || (nPointNumber!=tagIterator.key()))
        {
            qDebug()<<"----"<<number<<maximumTag<<nPointNumber<<tagIterator.key();
            *nASDUVarSQ |= number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            nPointNumber = tagIterator.key();
            pCur = pAddress;
            *pCur++ = nPointNumber & 0x000000ff; //信息体地址低位
            *pCur++ = (nPointNumber & 0x0000ff00)>>8; //信息体地址高位
            *pCur++ = (nPointNumber & 0x00ff0000)>>16; //信息体地址高位
            number = 0;
            qDebug()<<nPointNumber<<"if ( (++number>=maximumTag) || (nPointNumber!=tagIterator.key()))";
        }
    }
}
bool CProtocolDeliverPRT_IEC_104::Send_I_YC(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    // 标度化遥测值    带品质描述、不带时标
    ///功能：总召唤遥测帧返回
    ///参数：nSendNo 发送序号 nReceiveNo 设备地址 nCommonAddress公共地址
    /// ADSUType ASDU类型
    ///
    /// 返回值：true
    /// 建立日期: 2014-09-12
    qDebug()<<"组合遥测帧发送";
    if (YC_PointMap.isEmpty())
        return false;
    qDebug()<<"组合遥测帧发送2";

    QMap<int,CPointIEC_104 *>::iterator tagIterator;
    tagIterator = YC_PointMap.begin();

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    BYTE *nASDUVarSQ;//可变结构限定词
    BYTE *pAddress;

    int number = 0;///计数
    int maximumTag = 0;///最大信息体数
    *pCur++ = M_ME_NB_1;//类型标识

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
    ASDU101_SVA sva;///标度化值
    ASDU101_QDS qds;
    if (maximumTag == 0)
        maximumTag = 240/(sva.GetSize()+qds.GetSize());
    if (maximumTag > 127)
        maximumTag = 127;
    for (;;)
    {
        CTagI *tag = m_pRTDB->FindTag(tagIterator.value()->m_strRTDBName);
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
                qDebug()<<"AAAAAAAAAAA----------"<<sva.m_nValue<<tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
                qds.OV  = 0;
                qds.RES = 0;
                qds.BL  = 0;
                qds.SB  = 0;
                qds.NT  = 0;
                qds.IV  = 0;//有效
            }
        }
        memcpy(pCur,(BYTE *)&sva,sva.GetSize());
        pCur += sva.GetSize();
        memcpy(pCur,(BYTE *)&qds,qds.GetSize());
        pCur += qds.GetSize();

        if (++tagIterator == YC_PointMap.end())
        {
            *nASDUVarSQ |= ++number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            return true;
        }
        qDebug() <<"遥测tagIterator .key()="<<tagIterator.key()<<"nPointnumber="<<nPointNumber;
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

bool CProtocolDeliverPRT_IEC_104::Send_I_YC_Float(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    // 标度化遥测值    带品质描述、不带时标
    ///功能：总召唤遥测帧返回
    ///参数：nSendNo 发送序号 nReceiveNo 设备地址 nCommonAddress公共地址
    /// ADSUType ASDU类型
    ///
    /// 返回值：true
    /// 建立日期: 2014-09-12
    qDebug()<<"组合遥测帧发送";
    if (YC_PointMap.isEmpty())
        return false;
    qDebug()<<"组合遥测帧发送2";

    QMap<int,CPointIEC_104 *>::iterator tagIterator;
    tagIterator = YC_PointMap.begin();

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    BYTE *nASDUVarSQ;//可变结构限定词
    BYTE *pAddress;

    int number = 0;///计数
    int maximumTag = 0;///最大信息体数
    *pCur++ = M_ME_NC_1;//类型标识

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
//    ASDU101_SVA sva;///标度化值

    ASDU101_R32 r32;///短浮点数
    ASDU101_QDS qds;

    if (maximumTag == 0)
        maximumTag = 240/(r32.GetSize()+qds.GetSize());
    if (maximumTag > 127)
        maximumTag = 127;
    for (;;)
    {
        CTagI *tag = m_pRTDB->FindTag(tagIterator.value()->m_strRTDBName);
//        sva.m_nValue = 0;//每次将原值此初始化
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
//                sva.m_nValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
                r32.m_nValue = tag->GetPointCollection()->GetValue()->GetVarValue().toFloat();
//                qDebug()<<"AAAAAAAAAAA----------"<<sva.m_nValue<<tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
                qDebug()<<"AAAAAAAAAAA----------"<<r32.m_nValue<<tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
                qds.OV  = 0;
                qds.RES = 0;
                qds.BL  = 0;
                qds.SB  = 0;
                qds.NT  = 0;
                qds.IV  = 0;//有效
            }
        }
//        memcpy(pCur,(BYTE *)&sva,sva.GetSize());
//        pCur += sva.GetSize();
        memcpy(pCur,(BYTE *)&r32,r32.GetSize());
        pCur += r32.GetSize();
        memcpy(pCur,(BYTE *)&qds,qds.GetSize());
        pCur += qds.GetSize();

        if (++tagIterator == YC_PointMap.end())
        {
            *nASDUVarSQ |= ++number;
            Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
            MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
            return true;
        }
        qDebug() <<"遥测tagIterator .key()="<<tagIterator.key()<<"nPointnumber="<<nPointNumber;
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
bool CProtocolDeliverPRT_IEC_104::Send_IntrogenCallActivationEnd(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    //功能：总召唤激活结束
    //参数：nSendNo 发送序号
    //      nReceiveNo  接收序号 nAddress 设备地址 nCommonAddress公共地址
    //返回值：true
    //建立日期: 2007-09-14
    //C_IC_NA_1 ACT
    //说明：格式为：

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    *pCur++ = C_IC_NA_1;//0x64;//100 类型标识
    *pCur++ = 1;   //结构限定词
    *pCur++ = 0x0A;   //传输原因 0x0A=激活结束
    *pCur++ = 0; //传输原因高位

    *pCur++ = nCommonAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    *pCur++ = 0; //信息体地址低位
    *pCur++ = 0; //信息体地址高位
    *pCur++ = 0; //信息体地址高位

    *pCur++ = 0x14;//总召唤限定词(Qoi)

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
}
void CProtocolDeliverPRT_IEC_104::ProcessASDU101Type_C_CI_NA_1(ASDU101 &/*asdu*/)
{
    ////处理电度量召唤
    Send_IntrogenCallDDLActivationOK(m_nSendIdx, m_nReciveIdx, m_nCommonAddress);
    //////////////////////////////电度量///////////////////////////

    Send_I_YM(m_nSendIdx, m_nReciveIdx, m_nCommonAddress);

    Send_IntrogenCallDDLActivationEnd(m_nSendIdx, m_nReciveIdx, m_nCommonAddress);
}
bool CProtocolDeliverPRT_IEC_104::Send_IntrogenCallDDLActivationOK(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{

    //功能：电度总召唤激活确认
    //参数：nSendNo 发送序号
    //      nReceiveNo  接收序号 nAddress 设备地址 nCommonAddress公共地址
    //返回值：true
    //建立日期: 2007-09-14
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
bool CProtocolDeliverPRT_IEC_104::Send_I_YM(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
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
        ++nPointNumber;
        if ((++number>=maximumTag) || (nPointNumber!=tagIterator.key()))
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
bool CProtocolDeliverPRT_IEC_104::Send_IntrogenCallDDLActivationEnd(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    //功能：电度总召唤激活结束
    //参数：nSendNo 发送序号
    //      nReceiveNo  接收序号 nAddress 设备地址 nCommonAddress公共地址
    //返回值：true
    //建立日期: 2007-09-14
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

bool CProtocolDeliverPRT_IEC_104::MakeQuery_I(WORD &nSendNo, WORD nReceiveNo, BYTE *pAsdu, int nAsduSize)
{
    //功能：合成可变帧报文
    //参数：nSendNo 发送序号
    //      nReceiveNo  接收序号 pAsdu asdu指针   nAsduSize 大小
    //返回值：true
    //建立日期: 2007-09-14
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
    SetData(szSend, pCur-szSend);
    return true;
}


void CProtocolDeliverPRT_IEC_104::DecodeSFrame(CIEC104DeliverQuery &Query_)
{
    /*!
    * \brief  功能概述 解析S帧
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    qDebug()<<"解 S 帧";
    //功能:处理 S帧
    //参数:无
    //返回值:无
    //建立日期:2007-09-14
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

void CProtocolDeliverPRT_IEC_104::DecodeUFrame(CIEC104DeliverQuery &Query_)
{
    /*!
    * \brief  功能概述 解析U帧
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
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
bool CProtocolDeliverPRT_IEC_104::Send_U_Confirm(bool bStart_, bool bStop_, bool bTest_)
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
    return SetData(szSend,pCur-szSend);
}
#if 0
bool CProtocolDeliverPRT_IEC_104::SetData(const BYTE *pSource_, int nLength_)
{
    m_ByteArrayFrame = QByteArray((const char*)pSource_,nLength_);
    if (m_TcpSocket == NULL)
    {
//        QTimer::singleShot(1000,this,SLOT(OnOpenSocket()));
        return false;
    }
    if (m_TcpSocket->isOpen())
    {
        ShowMessage(0,m_ByteArrayFrame.toHex());
        m_TcpSocket->write(m_ByteArrayFrame);
        return true;
    }
//    QTimer::singleShot(1000,this,SLOT(OnOpenSocket()));
    return false;
}
#endif
bool CProtocolDeliverPRT_IEC_104::SetData(const BYTE *pSource_, int nLength_)
{
    m_ByteArrayFrame = QByteArray((const char*)pSource_,nLength_);
    m_Lock.lock();
    m_SendFrameList.append(m_ByteArrayFrame);
    m_Lock.unlock();
    return true;
}
void CProtocolDeliverPRT_IEC_104::init104Param()
{
    m_nSendIdx = 0;
    m_nReciveIdx = 0;
    InitPointMapValue();
}

void CProtocolDeliverPRT_IEC_104::InitPointMapValue()
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

void CProtocolDeliverPRT_IEC_104::DecodeStartFrame(CIEC104DeliverQuery &/*Query_*/)
{
    /*!
    * \brief  功能概述 解析协议标识帧
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */

}
