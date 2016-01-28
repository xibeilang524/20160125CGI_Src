/*!
 * \file    ciec104response.h
 * \brief   概述 CIEC104Response类的头文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/25
 *
 * \b       修改记录：
 * \li      2015/5/25
 *  添加注释
 */

#ifndef CIEC104RESPONSE_H
#define CIEC104RESPONSE_H

#include <QString>
#include "IEC_104.h"
//#include "../PortInclude/CBaseFrame.h"

#define INVALID_VALUE 99


/*!
 * \enum
 * \brief  概述 定义链路层帧格式
 * \author zzy
 * \date   2015/5/25
 * 详细概述
 */
#ifndef IEC104FRAME_TYPE
#define IEC104FRAME_TYPE
typedef enum IEC104frameType
{
    UNKNOWN_IEC104_FRAME_TYPE=-1,
    IEC104_I_TYPE,	///< 计数的信息传输格式帧(I Frame)
    IEC104_S_TYPE,	///< 计数的监视功能格式(S Frame)
    IEC104_U_TYPE,	///< 不计数的控制功能(U Frame)
}IEC104FrameType;
#endif
/*!
 * \class  CIEC104Response
 * \brief  概述 接收到的报文类
 * \author zzy
 * \date   2015/5/25
 * 详细概述 接收到报文之后首先要SetData(const char *pSource, int nSize);
 */
class CIEC104Response /*:public CBaseFrame*/
{
public:
    CIEC104Response();

public:
    void Src_SetData(const QByteArray byteArraySource);
    void Src_SetData(const char *pSource, int nSize);
    ASDU101 * GetAsdu();
    int GetFrameType();
    QString GetFrameName();
    bool IsStartFrame();
    bool IsStopFrame();
    bool IsTestFrame();
    WORD GetReceiveFrameNo();
    WORD GetSendFrameNo();
    int  GetInfoSize() const;
    BYTE *GetBuffer(int nStartByte = 0) const ;

private:
    unsigned int GetUInt( int nStartByte, int nSize ) const;
    void AdjustIndex(int &nIndex)const;
    bool IsValid() const;
    int SetBufferSize(int nNewSize);
    void ClearBuffer();
private:
    BYTE *m_pBufferHead;        ///< 缓冲区头指针
    int   m_nBufferSize;        ///< 缓冲区实际大小
    int   m_nInfomationSize;    ///< 缓冲区中信息长度
    int   m_nMaxBufferSize;     ///< 缓冲区最大长度
    bool  m_bRefBuffer;         ///< 是否处于引用Buffer状态
    QString m_strFrameName;
};

#endif // CIEC104RESPONSE_H
