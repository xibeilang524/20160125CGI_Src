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

#include "ciec104deliverquery.h"

CIEC104DeliverQuery::CIEC104DeliverQuery()
{
}

bool CIEC104DeliverQuery::Is104Frame()
{
    if (GetUInt(0,1) == 0x68)
        return true;
    return false;
}

int CIEC104DeliverQuery::GetFrameType()
{
    //获取帧类型
    BYTE nControlByte1 = GetUInt(2,1);
    if (nControlByte1 == 0xFF)
    {
        return IEC104_Start_TYPE;
    }
    switch(nControlByte1&3)
    {
    case 1:
        return IEC104_S_TYPE;
    case 3:
        return IEC104_U_TYPE;
    default:
        return IEC104_I_TYPE;
    }
}

WORD CIEC104DeliverQuery::GetReceiveFrameNo()
{
    Q_ASSERT(GetFrameType() != IEC104_U_TYPE );
    WORD nReceiveFrameNo = GetUInt(4,2);
    return nReceiveFrameNo>>1;
}

WORD CIEC104DeliverQuery::GetSendFrameNo()
{
    Q_ASSERT(GetFrameType() == IEC104_I_TYPE );
    WORD nSendFrameNo = GetUInt(2,2);
    return nSendFrameNo >>1;
}

ASDU101 *CIEC104DeliverQuery::GetAsdu()
{
    //功能：取得一个ASDU101
    //参数:
    //返回值:
    //建立日期:2007-08-29
    //修改日期:
    //作者:
    //说明:
    if(GetFrameType() != IEC104_I_TYPE)
        return NULL;
    if(GetInfoSize()<10)
        return NULL;
    return (ASDU101 *)GetBuffer(6);
}
BYTE *CIEC104DeliverQuery::GetBuffer(int nStartByte) const
{
    Q_ASSERT(IsValid());
//	AdjustIndex(nStartByte);
    Q_ASSERT(nStartByte >= 0);
    Q_ASSERT(nStartByte < m_nInfomationSize);
    return m_pBufferHead + nStartByte;
}
int CIEC104DeliverQuery::GetInfoSize() const
{
    return m_nInfomationSize;
}
bool CIEC104DeliverQuery::IsValid() const
{
    Q_ASSERT(m_pBufferHead != NULL);   //缓冲区头指针
//    Q_ASSERT(m_nBufferSize > 0);        //缓冲区实际大小
//    Q_ASSERT(m_nBufferSize <= m_nMaxBufferSize);
    Q_ASSERT(m_nInfomationSize >= 0);//缓冲区中信息长度
//    Q_ASSERT(m_nInfomationSize <= m_nBufferSize);
    return true;
}
bool CIEC104DeliverQuery::IsStartFrame()
{
    if (GetUInt(2,1) == 0x07)
        return true;
    return false;
}

bool CIEC104DeliverQuery::IsStopFrame()
{
    if (GetUInt(2,1) == 0x13)
        return true;
    return false;
}

bool CIEC104DeliverQuery::IsTestFrame()
{
    if (GetUInt(2,1) == 0x43)
        return true;
    return false;
}

void CIEC104DeliverQuery::SetData(const QByteArray &bytearrayFrame_)
{
    ClearBuffer();
    memcpy(m_ByteBuffer,bytearrayFrame_.data(),bytearrayFrame_.count());
    m_pBufferHead = m_ByteBuffer;
    m_nInfomationSize = bytearrayFrame_.count();
}

void CIEC104DeliverQuery::ClearBuffer()
{
    memset(m_ByteBuffer,0,BUFFER_SIZE);
}

unsigned int CIEC104DeliverQuery::GetUInt(int nStartByte, int nSize) const
{
//	Q_ASSERT(IsValid());
//	AdjustIndex(nStartByte);
    Q_ASSERT(nStartByte >= 0);
    Q_ASSERT(nSize > 0);
    Q_ASSERT(nStartByte+nSize <= m_nInfomationSize);

    unsigned int nReturnValue = 0;
    if (nStartByte>=0 && nStartByte+nSize<=m_nInfomationSize)
    {
        switch (nSize)
        {
        case 1:
            nReturnValue = *(unsigned  char *)(m_pBufferHead + nStartByte);
            break;
        case 2:
            nReturnValue = *(unsigned short int *)(m_pBufferHead + nStartByte);
            break;
        case 4:
            nReturnValue = *(unsigned int *)(m_pBufferHead + nStartByte);
            break;
        case 3://不常见
            nReturnValue = *(unsigned int *)(m_pBufferHead + nStartByte) & 0xFFFFFF;
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
    return nReturnValue;
}
