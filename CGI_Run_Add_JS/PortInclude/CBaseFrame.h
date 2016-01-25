#ifndef CBASEFRAME_H
#define CBASEFRAME_H

#include "CDecodeBuffer_Hl.h"
#include <QDebug>

//帧类型定义
enum FRAME_TYPE
{//+100 ~ -100为系统使用,如果使用自定方式,请选择这个范围以外的数据值
    //约定<0为各种出错情况,由GetAFrame自动处理,>=0为合格数据的种情况,由用户处理
    FRAME_TYPE_UNDEFINE=-100,      //未定义的帧类型
    FRAME_TYPE_OTHER_NODE=-2,     //校验正确,但从机地址不对(是其它站的数据
    FRAME_TYPE_LENGTH_SMALL=-3,   //长度不足
    FRAME_TYPE_LENGTH_BIG=-4,     //长度过长
    FRAME_TYPE_ERROR=-100,        //错误帧   包含校验码不对
    FRAME_TYPE_INVALID=0,         //帧无效   不合格数据或不完整数据或当前无法判定数据
    FRAME_TYPE_OK=100,            //完全合格数据

    FRAME_TYPE_MAXVALUE=1000
};

class CBaseFrame : public CDecodeBuffer_Hl
{
    Q_OBJECT
public:
    CBaseFrame();
    virtual ~CBaseFrame();
    const QByteArray GetToHexFrame();
    const QByteArray GetToAsciiFrame();
public:
    virtual QString GetFrameName();
    static void strRev(BYTE *s,int nLength_);

public:
    int  m_nFrameType;      //帧类型
    QString m_strFrameName; //帧名称

};

#endif // CBASEFRAME_H
