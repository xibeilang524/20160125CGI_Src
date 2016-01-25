#ifndef CFRAMESHAREMEMORY_H
#define CFRAMESHAREMEMORY_H

#include <QObject>
#include <QSharedMemory>
#define FRAMESTART 804  ///< 经过计算得出应该为从第804个字节开始写报文
class CFrameSharedMemory : public QObject
{
    Q_OBJECT
public:
    explicit CFrameSharedMemory(QString strSharedMemoryKey_,int nSharedMemorySize_,QObject *parent = 0);
    bool CreateSharedMemory();
    void ShowMessage(int type_, QString strMessage_);


signals:
    void SignalShowMessage(int type_, QString strMessage_);

private slots:
    void SlotShowMessage(int type_, QString strMessage_);
private:
    void FirstWriteFrame(int type_, QString strMessage_);
private:
    QString m_strSharedMemoryKey;
    int m_nSharedMemorySize;
    QSharedMemory *m_pSharedMemory;
    //////////////////////////////////////////////////////////////////////////////////////////////////
    /// -------------------------------------------------------------------------------------------///
    /// | m_nNewCount | m_nTotalCount | start | size | start | size |...100Group...| start | size |///
    /// |------------------------------------------------------------------------------------------///
    /// |   2byte     |   2byte       | 4byte | 4byte| 4byte | 4byte|              | 4tbyte| 4byte|///
    /// -------------------------------------------------------------------------------------------///
    //////////////////////////////////////////////////////////////////////////////////////////////////
    short int m_nNewCount;///< 共享内存中的第0、1字节代表的意思是上次写到第XX个段，最大不超过100
    short int m_nTotalCount;///< 共享内存中的第2、3字节代表的意思是共享内存中最后一组的正确的start和size，最大不超过100
    int m_nStart;///< 记录start
    int m_nSize;///< 记录size
    char *m_pSharedMemoryStart;
};

#endif // CFRAMESHAREMEMORY_H
