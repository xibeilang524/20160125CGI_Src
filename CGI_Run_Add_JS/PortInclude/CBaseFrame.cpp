#include "CBaseFrame.h"



CBaseFrame::CBaseFrame()
{
    m_nFrameType = FRAME_TYPE_UNDEFINE;
    m_strFrameName = "";
}

CBaseFrame::~CBaseFrame()
{

}

const QByteArray CBaseFrame::GetToHexFrame()
{
    return QByteArray((char *)GetBuffer(),GetInfoSize()).toHex().toUpper();
}

const QByteArray CBaseFrame::GetToAsciiFrame()
{
    return QByteArray((char *)GetBuffer(),GetInfoSize()).toUpper();
}

QString CBaseFrame::GetFrameName()
{//取得接收帧名称
    return m_strFrameName;
}

/*!
 \brief 反转

 \fn CBaseFrame::strRev
 \param s
 \param nLength_
*/
void CBaseFrame::strRev(BYTE *s, int nLength_)
{
    for(BYTE *end = s + nLength_ - 1; end > s ; --end, ++s)
    {
        *s ^= *end;
        *end ^= *s;
        *s ^= *end;
    }
}
