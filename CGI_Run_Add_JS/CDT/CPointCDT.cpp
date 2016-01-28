#include "CPointCDT.h"
#include "../PortInclude/CTagI.h"

CPointCDT::CPointCDT()
{
}

int CPointCDT::GetDeviceAddress()
{
    return m_nSrcStationAdd;
}

void CPointCDT::SetValue(CPointI* pPoint_,QVariant VarProject_, QVariant VarOriginal_)
{
    if (m_pTag)
    {
        m_pTag->SetValue(pPoint_,VarProject_,VarOriginal_);
    }
}
