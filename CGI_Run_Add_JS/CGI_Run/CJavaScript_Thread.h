#ifndef CJAVASCRIPT_THREAD_H
#define CJAVASCRIPT_THREAD_H

#include <QThread>
#include <QDomElement>
#include <QLibrary>
#include "Src/crtdbbase.h"
#include "../PortInclude/CJavaScript_ProtocolI.h"
class CJavaScript_Thread : public QThread
{
    Q_OBJECT
public:
    explicit CJavaScript_Thread(CRTDBBase *pRealTimeDB_,const QDomElement &ControlElement_,CJavaScript_ProtocolI::JAVASCRIPT_CONTROL_TYPE nType_,QObject *parent = 0);

    bool GetbIsValid() const;
    void SetBIsValid(bool bIsValid);

private:
    void run() Q_DECL_OVERRIDE;
signals:

public slots:
private:
    QString                 m_LibName;///< 库文件的路径
    QLibrary                mylib;///< 库文件
    CRTDBBase              *m_pRealTimeDB;
    QDomElement             m_ControlElement;
    CJavaScript_ProtocolI  *m_pProtocolI;///< 库中导出的类
    CJavaScript_ProtocolI::JAVASCRIPT_CONTROL_TYPE m_nType;
    bool m_bIsValid;


};

#endif // CJAVASCRIPT_THREAD_H
