#ifndef CJAVASCRIPT_PROTOCOLI_H
#define CJAVASCRIPT_PROTOCOLI_H

#include <QtCore/qglobal.h>
#if defined(CGI_JAVASCRIPT_LIBRARY)
#  define CGI_JAVASCRIPTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CGI_JAVASCRIPTSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include "CRTDBI.h"
#include "cpointbase.h"
#include "XmlParser/CXmlParser.h"
#include <QDomDocument>
#include <QDomElement>



class CGI_JAVASCRIPTSHARED_EXPORT CJavaScript_ProtocolI : public QObject
{
    Q_OBJECT
public:
    enum JAVASCRIPT_CONTROL_TYPE{
        Control_Undefined = 0,  ///< 未定义类型
        Control_Start,          ///< 启动
        Control_Cycle,          ///< 循环
        Control_Timing,         ///< 定时
        Control_DateChange      ///< 数据改变
    };
    explicit CJavaScript_ProtocolI(QObject *parent = 0):QObject(parent){}
    virtual bool OnCreateChannel(CJavaScript_ProtocolI::JAVASCRIPT_CONTROL_TYPE nType_,const QDomElement elemControl_, CRTDBI *pRealDatabase) = 0;
    virtual bool OnTagChange(CPointI *pPoint_, QVariant VarSet_) = 0;

signals:

public slots:

};
extern "C" CGI_JAVASCRIPTSHARED_EXPORT CJavaScript_ProtocolI *CreateDriver_JavaScript(QObject *parent);

#endif // CJAVASCRIPT_PROTOCOLI_H
