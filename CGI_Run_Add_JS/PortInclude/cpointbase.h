/*!
 * \file    cpointbase.h
 * \brief   概述 CPointBase的头文件
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

#ifndef CPOINTBASE_H
#define CPOINTBASE_H
#include "../PortInclude/CPointI.h"
#include <QString>
#include "../PortInclude/CValueI.h"
class CTagI;
class CModbusRtuProtocol;
class CProtocolI;

/*!
 * \class  CPointBase
 * \brief  概述 转发和采集驱动中点的基类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CPointBase : public CPointI
{
public:
    CPointBase();
    virtual ~CPointBase();

public:
    virtual bool OnValueChange(QVariant valueSet_);
    virtual CValueI *GetValue() ;
    ///获取设备地址
    virtual int GetDeviceAddress();

public:
    virtual bool OnCreate(const QString strConfig);
    QString GetConfigString(const QString strAttribName);
    int    GetConfigInt(const QString strAttribName);
    double GetConfigDouble(const QString strAttribName);

public:
    virtual void GetBinary(BYTE *pBuffer, int nBufferSize);
    virtual void SetBinary(BYTE *pBuffer,int nBufferSize, int nTime_ = 0,int nQulity_ = 0);

public:
    virtual QString GetPointName();
    virtual void SetPointName(const QString strTagName_);

    //注释
    virtual QString GetPointComment();
    virtual void    SetPointComment(const QString strTagComment_);

    //关联的db变量名 //可以没有
    //关联的db量
    virtual CTagI * GetTag();
    virtual void ConnectTag(CTagI *pTag_) ;
    virtual void DisConnectTag();

    //变量类型  ai ao di do mem
    virtual int GetPointType();
    virtual void SetPointtype(int nType_);


    //变量所在的通道
    virtual CProtocolI *GetProtocol();
    virtual void SetProtocol(CProtocolI *pProtocol_);
    //变量所在的结点
//    virtual CDeviceBase *GetDevice(void);
//    virtual void SetDevice(CDeviceBase *pDevice);

//Data Member
public:
    QString m_strPointName;     ///< 变量名称，可以没有
    QString m_strPointComment;  ///< 变量注释，可以没有
    QString m_strRTDBName;      ///< 关联的db变量名，可以没有
    int m_nTagType;             ///< 变量类型  ai 1 ao 2 di 3 do 4 mem 5
//    TAG_TYPE_AI = 1,
//    TAG_TYPE_AO,
//    TAG_TYPE_DI,
//    TAG_TYPE_DO,
//    TAG_TYPE_MEM,
    QString m_strConfig;
    CProtocolI *m_pProtocol;    ///< 变量所在的通道
    CTagI         *m_pTag;      ///< 变量所在tag
    CValueI       *m_pValue;    ///< 变量的值
    int m_nScanTime;
    int m_nID;
    float m_fKValue;    ///< 系数
    float m_fBaseValue; ///< 基数
};

#endif // CPOINTBASE_H
