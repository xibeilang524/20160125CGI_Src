#ifndef CDECODEBUFFER_HL_H
#define CDECODEBUFFER_HL_H

#include "CDecodeBuffer.h"

class CDecodeBuffer_Hl : public CDecodeBuffer
{
    Q_OBJECT
public:
    CDecodeBuffer_Hl();
    virtual ~CDecodeBuffer_Hl();
public:
    void ReverseWord(int nOffset_, int nInfoSize_, int nGroupSize_=2);
    void ReverseByte(int nOffset_, int nInfoSize_,int nGroupSize_=2) ;
    unsigned int GetUIntValue_Hl(int nOffset_, int nSize_) const;
    int GetIntValue_Hl(int nOffset_, int nSize_) const;
    DWORD GetBcdValue_Hl(int nOffset_, int nByteSize_) const;
    double GetFloatValue_Hl(int nOffset_, int nSize_) const;

signals:

public slots:

};

#endif // CDECODEBUFFER_HL_H
