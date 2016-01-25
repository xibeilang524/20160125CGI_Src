#ifndef CDECODEBUFFER_H
#define CDECODEBUFFER_H

#include "CBufferPool.h"


class CDecodeBuffer : public CBufferPool
{
    Q_OBJECT
public:
    CDecodeBuffer();
    virtual ~CDecodeBuffer();
    //存取函数
public:
    //解释位
    BYTE GetBit(int nByteStart_, int nBitOffset_) const;
    unsigned int GetMidBitValue_u(int nOffset_, int nBitStart_, int nBitNumber_) const;
    int GetMidBitValue_s(int nOffset_, int nBitStart_, int nBitNumber_) const;
    //解释AScii
    float GetAsciiFloatValue(int nOffset_, int nByteSize_) const;
    int GetAsciiDecValue(int nOffset_, int nByteSize_) const;
    DWORD GetAsciiHexValue(int nOffset_, int nByteSize_) const;
    //解释BCD
    DWORD GetBcdValue(int nOffset_, int nByteSize_) const;
    //解释数值
    double GetFloatValue(int nOffset_, int nSize_) const;
    unsigned int GetUIntValue(int nOffset_, int nSize_) const;
    int GetIntValue(int nOffset_, int nSize_) const;
//修改函数
public:
    void SetBit(int nByteStart_, int nBitOffset_, BYTE nValue_) ;
    void SetValue(int nOffset_, const void *pInfo_, int nInfoSize_);
signals:

public slots:

};

#endif // CDECODEBUFFER_H
