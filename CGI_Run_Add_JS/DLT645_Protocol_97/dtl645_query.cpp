#include "dlt645_query.h"
#include "dlt645_protocol_97.h"
#include "string.h"
#include "../../PortInclude/CBufferPool.h"
#include "../../PortInclude/CValueI.h"
/**
 ** Brief: 构造函数
 ** Parameter：
 ** Author：G.onion
 ** Creat Date：2015/09/24
 ** Last Modified：2015/09/24
 **
 **/
DLT645_Query::DLT645_Query()
{
}
/**
 ** Brief:报文生成函数
 ** Parameter：*szAddress：设备地址  _nNumber_DI：报文数据项标示  nFunction：控制码
 ** Author：G.onion
 ** Creat Date：2015/09/24
 ** Last Modified：2015/10/08
 **
 **/

void DLT645_Query::MakeQuery(void *nDeviceAddress, unsigned int nNumber_DI, int nFunction)
{
    BYTE Send[300] = {0};
    BYTE *pCur=Send;
    //	*pCur++=0xfe;  //前导字符
    //	*pCur++=0xfe;
    //	*pCur++=0xfe;
    //	*pCur++=0xfe;

    *pCur++ = 0x68;
    memcpy(pCur,nDeviceAddress,6);  //地址域
    pCur += 6;
    *pCur++ = 0x68;
    *pCur++ = 0x01;
    *pCur++ = nFunction;
//    unsigned int *p = nNumber_DI;
    memcpy(pCur,&nNumber_DI,2);
    for(int i=0;i<2;i++)	//要加33h处理
    {
        *pCur++ += 0x33;    //=>*pCur+=0x33;pCur++;
    }
    BYTE value = (BYTE)MakeSum(Send,pCur-Send);
    *pCur++ = value;
    *pCur++ = 0x16;

    SetData(Send,pCur-Send);
}
/**
 ** Brief:设置发送报文
 ** Parameter:*pSource_:报文起始位置头指针 nLength_：报文长度
 ** Author：G.onion
 ** Creat Date：2015/09/24
 ** Last Modified:2015/09/24
 **
 */
void  DLT645_Query::SetData(const BYTE *pSource_, int nLength_)
{
    m_ByteArrayFrame = QByteArray((const char*)pSource_,nLength_);
}

/**
 **
 **  Brief：校验和生成函数
 **  Parameter： *pInfo_：报文起始地址指针  nSize_：报文长度
 **  Author：G.onion
 **  Creat Date：2015/09/24
 **  Last Modified：2015/09/24
 **
 **/
DWORD DLT645_Query::MakeSum(const void *pInfo_, size_t nSize_)
{
    DWORD CheckSum = 0;
    for(size_t i=0; i<nSize_; i++)
    {
        CheckSum += ((BYTE *)pInfo_)[i];
    }
    return CheckSum;
}
