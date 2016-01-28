#include "CCDTResponse.h"

CCDTResponse::CCDTResponse()
{
}
/*!
 \brief 取得信息字个数

 \fn CCDTResponse::GetWordOfInfo
 \return BYTE
*/
BYTE CCDTResponse::GetWordOfInfo()
{
    BYTE nInfo = 0;
    if(GetBit(6,6)>0)
    {
        nInfo = GetUIntValue(8,1);
    }
    return nInfo;
}
