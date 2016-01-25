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

#ifndef CIEC104DELIVERQUERY_H
#define CIEC104DELIVERQUERY_H
#include "IEC_104.h"

//#include <windef.h>
#include <QByteArray>
// 定义链路层帧格式
typedef enum{
    UNKNOWN_IEC104_FRAME_TYPE=-1,
    IEC104_I_TYPE,	// 计数的信息传输格式帧(I Frame)
    IEC104_S_TYPE,	// 计数的监视功能格式、(S Frame)
    IEC104_U_TYPE,	// 不计数的控制功能、、(U Frame)
    IEC104_Start_TYPE  /// 协议标识帧类型
}IEC104FrameType;

class CIEC104DeliverQuery
{
public:
    CIEC104DeliverQuery();
    enum{
        BUFFER_SIZE = 256,
    };
    void SetData(const QByteArray &bytearrayFrame_);
    bool Is104Frame();//是否为104帧
    int GetFrameType();//获取帧类型
    WORD GetReceiveFrameNo();//获取接收序号
    WORD GetSendFrameNo();//获取发送序号
    ASDU101 *GetAsdu();
    virtual int  GetInfoSize() const;
    BYTE *GetBuffer(int nStartByte) const;
    virtual bool IsValid() const;
// U 帧
    bool IsStartFrame();
    bool IsStopFrame();
    bool IsTestFrame();
private:
    void ClearBuffer();
private:
    unsigned int GetUInt(int nStartByte, int nSize) const;

private://变量
    BYTE m_ByteBuffer[BUFFER_SIZE];
    BYTE *m_pBufferHead;
    int m_nInfomationSize;
};

#endif // CIEC104DELIVERQUERY_H
