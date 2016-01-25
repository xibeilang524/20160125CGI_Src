/*!
 * \file
 * \brief   概述
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/3/24
 *
 * \b       修改记录：
 * \li      2015/3/24
 *  添加注释
 */

#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
//#include <windef.h>

#include "cprotocolbase.h"
#include "../PortInclude/CRTDBI.h"
#include "../PortInclude/CTagI.h"
#include "../PortInclude/cvaluebase.h"
//QFile DebugDeliverIEC104File;
//QTextStream OutDeliverIEC104(&DebugDeliverIEC104File);
void DebugDeliverIEC104Out(QString /*strDebug*/)
{

//    out.setFieldWidth(15);
//    out.setFieldAlignment(QTextStream::AlignLeft);
//    OutDeliverIEC104<<strDebug<<"\r\n";
//    DebugDeliverIEC104File.flush();
}
bool CProtocolBase::OnCreateChannel(const QDomElement &/*ChannelElement_*/, ChannelType /*nChannelType_*/, int /*nComOrCanNumber_*/, CRTDBI */*pDatabaseReturn_*/)
{
    return true;
}

void CProtocolBase::SetShowMessage(void (*ShowMessage_)(int, QString, int))
{
    m_ShowMessage = ShowMessage_;
}

void CProtocolBase::SetChannelNumber(int nChannelNumber_)
{
    m_nChannelNumber = nChannelNumber_;
}

bool CProtocolBase::GetLinkStatus()
{
    return true;
}

void CProtocolBase::ShowMessage(int type, QString strMessage)
{
    ///用于显示报文的函数
    /// type=0发送
    /// type=1接收
    /// type=2向窗口打印调试信息
//    m_ShowMessage(type,strMessage,m_nChannelNumber);
    QString str;
    switch (type) {
    case 0:
        str = QString("转发-->发送\n"+strMessage+'\n'+QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz"));
        break;
    case 1:
        str = QString("转发-->接收\n"+strMessage+'\n'+QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz"));
        break;
    case 2:
        str = QString("转发-->Debug\n"+strMessage+'\n'+QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz"));
        break;
    default:
        break;
    }
//    DebugDeliverIEC104Out(str);
    qDebug()<<str;
}

int CProtocolBase::GetSysAbsTime()
{
    ///获取绝对秒
    int nTime = QDateTime::currentDateTime().secsTo(QDateTime(QDate(1970,1,1),QTime(0,0,0,0)));
    if(nTime < 0)
    {
        nTime = -nTime;
    }
    return nTime;
}
