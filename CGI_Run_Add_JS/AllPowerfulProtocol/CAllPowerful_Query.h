#ifndef CALLPOWERFUL_QUERY_H
#define CALLPOWERFUL_QUERY_H

#include "../PortInclude/CBaseFrame.h"
#include "CAllPowerful_Point.h"

class CAllPowerful_Query : public CBaseFrame
{
public:
    CAllPowerful_Query();
    bool AddPoint(CAllPowerful_Point *pPoint_);
    QByteArray GetbaSendFrame() const;
    void SetBaSendFrame(const QByteArray &baSendFrame);
    int GetMinResponseFrameLength();
    typedef QMap<int,CAllPowerful_Point *> ByteCount_PointMAP;///int Byte Count
    QMap<int, CAllPowerful_Query::ByteCount_PointMAP *> *Get_pAllPowerful_PointMAP() ;

private:
    QMap<int ,ByteCount_PointMAP*> m_AllPowerful_PointMAP;
    QByteArray m_baAllPowerfulSendFrame;
    int m_nMinResponseFrameLength;///< 应该返回帧的最小长度

};

#endif // CALLPOWERFUL_QUERY_H
