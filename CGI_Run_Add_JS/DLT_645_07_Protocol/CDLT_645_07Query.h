/*!
 * \file    CDLT_645_07Query.h
 * \brief   概述 CDLT_645_07Query类的头文件
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

#ifndef CDLT_645_07QUERY_H
#define CDLT_645_07QUERY_H

#include "../PortInclude/CValueI.h"
#include "CBaseFrameCDT.h"
/*!
 * \class  CDLT_645_07Query
 * \brief  概述 发送报文管理类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CDLT_645_07Query : public CBaseFrameCDT
{
    Q_OBJECT
public:
    CDLT_645_07Query();
    void MakeQuery(void *szAddress_, BYTE nControl_, void *szData_, BYTE nDataSize_);

    DWORD MakeSum(const void *pInfo_, size_t nSize_);
    void SetData_2(const BYTE *pSource_, int nLength_);
    QByteArray m_ByteArrayFrame;
    QByteArray m_arrayAddress;

};

#endif // CDLT_645_07QUERY_H
