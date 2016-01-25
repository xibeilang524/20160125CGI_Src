/*!
 * \file    cprotocoli.h
 * \brief   概述 CProtocolI类的头文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/27
 *
 * \b       修改记录：
 * \li      2015/5/27
 *  添加注释
 */

#ifndef CPROTOCOLI_H
#define CPROTOCOLI_H

#include <QtCore/qglobal.h>

#if defined(MODBUSRTU_LIBRARY)
#  define MODBUSRTUSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MODBUSRTUSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>

#include "../PortInclude/CPRTMapI.h"
#include "../PortInclude/CPRTI.h"
#include <QDomElement>
class CRTDBI;
class CPointBase;
class CValueI;


#ifndef ChannelType_H
#define ChannelType_H
enum ChannelType{
    ChannelType_COM = 0,
    ChannelType_NET,
    ChannelType_CAN,
    ChannelType_Monitor104,
};
#endif

/*!
 * \class  CProtocolI
 * \brief  概述 采集和转发驱动的纯虚基类，驱动接口导出基类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class MODBUSRTUSHARED_EXPORT CProtocolI : public QObject
{
    Q_OBJECT
public:
//  供运行系统调用的函数
    /// 创建通道
    virtual bool OnCreateChannel(const QString strChannelFileName_,CRTDBI *pDatabaseReturn_) = 0;
    virtual bool OnCreateChannel(const QDomElement &ChannelElement_,ChannelType nChannelType_, int nComOrCanNumber_,CRTDBI *pDatabaseReturn_) = 0;
    /// 启动通道
    virtual bool OnStart() = 0;
    /// 扫描通道
    virtual bool OnScan() = 0;
    /// 停止通道
    virtual bool OnStop() = 0;
    /// 初始化显示函数的函数指针
    virtual void SetShowMessage(void (*ShowMessage_)(int,QString,int)) = 0;
    /// 设置此通道的通道号
    virtual void SetChannelNumber(int) = 0;
    /// 用于响应点的值发生变化的函数
    virtual bool OnTagChange(CPointBase *pPointBase_, QVariant VarSet_) = 0;
    /// 初始化通道管理类对象的指针
    virtual void SetPRTMap(CPRTMapI *pPRTMapI_) = 0;
    /// 供其它驱动调用的函数，用于报文的透明转发
    virtual qint64 Write(const char * data, qint64 maxSize) = 0;
    /// 供其它驱动调用的函数，用于报文的透明转发
    virtual qint64 Write(const char * data) = 0;
    /// 供其它驱动调用的函数，用于报文的透明转发
    virtual qint64 Write(const QByteArray  byteArray) = 0;
    /// 获取该通道组态时组态的通道号
    virtual int GetZTChannelNumber() = 0;
    /// 获取组态时组态的转发到（）通道列表
    virtual QList<int> GetZTDeliversChannelNumberList() = 0;
    /// 此函数用于监视工具的驱动，作用是向驱动中写发送和接收的报文
    virtual void WriteMessage(int type,QString message,int nChannelNumber) = 0;
    /// 获取连接状态
    virtual bool GetLinkStatus() = 0;

    enum SHOWMESSAGE_TYPE{
        SHOWMESSAGE_TYPE_SEND=0,
        SHOWMESSAGE_TYPE_RECEIVE,
        SHOWMESSAGE_TYPE_DEBUG
    };
};
extern "C" MODBUSRTUSHARED_EXPORT CProtocolI * CreateDriver(QObject *parent);

#endif // CPROTOCOLI_H
