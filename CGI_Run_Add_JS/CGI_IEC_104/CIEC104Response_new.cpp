#include "CIEC104Response_new.h"

CIEC104Response_new::CIEC104Response_new()
{
}
int CIEC104Response_new::GetFrameType()
{
    BYTE nControlByte1 = GetUIntValue(2,1);
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
QString CIEC104Response_new::GetFrameName()
{
    BYTE nControlByte1 = GetUIntValue(2,1);
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
 * \brief  功能概述 获取报文中ASDU部分的指针，取得一个ASDU101
 * \param  参数描述 无
 * \return 返回值描述 返回报文中ASDU部分的指针
 * \author zzy
 * \date   2015/5/25
 */
ASDU101 *CIEC104Response_new::GetAsdu()
{
    if(GetFrameType() != IEC104_I_TYPE)
        return NULL;
    if(GetInfoSize()<10)
        return NULL;
    return (ASDU101 *)GetBuffer(6);
}

/*!
 * \brief  功能概述 是否为启动帧
 * \param  参数描述 无
 * \return 返回值描述 如果是启动帧则返回true，否则返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Response_new::IsStartFrame()
{
    Q_ASSERT(GetFrameType() == IEC104_U_TYPE );
    BYTE nControlByte1 = GetUIntValue(2,1);
    return (nControlByte1&0x8)!=0;
}

/*!
 * \brief  功能概述 是否为停止帧
 * \param  参数描述 无
 * \return 返回值描述 如果是停止帧则返回true，否则返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Response_new::IsStopFrame()
{
    Q_ASSERT(GetFrameType() == IEC104_U_TYPE );
    BYTE nControlByte1 = GetUIntValue(2,1);
    return (nControlByte1&0x20)!=0;
}

/*!
 * \brief  功能概述 是否为测试帧
 * \param  参数描述 无
 * \return 返回值描述 如果为测试帧则返回true，否则返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Response_new::IsTestFrame()
{
    Q_ASSERT(GetFrameType() == IEC104_U_TYPE );
    BYTE nControlByte1 = GetUIntValue(2,1);
    return (nControlByte1&0x80)!=0;
}

/*!
 * \brief  功能概述 获取发送序号
 * \param  参数描述 无
 * \return 返回值描述 返回报文中的发送序号
 * \author zzy
 * \date   2015/5/25
 */
WORD CIEC104Response_new::GetSendFrameNo()
{
    Q_ASSERT(GetFrameType() == IEC104_I_TYPE );
    WORD nSendFrameNo = GetUIntValue(2,2);
    return nSendFrameNo >>1;
}

/*!
 * \brief  功能概述 获取接收序号
 * \param  参数描述 无
 * \return 返回值描述 返回接受序号
 * \author zzy
 * \date   2015/5/25
 */
WORD CIEC104Response_new::GetReceiveFrameNo()
{
    Q_ASSERT(GetFrameType() != IEC104_U_TYPE );
    WORD nReceiveFrameNo = GetUIntValue(4,2);
    return nReceiveFrameNo>>1;
}
