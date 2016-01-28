#ifndef CJAVASCRIPT_POINTBASE_H
#define CJAVASCRIPT_POINTBASE_H

#include "../PortInclude/CPointI.h"
#include "../PortInclude/CJavaScript_ProtocolI.h"

class Point
{
public:
    Point()
    {
        m_pTag = NULL;
    }

    QString  m_strLink;
    CTagI   *m_pTag;
    QVariant m_SrcValue;///< 原来的值
};

class CJavaScript_PointBase : public CPointI
{
public:
    CJavaScript_PointBase();
    bool OnValueChange(QVariant valueSet_);///< 响应变量什发生变化
    CValueI *GetValue();///< 获取值
    int GetDeviceAddress();///< 获取设备地址
    CJavaScript_ProtocolI *m_pProtocol;
    CValueI     *m_pValue;    ///< 变量的值
    CTagI       *m_pTag;      ///< 变量所在tag

    QString     m_strLink;
    QString     m_nstrName;
};

#endif // CJAVASCRIPT_POINTBASE_H
