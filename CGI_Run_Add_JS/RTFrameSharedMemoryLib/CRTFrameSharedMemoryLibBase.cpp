#include "CRTFrameSharedMemoryLibBase.h"
#include <QDebug>
CRTFrameSharedMemoryLibI * CreateDriver()
{
    CRTFrameSharedMemoryLibBase *pRTFrameSharedMemoryLibI = new CRTFrameSharedMemoryLibBase;
    return pRTFrameSharedMemoryLibI;
}

CRTFrameSharedMemoryLibBase::CRTFrameSharedMemoryLibBase()
{
}

QString CRTFrameSharedMemoryLibBase::GetFrame(QString strChannelName_)
{
    QString strSharedMemoryFileName("__"+strChannelName_+"__");
    shareMem.setKey(strSharedMemoryFileName);
    QString result;
    if (shareMem.attach())
    {
        qDebug()<<"attach yi fu secceed.";
        return ReadSharedMemory(result);
    }else
    {
        qDebug()<<"attach yi fu failed";/// yi fu shi bai

        if (shareMem.isAttached())
        {
            return ReadSharedMemory(result);
        }
        return QString();
    }

    return QString();
}

QString & CRTFrameSharedMemoryLibBase::ReadSharedMemory(QString &strFrame)
{
    shareMem.lock();
    char *to = (char *)shareMem.data();
    m_nNewCount   = *(unsigned short int *)to;
    m_nTotalCount = *(unsigned short int *)(to+sizeof(unsigned short int));
    QByteArray array(to,1024*30);
    memset(to,0,1024*30);
    shareMem.unlock();
    qDebug()<<array.toHex();
    qDebug()<<__func__<<m_nNewCount<<m_nTotalCount;
    for (int i = m_nNewCount; i < 100; ++i)
    {
        int nStartSize = *(int *)(array.data()+(2*i+1)*4);///< 开始字节
        int nSizeCount = *(int *)(array.data()+(2*i+2)*4);///< 字节个数
        if (nStartSize == 0)
            continue;
        if (nSizeCount == 0)
            continue;
        int ntype = *(int *)(array.data()+nStartSize);///< 报文类型
        QByteArray oneFrame((char *)(array.data()+nStartSize+4),nSizeCount);
        qDebug()<<"开始字节="<<nStartSize<<" 字节个数="<<nSizeCount<<" 报文类型="<<ntype<<i;
        qDebug()<<oneFrame;
        strFrame.append(oneFrame);
    }
//    qDebug()<<"----------------------";
//    strFrame.append("-------------------------\n");
    for (int i = 0; i < m_nNewCount; ++i)
    {
        int nStartSize = *(int *)(array.data()+(2*i+1)*4);///< 开始字节
        int nSizeCount = *(int *)(array.data()+(2*i+2)*4);///< 字节个数
        if (nStartSize == 0)
            continue;
        if (nSizeCount == 0)
            continue;
        int ntype = *(int *)(array.data()+nStartSize);///< 报文类型
        QByteArray oneFrame((char *)(array.data()+nStartSize+4),nSizeCount);
        qDebug()<<"开始字节="<<nStartSize<<" 字节个数="<<nSizeCount<<" 报文类型="<<ntype<<i;
        qDebug()<<oneFrame;
        strFrame.append(oneFrame);
    }
    return strFrame;
}
