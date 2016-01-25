/*!
 * \file    CValueI.h
 * \brief   概述 CValueI类的头文件
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

#ifndef CVALUEI_H
#define CVALUEI_H
#include <QObject>
#include <QVariant>
#include "typedef.h"

/*!
 * \enum   CONTROL
 * \brief  概述 控制结果(遥控选择、遥控执行、遥控撤销、遥设)
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
enum CONTROL{
    CONTROL_UNDEFINE = -1,
    CONTROL_FAILURE,//1
    CONTROL_SUCCEED,//2
    CONTROL_TIMEOUT,//3
};

/*!
 * \enum   DATA_TYPE
 * \brief  概述 数据类型
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
enum DATA_TYPE{
    DATA_TYPE_UNDEFINE=-1, ///< -1  未初始化
    DATA_TYPE_BOOLEAN,     ///< 0   bool
    DATA_TYPE_INT8U,       ///< 1   1字节无符号数
    DATA_TYPE_INT8S,       ///< 2   1字节有符号数
    DATA_TYPE_INT16U,      ///< 3   2字节无符号数
    DATA_TYPE_INT16S,      ///< 4   2字节有符号数
    DATA_TYPE_INT32U,      ///< 5   4字节无符号数
    DATA_TYPE_INT32S,      ///< 6   4字节有符号数
    DATA_TYPE_INT64U,      ///< 7   8字节无符号数
    DATA_TYPE_INT64S,      ///< 8   8字节有符号数
    DATA_TYPE_INT128U,     ///< 9   16字节有符号数
    DATA_TYPE_INT128S,     ///< 10  16字节有符号数
    DATA_TYPE_FP32,        ///< 11  4字节浮点数
    DATA_TYPE_FP64,        ///< 12  8字节浮点数
    DATA_TYPE_STRING,      ///< 13  ascii码,是字符串  以NULL结尾
    DATA_TYPE_UNICODE,     ///< 14  unicode码
    DATA_TYPE_BINARY,      ///< 15  二进制串

    //	DATA_TYPE_ASCII_FLOAT,  //14--ASCII码 实数
    //	DATA_TYPE_ASCII_INT,    //15--ASCII码 整数
    //	DATA_TYPE_ASCII_HEX,    //16--ASCII码表示的16进制数据
    //	DATA_TYPE_BCD,          //17--BCD码 不能是有符号数
    //	DATA_TYPE_BIT_U,        //18--位段 无符号数
    //	DATA_TYPE_BIT_S,         //19--位段 有符号数 最高位为符号位
} ;
class QString;
class CBinaryI;

/*!
 * \class  CValueI
 * \brief  概述 值的纯虚基类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CValueI : public QObject
{
    Q_OBJECT
public:
    /// 设置值
    virtual void SetVarValue(const QVariant varValue_) = 0;
    /// 获取值
    virtual const QVariant  GetVarValue() = 0;
    /// 设置变量是否修改成功及文字描述
    virtual void SetWriteResult(int nControlResult_,const QString strMessage_) = 0;
    /// 等待变量修改完成
    virtual bool WaitResult(QString strResultint ,int nMaxWaitMilliSecend_) = 0;
    /// 获取控制之后的状态
    virtual int GetnControlFlag() = 0;

};

#endif // CVALUEI_H
