#include "CBufferPool.h"
#include <QDebug>

//CBufferPool::CBufferPool(QObject *parent) :
//    QObject(parent)
//{
//}

/*!
 \brief 构造buffer 可以指定大小

 \fn CBufferPool::CBufferPool
 \param nBufferSize_ 指定的大小
 \param parent_ 父对象指针
 \author zzy
 \date   2015/6/10
*/
CBufferPool::CBufferPool(int nBufferSize_, QObject *parent_) :
    QObject(parent_)
{
    m_pBufferHead = NULL;
    m_nInfomationSize = 0;
    m_nBufferSize = 0;
    m_bRefBuffer = false;

    if (nBufferSize_ > 0)
    {
        SetBufferSize(nBufferSize_);
    }
}

/*!
 \brief 构造buffer 同时引用指定内容

 \fn CBufferPool::CBufferPool
 \param pDataSource_ 内容的地址
 \param nDataLength_ 内容的长度
 \param parent_ 父对象指针
 \author zzy
 \date   2015/6/10
*/
CBufferPool::CBufferPool(void *pDataSource_, int nDataLength_, QObject *parent_) :
    QObject(parent_)
{
    //构造buffer 同时 引用 指定内容
    //参数 pDataSource
    //     pDataLength
    Q_ASSERT(pDataSource_ != NULL);
    Q_ASSERT(nDataLength_ >0 );

    m_pBufferHead = NULL;
    m_nInfomationSize = 0;
    m_nBufferSize = 0;
    m_bRefBuffer = false;

    RefBuffer(pDataSource_, nDataLength_);
}

/*!
 \brief 析构函数

 \fn CBufferPool::~CBufferPool
 \author zzy
 \date   2015/6/10
*/
CBufferPool::~CBufferPool()
{
    ClearBuffer();
}

void CBufferPool::SetInfoSize(int nNewSize)
{
    Q_ASSERT(nNewSize >= 0);
    Q_ASSERT(nNewSize <= m_nBufferSize);
    m_nInfomationSize = Min(nNewSize, m_nBufferSize);
}

/*!
 \brief 取得有效信息长度

 \fn CBufferPool::GetInfoSize
 \return int 返回有效信息长度
 \author zzy
 \date   2015/6/10
*/
int CBufferPool::GetInfoSize() const
{
    Q_ASSERT(IsValid());

    return m_nInfomationSize;
}

/*!
 \brief 取得缓冲区长度

 \fn CBufferPool::GetBufferSize
 \return int 返回缓冲区长度
 \author zzy
 \date   2015/6/10
*/
int CBufferPool::GetBufferSize() const
{
    Q_ASSERT(IsValid());

    return m_nBufferSize;
}

/*!
 \brief 取得空缓冲区大小

 \fn CBufferPool::GetEmptySize
 \return int 返回空缓冲区大小
 \author zzy
 \date   2015/6/10
*/
int CBufferPool::GetEmptySize() const
{
    Q_ASSERT(IsValid());

    return m_nBufferSize-m_nInfomationSize;
}

/*!
 \brief 取得缓冲区 偏移量 地址

 \fn CBufferPool::GetBuffer
 \param nStart_ 偏移量
 \return BYTE 返回需要的缓冲区头指针
 \author zzy
 \date   2015/6/10
*/
BYTE *CBufferPool::GetBuffer(int nStart_) const
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nStart_ >= 0);
    Q_ASSERT(nStart_ < m_nInfomationSize);

    return m_pBufferHead + nStart_;
}

/*!
 \brief 取得可用缓冲区地址

 \fn CBufferPool::GetEmptyBuffer
 \return BYTE 返回可用缓冲区的头指针
 \author zzy
 \date   2015/6/10
*/
BYTE *CBufferPool::GetEmptyBuffer()
{
    Q_ASSERT(IsValid());

    return m_pBufferHead + m_nInfomationSize;
}

/*!
 \brief 判断是否处于引用Buffer状态

 \fn CBufferPool::IsRefBuffer
 \return bool 是否处于引用Buffer状态
 \author zzy
 \date   2015/6/10
*/
bool CBufferPool::IsRefBuffer() const
{
    Q_ASSERT(IsValid());

    return m_bRefBuffer;
}

/*!
 \brief 常用运算符，实现 BYTE * 运算，可以进行类型转换

 \fn CBufferPool::operator BYTE *
 \return CBufferPool::operator BYTE 缓冲区头指针
 \author zzy
 \date   2015/6/10
*/
CBufferPool::operator BYTE *()
{
    //功能:
    Q_ASSERT(IsValid());

    return m_pBufferHead;
}

/*!
 \brief 实现[]取字节运算

 \fn CBufferPool::operator []
 \param nOffset_ 要取的字节偏移量
 \return BYTE &CBufferPool::operator 返回要取的字节
 \author zzy
 \date   2015/6/10
*/
BYTE &CBufferPool::operator [](int nOffset_)
{
    //功能:
    Q_ASSERT(IsValid());
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nOffset_ < m_nInfomationSize);

    return m_pBufferHead[nOffset_];
}

/*!
 \brief 设置缓冲区内容

 \fn CBufferPool::SetData
 \param pDataSource_ 源内容地址
 \param nDataLength_ 源内容长度
 \param bAllocNew_ 强制重新分配空间
 \author zzy
 \date   2015/6/10
*/
void CBufferPool::SetData(const void *pDataSource_, int nDataLength_, bool bAllocNew_)
{
    Q_ASSERT(pDataSource_ != NULL);
    Q_ASSERT(nDataLength_ > 0);
//    Q_ASSERT(AfxIsValidAddress(pDataSource_, nDataLength_));
    if (!m_bRefBuffer)
    {
        int nOffset = (BYTE *)pDataSource_-m_pBufferHead;
        Q_ASSERT(nOffset<0 || nOffset>m_nBufferSize);  //内存不重叠
        qDebug()<<nOffset;
    }

    if (nDataLength_>m_nBufferSize || bAllocNew_)//分配新空间
    {
        int nBufferSize = SetBufferSize(nDataLength_);
        Q_ASSERT(nDataLength_ == nBufferSize);

        nDataLength_ = Min(nDataLength_, nBufferSize);//数据太长时只取前面的部分
        memcpy(m_pBufferHead, pDataSource_, nDataLength_);
        m_nInfomationSize = nDataLength_;
    }
    else if (nDataLength_ > 0)//重复利用以前分配的旧空间
    {
        memcpy(m_pBufferHead, pDataSource_, nDataLength_);
        m_nInfomationSize = nDataLength_;
    }
    else //if(nDataLength=<0)
    {
        m_nInfomationSize = 0;
    }
}

/*!
 \brief 在缓冲区后增加数据

 \fn CBufferPool::AppendData
 \param pDataSource_ 源内容地址
 \param nDataLength_ 源内容长度
 \return int 当前缓冲区中信息长度
 \author zzy
 \date   2015/6/10
*/
int CBufferPool::AppendData(const void *pDataSource_, int nDataLength_)
{
    Q_ASSERT(pDataSource_ != NULL);
    Q_ASSERT(nDataLength_ > 0);
//    Q_ASSERT(AfxIsValidAddress(pDataSource_,nDataLength_));

    int nNewSize = nDataLength_+m_nInfomationSize;
    Q_ASSERT(nNewSize <= MAX_BUFFER_SIZE);

    if (nNewSize > m_nBufferSize)//原空间不足,不可以重复利用空间
    {
        ResetBufferSize(nNewSize);
    }
//	if (m_nInfomationSize+nDataLength > m_nBufferSize)
//	{
//		Fifo(m_nInfomationSize+nDataLength-m_nBufferSize);//对超出的部分进行Fifo
//	}
    memcpy(m_pBufferHead+m_nInfomationSize, pDataSource_, nDataLength_);
    m_nInfomationSize = m_nInfomationSize + nDataLength_;
    return m_nInfomationSize;
}

/*!
 \brief 进行Fifo操作

 \fn CBufferPool::Fifo
 \param nSize_ 移出的字节数
 \param nStart_ 移出的开始位置
 \return int 当前缓冲区中信息数
 \author zzy
 \date   2015/6/10
*/
int CBufferPool::Fifo(int nSize_, int nStart_)
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nSize_ >= 0);
    Q_ASSERT(nStart_ >= 0);

    if (nSize_>0 && m_nInfomationSize>0 && nStart_<m_nInfomationSize)
    {
        BYTE *pStart = m_pBufferHead+nStart_;
        int nShitfSize = Min(nSize_, m_nInfomationSize-nStart_);//移出字节数
        int nLeftSize = m_nInfomationSize-nStart_-nShitfSize; //剩余字节数
        if (nLeftSize > 0)
        {
            memmove(pStart, pStart+nShitfSize, nLeftSize);//这里可能有重叠,不可以用memcpy
        }
        m_nInfomationSize = nStart_+nLeftSize;
    }
    return m_nInfomationSize;
}

/*!
 \brief 引用一缓冲区

 \fn CBufferPool::RefBuffer
 \param pData_ 引用的缓冲区地址
 \param nDataLength_ 引用的缓冲区长度
 \author zzy
 \date   2015/6/10
*/
void CBufferPool::RefBuffer(void *pData_, int nDataLength_)
{
    Q_ASSERT(pData_ != NULL);
    Q_ASSERT(nDataLength_ > 0);
//    Q_ASSERT(AfxIsValidAddress(pData_, nDataLength_));
    if (!m_bRefBuffer)
    {
        int nOffset = (BYTE *)pData_-m_pBufferHead;
        Q_ASSERT(nOffset<0 || nOffset>m_nBufferSize);  //内存不重叠
        qDebug()<<nOffset;
    }

    ClearBuffer();
    m_pBufferHead = (BYTE *)pData_;		//设置缓冲区地址
    m_nInfomationSize = nDataLength_;
    m_nBufferSize = nDataLength_;

    m_bRefBuffer = true;
}

/*!
 \brief 删除缓冲区

 \fn CBufferPool::ClearBuffer
*/
void CBufferPool::ClearBuffer()
{
    if (m_pBufferHead!=NULL && !m_bRefBuffer)
    {
        delete []m_pBufferHead;
    }
    m_bRefBuffer = false;
    m_pBufferHead  = NULL;
    m_nInfomationSize = 0;
    m_nBufferSize = 0;
}

/*!
 \brief 设置缓冲区大小
        原信息内容丢失  nNewSize<=0表示清除缓冲区//有效信息的长度为0
 \fn CBufferPool::SetBufferSize
 \param nNewSize_ 缓冲区新长度
 \return int 缓冲区当前长度
 \author zzy
 \date   2015/6/10
*/
int CBufferPool::SetBufferSize(int nNewSize_)
{
    Q_ASSERT(nNewSize_ >= 0);
    Q_ASSERT(nNewSize_ <= MAX_BUFFER_SIZE);

    ClearBuffer();
    if (nNewSize_ > 0)
    {
        BYTE *pBuffer = new BYTE[nNewSize_];
        m_pBufferHead = pBuffer;
        m_nBufferSize = nNewSize_;
    }
    return m_nBufferSize;
}

/*!
 \brief 改变缓冲区大小
        保持原信息内容不变  nNewSize<=0表示清除缓冲区
 \fn CBufferPool::ResetBufferSize
 \param nNewSize_ 缓冲区新长度
 \return int 缓冲区当前长度
 \author zzy
 \date   2015/6/10
*/
int CBufferPool::ResetBufferSize(int nNewSize_)
{
    Q_ASSERT(nNewSize_ >= 0);
    Q_ASSERT(nNewSize_ <= MAX_BUFFER_SIZE);

    if (nNewSize_ > 0)
    {
        BYTE *pBuffer = new BYTE[nNewSize_];
        int nOldInfoSize = Min(m_nInfomationSize, nNewSize_);
        if (nNewSize_ < m_nInfomationSize)//缩小的
        {
            memcpy(pBuffer, m_pBufferHead+m_nInfomationSize-nNewSize_, nNewSize_);//取后部
        }
        else if (m_nInfomationSize > 0)
        {
            memcpy(pBuffer, m_pBufferHead, m_nInfomationSize);//取全部
        }
        ClearBuffer();
        m_pBufferHead = pBuffer;
        m_nInfomationSize = nOldInfoSize;//有效信息的长度不变
        m_nBufferSize = nNewSize_;
    }
    else
    {
        ClearBuffer();
    }
    return m_nBufferSize;
}

/*!
 \brief 改变有效信息长度

 \fn CBufferPool::ResetInfoSize
 \param nNewSize_ 缓冲区新长度
 \return int 缓冲区当前长度
 \author zzy
 \date   2015/6/10
*/
int CBufferPool::ResetInfoSize(int nNewSize_)
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nNewSize_ >= 0);

    if (nNewSize_>=0 && m_nBufferSize>0)
    {
        m_nInfomationSize = Min(nNewSize_, m_nBufferSize);
    }
    return m_nInfomationSize;
}

int CBufferPool::Move(BYTE *pTarget, int nMoveSize)
{
    Q_ASSERT(IsValid());
    Q_ASSERT(pTarget != NULL);
    Q_ASSERT(nMoveSize >= 0);

    int  nRealMove = Min(nMoveSize,m_nInfomationSize);
    if(nRealMove > 0)
    {
        memcpy(pTarget,m_pBufferHead,nRealMove);
        Fifo(nRealMove);
    }
    return nRealMove;
}

int CBufferPool::Copy(void *pTarget, int nCopySize)
{
    Q_ASSERT(IsValid());
    Q_ASSERT(pTarget != NULL);
    Q_ASSERT(nCopySize >= 0);
    int nRealCopy = Min(nCopySize,m_nInfomationSize);//要改正，？？不够长就不要复制了
    if(nRealCopy > 0)
    {
        memcpy(pTarget,m_pBufferHead,nRealCopy);
    }
    return nRealCopy;
}

int CBufferPool::Compare(int nStartByte, void *pTarget, int nCompareSize) const
{
    Q_ASSERT(IsValid());
    AdjustIndex(nStartByte);
    Q_ASSERT(nStartByte >= 0);
    Q_ASSERT(nStartByte < m_nInfomationSize);

    Q_ASSERT(pTarget != NULL);
    Q_ASSERT(nCompareSize >= 0);

    if(nCompareSize > m_nInfomationSize-nStartByte)
        return -2;
    return memcmp(m_pBufferHead+nStartByte,pTarget,nCompareSize);
}

void CBufferPool::AdjustIndex(int &nIndex) const
{
    nIndex =  nIndex>=0?nIndex:m_nInfomationSize+nIndex;
}
/*!
 \brief 测试类的有效性

 \fn CBufferPool::IsValid
 \return bool
*/
bool CBufferPool::IsValid() const
{
    Q_ASSERT(m_pBufferHead != NULL);        //缓冲区头指针
    Q_ASSERT(m_nBufferSize >0);        //缓冲区实际大小
    Q_ASSERT(m_nBufferSize < MAX_BUFFER_SIZE);
    Q_ASSERT(m_nInfomationSize >= 0);//缓冲区中信息长度
    Q_ASSERT(m_nInfomationSize <= m_nBufferSize);
    return true;
}
