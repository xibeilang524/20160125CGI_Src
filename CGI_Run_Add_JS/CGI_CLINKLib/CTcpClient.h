#ifndef CTCPCLIENT_H
#define CTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QTimerEvent>
#include <QDateTime>

//#define ABS(a)  (((a)<0)?(-(a)):(a))
//#define ABSDEC(a,b)	(((a)>=(b)) ? ((a)-(b)):((b)-(a)))
class CTcpClient : public QObject
{
    Q_OBJECT
public:
    explicit CTcpClient(QObject *parent = 0);
    void timerEvent(QTimerEvent *event);
public:
    bool GetLinkState() const;
    bool SetTcpServerIP_Port(const QHostAddress  &TcpServer_HostAddress_, const quint16 nTcpServer_Port_);
    bool ChangeTCPServerIP_Port(const QHostAddress  &TcpServer_HostAddress_, const quint16 nTcpServer_Port_);
    void SetCloseTcpSocket();
    qint64	write(const char * data_, qint64 maxSize_);
    qint64	write(const char * data_);
    qint64	write(const QByteArray & byteArray_);
signals:
    void Signal_TcpSocket_Response(const QByteArray Respond);///< 连接状态
    void Signal_ConnectState(bool);///< 连接状态
private slots:
    void Slot_connectToHost();
    void Slot_TcpSocket_connected();
    void Slot_TcpSocket_disconnected();
    void Slot_TcpSocket_error(QAbstractSocket::SocketError nSocketError);
    void Slot_TcpSocket_stateChanged(QAbstractSocket::SocketState nSocketState_);
    void Slot_TcpSocket_readyRead();

private:
    QTcpSocket *m_pTcpSocket;
    bool m_bTcpSocket_IsConnect;
    QHostAddress m_TcpServer_HostAddress;
    quint16 m_nTcpServer_Port;

    int m_nTimerID;
    int m_nTcpClientLastReceiveTime;
    int Absolute(int nValue_);
    int AbsoluteDEC(int nLeft,int nRight_);
    int GetSysAbsTime();
};

#endif // CTCPCLIENT_H
