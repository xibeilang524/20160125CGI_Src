#ifndef CMYNETWORKBASE_H
#define CMYNETWORKBASE_H

#include "CMyNetWorkI.h"
#include "CTcpClient.h"
#include "CTcpServer.h"

class CMyNetWorkBase : public CMyNetWorkI
{
    Q_OBJECT
public:
    explicit CMyNetWorkBase(QObject *parent = 0);
    bool SetTcpServerIP_Port(const QHostAddress  &TcpServer_HostAddress_, const quint16 nTcpServer_Port_, CMyNetWorkI::MyNetWork_LinkType nType=MyNetWork_LinkType_UnknownSocketType);
    virtual bool ChangeTCPServerIP_Port(const QHostAddress  &TcpServer_HostAddress_, const quint16 nTcpServer_Port_);
    bool GetLinkState() const;
    void SetCloseTcpSocket();

    qint64	write(const char * data_, qint64 maxSize_);
    qint64	write(const char * data_);
    qint64	write(const QByteArray & byteArray_);
signals:

public slots:
private:
    MyNetWork_LinkType m_nLinkType;
    CTcpServer *m_pTcpServer;
    CTcpClient *m_pTcpClient;

};

#endif // CMYNETWORKBASE_H
