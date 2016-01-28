#include "CBaseFrameCDT.h"

CBaseFrameCDT::CBaseFrameCDT()
{
}
/*!
 \brief 计算CDT的校验码值   g（x）= x8+ x2 + x +1

 \fn CBaseFrameCDT::MakeCRC_CDT
 \param pInfo 信息地址
 \param nLen 信息长度
 \return BYTE  crc值
*/
BYTE CBaseFrameCDT::MakeCRC_CDT(const void *pInfo, int nLen)
{
    Q_ASSERT(pInfo != NULL);
    Q_ASSERT(nLen > 0);
    if(!(pInfo && nLen>0))
        return 0;

    static bool bFirstCalc=true;
    static BYTE nCRCTable[256];
    if(bFirstCalc)
    {//计算余数表
        bFirstCalc=false;
        for(int i=0;i<256;i++)
        {
            BYTE nCode=i;
            for(int j=0;j<8;j++)
            {
                if((nCode&0x80)!=0)
                {
                    nCode=nCode<<1;
                    nCode=nCode^0x07;
                }
                else
                {
                    nCode=nCode<<1;
                }
            }
            nCRCTable[i]=nCode;
        }
    }

    BYTE *pCur = (BYTE *)pInfo;
    BYTE nCRC = 0;
    BYTE nPosition =0;
    for(int i=0;i<nLen;i++)
    {
        nPosition = pCur[i]^nCRC;
        nCRC = nCRCTable[nPosition];
    }
    nCRC = ~nCRC;
    return nCRC;
}
