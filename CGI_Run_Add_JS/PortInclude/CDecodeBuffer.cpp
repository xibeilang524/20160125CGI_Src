#include "CDecodeBuffer.h"
#include <QDebug>


CDecodeBuffer::CDecodeBuffer()
{

}

CDecodeBuffer::~CDecodeBuffer()
{

}

/*!
 \brief 取缓冲区中的一个位

 \fn CDecodeBuffer::GetBit
 \param nByteStart_ 开始字节
 \param nBitOffset_ 位偏移
 \return BYTE 返回获取的位
 \author zzy
 \date   2015/6/10
*/
BYTE CDecodeBuffer::GetBit(int nByteStart_, int nBitOffset_) const
{
    //
    // nByteStart
    Q_ASSERT(IsValid());
    Q_ASSERT(nByteStart_ >= 0);
    Q_ASSERT(nBitOffset_ >= 0);

    //调整字节偏移　及　位偏移
    nBitOffset_ = nByteStart_*8+nBitOffset_;
    nByteStart_ = nBitOffset_/8;
    nBitOffset_ %= 8;

    Q_ASSERT(nByteStart_ < m_nInfomationSize);

    BYTE nReturn=0;
    if (nByteStart_ < m_nInfomationSize)
    {
        nReturn = GET_BIT(m_pBufferHead[nByteStart_],nBitOffset_);
    }
    return nReturn;
}

/*!
 \brief 9--位段 无符号数

 \fn CDecodeBuffer::GetMidBitValue_u
 \param nOffset_
 \param nBitStart_
 \param nBitNumber_
 \return unsigned int
 \author zzy
 \date   2015/6/10
*/
unsigned int CDecodeBuffer::GetMidBitValue_u(int nOffset_, int nBitStart_, int nBitNumber_) const
{

    Q_ASSERT(IsValid());
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nBitStart_ >= 0);
    Q_ASSERT(nBitNumber_ >= 0);
    Q_ASSERT(nOffset_+(nBitStart_+nBitNumber_+7)/8 <= m_nInfomationSize);

    int nReturnValue = INVALID_VALUE;
    if (nOffset_+(nBitStart_+nBitNumber_+7)/8 <= m_nInfomationSize)
    {
        nReturnValue = CMyFunction::GetMidBitValue(*(unsigned int *)(m_pBufferHead+nOffset_),nBitStart_,nBitNumber_,false);
    }
    return nReturnValue;
}

/*!
 \brief 8--位段 有符号数 最高位为符号位

 \fn CDecodeBuffer::GetMidBitValue_s
 \param nOffset_
 \param nBitStart_
 \param nBitNumber_
 \return int
*/
int CDecodeBuffer::GetMidBitValue_s(int nOffset_, int nBitStart_, int nBitNumber_) const
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nBitStart_ >= 0);
    Q_ASSERT(nBitNumber_ >= 0);
    Q_ASSERT(nOffset_+(nBitStart_+nBitNumber_+7)/8 <= m_nInfomationSize);

    int nReturnValue = INVALID_VALUE;
    if (nOffset_+(nBitStart_+nBitNumber_+7)/8 <= m_nInfomationSize)
    {
        nReturnValue = CMyFunction::GetMidBitValue(*(unsigned int *)(m_pBufferHead+nOffset_),nBitStart_,nBitNumber_,true);
    }
    return nReturnValue;
}

/*!
 \brief 5--ASCII码 实数

 \fn CDecodeBuffer::GetAsciiFloatValue
 \param nOffset_
 \param nByteSize_
 \return float
 \author zzy
 \date   2015/6/10
*/
float CDecodeBuffer::GetAsciiFloatValue(int nOffset_, int nByteSize_) const
{
    //
    Q_ASSERT(IsValid());
    Q_ASSERT(nByteSize_ >= 0);
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nOffset_+nByteSize_ <= m_nInfomationSize);

    float nReturnValue = INVALID_VALUE;
    if (nOffset_>=0 && nOffset_+nByteSize_<=m_nInfomationSize)
    {
        char szFormat[20] = {0};
        sprintf(szFormat, "%%%df", nByteSize_);
        int nResult = sscanf((char *)(m_pBufferHead+nOffset_), szFormat, &nReturnValue);
        Q_ASSERT(nResult == 1);
        qDebug()<<nResult;
    }
    return nReturnValue;
}

/*!
 \brief 6--ASCII码 整数

 \fn CDecodeBuffer::GetAsciiDecValue
 \param nOffset_
 \param nByteSize_
 \return int
 \author zzy
 \date   2015/6/10
*/
int CDecodeBuffer::GetAsciiDecValue(int nOffset_, int nByteSize_) const
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nByteSize_ >= 0);
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nOffset_+nByteSize_ <= m_nInfomationSize);

    int nReturnValue = INVALID_VALUE;
    if (nOffset_>=0 && nOffset_+nByteSize_<=m_nInfomationSize)
    {
        char szFormat[20] = {0};
        sprintf(szFormat, "%%%di", nByteSize_);//支持以0x开头的16进制格式
        int nResult = sscanf((char *)(m_pBufferHead+nOffset_),szFormat,&nReturnValue);
        Q_ASSERT(nResult == 1);
        qDebug()<<nResult;
    }
    return nReturnValue;
}

/*!
 \brief ASCII码表示的16进制数据

 \fn CDecodeBuffer::GetAsciiHexValue
 \param nOffset_
 \param nByteSize_
 \return DWORD
*/
DWORD CDecodeBuffer::GetAsciiHexValue(int nOffset_, int nByteSize_) const
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nByteSize_ >= 0);
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nOffset_+nByteSize_ <= m_nInfomationSize);

    DWORD nReturnValue = INVALID_VALUE;
    if (nOffset_>=0 && nOffset_+nByteSize_<=m_nInfomationSize)
    {
        char szFormat[20] = {0};
        sprintf(szFormat,"%%%dx", nByteSize_);//支持以0x开头的16进制格式
        int nResult = sscanf((char *)(m_pBufferHead+nOffset_), szFormat, &nReturnValue);
        Q_ASSERT(nResult == 1);
        qDebug()<<nResult;
    }
    return nReturnValue;
}

/*!
 \brief 4--BCD码 不能是有符号数

 \fn CDecodeBuffer::GetBcdValue
 \param nOffset_
 \param nByteSize_
 \return DWORD
*/
DWORD CDecodeBuffer::GetBcdValue(int nOffset_, int nByteSize_) const
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nByteSize_ >= 0);
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nOffset_+nByteSize_ <= m_nInfomationSize);

    DWORD nReturnValue = INVALID_VALUE;
    if (nOffset_>=0 && nOffset_+nByteSize_<=m_nInfomationSize)
    {
        nReturnValue = CMyFunction::BCD2Int(m_pBufferHead+nOffset_,nByteSize_);
    }
    return nReturnValue;
}

/*!
 \brief 取出缓冲区中的一个浮点数

 \fn CDecodeBuffer::GetFloatValue
 \param nOffset_ 开始位置
 \param nSize_ 字节数
 \return double 如可在此位置返回正常值 否则为INVALID_VALUE
*/
double CDecodeBuffer::GetFloatValue(int nOffset_, int nSize_) const
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nOffset_+nSize_ <= m_nInfomationSize);

    double nReturnValue = INVALID_VALUE;
    if (nOffset_>=0 && nOffset_+nSize_<=m_nInfomationSize)
    {
        switch(nSize_)
        {
        case 4:
            nReturnValue = *(float *)(m_pBufferHead + nOffset_);
            break;
        case 8:
            nReturnValue = *(double *)(m_pBufferHead + nOffset_);
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
    return nReturnValue;
}

#if defined(Q_PROCESSOR_ARM) ///< 嵌入式系统取值方法
/*!
 \brief 取出缓冲区中的一个无符号整数

 \fn CDecodeBuffer::GetUIntValue
 \param nOffset_ 开始位置
 \param nSize_ 字节数
 \return unsigned int 如可在此位置返回正常值 否则为INVALID_VALUE
*/
unsigned int CDecodeBuffer::GetUIntValue(int nOffset_, int nSize_) const
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nOffset_+nSize_ <= m_nInfomationSize);

    unsigned int nReturnValue = INVALID_VALUE;
    if (nOffset_>=0 && nOffset_+nSize_<=m_nInfomationSize)
    {
        switch (nSize_)
        {
        case 1:
            nReturnValue = *(unsigned  char *)(m_pBufferHead + nOffset_);
            break;
        case 2:
            memcpy(&nReturnValue,m_pBufferHead + nOffset_,2);
//            nReturnValue = *(unsigned short int *)(m_pBufferHead + nOffset_);
            break;
        case 4:
            memcpy(&nReturnValue,m_pBufferHead + nOffset_,4);
//            nReturnValue = *(unsigned int *)(m_pBufferHead + nOffset_);
            break;
        case 3://不常见
            memcpy(&nReturnValue,m_pBufferHead + nOffset_,3);
//            nReturnValue = *(unsigned int *)(m_pBufferHead + nOffset_) & 0xFFFFFF;
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
    return nReturnValue;
}
#else ///< 非嵌入式系统取值方法
/*!
 \brief 取出缓冲区中的一个无符号整数

 \fn CDecodeBuffer::GetUIntValue
 \param nOffset_ 开始位置
 \param nSize_ 字节数
 \return unsigned int 如可在此位置返回正常值 否则为INVALID_VALUE
*/
unsigned int CDecodeBuffer::GetUIntValue(int nOffset_, int nSize_) const
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nOffset_+nSize_ <= m_nInfomationSize);

    unsigned int nReturnValue = INVALID_VALUE;
    if (nOffset_>=0 && nOffset_+nSize_<=m_nInfomationSize)
    {
        switch (nSize_)
        {
        case 1:
            nReturnValue = *(unsigned  char *)(m_pBufferHead + nOffset_);
            break;
        case 2:
            nReturnValue = *(unsigned short int *)(m_pBufferHead + nOffset_);
            break;
        case 4:
            nReturnValue = *(unsigned int *)(m_pBufferHead + nOffset_);
            break;
        case 3://不常见
            nReturnValue = *(unsigned int *)(m_pBufferHead + nOffset_) & 0xFFFFFF;
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
    return nReturnValue;
}
#endif

#if defined(Q_PROCESSOR_ARM) ///< 嵌入式系统取值方法
/*!
 \brief 取出缓冲区中的一个有符号整数

 \fn CDecodeBuffer::GetIntValue
 \param nOffset_ 开始位置
 \param nSize_ 字节数
 \return int 如可在此位置返回正常值 否则为INVALID_VALUE
*/
int CDecodeBuffer::GetIntValue(int nOffset_, int nSize_) const
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nOffset_+nSize_ <= m_nInfomationSize);

    int nReturnValue = INVALID_VALUE;
    if (nOffset_>=0 && nOffset_+nSize_<=m_nInfomationSize)
    {
        switch (nSize_)
        {
        case 1:
            nReturnValue = *(signed char *)(m_pBufferHead + nOffset_);
            break;
        case 2:
            memcpy(&nReturnValue,m_pBufferHead + nOffset_,2);
//            nReturnValue = *(short int *)(m_pBufferHead + nOffset_);
            break;
        case 4:
            memcpy(&nReturnValue,m_pBufferHead + nOffset_,4);
//            nReturnValue = *(int *)(m_pBufferHead + nOffset_);
            break;
        case 3://不常见
            {//使用结构实现
//                struct THREE_BYTE
//                {
//                    int VALUE:24;
//                    int RES:8;
//                }*pValue;
//                pValue = (THREE_BYTE *)(m_pBufferHead + nOffset_);
//                nReturnValue = pValue->VALUE;
            }
            memcpy(&nReturnValue,m_pBufferHead + nOffset_,3);
            {//另一种方式
        //		int *pValue = (int *)(m_pBufferHead + nOffset);
        //		if(*pValue & 0x800000)
        //		{
        //			nReturnValue = (*pValue * -1) & 0x7fffff;
        //		}
        ///		else
        //		{
        //			nReturnValue =(*pValue) & 0x7fffff;
        //		}
            }
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
    return nReturnValue;
}
#else ///< 非嵌入式系统取值方法
/*!
 \brief 取出缓冲区中的一个有符号整数

 \fn CDecodeBuffer::GetIntValue
 \param nOffset_ 开始位置
 \param nSize_ 字节数
 \return int 如可在此位置返回正常值 否则为INVALID_VALUE
*/
int CDecodeBuffer::GetIntValue(int nOffset_, int nSize_) const
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nOffset_+nSize_ <= m_nInfomationSize);

    int nReturnValue = INVALID_VALUE;
    if (nOffset_>=0 && nOffset_+nSize_<=m_nInfomationSize)
    {
        switch (nSize_)
        {
        case 1:
            nReturnValue = *(signed char *)(m_pBufferHead + nOffset_);
            break;
        case 2:
            nReturnValue = *(short int *)(m_pBufferHead + nOffset_);
            break;
        case 4:
            nReturnValue = *(int *)(m_pBufferHead + nOffset_);
            break;
        case 3://不常见
            {//使用结构实现
                struct THREE_BYTE
                {
                    int VALUE:24;
                    int RES:8;
                }*pValue;
                pValue = (THREE_BYTE *)(m_pBufferHead + nOffset_);
                nReturnValue = pValue->VALUE;
            }
            {//另一种方式
        //		int *pValue = (int *)(m_pBufferHead + nOffset);
        //		if(*pValue & 0x800000)
        //		{
        //			nReturnValue = (*pValue * -1) & 0x7fffff;
        //		}
        ///		else
        //		{
        //			nReturnValue =(*pValue) & 0x7fffff;
        //		}
            }
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
    return nReturnValue;
}
#endif

/*!
 \brief 设备缓冲区中的一个位

 \fn CDecodeBuffer::SetBit
 \param nByteStart_ 开始字节
 \param nBitOffset_ 位偏移
 \param nValue_ 设置的值
 \author zzy
 \date   2015/6/10
*/
void CDecodeBuffer::SetBit(int nByteStart_, int nBitOffset_, BYTE nValue_)
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nByteStart_ >= 0);
    Q_ASSERT(nBitOffset_ >= 0);

    //调整字节偏移　及　位偏移
    nBitOffset_ = nByteStart_*8+nBitOffset_;
    nByteStart_ = nBitOffset_/8;
    nBitOffset_ %= 8;

    Q_ASSERT(nByteStart_ < m_nInfomationSize);

    if (nByteStart_ < m_nInfomationSize)
    {
        if (nValue_ > 0)
        {
            m_pBufferHead[nByteStart_] = SET_BIT(m_pBufferHead[nByteStart_], nBitOffset_);
        }
        else
        {
            m_pBufferHead[nByteStart_] = RESET_BIT(m_pBufferHead[nByteStart_], nBitOffset_);
        }
    }
}

/*!
 \brief 修改缓冲区内容

 \fn CDecodeBuffer::SetValue
 \param nOffset_ 开始位置
 \param pInfo_ 内容地址
 \param nInfoSize_ 字节数
 \author zzy
 \date   2015/6/10
*/
void CDecodeBuffer::SetValue(int nOffset_, const void *pInfo_, int nInfoSize_)
{
    //用法
    //   int nValue=321; CDecodeBuffer frame;
    //   frame.SetValue(0,&nValue,sizeof(nValue));
    Q_ASSERT(IsValid());
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nOffset_+nInfoSize_ <= m_nInfomationSize);

    if (nOffset_>=0 && nOffset_+nInfoSize_<=m_nInfomationSize)
    {
        memcpy(m_pBufferHead+nOffset_,pInfo_,nInfoSize_);
    }
}
