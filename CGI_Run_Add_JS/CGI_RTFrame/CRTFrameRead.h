#ifndef CRTFRAMEREAD_H
#define CRTFRAMEREAD_H

#include <QObject>
#include <QSharedMemory>
#include <QLibrary>

class CRTFrameSharedMemoryLibI;
class CRTFrameRead : public QObject
{
    Q_OBJECT
public:
    explicit CRTFrameRead(QObject *parent = 0);
    QString GetFrame(QString strChannelName_);


private:
    CRTFrameSharedMemoryLibI *m_pRTFrameSharedMemoryLibI;///< 库中导出的类
    QString m_LibName;///< 库文件的路径
    QLibrary mylib;///< 库文件
    QSharedMemory shareMem;
};

#endif // CRTFRAMEREAD_H
