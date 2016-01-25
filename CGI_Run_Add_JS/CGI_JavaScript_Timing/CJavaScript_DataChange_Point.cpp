#include "CJavaScript_DataChange_Point.h"

CJavaScript_DataChange_Point::CJavaScript_DataChange_Point()
{
}

bool CJavaScript_DataChange_Point::AddJavaScript(JavaScript *pJavaScript_)
{
    m_JavaScriptList.append(pJavaScript_);
    return true;
}
