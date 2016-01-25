/*!
 * \file    cprotocolbase.h
 * \brief   概述 CProtocolBase类的头文件
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

#ifndef CPROTOCOLBASE_H
#define CPROTOCOLBASE_H

#include "cprotocoli.h"

#include <QFile>
#include <QTextStream>
#include <QDomElement>
#include "CGI_CLINKI.h"
#include "CFrameSharedMemory.h"
#include <QLibrary>

#define ABS(a)  (((a)<0)?(-(a)):(a))
#define ABSDEC(a,b)	(((a)>=(b)) ? ((a)-(b)):((b)-(a)))
class QDomElement;

/*!
 * \class  CProtocolBase
 * \brief  概述 所有采集和转发驱动的基类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CProtocolBase : public CProtocolI
{
    Q_OBJECT
public:
    CProtocolBase();
//  供运行系统调用的函数
    virtual void SetShowMessage(void (*ShowMessage_)(int,QString,int));
    virtual void SetChannelNumber(int nChannelNumber_);

    virtual bool OnTagChange(CPointBase *pPointBase_, QVariant VarSet_) = 0;
    virtual bool OnCreateChannel(const QString strChannelFileName_,CRTDBI *pDatabaseReturn);
    virtual bool OnCreateChannel(const QDomElement &ChannelElement_,ChannelType nChannelType_, int nComOrCanNumber_,CRTDBI *pDatabaseReturn_);
    virtual bool OnStart();
    virtual bool OnScan();
    virtual bool OnStop();
    virtual void SetPRTMap(CPRTMapI *pPRTMapI_);
    virtual qint64 Write(const char * data, qint64 maxSize);
    virtual qint64 Write(const char * data);
    virtual qint64 Write(const QByteArray  byteArray);
    virtual int GetZTChannelNumber();
    virtual QList<int> GetZTDeliversChannelNumberList();
    virtual void WriteMessage(int type_, QString message_, int nChannelNumber_);
    virtual bool GetLinkStatus();
public slots:
    virtual void ProcessRespond(QByteArray) = 0;
    virtual void slotConnect(bool) = 0;

public:
    virtual void ShowMessage(int type_ , QString strMessage_);

    bool SetInitLink(const QDomElement &Top_,const QDomElement &MainPort_);
    bool SetInitLink(ChannelType nChannelType_,const QDomElement &MainPort_);
    CGI_CLINKI *m_pLink;///< 链路类
    int GetSysAbsTime();
    int GetSysAbsTimeMSECS();
signals:


protected:
    void InitParameter();
    void SetPROTOCOL_TYPE(PROTOCOL_TYPE nPROTOCOL_TYPE_);
    int m_nLastTime;///< 最后发送时间
    CRTDBI *m_pRTDB;///< 实时数据库
    CPRTMapI *m_pPRTMapI;///< 驱动导出类指针和驱动的信息map
    int m_nChannelNumber;///< 运行时run自动分配的通道号
    int m_nZTChannelNumber;///< 组态时组态的此通道的通道号
    QList<int> m_ZTDeliversChannelNumberList;///< 将报文转至某几个通道
    QFile m_FileLog;
    bool m_IsSaveFrame;///< 报文是否存储
    QTextStream m_TextStreamOut;
    QString m_strChannelFileName;
private:
    void (*m_ShowMessage)(int,QString,int);///< 显示报文信息的函数指针
    CFrameSharedMemory *m_pFrameSharedMemory;///< 贡献内存类
//    bool m_bWriteFrameSharedMemoryFlag;///< 是否写共享内存
    bool m_bCreateStatusFrameSharedMemory;///< 创建共享内存是否成功
    int m_nPROTOCOL_TYPE;///< 驱动类型
    QLibrary m_CGI_CLinkLib;
};

#endif // CPROTOCOLBASE_H
