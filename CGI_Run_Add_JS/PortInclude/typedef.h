/*!
 * \file    typedef.h
 * \brief   概述 通用数据类型的定义文件
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

#ifndef TYPEDEF_H
#define TYPEDEF_H

//通用数据类型 come from ucos
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef unsigned char       BOOLEAN;
typedef unsigned char       INT8U;
typedef signed   char       INT8S;
typedef unsigned short      INT16U;
typedef signed   short      INT16S;
typedef unsigned int        INT32U;
typedef signed   int        INT32S;
typedef long     long       INT64S;
typedef unsigned long long  INT64U;
typedef float               FP32;
typedef double              FP64;

#define LOBYTE(w) ((BYTE) (((DWORD) (w)) & 0xff))
#define HIBYTE(w) ((BYTE) ((((DWORD) (w)) >> 8) & 0xff))

#ifndef Max
#define Max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef Min
#define Min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif
