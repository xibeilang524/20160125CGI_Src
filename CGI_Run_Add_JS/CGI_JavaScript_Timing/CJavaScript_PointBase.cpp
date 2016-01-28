#include "CJavaScript_PointBase.h"
#include "../PortInclude/cvaluebase.h"

CJavaScript_PointBase::CJavaScript_PointBase()
{
    m_pProtocol = NULL;
    m_pTag= NULL;
    m_pValue = new CValueBase;
}

bool CJavaScript_PointBase::OnValueChange(QVariant valueSet_)
{
    Q_ASSERT(m_pValue != NULL);
    return m_pProtocol->OnTagChange(this,valueSet_);
}

CValueI *CJavaScript_PointBase::GetValue()
{
    Q_ASSERT(m_pValue != NULL);
    return m_pValue;
}

int CJavaScript_PointBase::GetDeviceAddress()
{
    return 0;
}
