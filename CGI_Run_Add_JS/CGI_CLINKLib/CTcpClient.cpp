#include "CTcpClient.h"

CTcpClient::CTcpClient(QObject *parent) :
    QObject(parent)
{
    m_pTcpSocket = NULL;
    m_bTcpSocket_IsConnect = false;
    m_nTimerID = 0;
    m_nTcpClientLastReceiveTime = 0;
    m_nTimerID = startTimer(1000);
}

int CTcpClient::GetSysAbsTime()
{
    ///获取绝对秒
    int nTime = QDateTime::currentDateTime().secsTo(QDateTime(QDate(1970,1,1),QTime(0,0,0,0)));
    if(nTime < 0)
    {
        nTime = -nTime;
    }
    return nTime;
}
void CTcpClient::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nTimerID)
    {
        qDebug()<<__func__<<m_bTcpSocket_IsConnect;
        if (m_bTcpSocket_IsConnect)
        {
            if (Absolute(GetSysAbsTime() - m_nTcpClientLastReceiveTime) > 30)
            {///< 30秒没有收到数据则断开连接
                SetCloseTcpSocket();
            }
        }else
        {

        }

    }
}

bool CTcpClient::GetLinkState() const
{
    return m_bTcpSocket_IsConnect;
}

bool CTcpClient::SetTcpServerIP_Port(const QHostAddress &TcpServer_HostAddress_, const quint16 nTcpServer_Port_)
{
    m_TcpServer_HostAddress = TcpServer_HostAddress_;
    m_nTcpServer_Port = nTcpServer_Port_;
    if (m_bTcpSocket_IsConnect)
    {

    }else
    {
        m_pTcpSocket = new QTcpSocket(this);
        m_pTcpSocket->connectToHost(m_TcpServer_HostAddress,m_nTcpServer_Port);
        m_bTcpSocket_IsConnect = m_pTcpSocket->waitForConnected(5*1000);
        if (m_bTcpSocket_IsConnect)
        {
            m_nTcpClientLastReceiveTime = GetSysAbsTime();
            connect(m_pTcpSocket,SIGNAL(connected()),this,SLOT(Slot_TcpSocket_connected()));
            connect(m_pTcpSocket,SIGNAL(disconnected()),m_pTcpSocket,SLOT(deleteLater()));
            connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(Slot_TcpSocket_disconnected()));
            connect(m_pTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(Slot_TcpSocket_error(QAbstractSocket::SocketError)));
            connect(m_pTcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(Slot_TcpSocket_stateChanged(QAbstractSocket::SocketState)));
            connect(m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(Slot_TcpSocket_readyRead()));
            emit Signal_ConnectState(m_bTcpSocket_IsConnect);
        }else
        {
            m_pTcpSocket->deleteLater();
            QTimer::singleShot(10000,this,SLOT(Slot_connectToHost()));
        }
        qDebug()<<__func__<<"  m_bTcpSocket_IsConnect ="<<m_bTcpSocket_IsConnect;
    }
    return m_bTcpSocket_IsConnect;
}
/*!
 \brief 修改作为TCP客户端连接的服务端的IP地址和端口号

 \fn CTcpClient::ChangeTCPServerIP_Port
 \param TcpServer_HostAddress_ 服务端的IP地址
 \param nTcpServer_Port_ 服务端的端口号
 \return bool
*/
bool CTcpClient::ChangeTCPServerIP_Port(const QHostAddress &TcpServer_HostAddress_, const quint16 nTcpServer_Port_)
{
    m_TcpServer_HostAddress = TcpServer_HostAddress_;
    m_nTcpServer_Port = nTcpServer_Port_;
    SetCloseTcpSocket();
    QTimer::singleShot(1000,this,SLOT(Slot_connectToHost()));
    return true;
}

void CTcpClient::SetCloseTcpSocket()
{
    qDebug()<<__func__<<"断开连接 m_bIsConnect ="<<m_bTcpSocket_IsConnect;
    if (m_bTcpSocket_IsConnect)
    {
        m_pTcpSocket->disconnectFromHost();
        qDebug()<<__func__<<"m_pTcpSocket->disconnectFromHost(); hou";
        if (m_pTcpSocket->state() == QAbstractSocket::UnconnectedState || (m_pTcpSocket->waitForDisconnected()) )
        {
            qDebug()<<__func__<<"m_pTcpSocket->waitForDisconnected(); successfully";
        }else
        {
            qDebug()<<__func__<<"m_pTcpSocket->waitForDisconnected(); failure";
        }
    }
}

qint64 CTcpClient::write(const char *data_, qint64 maxSize_)
{
    if (m_bTcpSocket_IsConnect)
    {
        qDebug()<<__func__<<"m_bTcpSocket_IsConnect ="<<m_bTcpSocket_IsConnect;
        int nWrite = m_pTcpSocket->write(data_,maxSize_);
        bool bwaitForBytesWritten = m_pTcpSocket->waitForBytesWritten(5*1000);
        //    m_pTcpSocket->isOpen();
        if (nWrite)
        {
            return nWrite;
        }else
        {

        }
        if (bwaitForBytesWritten)
        {
            qDebug()<<"m_pTcpSocket->waitForBytesWritten() true  nWrite ="<<nWrite;
            return bwaitForBytesWritten;
        }else
        {
            qDebug()<<"m_pTcpSocket->waitForBytesWritten() false nWrite ="<<nWrite;
            return bwaitForBytesWritten;
        }
    }else
    {
        return 0;
    }
}

qint64 CTcpClient::write(const char *data_)
{
    if (m_bTcpSocket_IsConnect)
    {
        qDebug()<<__func__<<"m_bTcpSocket_IsConnect ="<<m_bTcpSocket_IsConnect;
        int nWrite = m_pTcpSocket->write(data_);
        bool bwaitForBytesWritten = m_pTcpSocket->waitForBytesWritten(5*1000);
        //    m_pTcpSocket->isOpen();
        if (nWrite)
        {
            return nWrite;
        }else
        {

        }
        if (bwaitForBytesWritten)
        {
            qDebug()<<"m_pTcpSocket->waitForBytesWritten() true  nWrite ="<<nWrite;
            return bwaitForBytesWritten;
        }else
        {
            qDebug()<<"m_pTcpSocket->waitForBytesWritten() false nWrite ="<<nWrite;
            return bwaitForBytesWritten;
        }
    }else
    {
        return 0;
    }
}

qint64 CTcpClient::write(const QByteArray &byteArray_)
{
    if (m_bTcpSocket_IsConnect)
    {
        qDebug()<<__func__<<"m_bTcpSocket_IsConnect ="<<m_bTcpSocket_IsConnect;
        int nWrite = m_pTcpSocket->write(byteArray_);
        bool bwaitForBytesWritten = m_pTcpSocket->waitForBytesWritten(5*1000);
        //    m_pTcpSocket->isOpen();
        if (nWrite)
        {
            return nWrite;
        }else
        {

        }
        if (bwaitForBytesWritten)
        {
            qDebug()<<"m_pTcpSocket->waitForBytesWritten() true  nWrite ="<<nWrite;
            return bwaitForBytesWritten;
        }else
        {
            qDebug()<<"m_pTcpSocket->waitForBytesWritten() false nWrite ="<<nWrite;
            return bwaitForBytesWritten;
        }
    }else
    {
        return 0;
    }
}

void CTcpClient::Slot_connectToHost()
{
    if (m_bTcpSocket_IsConnect)
    {

    }else
    {
        m_pTcpSocket = new QTcpSocket(this);
        m_pTcpSocket->connectToHost(m_TcpServer_HostAddress,m_nTcpServer_Port);
        m_bTcpSocket_IsConnect = m_pTcpSocket->waitForConnected(5*1000);
        if (m_bTcpSocket_IsConnect)
        {
            m_nTcpClientLastReceiveTime = GetSysAbsTime();
            connect(m_pTcpSocket,SIGNAL(connected()),this,SLOT(Slot_TcpSocket_connected()));
            connect(m_pTcpSocket,SIGNAL(disconnected()),m_pTcpSocket,SLOT(deleteLater()));
            connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(Slot_TcpSocket_disconnected()));
            connect(m_pTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(Slot_TcpSocket_error(QAbstractSocket::SocketError)));
            connect(m_pTcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(Slot_TcpSocket_stateChanged(QAbstractSocket::SocketState)));
            connect(m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(Slot_TcpSocket_readyRead()));
            emit Signal_ConnectState(m_bTcpSocket_IsConnect);
        }else
        {
            m_pTcpSocket->deleteLater();
            QTimer::singleShot(10000,this,SLOT(Slot_connectToHost()));
        }
        qDebug()<<__func__<<"  m_bTcpSocket_IsConnect ="<<m_bTcpSocket_IsConnect;
    }
}

void CTcpClient::Slot_TcpSocket_connected()
{
    qDebug()<<__func__<<"Slot_Connected() chenggong m_bIsConnect ="<<m_bTcpSocket_IsConnect;
    m_bTcpSocket_IsConnect = true;
}

void CTcpClient::Slot_TcpSocket_disconnected()
{
    qDebug()<<__func__<<"m_bIsConnect ="<<m_bTcpSocket_IsConnect;
    m_bTcpSocket_IsConnect = false;
    QTimer::singleShot(10000,this,SLOT(Slot_connectToHost()));
    emit Signal_ConnectState(m_bTcpSocket_IsConnect);
}

void CTcpClient::Slot_TcpSocket_error(QAbstractSocket::SocketError nSocketError)
{
    qDebug()<<__func__<<"nSocketError ="<<nSocketError<<"  m_pTcpSocket->error() ="<<m_pTcpSocket->error()<<m_pTcpSocket->errorString();
    m_bTcpSocket_IsConnect = false;
//    QTimer::singleShot(10000,this,SLOT(Slot_connectToHost()));

}

void CTcpClient::Slot_TcpSocket_stateChanged(QAbstractSocket::SocketState nSocketState_)
{
    qDebug()<<__func__<<"Slot_Connected() chenggong  m_pTcpSocket->state() ="<<m_pTcpSocket->state()<<"  nSocketState_ ="<<nSocketState_<<"  m_bIsConnect ="<<m_bTcpSocket_IsConnect;
    switch (nSocketState_) {
    case QAbstractSocket::UnconnectedState:
        m_bTcpSocket_IsConnect = false;
        break;
    case QAbstractSocket::HostLookupState:

        break;
    case QAbstractSocket::ConnectingState:

        break;
    case QAbstractSocket::ConnectedState:

        break;
    case QAbstractSocket::BoundState:

        break;
    case QAbstractSocket::ClosingState:
        m_bTcpSocket_IsConnect = false;
        break;
    case QAbstractSocket::ListeningState:

        break;
    default:
        break;
    }
}

void CTcpClient::Slot_TcpSocket_readyRead()
{
    m_nTcpClientLastReceiveTime = GetSysAbsTime();
    QByteArray Response = m_pTcpSocket->readAll();
    emit Signal_TcpSocket_Response(Response);
}

int CTcpClient::Absolute(int nValue_)
{
    return (((nValue_)<0) ? (-(nValue_)) : (nValue_));
}

int CTcpClient::AbsoluteDEC(int nLeft, int nRight_)
{
    return (((nLeft)>=(nRight_)) ? ((nLeft)-(nRight_)):((nRight_)-(nLeft)));
}
