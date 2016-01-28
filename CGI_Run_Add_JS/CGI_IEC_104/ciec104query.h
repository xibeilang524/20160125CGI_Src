/*!
 * \file    ciec104query.h
 * \brief   概述 CIEC104Query类的头文件
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

#ifndef CIEC104QUERY_H
#define CIEC104QUERY_H

#include <QByteArray>
#include "IEC_104.h"
#include "../PortInclude/CBaseFrame.h"
/*!
 * \class  CIEC104Query
 * \brief  概述 IEC-104协议接收到的报文
 * \author zzy
 * \date   2015/5/25
 * 详细概述
 */
class CIEC104Query : public CBaseFrame
{
public:
    CIEC104Query();
    enum{BUFFER_SIZE = 260};
public:
    bool MakeQuery_U_Command(bool bStart_, bool bStop_, bool bTest_);
    bool MakeQuery_U_Confirm(bool bStart_, bool bStop_, bool bTest_);
    bool MakeQuery_IntrogenCall(WORD &nSendNo,WORD nReceiveNo,BYTE nCommonAddress);
    bool MakeQuery_S(WORD nReceiveNo);
    bool MakeQuery_CallPlus(WORD &nSendNo,WORD nReceiveNo, BYTE nCommonAddress);
    bool MakeQuery_AdjustTime(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress);
    bool MakeQuery_RemoteControl(WORD &nSendNo,WORD nReceiveNo, BYTE nCommonAddress,int nPoint, bool bValue,int nStep);
    bool MakeQuery_I_YS(WORD &nSendNo,WORD nReceiveNo, BYTE nCommonAddress,ASDU101_TYPE nType,unsigned int nPointNumber, QVariant varValue);
    bool MakeQuery_I_YK(WORD &nSendNo,WORD nReceiveNo, BYTE nCommonAddress,ASDU101_TYPE nType,BYTE nYKOutType,BYTE nSelectExcute,unsigned int nPointNumber, QVariant varValue);
    const QByteArray &GetByteArrayFrame();
private:
    bool MakeQuery_I(WORD &nSendNo,WORD nReceiveNo, BYTE *pAsdu, int nAsduSize);
private:
    bool Src_SetData(const BYTE *pSource_, int nLength_);
    QByteArray m_ByteArrayFrame;
};

#endif // CIEC104QUERY_H
