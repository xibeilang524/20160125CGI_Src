/*!
 * \file    ciec104response.cpp
 * \brief   概述 CIEC104Response类的源文件
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

#include "ciec104response.h"

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
CIEC104Response::CIEC104Response()
{
    m_pBufferHead = NULL;
    m_nBufferSize = 0;
    m_nInfomationSize = 0;
    m_nMaxBufferSize = 1024*1024*10;//缓冲区最大长度 10k;
    m_bRefBuffer = false;
}

/*!
 * \brief  功能概述 获取报文中ASDU部分的指针，取得一个ASDU101
 * \param  参数描述 无
 * \return 返回值描述 返回报文中ASDU部分的指针
 * \author zzy
 * \date   2015/5/25
 */
ASDU101 *CIEC104Response::GetAsdu()
{
    if(GetFrameType() != IEC104_I_TYPE)
        return NULL;
    if(GetInfoSize()<10)
        return NULL;
    return (ASDU101 *)GetBuffer(6);
}

/*!
 * \brief  功能概述 获取报文类型
 * \param  参数描述 无
 * \return 返回值描述 返回报文类型
 * \author zzy
 * \date   2015/5/25
 */
int CIEC104Response::GetFrameType()
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
 * \brief  功能概述 获取帧的名字
 * \param  参数描述 无
 * \return 返回值描述 返回帧的名字
 * \author zzy
 * \date   2015/5/25
 */
QString CIEC104Response::GetFrameName()
{
    BYTE nControlByte1 = GetUInt(2,1);
    QString strTemp;
    switch(nControlByte1&3)
    {
    case 1:
        //m_strFrameName = "S帧";
        strTemp = QString("S帧 Recieve %1").arg(GetReceiveFrameNo());
        break;
    case 3:
        strTemp = "U帧";
        break;
    default:
        //m_strFrameName = "I帧";
        strTemp = QString("I帧 Recieve %1,Send %2").arg(GetReceiveFrameNo()).arg(GetSendFrameNo());
        break;
    }
    m_strFrameName = "(" + strTemp + ")";
    return m_strFrameName;
}

/*!
 * \brief  功能概述 是否为启动帧
 * \param  参数描述 无
 * \return 返回值描述 如果是启动帧则返回true，否则返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Response::IsStartFrame()
{
    Q_ASSERT(GetFrameType() == IEC104_U_TYPE );
    BYTE nControlByte1 = GetUInt(2,1);
    return (nControlByte1&0x8)!=0;
}

/*!
 * \brief  功能概述 是否为停止帧
 * \param  参数描述 无
 * \return 返回值描述 如果是停止帧则返回true，否则返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Response::IsStopFrame()
{
    Q_ASSERT(GetFrameType() == IEC104_U_TYPE );
    BYTE nControlByte1 = GetUInt(2,1);
    return (nControlByte1&0x20)!=0;
}

/*!
 * \brief  功能概述 是否为测试帧
 * \param  参数描述 无
 * \return 返回值描述 如果为测试帧则返回true，否则返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Response::IsTestFrame()
{
    Q_ASSERT(GetFrameType() == IEC104_U_TYPE );
    BYTE nControlByte1 = GetUInt(2,1);
    return (nControlByte1&0x80)!=0;
}

/*!
 * \brief  功能概述 设置缓冲区内容
 * \param  参数描述 byteArraySource是接收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CIEC104Response::Src_SetData(const QByteArray byteArraySource)
{
    const char *pSource = byteArraySource.data();
    int nSize = byteArraySource.count();
    Q_ASSERT(pSource != NULL);
    Q_ASSERT(nSize >= 0);
    Q_ASSERT(nSize <= m_nMaxBufferSize);
    if(nSize > m_nMaxBufferSize)
    {
        nSize = m_nMaxBufferSize;
    }
    if (nSize > m_nBufferSize)//分配新空间
    {
        SetBufferSize(nSize);
    }
    memmove(m_pBufferHead, pSource, nSize);
    m_nInfomationSize = nSize;
}

/*!
 * \brief  功能概述 设置缓冲区内容
 * \param  参数描述 pSource是报文的头指针，nSize是报文的总长度
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CIEC104Response::Src_SetData(const char *pSource, int nSize)
{
    Q_ASSERT(pSource != NULL);
    Q_ASSERT(nSize >= 0);
    Q_ASSERT(nSize <= m_nMaxBufferSize);
    if(nSize > m_nMaxBufferSize)
    {
        nSize = m_nMaxBufferSize;
    }
    if (nSize > m_nBufferSize)//分配新空间
    {
        SetBufferSize(nSize);
    }
    memmove(m_pBufferHead, pSource, nSize);
    m_nInfomationSize = nSize;
}

/*!
 * \brief  功能概述 获取报文中的值
 * \param  参数描述 nStartByte是开始字节，nSize是字节个数
 * \return 返回值描述 返回报文中获取的值
 * \author zzy
 * \date   2015/5/25
 */
unsigned int CIEC104Response::GetUInt( int nStartByte, int nSize ) const
{
    Q_ASSERT(IsValid());
    AdjustIndex(nStartByte);
    Q_ASSERT(nStartByte >= 0);
    Q_ASSERT(nSize > 0);
    Q_ASSERT(nStartByte+nSize <= m_nInfomationSize);

    unsigned int nReturnValue = INVALID_VALUE;
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

/*!
 * \brief  功能概述 负数表示从数组的后面取  使用中可能出现比较怪的问题
 * \param  参数描述 nIndex是字节索引
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CIEC104Response::AdjustIndex(int &nIndex)const
{
    nIndex =  nIndex>=0?nIndex:m_nInfomationSize+nIndex;
}

/*!
 * \brief  功能概述
 * \param  参数描述
 * \return 返回值描述
 * \author zzy
 * \date   2015/5/25
 */
int CIEC104Response::GetInfoSize() const
{
    return m_nInfomationSize;
}

/*!
 * \brief  功能概述 获取缓冲区内容
 * \param  参数描述 nStartByte是开始字节
 * \return 返回值描述 返回缓冲区指针
 * \author zzy
 * \date   2015/5/25
 */
BYTE *CIEC104Response::GetBuffer(int nStartByte) const
{
    Q_ASSERT(IsValid());
    AdjustIndex(nStartByte);
    Q_ASSERT(nStartByte >= 0);
    Q_ASSERT(nStartByte < m_nInfomationSize);
    return m_pBufferHead + nStartByte;
}

/*!
 * \brief  功能概述 判断参数是否正确
 * \param  参数描述 无
 * \return 返回值描述 正确返回true，错误返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Response::IsValid() const
{
    Q_ASSERT(m_pBufferHead != NULL);   //缓冲区头指针
    Q_ASSERT(m_nBufferSize > 0);        //缓冲区实际大小
    Q_ASSERT(m_nBufferSize <= m_nMaxBufferSize);
    Q_ASSERT(m_nInfomationSize >= 0);//缓冲区中信息长度
    Q_ASSERT(m_nInfomationSize <= m_nBufferSize);
    return true;
}

/*!
 * \brief  功能概述 获取发送序号
 * \param  参数描述 无
 * \return 返回值描述 返回报文中的发送序号
 * \author zzy
 * \date   2015/5/25
 */
WORD CIEC104Response::GetSendFrameNo()
{
    Q_ASSERT(GetFrameType() == IEC104_I_TYPE );
    WORD nSendFrameNo = GetUInt(2,2);
    return nSendFrameNo >>1;
}

/*!
 * \brief  功能概述 获取接收序号
 * \param  参数描述 无
 * \return 返回值描述 返回接受序号
 * \author zzy
 * \date   2015/5/25
 */
WORD CIEC104Response::GetReceiveFrameNo()
{
    Q_ASSERT(GetFrameType() != IEC104_U_TYPE );
    WORD nReceiveFrameNo = GetUInt(4,2);
    return nReceiveFrameNo>>1;
}

/*!
 * \brief  功能概述 设置缓冲区大小
 * \param  参数描述 nNewSize是新的数据大小
 * \return 返回值描述 返回缓冲区的大小
 * \author zzy
 * \date   2015/5/25
 */
int CIEC104Response::SetBufferSize(int nNewSize)
{
    Q_ASSERT(nNewSize >= 0);
    Q_ASSERT(nNewSize <= m_nMaxBufferSize);

    ClearBuffer();
    if (nNewSize > 0)
    {
        nNewSize = nNewSize > m_nMaxBufferSize ? nNewSize:m_nMaxBufferSize;
        m_pBufferHead = new BYTE[nNewSize];
        m_nBufferSize = nNewSize;
        m_nInfomationSize = 0;
    }
    return m_nBufferSize;
}

/*!
 * \brief  功能概述 清除缓冲区
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CIEC104Response::ClearBuffer()
{
    if (m_pBufferHead != NULL && m_nBufferSize > 0 && !m_bRefBuffer)
    {
        delete []m_pBufferHead;
    }
    m_bRefBuffer = false;
    m_pBufferHead  = NULL;
    m_nInfomationSize = 0;
    m_nBufferSize = 0;
}
