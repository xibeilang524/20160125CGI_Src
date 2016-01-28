#include "CWriteFrame.h"
#include "CProtocolDeliverPRT_IEC_104.h"


#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QTimer>

CWriteFrame::CWriteFrame(QString strIPAddress_, QObject *parent) :
    QObject(parent),
    m_strTcpIp(strIPAddress_)
{
    m_tcpServer = NULL;
//    pTcpSocket->m_pTcpSocket = NULL;
}

void CWriteFrame::WriteMessage(int type, QString message, int nChannelNumber)
{
//    qDebug()<<"CWriteFrame class :"<<type<<message<<nChannelNumber;
    for (int i = 0; i < m_CTcpSocketList.count(); ++i)
    {
        if (m_CTcpSocketList.at(i)->GetChannelNumber() == nChannelNumber)
        {
            m_CTcpSocketList.at(i)->WriteMessage(type,message);
        }
    }
}
void CWriteFrame::TCPConnect()
{
    if (m_tcpServer == NULL)
    {
        m_tcpServer = new QTcpServer(this);
        connect(m_tcpServer,SIGNAL(newConnection()),
                this,SLOT(OnserverConnected()));
    }
    if (m_tcpServer->listen(QHostAddress(m_strTcpIp),FramePort))
    {
        qDebug()<<"open succeed -------"<<m_strTcpIp<<FramePort;
        return;
    }else
    {
        qDebug()<<"open faild-------"<<m_strTcpIp<<FramePort;
        QTimer::singleShot(5000,this,SLOT(TCPConnect()));
    }
}
void CWriteFrame::OnserverConnected()
{
    CTCPSocket *pTcpSocket = new CTCPSocket;
    connect(pTcpSocket,SIGNAL(signalTcpSocketDisConnected(CTCPSocket*)),this,SLOT(slotDeleteCTcpSocket(CTCPSocket*)));
    m_CTcpSocketList.append(pTcpSocket);
    pTcpSocket->SetTcpSocket(m_tcpServer->nextPendingConnection());
}

void CWriteFrame::slotDeleteCTcpSocket(CTCPSocket *pTcpSocket)
{
    qDebug()<<"CWriteFrame::断开链接"<<pTcpSocket->GetChannelNumber();
    for (int i = 0; i < m_CTcpSocketList.count(); ++i)
    {
        if (m_CTcpSocketList.at(i) == pTcpSocket)
        {
            m_CTcpSocketList.removeAt(i);
            pTcpSocket->SetChannelNumber(-1);
            qDebug()<<"CWriteFrame::断开链接 delete 掉了";
            delete pTcpSocket;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
CTCPSocket::CTCPSocket(QObject *parent):
    QObject(parent),
    m_nChannelNumber(-1),
    m_pTcpSocket(NULL)
{

}

void CTCPSocket::SetTcpSocket(QTcpSocket *pTcpSocket_)
{
    m_pTcpSocket = pTcpSocket_;
    connect(m_pTcpSocket,SIGNAL(disconnected()), this,SLOT(SocketDeleteLater()));
    connect(m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(ProcessRespond()));
     //readyRead()表示服务端发送数据过来即发动信号，接着revData()进行处理。
    connect(m_pTcpSocket,SIGNAL(connected()),this,SLOT(OnConnected()));
    //连接成功时，返回相应信息
    connect(m_pTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));
    //当出现错误时返回错误信息
}

int CTCPSocket::GetChannelNumber() const
{
    return m_nChannelNumber;
}
void CTCPSocket::WriteMessage(int type, QString message)
{
//    qDebug()<<"CTCPSocket::"<<type<<message;
    QByteArray aMessage = message.toLatin1();
    aMessage = aMessage.fromHex(aMessage);
    BYTE head[4];
    BYTE *pCur = head;
    *pCur++ = 0xFF;
    int nLength = aMessage.count()+4;
    WORD nTemp = nLength;
    *pCur++ = HIBYTE(nTemp);
    *pCur++ = LOBYTE(nTemp);
    *pCur++ = type;
    QByteArray FrameHead((const char*)head,4);
    FrameHead = FrameHead.append(aMessage);

    if (m_pTcpSocket == NULL)
        return;
    if (!m_pTcpSocket->isOpen())
        return;
    m_pTcpSocket->write(FrameHead);
}

void CTCPSocket::SetChannelNumber(const int nChannelNumber)
{
    m_nChannelNumber = nChannelNumber;
}
void CTCPSocket::SocketDeleteLater()
{
    qDebug()<<"CTCPSocket::m_TcpSocekt 前"<<m_pTcpSocket;
    m_pTcpSocket->deleteLater();
    qDebug()<<"CTCPSocket::m_TcpSocekt 后"<<m_pTcpSocket;
    m_pTcpSocket = NULL;
    emit signalTcpSocketDisConnected(this);
}
void CTCPSocket::OnConnected()
{
    /*!
    * \brief  功能概述 链接成功
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */

    qDebug() << "CTCPSocket::connect success"<<"成功";
}

void CTCPSocket::displayError(QAbstractSocket::SocketError)
{
    /*!
    * \brief  功能概述 未连接服务器,重新连接
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    qDebug()<<"CTCPSocket::"<<m_pTcpSocket->errorString()<<"断开链接";
}
void CTCPSocket::ProcessRespond()
{
    /*!
    * \brief  功能概述 解包函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    QByteArray datas = m_pTcpSocket->readAll();
    if (datas.isEmpty())
        return;
    qDebug()<<"CTCPSocket::Frame"<<datas.toHex();
    if (datas.count() < 2)
        return;
    if ((BYTE)datas.at(0) == 0xFF)
    {

        m_nChannelNumber = (int)datas.at(1);
        qDebug()<<"CTCPSocket::m_nchannelNumber="<<m_nChannelNumber;
//        while(!datas.isEmpty())
//        {
//            int nReceiveDataCount = datas.count();
//            if (nReceiveDataCount < 6)
//                return;
//            int nReadFrameDateLen = (BYTE)datas.at(1);
//            if (nReadFrameDateLen+2 > nReceiveDataCount)
//                return;
//            QByteArray array = datas.left(nReadFrameDateLen+2);
//            datas.remove(0,nReadFrameDateLen+2);
//            DecodeFrame(array);
//        }
    }
}
