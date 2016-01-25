#ifndef CJAVASCRIPT_DATACHANGE_POINT_H
#define CJAVASCRIPT_DATACHANGE_POINT_H

#include "CJavaScript_PointBase.h"
#include <QMap>


//struct PointStruct
//{
//    QString m_strLink;
//    QString m_strName;
//    Point *m_pPoint;
//};

class CJavaScript_DataChange_Point;

struct PointStruct
{
    QString m_strLink;
    QString m_strName;
    CJavaScript_DataChange_Point *m_pPoint;
};
class JavaScript
{
public:
    QString m_strScript;
    QMap<QString,PointStruct*> m_PointStructMap;/// Link 和 Name
};

class CJavaScript_DataChange_Point : public CJavaScript_PointBase
{
public:
    CJavaScript_DataChange_Point();
    bool AddJavaScript(JavaScript *pJavaScript_);

//private:

    QVariant m_SrcValue;///< 原来的值
    QList<JavaScript *> m_JavaScriptList;

};

#endif // CJAVASCRIPT_DATACHANGE_POINT_H
