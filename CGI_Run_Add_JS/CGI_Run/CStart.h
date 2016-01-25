#ifndef CSTART_H
#define CSTART_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QDebug>
#include "../PortInclude/XmlParser/CXmlParser.h"
#include "Src/crtdbbase.h"
#include "CChannelThread_CGI.h"
#include <QTimerEvent>
#include "CRTDBSharedMemoryThread.h"
#include "CDatabaseThread.h"

class CStart : public QObject
{
    Q_OBJECT
public:
    explicit CStart(QObject *parent = 0);
    void InitMonitorIP(QString strFileName_Informtion_);
    void timerEvent(QTimerEvent *event);
    void FindFile();

    void InitRealTimeDB();
    void ReadFilePoint(QStringList &strFileNameList);
    bool ReadFilePoint(QString strFileName);

    bool InitChannel();

    void Init_JavaScript_Start();
signals:

public slots:

private:
    QStringList m_ComFileList;
    QStringList m_NetFileList;
    QStringList m_CANFileList;
    QStringList m_DBFileList;
    QStringList m_JSFileList;
    int m_nTagID;/// 点的ID
    int m_nDisplayPointMessageTimerID;
    QMap<int,CSharedMemoryTag *> *m_pSharedMemoryTagMap;
    CRTDBSharedMemoryThread *m_pRTDBSharedMemoryThread;
    CRTDBBase *m_pRealTimeDB;
    QList<QThread *> m_ThreadList;
    bool JavaScript_ControlStart_JavaScript(const QDomElement &elemJavaScript_);
};

#endif // CSTART_H
