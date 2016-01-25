#ifndef CGI_CLINKI_H
#define CGI_CLINKI_H

#include <QtCore/qglobal.h>
#include <QObject>
#include <QDomElement>
#include <QHostAddress>
#if defined(CGI_CLINKLIB_LIBRARY)
#  define CGI_CLINKLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CGI_CLINKLIBSHARED_EXPORT Q_DECL_IMPORT
#endif


class CGI_CLINKLIBSHARED_EXPORT CGI_CLINKI : public QObject
{

public:
    CGI_CLINKI(QObject *parent):QObject(parent){}
    virtual bool CGI_SetInitLink(int nChannelType_,const QDomElement &MainPort_) = 0;
    virtual int GetLinkType()const = 0;
    virtual int GetLinkState() const = 0;
    virtual qint64 write(const QByteArray &data) = 0;
    virtual qint64 write(const char *data, qint64 len) = 0;
    virtual bool ChangeTCPServerIP_Port(const QHostAddress  &TcpServer_HostAddress_, const quint16 nTcpServer_Port_) = 0;
    virtual void SetCloseLink() = 0;
signals:
    void ReadyRead(const QByteArray Respond);
    void signalConnect(bool);
    void disconnected();

private slots:

};
extern "C" CGI_CLINKLIBSHARED_EXPORT CGI_CLINKI * CreateCGI_CLINKI(QObject *parent);

#endif // CGI_CLINKI_H
