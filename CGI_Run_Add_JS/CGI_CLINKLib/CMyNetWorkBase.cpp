#include "CMyNetWorkBase.h"

CMyNetWorkBase::CMyNetWorkBase(QObject *parent) :
    CMyNetWorkI(parent)
{
    m_nLinkType = MyNetWork_LinkType_UnknownSocketType;
    m_pTcpServer = NULL;
    m_pTcpClient = NULL;
}

/*!
 \brief 初始化网络链路

 \fn CMyNetWorkBase::SetTcpServerIP_Port
 \param TcpServer_HostAddress_ 服务器IP地址
 \param nTcpServer_Port_ 服务器端口号
 \param nType TCP链路的类型 分为服务端和客户端
 \return bool 打开成功返回true，否则返回false
*/
bool CMyNetWorkBase::SetTcpServerIP_Port(const QHostAddress &TcpServer_HostAddress_, const quint16 nTcpServer_Port_, CMyNetWorkI::MyNetWork_LinkType nType)
{
    bool result = false;
    m_nLinkType = nType;
    switch (m_nLinkType) {
    case MyNetWork_LinkType_UnknownSocketType:
        break;
    case MyNetWork_LinkType_TcpServer:
    {
        if (NULL == m_pTcpServer)
        {
            m_pTcpServer = new CTcpServer(this);
            connect(m_pTcpServer,SIGNAL(Signal_ConnectState(bool)),this,SIGNAL(Signal_ConnectState(bool)));
            connect(m_pTcpServer,SIGNAL(Signal_TcpServer_TcpSocket_Response(QByteArray)),this,SIGNAL(Signal_Response(QByteArray)));
        }else
        {

        }
        result = m_pTcpServer->SetTcpServerIP_Port(TcpServer_HostAddress_,nTcpServer_Port_);
    }
        break;
    case MyNetWork_LinkType_TcpClient:
    {
        if (NULL == m_pTcpClient)
        {
            m_pTcpClient = new CTcpClient(this);
            connect(m_pTcpClient,SIGNAL(Signal_ConnectState(bool)),this,SIGNAL(Signal_ConnectState(bool)));
            connect(m_pTcpClient,SIGNAL(Signal_TcpSocket_Response(QByteArray)),this,SIGNAL(Signal_Response(QByteArray)));

        }else
        {

        }
        result = m_pTcpClient->SetTcpServerIP_Port(TcpServer_HostAddress_,nTcpServer_Port_);
    }
        break;
    default:
        break;
    }
    return result;
}
/*!
 \brief 修改作为TCP客户端连接的服务端的IP地址和端口号

 \fn CMyNetWorkBase::ChangeTCPServerIP_Port
 \param TcpServer_HostAddress_ 服务端的IP地址
 \param nTcpServer_Port_ 服务端的端口号
 \return bool
*/
bool CMyNetWorkBase::ChangeTCPServerIP_Port(const QHostAddress &TcpServer_HostAddress_, const quint16 nTcpServer_Port_)
{
    bool result = false;
    switch (m_nLinkType) {
    case MyNetWork_LinkType_UnknownSocketType:
        break;
    case MyNetWork_LinkType_TcpServer:
        break;
    case MyNetWork_LinkType_TcpClient:
    {
        result = m_pTcpClient->ChangeTCPServerIP_Port(TcpServer_HostAddress_,nTcpServer_Port_);
    }
        break;
    default:
        break;
    }
    return result;
}

bool CMyNetWorkBase::GetLinkState() const
{
    bool result = false;
    switch (m_nLinkType) {
    case MyNetWork_LinkType_UnknownSocketType:
        break;
    case MyNetWork_LinkType_TcpServer:
        result = m_pTcpServer->GetLinkState();
        break;
    case MyNetWork_LinkType_TcpClient:
        result = m_pTcpClient->GetLinkState();
        break;
    default:
        break;
    }
    return result;
}

void CMyNetWorkBase::SetCloseTcpSocket()
{
    switch (m_nLinkType) {
    case MyNetWork_LinkType_UnknownSocketType:
        break;
    case MyNetWork_LinkType_TcpServer:
        m_pTcpServer->SetTcpServer_CloseTcpSocket();
        break;
    case MyNetWork_LinkType_TcpClient:
        m_pTcpClient->SetCloseTcpSocket();
        break;
    default:
        break;
    }
}

qint64 CMyNetWorkBase::write(const char *data_, qint64 maxSize_)
{
    qint64 result = 0;
    switch (m_nLinkType) {
    case MyNetWork_LinkType_UnknownSocketType:
        break;
    case MyNetWork_LinkType_TcpServer:
        result = m_pTcpServer->write(data_,maxSize_);
        break;
    case MyNetWork_LinkType_TcpClient:
        result = m_pTcpClient->write(data_,maxSize_);
        break;
    default:
        break;
    }
    return result;
}

qint64 CMyNetWorkBase::write(const char *data_)
{
    qint64 result = 0;
    switch (m_nLinkType) {
    case MyNetWork_LinkType_UnknownSocketType:
        break;
    case MyNetWork_LinkType_TcpServer:
        result = m_pTcpServer->write(data_);
        break;
    case MyNetWork_LinkType_TcpClient:
        result = m_pTcpClient->write(data_);
        break;
    default:
        break;
    }
    return result;
}

qint64 CMyNetWorkBase::write(const QByteArray &byteArray_)
{
    qint64 result = 0;
    switch (m_nLinkType) {
    case MyNetWork_LinkType_UnknownSocketType:
        break;
    case MyNetWork_LinkType_TcpServer:
        result = m_pTcpServer->write(byteArray_);
        break;
    case MyNetWork_LinkType_TcpClient:
        result = m_pTcpClient->write(byteArray_);
        break;
    default:
        break;
    }
    return result;
}
