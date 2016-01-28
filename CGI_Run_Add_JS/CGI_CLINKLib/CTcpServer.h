#ifndef CTCPSERVER_H
#define CTCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QTimerEvent>

class CTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit CTcpServer(QObject *parent = 0);
    void timerEvent(QTimerEvent *event);
    ~CTcpServer();
public:
    bool GetLinkState() const;
    bool SetTcpServerIP_Port(const QHostAddress  &TcpServer_HostAddress_, const quint16 nTcpServer_Port_);
    void SetTcpServer_CloseTcpSocket();
    qint64	write(const char * data_, qint64 maxSize_);
    qint64	write(const char * data_);
    qint64	write(const QByteArray & byteArray_);
signals:
    void Signal_TcpServer_TcpSocket_Response(const QByteArray Respond);
    void Signal_ConnectState(bool);

private slots:
    bool Slot_TcpServer_Listen();
    void Slot_TcpServer_acceptError(QAbstractSocket::SocketError nSoocketError_);
    void Slot_TcpServer_TcpSocket_readyRead();
    void Slot_TcpServer_newConnection();
    void Slot_TcpServer_TcpSocket_error(QAbstractSocket::SocketError nSocketError_);
    void Slot_TcpServer_TcpSocket_disconnected();
private:

    int GetSysAbsTime();
    int Absolute(int nValue_);
    int AbsoluteDEC(int nLeft,int nRight_);


    QTcpServer *m_pTcpServer;
    QTcpSocket *m_pTcpServer_TcpSocket;
    bool m_bTcpServer_TcpSocket_IsConnect;
    bool m_bTcpServer_IsListening;
    QHostAddress m_TcpServer_HostAddress;
    quint16 m_nTcpServer_Port;


    int m_nTimerID;
    int m_nTcpClientLastReceiveTime;
};

#endif // CTCPSERVER_H
