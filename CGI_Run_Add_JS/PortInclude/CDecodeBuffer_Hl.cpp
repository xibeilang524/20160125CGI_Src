#include "CDecodeBuffer_Hl.h"


CDecodeBuffer_Hl::CDecodeBuffer_Hl()
{

}

CDecodeBuffer_Hl::~CDecodeBuffer_Hl()
{

}

/*!
 \brief 分组交换pInfo中内容  即0x12345678 ->0x5678 1234 高低字交换(nInfoSize =4,GroupSize=2)
        用于字符串到16进制时的变换
 \fn CDecodeBuffer_Hl::ReverseWord
 \param nOffset_ 开始位置
 \param nInfoSize_ 字节数
 \param nGroupSize_ 分组大小(组内成员个数）
 \author zzy
 \date   2015/6/10
*/
void CDecodeBuffer_Hl::ReverseWord(int nOffset_, int nInfoSize_, int nGroupSize_)
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nInfoSize_ >0);
    Q_ASSERT(nGroupSize_ > 0);
    Q_ASSERT(nOffset_+nInfoSize_ <= m_nInfomationSize);
    Q_ASSERT(nInfoSize_ >= nGroupSize_);
    Q_ASSERT(nInfoSize_ == (nInfoSize_/nGroupSize_)*nGroupSize_);//信息长度是分组长度的整数倍
    Q_ASSERT(nGroupSize_ > 1);//组内应有两个字以上

    WORD *pCur = (WORD *)(m_pBufferHead + nOffset_);
    WORD *pEnd = (WORD *)(m_pBufferHead + nOffset_ + nInfoSize_);

    for(;pCur<pEnd;pCur+=nGroupSize_)
    {
        for(int i=0;i<nGroupSize_/2;i++)
        {
            WORD nTemp=pCur[i];
            pCur[i]=pCur[nGroupSize_-i-1];
            pCur[nGroupSize_-i-1]=nTemp;
        }
    }
}

/*!
 \brief 分组交换pInfo中内容  即0x12345678 ->0x3412 7856   高低字节交换(nInfoSize =4,GroupSize=2)
        用于modbus 等高前低后的数据处理
 \fn CDecodeBuffer_Hl::ReverseByte
 \param nOffset_ 开始位置
 \param nInfoSize_ 字节数
 \param nGroupSize_ 分组大小(组内成员个数）
 \author zzy
 \date   2015/6/10
*/
void CDecodeBuffer_Hl::ReverseByte(int nOffset_, int nInfoSize_, int nGroupSize_)
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nInfoSize_ >0);
    Q_ASSERT(nGroupSize_ > 0);
    Q_ASSERT(nOffset_+nInfoSize_ <= m_nInfomationSize);

    Q_ASSERT(nInfoSize_ >= nGroupSize_);
    Q_ASSERT(nInfoSize_ == (nInfoSize_/nGroupSize_)*nGroupSize_);//信息长度是分组长度的整数倍
    Q_ASSERT(nGroupSize_ > 1);//组内应有两个字节以上

    BYTE *pCur = m_pBufferHead + nOffset_;
    BYTE *pEnd = pCur + nInfoSize_;
    for(; pCur<pEnd; pCur+=nGroupSize_)
    {
        for(int i=0; i<nGroupSize_/2; i++)
        {
            BYTE nTemp = pCur[i];
            pCur[i] = pCur[nGroupSize_-i-1];
            pCur[nGroupSize_-i-1] = nTemp;
        }
    }
}

/*!
 \brief 取出缓冲区中的一个无符号整数

 \fn CDecodeBuffer_Hl::GetUIntValue_Hl
 \param nOffset_ 开始位置
 \param nSize_ 字节数
 \return unsigned int 如可在此位置返回正常值 否则为INVALID_VALUE
 \author zzy
 \date   2015/6/10
*/
unsigned int CDecodeBuffer_Hl::GetUIntValue_Hl(int nOffset_, int nSize_) const
{
    //注意:高字节在前
    Q_ASSERT(IsValid());
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nSize_ > 0);
    Q_ASSERT(nOffset_+nSize_ <= m_nInfomationSize);

    MYVALUE nTemp;
    unsigned int nReturnValue = INVALID_VALUE;
    if (nOffset_>=0 && nOffset_+nSize_<=m_nInfomationSize)
    {
        switch (nSize_)
        {
        case 1:
            nTemp.m_Bytes[0] = m_pBufferHead[nOffset_+0];
            nReturnValue = nTemp.m_nByte;
            //	nReturnValue = *(unsigned  char *)(m_pBufferHead + nOffset);
            break;
        case 2:
            nTemp.m_Bytes[0] = m_pBufferHead[nOffset_+1];
            nTemp.m_Bytes[1] = m_pBufferHead[nOffset_+0];
            nReturnValue = nTemp.m_nWord;;
            //	nReturnValue = *(unsigned short int *)(m_pBufferHead + nOffset);
        //	CMyFunction::ReverseByte(&nReturnValue, 2, 2);
            break;
        case 3://不常用
            nTemp.m_Bytes[0] = m_pBufferHead[nOffset_+2];
            nTemp.m_Bytes[1] = m_pBufferHead[nOffset_+1];
            nTemp.m_Bytes[2] = m_pBufferHead[nOffset_+0];
            nReturnValue = nTemp.m_nUint24;

            //nReturnValue = *(unsigned int *)(m_pBufferHead + nOffset);
            //ReverseByte(&nReturnValue, 4, 4);
            ///nReturnValue = (m_pBufferHead[nOffset]*256 + m_pBufferHead[nOffset+1])*256 +m_pBufferHead[nOffset+2];
            break;
        case 4:
            nTemp.m_Bytes[0] = m_pBufferHead[nOffset_+3];
            nTemp.m_Bytes[1] = m_pBufferHead[nOffset_+2];
            nTemp.m_Bytes[2] = m_pBufferHead[nOffset_+1];
            nTemp.m_Bytes[3] = m_pBufferHead[nOffset_+0];
            nReturnValue = nTemp.m_nUint;

            //nReturnValue = *(unsigned int *)(m_pBufferHead + nOffset);
            //CMyFunction::ReverseByte(&nReturnValue, 4, 4);
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
    return nReturnValue;
}

/*!
 \brief 取出缓冲区中的一个有符号整数

 \fn CDecodeBuffer_Hl::GetIntValue_Hl
 \param nOffset_ 开始位置
 \param nSize_ 字节数
 \return int 如可在此位置返回正常值 否则为INVALID_VALUE
 \author zzy
 \date   2015/6/10
*/
int CDecodeBuffer_Hl::GetIntValue_Hl(int nOffset_, int nSize_) const
{
    Q_ASSERT(IsValid());
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nSize_ > 0);
    Q_ASSERT(nOffset_+nSize_ <= m_nInfomationSize);

    MYVALUE nTemp;
    int nReturnValue = INVALID_VALUE;
    if (nOffset_>=0 && nOffset_+nSize_<=m_nInfomationSize)
    {
        switch (nSize_)
        {
        case 1:
            nTemp.m_Bytes[0] = m_pBufferHead[nOffset_+0];
            nReturnValue = nTemp.m_nChar;

            //nReturnValue = *(signed char *)(m_pBufferHead + nOffset);
            break;
        case 2:
            nTemp.m_Bytes[0] = m_pBufferHead[nOffset_+1];
            nTemp.m_Bytes[1] = m_pBufferHead[nOffset_+0];
            nReturnValue = nTemp.m_nShortInt;

        /*  错误：如value = 32700
            nReturnValue = *(short int *)(m_pBufferHead + nOffset);
            CMyFunction::ReverseByte(&nReturnValue, 2, 2);
            */
            break;
        case 3://不常用
            nTemp.m_Bytes[0] = m_pBufferHead[nOffset_+2];
            nTemp.m_Bytes[1] = m_pBufferHead[nOffset_+1];
            nTemp.m_Bytes[2] = m_pBufferHead[nOffset_+0];
            nReturnValue = nTemp.m_nInt24;
            break;
        case 4:
            nTemp.m_Bytes[0] = m_pBufferHead[nOffset_+3];
            nTemp.m_Bytes[1] = m_pBufferHead[nOffset_+2];
            nTemp.m_Bytes[2] = m_pBufferHead[nOffset_+1];
            nTemp.m_Bytes[3] = m_pBufferHead[nOffset_+0];
            nReturnValue = nTemp.m_nInt;

        //	nReturnValue = *(int *)(m_pBufferHead + nOffset);
        //	CMyFunction::ReverseByte(&nReturnValue, 4, 4);
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
    return nReturnValue;
}

/*!
 \brief 取出缓冲区中的一个BCD码 不能是有符号数

 \fn CDecodeBuffer_Hl::GetBcdValue_Hl
 \param nOffset_ 开始位置
 \param nByteSize_ 字节数
 \return DWORD 如可在此位置返回正常值 否则为INVALID_VALUE
 \author zzy
 \date   2015/6/10
*/
DWORD CDecodeBuffer_Hl::GetBcdValue_Hl(int nOffset_, int nByteSize_) const
{
    //注意:高字节在前
    Q_ASSERT(IsValid());
    Q_ASSERT(nByteSize_ >= 0);
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nOffset_+nByteSize_ <= m_nInfomationSize);

    DWORD nReturnValue = INVALID_VALUE;
    if (nOffset_>=0 && nOffset_+nByteSize_<=m_nInfomationSize)
    {
        nReturnValue = CMyFunction::BCD2Int_HL(m_pBufferHead+nOffset_, nByteSize_);
    }

    return nReturnValue;
}

/*!
 \brief 取出缓冲区中的一个浮点数

 \fn CDecodeBuffer_Hl::GetFloatValue_Hl
 \param nOffset_ 开始位置
 \param nSize_ 字节数
 \return double 如可在此位置返回正常值 否则为INVALID_VALUE
 \author zzy
 \date   2015/6/10
*/
double CDecodeBuffer_Hl::GetFloatValue_Hl(int nOffset_, int nSize_) const
{
    //注意:高字节在前
    Q_ASSERT(IsValid());
    Q_ASSERT(nOffset_ >= 0);
    Q_ASSERT(nOffset_+nSize_ <= m_nInfomationSize);

    double nReturnValue = INVALID_VALUE;
    if (nOffset_>=0 && nOffset_+nSize_<=m_nInfomationSize)
    {
        switch (nSize_)
        {
        case 4:
            {
                float fValue = *(float *)(m_pBufferHead + nOffset_);
                CMyFunction::ReverseByte(&fValue, 4, 4);
                nReturnValue = fValue;
            }
            //nReturnValue=*(float *)(m_pBufferHead+nOffset); 不可以,因为浮点数并不是低位的
            //ReverseByte(&nReturnValue,4,4);
            break;
        case 8:
            nReturnValue = *(double *)(m_pBufferHead + nOffset_);
            CMyFunction::ReverseByte(&nReturnValue, 8, 8);
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
    return nReturnValue;
}
