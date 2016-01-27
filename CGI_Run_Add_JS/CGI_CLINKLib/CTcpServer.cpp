#include "CTcpServer.h"

CTcpServer::CTcpServer(QObject *parent) :
    QObject(parent)
{
    m_pTcpServer = NULL;
    m_pTcpServer_TcpSocket = NULL;
    m_bTcpServer_TcpSocket_IsConnect = false;
    m_bTcpServer_IsListening = false;
    m_nTimerID = startTimer(1000);
}

void CTcpServer::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nTimerID)
    {
        qDebug()<<__func__<<m_bTcpServer_TcpSocket_IsConnect;
        if (m_bTcpServer_TcpSocket_IsConnect)
        {
            if (Absolute(GetSysAbsTime() - m_nTcpClientLastReceiveTime) > 30)
            {///< 30秒没有收到数据则断开连接
                SetTcpServer_CloseTcpSocket();
            }
        }else
        {

        }

    }
}

CTcpServer::~CTcpServer()
{
    m_pTcpServer = NULL;
    m_pTcpServer_TcpSocket = NULL;
    m_bTcpServer_TcpSocket_IsConnect = false;
    m_bTcpServer_IsListening = false;
}

bool CTcpServer::GetLinkState() const
{
    return m_bTcpServer_TcpSocket_IsConnect;
}

bool CTcpServer::SetTcpServerIP_Port(const QHostAddress &TcpServer_HostAddress_, const quint16 nTcpServer_Port_)
{
    m_TcpServer_HostAddress = TcpServer_HostAddress_;
    m_nTcpServer_Port = nTcpServer_Port_;
    if (NULL == m_pTcpServer)
    {
        m_pTcpServer = new QTcpServer(this);
        connect(m_pTcpServer,SIGNAL(acceptError(QAbstractSocket::SocketError)),this,SLOT(Slot_TcpServer_acceptError(QAbstractSocket::SocketError)));
        connect(m_pTcpServer,SIGNAL(newConnection()),this,SLOT(Slot_TcpServer_newConnection()));
    }else
    {

    }

    if (m_bTcpServer_IsListening)
    {

    }else
    {
        m_bTcpServer_IsListening = m_pTcpServer->listen(m_TcpServer_HostAddress,m_nTcpServer_Port);
        //    int nListen2 = m_pTcpServer->waitForNewConnection();
        qDebug()<<__func__<<"m_bTcpServer_IsListening ="<<m_bTcpServer_IsListening;
        if (m_bTcpServer_IsListening)
        {

        }else
        {
            QTimer::singleShot(10000,this,SLOT(Slot_TcpServer_Listen()));
        }
    }
    return m_bTcpServer_IsListening;
}

void CTcpServer::SetTcpServer_CloseTcpSocket()
{
    qDebug()<<__func__;
    if (m_bTcpServer_TcpSocket_IsConnect)
    {
        m_pTcpServer_TcpSocket->disconnectFromHost();
        qDebug()<<__func__<<"m_pTcpSocket->disconnectFromHost();";
        if (m_pTcpServer_TcpSocket->state() == QAbstractSocket::UnconnectedState || m_pTcpServer_TcpSocket->waitForDisconnected(5*1000))
        {
            qDebug()<<__func__<<"if (m_pTcpSocket->waitForDisconnected()) true";
        }else
        {
            qDebug()<<__func__<<"if (m_pTcpSocket->waitForDisconnected()) false";
        }
    }else
    {
        qDebug()<<__func__<<"LianJieDuanKaiLe."<<"m_bIsConnect"<<m_bTcpServer_TcpSocket_IsConnect;
    }
    m_bTcpServer_TcpSocket_IsConnect = false;
}

qint64 CTcpServer::write(const char *data_, qint64 maxSize_)
{
    if (m_bTcpServer_TcpSocket_IsConnect)
    {
        qDebug()<<__func__<<"m_bIsConnect"<<m_bTcpServer_TcpSocket_IsConnect;
        int nWrite = m_pTcpServer_TcpSocket->write(data_,maxSize_);
        bool bwaitForBytesWritten = m_pTcpServer_TcpSocket->waitForBytesWritten(5*1000);
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
        qDebug()<<__func__<<"LianJieDuanKaiLe."<<"m_bIsConnect"<<m_bTcpServer_TcpSocket_IsConnect;
        return 0;
    }
}

qint64 CTcpServer::write(const char *data_)
{
    if (m_bTcpServer_TcpSocket_IsConnect)
    {
        qDebug()<<__func__<<"m_bTcpServer_TcpSocket_IsConnect ="<<m_bTcpServer_TcpSocket_IsConnect;
        int nWrite = m_pTcpServer_TcpSocket->write(data_);
        bool bwaitForBytesWritten = m_pTcpServer_TcpSocket->waitForBytesWritten(5*1000);
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
        qDebug()<<__func__<<"LianJieDuanKaiLe."<<"m_bIsConnect"<<m_bTcpServer_TcpSocket_IsConnect;
        return 0;
    }
}

qint64 CTcpServer::write(const QByteArray &byteArray_)
{
    if (m_bTcpServer_TcpSocket_IsConnect)
    {
        qDebug()<<__func__<<"m_bIsConnect"<<m_bTcpServer_TcpSocket_IsConnect;
        int nWrite = m_pTcpServer_TcpSocket->write(byteArray_);
        bool bwaitForBytesWritten = m_pTcpServer_TcpSocket->waitForBytesWritten(5*1000);
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
        qDebug()<<__func__<<"LianJieDuanKaiLe."<<"m_bIsConnect"<<m_bTcpServer_TcpSocket_IsConnect;
        return 0;
    }
}
bool CTcpServer::Slot_TcpServer_Listen()
{
    if (m_bTcpServer_IsListening)
    {
        return m_bTcpServer_IsListening;
    }else
    {
        m_bTcpServer_IsListening = m_pTcpServer->listen(m_TcpServer_HostAddress,m_nTcpServer_Port);
        if (m_bTcpServer_IsListening)
        {
            QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
            qDebug()<<debug<<"Listen Tcp Server"<<m_bTcpServer_IsListening<<m_TcpServer_HostAddress<<m_nTcpServer_Port;
            return true;
        }
        else
        {
            QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
            qDebug()<<debug<<"Listen Tcp Server"<<m_bTcpServer_IsListening<<m_TcpServer_HostAddress<<m_nTcpServer_Port;
            QTimer::singleShot(10000,this,SLOT(Slot_TcpServer_Listen()));
//            QTimer::singleShot(100,this,SLOT(Slot_TcpServer_Listen()));
            return false;
        }
    }

}
void CTcpServer::Slot_TcpServer_acceptError(QAbstractSocket::SocketError nSoocketError_)
{
    qDebug()<<__func__<<nSoocketError_<<m_pTcpServer->errorString();
}

void CTcpServer::Slot_TcpServer_TcpSocket_readyRead()
{
//    qDebug()<<__func__;
    QByteArray Response = m_pTcpServer_TcpSocket->readAll();
    m_nTcpClientLastReceiveTime = GetSysAbsTime();
    emit Signal_TcpServer_TcpSocket_Response(Response);
}

void CTcpServer::Slot_TcpServer_newConnection()
{
    qDebug()<<__func__;

    if (m_bTcpServer_TcpSocket_IsConnect)
    {
        m_pTcpServer->nextPendingConnection()->deleteLater();
    }else
    {
        m_pTcpServer_TcpSocket = m_pTcpServer->nextPendingConnection();
        qDebug()<<"m_pTcpServer->socketDescriptor()="<<m_pTcpServer->socketDescriptor();
        m_bTcpServer_TcpSocket_IsConnect = true;
        qDebug()<<"m_pTcpSocket="<<m_pTcpServer_TcpSocket;
        m_pTcpServer->socketDescriptor();
    //    connect(((QTcpSocket *)m_pLinkIODevice),SIGNAL(disconnected()), this,SLOT(SocketDeleteLater()));
        connect(m_pTcpServer_TcpSocket,SIGNAL(readyRead()),this,SLOT(Slot_TcpServer_TcpSocket_readyRead()));
        connect(m_pTcpServer_TcpSocket,SIGNAL(disconnected()),this,SLOT(Slot_TcpServer_TcpSocket_disconnected()));
        connect(m_pTcpServer_TcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(Slot_TcpServer_TcpSocket_error(QAbstractSocket::SocketError)));
        connect(m_pTcpServer_TcpSocket,SIGNAL(disconnected()),m_pTcpServer_TcpSocket,SLOT(deleteLater()));
        m_nTcpClientLastReceiveTime = GetSysAbsTime();
        emit Signal_ConnectState(m_bTcpServer_TcpSocket_IsConnect);
    }
}

void CTcpServer::Slot_TcpServer_TcpSocket_error(QAbstractSocket::SocketError nSocketError_)
{/// 先 1
    m_bTcpServer_TcpSocket_IsConnect = false;
    qDebug()<<__func__<<"nSocketError_="<<nSocketError_<<m_pTcpServer_TcpSocket->error()<<m_pTcpServer_TcpSocket->errorString()<<"m_pTcpSocket="<<m_pTcpServer_TcpSocket;

}

void CTcpServer::Slot_TcpServer_TcpSocket_disconnected()
{/// 后 2
    m_bTcpServer_TcpSocket_IsConnect = false;
    qDebug()<<__func__<<"m_pTcpSocket="<<m_pTcpServer_TcpSocket;
    emit Signal_ConnectState(m_bTcpServer_TcpSocket_IsConnect);
}

int CTcpServer::Absolute(int nValue_)
{
    return (((nValue_)<0) ? (-(nValue_)) : (nValue_));
}

int CTcpServer::AbsoluteDEC(int nLeft, int nRight_)
{
    return (((nLeft)>=(nRight_)) ? ((nLeft)-(nRight_)):((nRight_)-(nLeft)));
}

int CTcpServer::GetSysAbsTime()
{
    ///获取绝对秒
    int nTime = QDateTime::currentDateTime().secsTo(QDateTime(QDate(1970,1,1),QTime(0,0,0,0)));
    if(nTime < 0)
    {
        nTime = -nTime;
    }
    return nTime;
}
