#include "CMyFunction.h"



/*!
 \brief 把BCD码表示的数转换成整数

 \fn CMyFunction::BCD2Int
 \param pInfo_ 内存地址(紧凑形式)
 \param nSize_ 字节个数
 \return unsigned int 返回实际结果
 \author zzy
 \date   2015/6/10
*/
unsigned int CMyFunction::BCD2Int(const void *pInfo_, size_t nSize_)
{
    Q_ASSERT(pInfo_ != NULL);
    Q_ASSERT(nSize_ < 6);
    Q_ASSERT(nSize_ > 0);//整数最大值为10位数

    unsigned int nTotal = 0;
    unsigned int nBase = 1;
    BYTE *pData = (BYTE *)pInfo_;
    BCD_BYTE byte;
    //for(size_t i=0;i<nSize;i++)
    while (nSize_--)
    {
        byte.m_nByte = *pData++;
    //	nTotal=nTotal*100+byte.m_nHigh*10+byte.m_nLow;  //错误:高低字节不正确
    //	nTotal=nTotal+(byte.m_nHigh*10+byte.m_nLow)*pow(10,i*2);//使用pow不好
        nTotal = nTotal + (byte.HIGH * 10+byte.LOW) * nBase;
        nBase *= 100;
    }
    return nTotal;
}

/*!
 \brief 把BCD码表示的数转换成整数 高位在前

 \fn CMyFunction::BCD2Int_HL
 \param pInfo_ 内存地址(紧凑形式)
 \param nSize_ 字节个数
 \return unsigned int 返回实际结果
 \author zzy
 \date   2015/6/10
*/
unsigned int CMyFunction::BCD2Int_HL(const void *pInfo_, size_t nSize_)
{
    Q_ASSERT(pInfo_ != NULL);
    Q_ASSERT(nSize_ < 6);
    Q_ASSERT(nSize_ > 0);//整数最大值为10位数

    unsigned int nTotal = 0;
    unsigned int nBase = 1;
    BYTE *pData = (BYTE *)pInfo_ + nSize_ - 1;//取最后一字节
    BCD_BYTE byte;
    //for(size_t i=0;i<nSize;i++)
    while (nSize_--)
    {
        byte.m_nByte = *pData--;
    //	nTotal=nTotal*100+byte.m_nHigh*10+byte.m_nLow;  //错误:高低字节不正确
    //	nTotal=nTotal+(byte.m_nHigh*10+byte.m_nLow)*pow(10,i*2);//使用pow不好
        nTotal = nTotal + (byte.HIGH * 10+byte.LOW) * nBase;
        nBase *= 100;
    }
    return nTotal;
}

/*!
 \brief 取得变量的某一位为1

 \fn CMyFunction::GetBit
 \param nData_ 要取得的源数据
 \param nBit_ 要取得的位
 \return unsigned char 取得了后的结果
 \author zzy
 \date   2015/6/10
*/
unsigned char CMyFunction::GetBit(unsigned int nData_, unsigned int nBit_)
{
    Q_ASSERT(nBit_ < sizeof(nData_)*CHAR_BIT);

    return (nData_>>nBit_) & 1;
}

/*!
 \brief 设置变量的某一位为0

 \fn CMyFunction::ResetBit
 \param nData_ 要设置的源数据
 \param nBit_ 要设置的位
 \return unsigned int 设置后的结果
 \author zzy
 \date   2015/6/10
*/
unsigned int CMyFunction::ResetBit(unsigned int nData_, unsigned int nBit_)
{
    Q_ASSERT(nBit_ < sizeof(nData_)*CHAR_BIT);

    return ((nData_) & (~(1<<(nBit_))));
}

/*!
 \brief 设置变量的某一位为1

 \fn CMyFunction::SetBit
 \param nData_ 要设置的源数据
 \param nBit_ 要设置的位
 \return unsigned int 设置后的结果
 \author zzy
 \date   2015/6/10
*/
unsigned int CMyFunction::SetBit(unsigned int nData_, unsigned int nBit_)
{
    Q_ASSERT(nBit_ < sizeof(nData_)*CHAR_BIT);

    return ((nData_) | (1<<(nBit_)));
}

/*!
 \brief 取得一字中间几位的值

 \fn CMyFunction::GetMidBitValue
 \param nValue_ 要取值的数
 \param nBitOffset_ 开始取位的位置
 \param nBitNumber_ 取位的宽度
 \param bSigned_ 返回的是否为有符号数(即要取的nBitNumber的最高位是否为符号位)
 \return unsigned int 所取得的值
 \author zzy
 \date   2015/6/10
*/
unsigned int CMyFunction::GetMidBitValue(unsigned int nValue_, unsigned int nBitOffset_, unsigned int nBitNumber_, bool bSigned_)
{
    Q_ASSERT(nBitOffset_+nBitNumber_ <= 32);

    nValue_ = (nValue_>>nBitOffset_) & LOW_ONES(nBitNumber_);//取中间的bitnumber位
    if (bSigned_ && GetBit(nValue_, nBitNumber_-1))//返回是的正负的
    {
        nValue_ =	ResetBit(nValue_, nBitNumber_-1);//去除符号位
        nValue_ = ~nValue_ + 1;  //=-nValue;//也可以
    }
    return nValue_;
}

/*!
 \brief 分组交换pInfo中内容  即0x12345678 ->0x5678 1234 高低字交换(nInfoSize =4,GroupSize=2)

 \fn CMyFunction::ReverseWord
 \param pInfo_ 内容地址
 \param nInfoSize_ 字节数
 \param nGroupSize_ 分组大小(组内成员个数）
 \author zzy
 \date   2015/6/10
*/
void CMyFunction::ReverseWord(void *pInfo_, int nInfoSize_, int nGroupSize_)
{
    //用于字符串到16进制时的变换

    Q_ASSERT(pInfo_ != NULL);
    Q_ASSERT(nInfoSize_ > 0);
    Q_ASSERT(nGroupSize_ > 0);
    Q_ASSERT(nInfoSize_ >= nGroupSize_);
    Q_ASSERT(nInfoSize_ == (nInfoSize_/nGroupSize_)*nGroupSize_);//信息长度是分组长度的整数倍
    Q_ASSERT(nGroupSize_ > 1);//组内应有两个字以上

    WORD *pCur = (WORD *)pInfo_;
    WORD *pEnd = (WORD *)((BYTE *)pInfo_+nInfoSize_);

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

 \fn CMyFunction::ReverseByte
 \param pInfo_ 内容地址
 \param nInfoSize_ 字节数
 \param nGroupSize_ 分组大小(组内成员个数）
 \author zzy
 \date   2015/6/10
*/
void CMyFunction::ReverseByte(void *pInfo_, int nInfoSize_, int nGroupSize_)
{
    //用于modbus 等高前低后的数据处理
    Q_ASSERT(pInfo_ !=NULL);
    Q_ASSERT(nInfoSize_ > 0);
    Q_ASSERT(nGroupSize_ > 0);
    Q_ASSERT(nInfoSize_ >= nGroupSize_);
    Q_ASSERT(nInfoSize_ == (nInfoSize_/nGroupSize_)*nGroupSize_);//信息长度是分组长度的整数倍
    Q_ASSERT(nGroupSize_ > 1);//组内应有两个字节以上

    BYTE *pCur = (BYTE *)pInfo_;
    BYTE *pEnd = (BYTE *)pInfo_+nInfoSize_;
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
