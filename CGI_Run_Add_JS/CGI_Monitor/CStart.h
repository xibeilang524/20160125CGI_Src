#ifndef CSTART_H
#define CSTART_H

#include <QObject>
#include <QStringList>
#include "CInitRTDB.h"

class CStart : public QObject
{
    Q_OBJECT
public:
    explicit CStart(QObject *parent = 0);
    void FindFile();
    void InitMonitorIP(QString strFileName_Informtion_);
    const QStringList GetFileList(ChannelType nFileListType_) const;


signals:

public slots:

private:
    CInitRTDB *m_pInitRTDB;
    QStringList m_ComFileList;
    QStringList m_NetFileList;
    QStringList m_CANFileList;
};

#endif // CSTART_H
