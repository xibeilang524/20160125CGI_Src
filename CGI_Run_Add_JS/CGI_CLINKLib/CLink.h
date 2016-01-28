/*!
 * \file    CLink.h
 * \brief   概述 CLink类的头文件
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

#ifndef CLINK_H
#define CLINK_H
#include "../PortInclude/CGI_CLINKI.h"

#include "cserialport.h"

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QDomElement>
#include <QTime>
#include "CMyNetWorkI.h"

#if defined(Q_OS_LINUX)
#include "CCANThread.h"
#endif



#ifndef LINKTYPE
#define LINKTYPE
/*!
 * \enum   LinkType
 * \brief  概述 链路类型的枚举
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
enum LinkType{
    LinkType_Undefined = -1,///< 未定义链路类型
    LinkType_COM = 1,       ///< 串口类型
    LinkType_TCP,           ///< TCP端口类型
    LinkType_UDP,           ///< UDP端口类型
    LinkType_ShareMemory,   ///< 共享内存类型
    LinkType_Custom,        ///< 自定义端口类型
    LinkType_Can,
};
#endif



/*!
 * \class  CLink
 * \brief  概述 链路管理类
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
class CLink : public CGI_CLINKI
{
    Q_OBJECT
public:
    explicit CLink(QObject *parent = 0);
    bool CGI_SetInitLink(int nChannelType_,const QDomElement &MainPort_);
    int GetLinkType()const ;
    int GetLinkState() const;
    qint64 write(const QByteArray &data);
    qint64 write(const char *data, qint64 len);
    bool ChangeTCPServerIP_Port(const QHostAddress  &TcpServer_HostAddress_, const quint16 nTcpServer_Port_);
    void SetCloseLink();
private:
    bool SetInitCOM(const QDomElement &MainPort_);
    bool SetInitCAN(const QDomElement &MainPort_);
    bool SetInitUDP(const QDomElement &MainPort_);
    bool CGI_SetInitUDP(const QDomElement &MainPort_);
    bool SetInitShareMemory(const QDomElement &MainPort_);
    bool SetInitCustom(const QDomElement &MainPort_);
    QIODevice *m_pLinkIODevice;
    QDomElement m_EleMainPort;///< 链路参数
    bool m_bIsConnect;
    QTcpServer *m_pTcpServer;

    QHostAddress m_Receive; ///< UDP在通讯过程中的有数据接收的IP地址和端口号
    quint16 m_ReceivePort;  ///< UDP在通讯过程中的有数据接收的IP地址和端口号
    QHostAddress m_Sender;  ///< UDP在通讯过程中的有数据接收的IP地址和端口号
    quint16 m_SenderPort;   ///< UDP在通讯过程中的有数据接收的IP地址和端口号
signals:
    void ReadyRead(const QByteArray Respond);
    void signalConnect(bool);
public slots:
    void ProcessRespond();
private slots:
private:
    int m_nLinkType;///< 链路类型
    CMyNetWorkI *m_pMyNetWork;
    QHostAddress m_HostAddressTCPClient;
    int m_nPortTCPClient;
    QHostAddress m_HostAddressTCPServer;
    int m_nPortTCPServer;
#if defined(Q_OS_LINUX)
    CCANThread *m_pCanThread;
#endif
};

#endif // CLINK_H
