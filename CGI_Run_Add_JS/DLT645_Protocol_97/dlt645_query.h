#ifndef DTL645_QUERY_H
#define DTL645_QUERY_H
#include <QByteArray>
#include "dlt645_protocol_97.h"

class DLT645_Query:public CBaseFrame
{
public:
    DLT645_Query();
    void MakeQuery(void *nDeviceAddress, unsigned int nNumber_DI, int nFunction);
    void SetData(const BYTE *pSource_, int nLength_);
    DWORD MakeSum(const void *pInfo_, size_t nSize_);
public:
    QByteArray m_ByteArrayFrame;
    QByteArray pb_ArrayAddress;


//    int mq_iStartAddress;
};

#endif // DTL645_QUERY_H
