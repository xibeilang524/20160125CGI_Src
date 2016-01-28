#ifndef CRTDBSHAREDMEMORYTHREAD_H
#define CRTDBSHAREDMEMORYTHREAD_H

#include <QThread>
#include <QLibrary>
#include "../PortInclude/CRTDBSharedMemoryLibI.h"

class CRTDBSharedMemoryThread : public QThread
{
    Q_OBJECT
public:
    explicit CRTDBSharedMemoryThread(QObject *parent = 0);
    void SetpRTDB(QMap<int,CSharedMemoryTag *> *pRTDB_);
    CRTDBSharedMemoryLibI *m_pRTDBSharedMemoryLibI;///< 库中导出的类
    void SetValue(int nID_,QVariant VarProject_);
private:
    void run() Q_DECL_OVERRIDE;
    QString m_LibName;///< 库文件的路径
    QLibrary mylib;///< 库文件
    QMap<int,CSharedMemoryTag *> *m_pRTDB;
};

#endif // CRTDBSHAREDMEMORYTHREAD_H
