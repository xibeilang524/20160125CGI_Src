#ifndef MYNETWORK_H
#define MYNETWORK_H

#include <QObject>
#include <QHostAddress>

class CMyNetWorkI : public QObject
{
    Q_OBJECT
public:
    CMyNetWorkI(QObject *parent):QObject(parent){}

    enum MyNetWork_LinkType {
        MyNetWork_LinkType_UnknownSocketType = -1,
        MyNetWork_LinkType_TcpServer,
        MyNetWork_LinkType_TcpClient,
    };
    virtual bool SetTcpServerIP_Port(const QHostAddress  &TcpServer_HostAddress_, const quint16 nTcpServer_Port_, CMyNetWorkI::MyNetWork_LinkType nType=MyNetWork_LinkType_UnknownSocketType) = 0;
    virtual bool ChangeTCPServerIP_Port(const QHostAddress  &TcpServer_HostAddress_, const quint16 nTcpServer_Port_) = 0;
    virtual bool GetLinkState() const = 0;
    virtual void SetCloseTcpSocket() = 0;

    virtual qint64	write(const char * data_, qint64 maxSize_) = 0;
    virtual qint64	write(const char * data_) = 0;
    virtual qint64	write(const QByteArray & byteArray_) = 0;
signals:
    void Signal_ConnectState(bool);
    void Signal_Response(QByteArray);

};

#endif // MYNETWORK_H
