#ifndef CINITRTDB_H
#define CINITRTDB_H

#include <QObject>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QDebug>
#include "Src/crtdbbase.h"
#include <QTimerEvent>
#include "../PortInclude/XmlParser/CXmlParser.h"
#include <QDomElement>
#include "EnumClass.h"

class CInitRTDB : public QObject
{
    Q_OBJECT
public:
    explicit CInitRTDB(QStringList *pComFileList_,QStringList *pNetFileList_,QStringList *pCANFileList_,QObject *parent = 0);
    void InitRealTimeDB();
private:
    void ReadFilePoint(QStringList *pStrFileNameList);
    bool ReadFilePoint(QString strFileName);

signals:

public slots:
private:

    QStringList *m_pComFileList;
    QStringList *m_pNetFileList;
    QStringList *m_pCANFileList;
    int m_nTagID;/// 点的ID
};

#endif // CINITRTDB_H
