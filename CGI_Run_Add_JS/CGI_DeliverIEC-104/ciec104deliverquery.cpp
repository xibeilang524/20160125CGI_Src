/*!
 * \file    ciec104deliverquery.cpp
 * \brief   概述 CIEC104DeliverQuery类的源文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/26
 *
 * \b       修改记录：
 * \li      2015/5/26
 *  添加注释
 */

#include "ciec104deliverquery.h"

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
CIEC104DeliverQuery::CIEC104DeliverQuery()
{
}

/*!
 * \brief  功能概述 判断是否为104帧
 * \param  参数描述 无
 * \return 返回值描述 是104帧返回true，否则返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CIEC104DeliverQuery::Is104Frame()
{
    if (GetUInt(0,1) == 0x68)
        return true;
    return false;
}

/*!
 * \brief  功能概述 获取帧类型
 * \param  参数描述 无
 * \return 返回值描述 返回帧类型
 * \author zzy
 * \date   2015/5/26
 */
int CIEC104DeliverQuery::GetFrameType()
{
    BYTE nControlByte1 = GetUInt(2,1);
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

/*!
 * \brief  功能概述 获取帧接收序号
 * \param  参数描述 无
 * \return 返回值描述 返回帧接收序号
 * \author zzy
 * \date   2015/5/26
 */
WORD CIEC104DeliverQuery::GetReceiveFrameNo()
{
    Q_ASSERT(GetFrameType() != IEC104_U_TYPE );
    WORD nReceiveFrameNo = GetUInt(4,2);
    return nReceiveFrameNo>>1;
}

/*!
 * \brief  功能概述 获取发送序号
 * \param  参数描述 无
 * \return 返回值描述 返回发送序号
 * \author zzy
 * \date   2015/5/26
 */
WORD CIEC104DeliverQuery::GetSendFrameNo()
{
    Q_ASSERT(GetFrameType() == IEC104_I_TYPE );
    WORD nSendFrameNo = GetUInt(2,2);
    return nSendFrameNo >>1;
}

/*!
 * \brief  功能概述 取得一个ASDU101
 * \param  参数描述 无
 * \return 返回值描述 返回一个ASDU指针
 * \author zzy
 * \date   2015/5/26
 */
ASDU101 *CIEC104DeliverQuery::GetAsdu()
{
    if(GetFrameType() != IEC104_I_TYPE)
        return NULL;
    if(GetInfoSize()<10)
        return NULL;
    return (ASDU101 *)GetBuffer(6);
}

/*!
 * \brief  功能概述 获取指定缓冲区头指针
 * \param  参数描述 nStartByte是开始字节
 * \return 返回值描述 返回指定缓冲区头指针
 * \author zzy
 * \date   2015/5/26
 */
BYTE *CIEC104DeliverQuery::GetBuffer(int nStartByte) const
{
    Q_ASSERT(IsValid());
//	AdjustIndex(nStartByte);
    Q_ASSERT(nStartByte >= 0);
    Q_ASSERT(nStartByte < m_nInfomationSize);
    return m_pBufferHead + nStartByte;
}

/*!
 * \brief  功能概述 获取缓冲区报文长度
 * \param  参数描述 无
 * \return 返回值描述 返回缓冲区报文长度
 * \author zzy
 * \date   2015/5/26
 */
int CIEC104DeliverQuery::GetInfoSize() const
{
    return m_nInfomationSize;
}

/*!
 * \brief  功能概述 判断参数是否正确
 * \param  参数描述 无
 * \return 返回值描述 正确返回true，错误返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104DeliverQuery::IsValid() const
{
    Q_ASSERT(m_pBufferHead != NULL);   //缓冲区头指针
//    Q_ASSERT(m_nBufferSize > 0);        //缓冲区实际大小
//    Q_ASSERT(m_nBufferSize <= m_nMaxBufferSize);
    Q_ASSERT(m_nInfomationSize >= 0);//缓冲区中信息长度
//    Q_ASSERT(m_nInfomationSize <= m_nBufferSize);
    return true;
}

/*!
 * \brief  功能概述 判断是否为启动帧
 * \param  参数描述 无
 * \return 返回值描述 若是启动帧返回true，不是返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CIEC104DeliverQuery::IsStartFrame()
{
    if (GetUInt(2,1) == 0x07)
        return true;
    return false;
}

/*!
 * \brief  功能概述 判断是否为停止帧
 * \param  参数描述 无
 * \return 返回值描述 若是停止帧返回true，不是返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CIEC104DeliverQuery::IsStopFrame()
{
    if (GetUInt(2,1) == 0x13)
        return true;
    return false;
}

/*!
 * \brief  功能概述 判断是否为测试帧
 * \param  参数描述 无
 * \return 返回值描述 若是测试帧返回true，不是返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CIEC104DeliverQuery::IsTestFrame()
{
    if (GetUInt(2,1) == 0x43)
        return true;
    return false;
}

/*!
 * \brief  功能概述 设置缓冲区内容
 * \param  参数描述 byteArraySource是接收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CIEC104DeliverQuery::SetData(const QByteArray &bytearrayFrame_)
{
    ClearBuffer();
    memcpy(m_ByteBuffer,bytearrayFrame_.data(),bytearrayFrame_.count());
    m_pBufferHead = m_ByteBuffer;
    m_nInfomationSize = bytearrayFrame_.count();
}

/*!
 * \brief  功能概述 清除缓冲区
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CIEC104DeliverQuery::ClearBuffer()
{
    memset(m_ByteBuffer,0,BUFFER_SIZE);
}

/*!
 * \brief  功能概述 获取报文中的值
 * \param  参数描述 nStartByte是开始字节，nSize是字节个数
 * \return 返回值描述 返回报文中获取的值
 * \author zzy
 * \date   2015/5/26
 */
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
