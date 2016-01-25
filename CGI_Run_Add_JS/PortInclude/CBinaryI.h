/*!
 * \file    CBinaryI.h
 * \brief   概述 CBinaryI纯虚基类的头文件
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

#ifndef CBINARYI_H
#define CBINARYI_H

#include "typedef.h"

/*!
 * \class  CBinaryI
 * \brief  概述 用电脑代码信息方法的 (计算机用语)
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CBinaryI
{
public:
    ///取得存储内容
    virtual const BYTE * GetBinary(int nLengthReturn_)= 0;
    ///设置存储内容
    virtual bool SetBinary(const BYTE *pBinary_,int nLength) = 0;
};

#endif // CBINARYI_H
