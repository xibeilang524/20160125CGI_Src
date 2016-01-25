/*!
 * \file    CDLT_645_07Query.cpp
 * \brief   概述 CDLT_645_07Query类的源文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/27
 *
 * \b       修改记录：
 * \li      2015/5/27
 *  添加注释
 */

#include "CDLT_645_07Query.h"

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
CDLT_645_07Query::CDLT_645_07Query()
{
}

/*!
 * \brief  功能概述 生成代码和
 * \param  参数描述 pInfo_是数据的头指针，nSize_是数据的个数
 * \return 返回值描述 返回生成的代码和
 * \author zzy
 * \date   2015/5/27
 */
DWORD CDLT_645_07Query::MakeSum(const void *pInfo_, size_t nSize_)
{
    Q_ASSERT(pInfo_ !=NULL);
    Q_ASSERT(nSize_ > 0);

    DWORD wResult = 0;
    for(size_t i=0; i<nSize_; i++)
    {
        wResult += ((BYTE *)pInfo_)[i];
    }
    return wResult;
}

/*!
 * \brief  功能概述 合成命令
 * \param  参数描述 szDataAddress_是表号，nControl_是控制码，szData_是数据域，nDatasize_是数据域大小
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CDLT_645_07Query::MakeQuery(void *szAddress_, BYTE nControl_, void *szData_, BYTE nDataSize_)
{
    Q_ASSERT(szData_ != NULL);
    Q_ASSERT(nDataSize_ > 0);

    BYTE szSend[250] = {0};
    BYTE *pCur=szSend;
    //	*pCur++=0x55;  //前导字符
    //	*pCur++=0x55;
    //	*pCur++=0xfe;
    //	*pCur++=0xfe;

    *pCur++ = 0x68;  //地址域
    memcpy(pCur,szAddress_,6);
    pCur += 6;
    *pCur++ = 0x68;

    *pCur++ = nControl_;
    *pCur++ = nDataSize_;

    memcpy(pCur,szData_,nDataSize_);
    for(int i=0;i<nDataSize_;i++)	//要加33h处理
    {
        *pCur++ += 0x33;//=>*pCur+=0x33;pCur++;
    }

    BYTE value = (BYTE)MakeSum(szSend,pCur-szSend);

//    *pCur++ = (BYTE)MakeSum(szSend,pCur-szSend);
    *pCur++ = value;
    *pCur++ = 0x16;

    SetData_2(szSend,pCur-szSend);
}

/*!
 * \brief  功能概述 设置发送报文
 * \param  参数描述 pSource_是合成的报文数据的头指针，nLength_是报文的字节数
 * \return 返回值描述
 * \author zzy
 * \date   2015/5/27
 */
void  CDLT_645_07Query::SetData_2(const BYTE *pSource_, int nLength_)
{
    m_ByteArrayFrame = QByteArray((const char*)pSource_,nLength_);
}
