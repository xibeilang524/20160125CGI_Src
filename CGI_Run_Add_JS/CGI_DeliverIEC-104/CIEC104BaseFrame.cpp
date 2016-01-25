#include "CIEC104BaseFrame.h"

CIEC104BaseFrame::CIEC104BaseFrame()
{
}
/*!
 * \brief  功能概述 获取帧类型
 * \param  参数描述 无
 * \return 返回值描述 返回帧类型
 * \author zzy
 * \date   2015/5/26
 */
int CIEC104BaseFrame::GetFrameType()
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
 * \brief  功能概述 获取帧接收序号
 * \param  参数描述 无
 * \return 返回值描述 返回帧接收序号
 * \author zzy
 * \date   2015/5/26
 */
WORD CIEC104BaseFrame::GetReceiveFrameNo()
{
    Q_ASSERT(GetFrameType() != IEC104_U_TYPE );
    WORD nReceiveFrameNo = GetUIntValue(4,2);
    return nReceiveFrameNo>>1;
}

/*!
 * \brief  功能概述 获取发送序号
 * \param  参数描述 无
 * \return 返回值描述 返回发送序号
 * \author zzy
 * \date   2015/5/26
 */
WORD CIEC104BaseFrame::GetSendFrameNo()
{
    Q_ASSERT(GetFrameType() == IEC104_I_TYPE );
    WORD nSendFrameNo = GetUIntValue(2,2);
    return nSendFrameNo >>1;
}

/*!
 * \brief  功能概述 取得一个ASDU101
 * \param  参数描述 无
 * \return 返回值描述 返回一个ASDU指针
 * \author zzy
 * \date   2015/5/26
 */
ASDU101 *CIEC104BaseFrame::GetAsdu()
{
    if(GetFrameType() != IEC104_I_TYPE)
        return NULL;
    if(GetInfoSize()<10)
        return NULL;
    return (ASDU101 *)GetBuffer(6);
}

/*!
 * \brief  功能概述 判断是否为启动帧
 * \param  参数描述 无
 * \return 返回值描述 若是启动帧返回true，不是返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CIEC104BaseFrame::IsStartFrame()
{
    if (GetUIntValue(2,1) == 0x07)
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
bool CIEC104BaseFrame::IsStopFrame()
{
    if (GetUIntValue(2,1) == 0x13)
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
bool CIEC104BaseFrame::IsTestFrame()
{
    if (GetUIntValue(2,1) == 0x43)
        return true;
    return false;
}
