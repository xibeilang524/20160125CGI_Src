#ifndef CIEC104BASEFRAME_H
#define CIEC104BASEFRAME_H

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
class CIEC104BaseFrame : public CBaseFrame
{
public:
    CIEC104BaseFrame();
    enum{
        BUFFER_SIZE = 1024,///< 缓冲区内容最大值
    };

    int GetFrameType();
    WORD GetReceiveFrameNo();
    WORD GetSendFrameNo();
    ASDU101 *GetAsdu();
    bool IsStartFrame();
    bool IsStopFrame();
    bool IsTestFrame();
};

#endif // CIEC104BASEFRAME_H
