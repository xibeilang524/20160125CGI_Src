/*!
 * \file    CLink.cpp
 * \brief   概述 CLink类的源文件
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

#include "CLink.h"
#include <QDebug>
#include <QUdpSocket>
#include <QTime>
#include "../PortInclude/cprotocoli.h"
#include "CMyNetWorkBase.h"

/*!
 \brief 库文件导出函数

 \fn CreateCMyNetWorkI
 \param parent
 \return CMyNetWorkI
*/
CGI_CLINKI * CreateCGI_CLINKI(QObject *parent)
{
    CLink *pCLink = new CLink(parent);
    return  pCLink;
}
/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 parent父对象的指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
CLink::CLink(QObject *parent) :
    CGI_CLINKI(parent)
{
    m_pLinkIODevice = NULL;
    m_pMyNetWork = NULL;
#if defined(Q_OS_LINUX)
    m_pCanThread = NULL;
#endif
}

bool CLink::CGI_SetInitLink(int nChannelType_, const QDomElement &MainPort_)
{
    m_EleMainPort = MainPort_;
    m_bIsConnect = false;
    switch (nChannelType_) {
    case ChannelType_COM:
    {
        m_nLinkType = LinkType_COM;
        m_bIsConnect = SetInitCOM(MainPort_);
        return m_bIsConnect;
    }
        break;
    case ChannelType_NET:
    {
        m_nLinkType = LinkType_TCP;
        m_bIsConnect = false;
        m_pMyNetWork = new CMyNetWorkBase(this);
        if (m_pMyNetWork != NULL)
        {
            connect(m_pMyNetWork,SIGNAL(Signal_ConnectState(bool)),this,SIGNAL(signalConnect(bool)));
            connect(m_pMyNetWork,SIGNAL(Signal_Response(QByteArray)),this,SIGNAL(ReadyRead(QByteArray)));
            if (MainPort_.tagName() == "Link")
            {
                if (MainPort_.attribute("ComMode") == "TCPClient")
                {
                    m_HostAddressTCPClient = QHostAddress(MainPort_.attribute("TCPClient_IP"));
                    m_nPortTCPClient = MainPort_.attribute("TCPClient_Port").toInt();
                    m_bIsConnect = m_pMyNetWork->SetTcpServerIP_Port(m_HostAddressTCPClient,m_nPortTCPClient,CMyNetWorkI::MyNetWork_LinkType_TcpClient);
                }else if (MainPort_.attribute("ComMode") == "TCPServer")
                {
                    m_HostAddressTCPServer = QHostAddress(MainPort_.attribute("TCPServer_IP"));
                    m_nPortTCPServer = MainPort_.attribute("TCPServer_Port").toInt();
                    m_bIsConnect = m_pMyNetWork->SetTcpServerIP_Port(m_HostAddressTCPServer,m_nPortTCPServer,CMyNetWorkI::MyNetWork_LinkType_TcpServer);
                }else if (MainPort_.attribute("ComMode") == "UDP")
                {
                    m_nLinkType = LinkType_UDP;

                }
                qDebug()<<MainPort_.attribute("ComMode");
            }else
            {
                qDebug()<<"if (MainPort_.tagName() != \"Link\")"<<MainPort_.tagName();
            }
        }
        return m_bIsConnect;
    }
        break;
    case ChannelType_CAN:
    {
        m_nLinkType = LinkType_Can;
#if defined(Q_OS_LINUX)
        m_bIsConnect = SetInitCAN(MainPort_);
#endif
        return m_bIsConnect;
    }
        break;
    default:
        break;
    }
    return m_bIsConnect;
}

int CLink::GetLinkType() const
{
    return m_nLinkType;
}

bool CLink::SetInitCOM(const QDomElement &MainPort_)
{
    /*!
    * \brief  功能概述 串口操作  初始化串口参数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/19
    */
    QString strPortName = MainPort_.attribute("Name");
    QString strBaud = MainPort_.attribute("BaudRate");
    QString strDataBit = MainPort_.attribute("DataBit");
    QString strStopBit = MainPort_.attribute("StopBit");
    QString strParity = MainPort_.attribute("Parity");
    if (NULL == m_pLinkIODevice)
    {
        m_pLinkIODevice = new CSerialPort;
    }
    CSerialPort *pSerialPort = (CSerialPort *)m_pLinkIODevice;
    connect(m_pLinkIODevice,SIGNAL(readyRead()),this,SLOT(ProcessRespond()));
    bool result = pSerialPort->OpenSerialPort(strPortName,strBaud,strDataBit,strStopBit,strParity);
    emit signalConnect(result);
    return result;
}


bool CLink::SetInitCAN(const QDomElement &MainPort_)
{
#if defined(Q_OS_LINUX)
    if (NULL == m_pCanThread)
    {
        m_pCanThread = new CCANThread;
        connect(m_pCanThread,SIGNAL(readyRead(QByteArray)),this,SIGNAL(ReadyRead(QByteArray)));
        return m_pCanThread->OpenCANChannel(MainPort_.attribute("Name"));
    }else
    {
        return m_pCanThread->GetLinkState();
    }
#endif
}


bool CLink::SetInitUDP(const QDomElement &MainPort_)
{
    if (NULL == m_pLinkIODevice)
    {
        m_pLinkIODevice = new QUdpSocket(this);
    }
    QUdpSocket *pLinkUdpSocket = (QUdpSocket*)m_pLinkIODevice;
    bool result = false;
    m_Receive = QHostAddress(MainPort_.attribute("IP"));
    m_ReceivePort = MainPort_.attribute("ReceivePort").toInt();
    m_Sender = QHostAddress(MainPort_.attribute("IP"));
    m_SenderPort = MainPort_.attribute("SendPort").toInt();
    qDebug()<<"m_ReceivePort="<<m_ReceivePort;

#ifdef Q_OS_WIN32
    result = pLinkUdpSocket->bind(MainPort_.attribute("ReceivePort").toInt(),QUdpSocket::ReuseAddressHint);
#else
    result = pLinkUdpSocket->bind(MainPort_.attribute("ReceivePort").toInt(),QUdpSocket::ShareAddress);
#endif

    connect(m_pLinkIODevice, SIGNAL(readyRead()),
            this, SLOT(ProcessRespond()));
    return result;
}

bool CLink::CGI_SetInitUDP(const QDomElement &/*MainPort_*/)
{
    return false;
}

bool CLink::SetInitShareMemory(const QDomElement &/*MainPort_*/)
{
    return false;
}

bool CLink::SetInitCustom(const QDomElement &/*MainPort_*/)
{
    return false;
}


int CLink::GetLinkState() const
{
    bool result = false;
    switch (m_nLinkType) {
    case LinkType_UDP:
    {
        if (NULL != m_pMyNetWork)
        {
            result = m_pMyNetWork->GetLinkState();
        }
    }
        break;
    case LinkType_COM:
    {
        result = m_bIsConnect;
    }
    case LinkType_TCP:
    {
        if (m_pMyNetWork != NULL)
        {
            result = m_pMyNetWork->GetLinkState();
        }
    }
        break;
    case LinkType_Can:
#if defined(Q_OS_LINUX)
        if (m_pCanThread != NULL)
        {
            result = m_pCanThread->GetLinkState();
        }
#endif
        break;
    default:
        break;
    }
    return result;
}

qint64 CLink::write(const QByteArray &data)
{
    switch (m_nLinkType) {
    case LinkType_UDP:
    {
        QUdpSocket *pLinkUdpSocket = (QUdpSocket*)m_pLinkIODevice;
//        pLinkUdpSocket->writeDatagram(data,QHostAddress::Broadcast,Port);
        return pLinkUdpSocket->writeDatagram(data,m_Sender,m_SenderPort);
    }
        break;
    case LinkType_COM:
    {
        if (NULL == m_pLinkIODevice)
        {
            return 0;
        }
        if (!m_bIsConnect)
            return 0;
        if (!m_pLinkIODevice->isOpen())
        {
            return 0;
        }
    }
        return m_pLinkIODevice->write(data);
    case LinkType_TCP:
    {
        if (m_pMyNetWork != NULL)
        {
            return m_pMyNetWork->write(data);
        }else
        {
            return 0;
        }
    }
        break;
    case LinkType_Can:
#if defined(Q_OS_LINUX)
        if (m_pCanThread != NULL)
        {
            return m_pCanThread->CANWrite(data.data(),data.count());
        }else
        {
            return 0;
        }
#endif
        return 0;
        break;
    default:
        return 0;
        break;
    }
}
qint64 CLink::write(const char *data, qint64 len)
{
    switch (m_nLinkType) {
    case LinkType_UDP:
    {
        QUdpSocket *pLinkUdpSocket = (QUdpSocket*)m_pLinkIODevice;
//        pLinkUdpSocket->writeDatagram(data,QHostAddress::Broadcast,Port);
//        qint64 writeDatagram(const char *data, qint64 len, const QHostAddress &host, quint16 port);
        return pLinkUdpSocket->writeDatagram(data,len,m_Sender,m_SenderPort);
    }
        break;
    case LinkType_COM:
    {
        if (NULL == m_pLinkIODevice)
        {
            return 0;
        }
        if (!m_bIsConnect)
            return 0;
        if (!m_pLinkIODevice->isOpen())
        {
            return 0;
        }
    }
        return m_pLinkIODevice->write(data,len);
    case LinkType_TCP:
    {
        if (m_pMyNetWork != NULL)
        {
            return m_pMyNetWork->write(data,len);
        }else
        {
            return 0;
        }
    }
        break;
    case LinkType_Can:
#if defined(Q_OS_LINUX)
        if (m_pCanThread != NULL)
        {
            return m_pCanThread->CANWrite(data,len);
        }else
        {
            return 0;
        }
#endif
        return 0;
        break;
    default:
        return 0;
        break;
    }
}
/*!
 \brief 修改作为TCP客户端连接的服务端的IP地址和端口号

 \fn CLink::ChangeTCPServerIP_Port
 \param TcpServer_HostAddress_ 服务端的IP地址
 \param nTcpServer_Port_ 服务端的端口号
 \return bool
*/
bool CLink::ChangeTCPServerIP_Port(const QHostAddress &TcpServer_HostAddress_, const quint16 nTcpServer_Port_)
{
    bool result = false;
    switch (m_nLinkType) {
    case LinkType_UDP:
    case LinkType_COM:
        break;
    case LinkType_TCP:
    {
        if (m_pMyNetWork != NULL)
        {
           result = m_pMyNetWork->ChangeTCPServerIP_Port(TcpServer_HostAddress_,nTcpServer_Port_);
        }
    }
        break;
    default:
        break;
    }
    return result;
}

void CLink::SetCloseLink()
{
    switch (m_nLinkType) {
    case LinkType_UDP:
    case LinkType_COM:
        break;
    case LinkType_TCP:
    {
        if (m_pMyNetWork != NULL)
        {
            return m_pMyNetWork->SetCloseTcpSocket();
        }else
        {
            return;
        }
    }
        break;
    case LinkType_Can:
        break;
    default:
        break;
    }
    return;
}
void CLink::ProcessRespond()
{
    switch (m_nLinkType) {
    case LinkType_UDP:
    {
        QUdpSocket *pLinkUdpSocket = (QUdpSocket*)m_pLinkIODevice;
        while (pLinkUdpSocket->hasPendingDatagrams())
        {
            QByteArray datagram;
            datagram.resize(pLinkUdpSocket->pendingDatagramSize());

            pLinkUdpSocket->readDatagram(datagram.data(), datagram.size(),
                                    &m_Sender, &m_SenderPort);
            qDebug()<<m_Sender<<" "<<m_SenderPort;
            if (!datagram.isEmpty())
                emit ReadyRead(datagram);
        }
    }
        break;
    case LinkType_COM:
    {
        QByteArray Respond = m_pLinkIODevice->readAll();
        if (!Respond.isEmpty())
            emit ReadyRead(Respond);
    }
        break;
    default:
        break;
    }
}

