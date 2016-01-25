#ifndef CWRITEFRAME_H
#define CWRITEFRAME_H

#include <QObject>
#include <QMap>
#include <QTcpSocket>



class CTCPSocket : public QObject
{
    Q_OBJECT
 public:
    explicit CTCPSocket(QObject *parent = 0);
    void SetTcpSocket(QTcpSocket *pTcpSocket_);
    int m_nChannelNumber;
    QTcpSocket *m_pTcpSocket;
    int GetChannelNumber() const;
    void SetChannelNumber(const int nChannelNumber);
signals:
    void signalTcpSocketDisConnected(CTCPSocket *);
public slots:
    void WriteMessage(int type, QString message);
    void SocketDeleteLater();
    void OnConnected();
    void displayError(QAbstractSocket::SocketError);
    void ProcessRespond();
private:

};

class QTcpServer;
class CWriteFrame : public QObject
{
    Q_OBJECT
public:
    explicit CWriteFrame(QString strIPAddress_,QObject *parent = 0);
private:
    QString m_strTcpIp;
    QTcpServer *m_tcpServer;///用于传输报文的服务端
    QTcpSocket *m_TcpSocket;///用于传输实时数据的客户端
    QMap<int,QTcpSocket *> m_TcpSocketFrameMap;
    QList<CTCPSocket*> m_CTcpSocketList;
signals:

public slots:
    void WriteMessage(int type,QString message,int nChannelNumber);

public slots:
    void TCPConnect();
    void OnserverConnected();
    void slotDeleteCTcpSocket(CTCPSocket *pTcpSocket);
};

#endif // CWRITEFRAME_H
