#ifndef CMYFUNCTION_H
#define CMYFUNCTION_H
#include <stddef.h>
#include "typedef.h"
#include <stdio.h>
#include <QString>
#include "CGI_MacroFunction.h"

class CMyFunction
{
public:

    //BCD 到整数
    static unsigned int  BCD2Int(const void *pInfo_, size_t nSize_);
    static unsigned int  BCD2Int_HL(const void *pInfo_, size_t nSize_);
    //取字节的位
    static unsigned char GetBit(unsigned int nData_, unsigned int nBit_);
    //复字节的位
    static unsigned int  ResetBit(unsigned int nData_, unsigned int nBit_);
    //设置字节的位
    static unsigned int  SetBit(unsigned int nData_, unsigned int nBit_);
    //取得一个字中间一些位的值
    static unsigned int GetMidBitValue(unsigned int nValue_, unsigned int nBitOffset_, unsigned int nBitNumber_, bool bSigned_);
    //按字节交换
    static void ReverseWord(void *pInfo_, int nInfoSize_, int nGroupSize_=2);
    //按字交换
    static void ReverseByte(void *pInfo_, int nInfoSize_,int nGroupSize_=2) ;
};

#endif // CMYFUNCTION_H
