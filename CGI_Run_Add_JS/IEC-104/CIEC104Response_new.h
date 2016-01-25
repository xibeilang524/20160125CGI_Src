#ifndef CIEC104RESPONSE_NEW_H
#define CIEC104RESPONSE_NEW_H
#include "../PortInclude/CBaseFrame.h"
#include "IEC_104.h"
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
class CIEC104Response_new : public CBaseFrame
{
public:
    CIEC104Response_new();
    int GetFrameType();
    QString GetFrameName();
    bool IsTestFrame();
    bool IsStartFrame();
    bool IsStopFrame();
    WORD GetSendFrameNo();
    WORD GetReceiveFrameNo();
    ASDU101 *GetAsdu();
};

#endif // CIEC104RESPONSE_NEW_H
