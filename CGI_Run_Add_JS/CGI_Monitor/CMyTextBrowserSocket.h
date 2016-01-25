/*!
 * \file
 * \brief   概述
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/4/9
 *
 * \b       修改记录：
 * \li      2015/4/9
 *  添加注释
 */

#ifndef CMYTEXTBROWSERSOCKET_H
#define CMYTEXTBROWSERSOCKET_H

#include "cmytextbrowser.h"
#include <QTcpSocket>
#include <QFile>
typedef unsigned char BYTE;
typedef unsigned short  WORD;
typedef unsigned long DWORD;
#define LOBYTE(w) ((BYTE) (((DWORD) (w)) & 0xff))
#define HIBYTE(w) ((BYTE) ((((DWORD) (w)) >> 8) & 0xff))
#define FramePort 9002
#define BUFFER_SIZE 10240

class CSignFrameDialog;
class QTcpSocket;

/*!
 * \class
 * \brief  概述
 * \author zzy
 * \date   2015/4/9
 * 详细概述
 */
class CMyTextBrowserSocket : public CMyTextBrowser
{
    Q_OBJECT
public:
    explicit CMyTextBrowserSocket(CMyTextBrowser *parent = 0);
    void closeEvent(QCloseEvent *);
    void Start();
    void SetChannelMessage(int nChannelNumber_,const QString &strWindowTitle_);
    bool SendStartFrame();
    bool SetData(const BYTE *pSource_, int nLength_);
    void DecodeFrame(QByteArray &datas);
    int GetChannelNumber();
    void PrintTXSignFrame(QString str, QColor color);///打印发送报文
    void PrintRXSignFrame(QString str, QColor color);///打印接收报文
    void SetTextFormat();
private:
    int m_nChannelNumber;
    QString m_strWindowTitle;
    QTcpSocket *m_pTcpSocket;
    bool m_bConnectOk;
    int m_nProtcolTimerId;
    QFile m_SaveFrameFile;
    QTextStream m_TextStreamOut;
signals:
    void signalClose(CMyTextBrowserSocket *);
public slots:
    void SlotSaveFrame(QString str);
    void SlotSignFrame();/// @note 特殊报文显示槽函数
    void ProcessRespond();
    void OnConnected();
    void displayError(QAbstractSocket::SocketError);
    void OnOpenSocket();
private:
    CSignFrameDialog *m_pSignFrameDialog;
};

#endif // CMYTEXTBROWSERSOCKET_H
