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

#ifndef CPOINTBASE_H
#define CPOINTBASE_H
#include "../PortInclude/CPointI.h"
#include "../PortInclude/typedef.h"
#include <QString>
#include "../PortInclude/CValueI.h"
class CTagI;
class CModbusRtuProtocol;
class CProtocolI;
class CPointBase : public CPointI
{
// Construction
public:
    CPointBase();
    virtual ~CPointBase();

// Overrides
public:
    virtual bool OnValueChange(QVariant valueSet_);//响应变量什发生变化
    virtual CValueI *GetValue() ;       //取得值
    virtual int GetDeviceAddress() = 0;

// Implementation
public:
    virtual bool OnCreate(const QString strConfig);
    QString GetConfigString(const QString strAttribName);
    int    GetConfigInt(const QString strAttribName);
    double GetConfigDouble(const QString strAttribName);

// Should be Override by sub class
public:
    virtual void GetBinary(BYTE *pBuffer,int nBufferSize);
    virtual void SetBinary(BYTE *pBuffer,int nBufferSize, int nTime_ = 0,int nQulity_ = 0);

public:
    virtual QString GetPointName();
    virtual void SetPointName(const QString strTagName);

    //注释
    virtual QString GetPointComment();
    virtual void    SetPointComment(const QString strTagComment);

    //关联的db变量名 //可以没有
    //关联的db量
    virtual CTagI * GetTag();
    virtual void ConnectTag(CTagI *pTag) ;
    virtual void DisConnectTag();

    //变量类型  ai ao di do mem
    virtual int GetPointType();
    virtual void SetPointtype(int nType);


    //变量所在的通道
    virtual CProtocolI *GetProtocol();
    virtual void SetProtocol(CProtocolI *pProtocol);
    //变量所在的结点
//    virtual CDeviceBase *GetDevice(void);
//    virtual void SetDevice(CDeviceBase *pDevice);

//Data Member
public:
    QString m_strPointName;      //变量名称       //可以没有
    QString m_strPointComment;   //变量注释       //可以没有
    QString m_strRTDBName;   //关联的db变量名 //可以没有
    int m_nTagType;             //变量类型  ai 1 ao 2 di 3 do 4 mem 5
//    TAG_TYPE_AI = 1,
//    TAG_TYPE_AO,
//    TAG_TYPE_DI,
//    TAG_TYPE_DO,
//    TAG_TYPE_MEM,
    QString m_strConfig;
    CProtocolI *m_pProtocol;     //变量所在的通道
    CTagI         *m_pTag;          //变量所在tag
    CValueI       *m_pValue;        //变量的值
    int m_nScanTime;
    int m_nID;
    float m_fKValue;///系数
    float m_fBaseValue;///基数
};

#endif // CPOINTBASE_H
