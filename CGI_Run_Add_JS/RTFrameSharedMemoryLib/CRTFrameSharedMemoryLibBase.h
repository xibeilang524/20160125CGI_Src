#ifndef CRTFRAMESHAREDMEMORYLIBBASE_H
#define CRTFRAMESHAREDMEMORYLIBBASE_H
#include "../PortInclude/CRTFrameSharedMemoryLibI.h"
#include <QSharedMemory>
#include <QObject>
class CRTFrameSharedMemoryLibBase : public CRTFrameSharedMemoryLibI
{
public:
    CRTFrameSharedMemoryLibBase();
    QString GetFrame(QString strChannelName_);
    QString &ReadSharedMemory(QString &strFrame);
    QSharedMemory shareMem;
    //////////////////////////////////////////////////////////////////////////////////////////////////
    /// -------------------------------------------------------------------------------------------///
    /// | m_nNewCount | m_nTotalCount | start | size | start | size |...100Group...| start | size |///
    /// |------------------------------------------------------------------------------------------///
    /// |   2byte     |   2byte       | 4byte | 4byte| 4byte | 4byte|              | 4tbyte| 4byte|///
    /// -------------------------------------------------------------------------------------------///
    //////////////////////////////////////////////////////////////////////////////////////////////////
    short int m_nNewCount;///< 共享内存中的第0、1字节代表的意思是上次写到第XX个段，最大不超过100
    short int m_nTotalCount;///< 共享内存中的第2、3字节代表的意思是共享内存中最后一组的正确的start和size，最大不超过100

};

#endif // CRTFRAMESHAREDMEMORYLIBBASE_H
