#ifndef CJAVASCRIPT_PROTOCOL_H
#define CJAVASCRIPT_PROTOCOL_H

#include "../PortInclude/CJavaScript_ProtocolI.h"
#include "CJavaScript_PointBase.h"
#include "CJavaScript_DataChange_Point.h"


class CJavaScript_Protocol : public CJavaScript_ProtocolI
{
    Q_OBJECT
public:
    CJavaScript_Protocol(QObject *parent = 0);
    bool OnCreateChannel(CJavaScript_ProtocolI::JAVASCRIPT_CONTROL_TYPE nType_,const QDomElement elemControl_, CRTDBI *pRealDatabase);
    bool OnTagChange(CPointI *pPoint_, QVariant VarSet_);

signals:
    void signal_TagChange(CPointI *pPoint_, QVariant VarSet_);
private slots:
    void slot_TagChange(CPointI *pPoint_, QVariant VarSet_);

private:
    CRTDBI *m_pRTDB;

    bool AddPoint(Point *pPoint_);
    Point *FindPoint(QString strLink_);

    QMap<QString,Point *> m_Protocol_PointMap;
    JAVASCRIPT_CONTROL_TYPE m_nType;

};

#endif // CJAVASCRIPT_PROTOCOL_H
