#include "CFrameSharedMemory.h"
#include <QDebug>

/*!
 \brief 构造函数，构造并初始化共享内存的Key和Size

 \fn CFrameSharedMemory::CFrameSharedMemory
 \param strSharedMemoryKey_ 共享内存的键
 \param nSharedMemorySize_ 共享内存的大小
 \param parent 父对象指针
 \author zzy
 \date   2015/6/9
*/
CFrameSharedMemory::CFrameSharedMemory(QString strSharedMemoryKey_, int nSharedMemorySize_, QObject *parent) :
    QObject(parent)
{
    m_strSharedMemoryKey = strSharedMemoryKey_;
    m_nSharedMemorySize = nSharedMemorySize_;
    m_pSharedMemory = NULL;
    m_pSharedMemoryStart = NULL;
    connect(this,SIGNAL(SignalShowMessage(int,QString)),this,SLOT(SlotShowMessage(int,QString)));


}

/*!
 \brief 创建共享内存，如果共享内存存在则执行依附attach函数，
因为创建的报文共享内存对于各个驱动来说大小一致，所以可以执行依附

 \fn CFrameSharedMemory::CreateSharedMemory
 \return bool 创建成功返回true，则返回false
 \author zzy
 \date   2015/6/9
*/
bool CFrameSharedMemory::CreateSharedMemory()
{
    if (NULL == m_pSharedMemory)
    {
        m_pSharedMemory = new QSharedMemory;
    }
    m_pSharedMemory->setKey(m_strSharedMemoryKey);
    if (m_pSharedMemory->create(m_nSharedMemorySize,QSharedMemory::ReadWrite))
    {
        qDebug()<<"CFrameSharedMemory: Create succeed"<<m_strSharedMemoryKey<<m_nSharedMemorySize;
        return true;
    }else
    {
        qDebug()<<"CFrameSharedMemory: Create failed."<<m_strSharedMemoryKey<<m_nSharedMemorySize;

        if (m_pSharedMemory->attach())
        {
            qDebug()<<"CFrameSharedMemory: attach yi fu secceed."<<m_strSharedMemoryKey<<m_nSharedMemorySize;
            return true;
        }else
        {
            qDebug()<<"CFrameSharedMemory: attach yi fu failed"<<m_strSharedMemoryKey<<m_nSharedMemorySize;/// yi fu shi bai
            return false;
        }
        return false;
    }
}

void CFrameSharedMemory::ShowMessage(int type_, QString strMessage_)
{
    emit SignalShowMessage(type_,strMessage_);
}

/*!
 \brief 向共享内存中写入报文信息

 \fn CFrameSharedMemory::SlotShowMessage
 \param type_ 报文类型
 \param strMessage_ 报文
 \author zzy
 \date   2015/6/9
*/
void CFrameSharedMemory::SlotShowMessage(int type_, QString strMessage_)
{
//    qDebug()<<"-------"<<type_<<strMessage_;
    m_pSharedMemoryStart = (char*)m_pSharedMemory->data();

    QByteArray bFrame = strMessage_.toUtf8();

//    bFrame.append(strMessage_.toUtf8());
//    qDebug()<<"bFrame"<<bFrame;
#if 1
    // Write into the shared memory
    m_pSharedMemory->lock();
    m_nNewCount   = *(unsigned short int *)m_pSharedMemoryStart;
    m_nTotalCount = *(unsigned short int *)(m_pSharedMemoryStart+sizeof(unsigned short int));
    if ( (m_nNewCount>=0) && (m_nNewCount<=100) )
    {
        if (m_nNewCount == 0)///< 第一次写
        {
            FirstWriteFrame(type_,strMessage_);
            m_pSharedMemory->unlock();
            return;
        }else ///< 不是第一次写
        {
            m_nStart = *(int *)(m_pSharedMemoryStart+(m_nNewCount)*2*sizeof(int)-4);
            m_nSize  = *(int *)(m_pSharedMemoryStart+(m_nNewCount)*2*sizeof(int));
            if (m_nStart < FRAMESTART)
            {
                m_nStart = FRAMESTART;///< FRAMESTART为804; 经过计算得出应该为从第804个字节开始写报文
            }
            m_nStart = m_nStart+m_nSize;///< 下一个报文的起始
            m_nSize  = bFrame.count()+4;///< 下一个报文的长度，报文类型占4字节，再加上实际的报文长度
            if ( (m_nStart+m_nSize) > m_nSharedMemorySize)
            {
                m_nNewCount = 0;
                FirstWriteFrame(type_,strMessage_);
                m_pSharedMemory->unlock();
                return;
            }else
            {
                *(unsigned short int *)m_pSharedMemoryStart = ++m_nNewCount;///< 设置开始两个字节                
                if (m_nTotalCount>=100)
                    m_nTotalCount = 99;
                *(unsigned short int *)(m_pSharedMemoryStart+2) = ++m_nTotalCount;///< 设置第2、3个字节
                *(int *)(m_pSharedMemoryStart+(m_nNewCount)*2*sizeof(int)-4) = m_nStart;
                *(int *)(m_pSharedMemoryStart+(m_nNewCount)*2*sizeof(int)) = m_nSize;
                *(int *)(m_pSharedMemoryStart+m_nStart) = type_;///< 设置报文类型
                memcpy(m_pSharedMemoryStart+m_nStart+4,bFrame.data(),bFrame.count());///< 设置报文部分
//                qDebug()<<m_nNewCount<<m_nTotalCount<<m_nStart<<m_nSize<<"baowen xie ru le ";
                m_pSharedMemory->unlock();
                return;
            }
        }
    }else if (m_nNewCount > 100)
    {
        m_nNewCount = 0;
        FirstWriteFrame(type_,strMessage_);
        m_pSharedMemory->unlock();
        return;
    }else
    {
        memset(m_pSharedMemoryStart,0,FRAMESTART);///< 开始的804个字节全部初始化
        m_pSharedMemory->unlock();
        return;
    }
#endif
}

/*!
 \brief 从头开始写数据，即写第一条数据，初始化时调用此函数，当写满时从头开始写，当写够100条时从头开始写

 \fn CFrameSharedMemory::FirstWriteFrame
 \param type_ 报文类型
 \param strMessage_ 报文
 \author zzy
 \date   2015/6/9
*/
void CFrameSharedMemory::FirstWriteFrame(int type_, QString strMessage_)
{
    QByteArray bFrame = strMessage_.toUtf8();
    m_nStart = FRAMESTART;///< FRAMESTART为804; 经过计算得出应该为从第804个字节开始写报文
    m_nSize  = bFrame.count()+4;///< 报文类型占4字节，实际报文长度

    if ( (m_nStart+m_nSize) > m_nSharedMemorySize)
    {
        return;
    }else
    {
        *(unsigned short int *)m_pSharedMemoryStart = ++m_nNewCount;///< 设置开始两个字节
        if (m_nTotalCount>=100)
            m_nTotalCount = 99;
        *(unsigned short int *)(m_pSharedMemoryStart+2) = ++m_nTotalCount;///< 设置第2、3个字节
        *(int *)(m_pSharedMemoryStart+(m_nNewCount)*2*sizeof(int)-4) = m_nStart;
        *(int *)(m_pSharedMemoryStart+(m_nNewCount)*2*sizeof(int)) = m_nSize;
        *(int *)(m_pSharedMemoryStart+m_nStart) = type_;///< 设置报文类型
        memcpy(m_pSharedMemoryStart+m_nStart+4,bFrame.data(),bFrame.count());///< 设置报文部分
        qDebug()<<m_nNewCount<<m_nTotalCount<<m_nStart<<m_nSize<<"SharedMemory Init succeed. baowen xie ru le ";
    }
}
