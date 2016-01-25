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

#ifndef CPROTOCOLBASE_H
#define CPROTOCOLBASE_H

#include "../PortInclude/cprotocoli.h"

class CProtocolBase : public CProtocolI
{
    Q_OBJECT
public:
//! 供运行系统调用的函数
    virtual bool OnCreateChannel(const QString strChannelFileName_,CRTDBI *pDatabaseReturn) = 0;//建立通道并返回通道自己的实时库  实时数据库
    virtual bool OnCreateChannel(const QDomElement &ChannelElement_,ChannelType nChannelType_, int nComOrCanNumber_,CRTDBI *pDatabaseReturn_);
    virtual bool OnStart() = 0; //启动通道
    virtual bool OnScan() = 0;  //扫描通道
    virtual bool OnStop() = 0;  //停止通道
    virtual void SetShowMessage(void (*ShowMessage_)(int,QString,int));
    virtual void SetChannelNumber(int nChannelNumber_);
    virtual bool OnTagChange(CPointBase *pPointBase_, QVariant VarSet_) = 0;
    virtual void SetPRTMap(CPRTMapI *pPRTMapI_) = 0;
    virtual qint64 Write(const char * data, qint64 maxSize) = 0;
    virtual qint64 Write(const char * data) = 0;
    virtual qint64 Write(const QByteArray  byteArray) = 0;
    virtual int GetZTChannelNumber() = 0;
    virtual QList<int> GetZTDeliversChannelNumberList() = 0;
    virtual bool GetLinkStatus();

public:
    enum SHOWMESSAGE_TYPE{
        SHOWMESSAGE_TYPE_SEND=0,
        SHOWMESSAGE_TYPE_RECEIVE,
        SHOWMESSAGE_TYPE_DEBUG
    };
    virtual void ShowMessage(int type ,QString strMessage);
    int GetSysAbsTime();///获取绝对秒
signals:

public slots:

protected:
    CRTDBI *m_pRTDB;///实时数据库
    CPRTMapI *m_pPRTMapI;///驱动导出类指针和驱动的信息map
    int m_nChannelNumber;///运行时run自动分配的通道号
    int m_nZTChannelNumber;///组态时组态的此通道的通道号
    QList<int> m_ZTDeliversChannelNumberList;///将报文转至某几个通道

private:
    void (*m_ShowMessage)(int,QString,int);
};

#endif // CPROTOCOLBASE_H
