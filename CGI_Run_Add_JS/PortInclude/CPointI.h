/*!
 * \file    CPointI.h
 * \brief   概述 CPoint类的纯虚基类
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/1/16
 *
 * \b       修改记录：
 * \li      2015/1/16
 *  添加注释
 */

#ifndef CPOINTI_H
#define CPOINTI_H

class CValueI;
#include <QVariant>

/*!
 * \enum   TAG_TYPE
 * \brief  概述 Tag类型
 * \author zzy
 * \date   2015/1/16
 * 详细概述
 */
enum TAG_TYPE
{
    TAG_TYPE_DI = 1,///< 遥信
    TAG_TYPE_AI,    ///< 遥测
    TAG_TYPE_MEM,   ///< 遥脉
    TAG_TYPE_DO,    ///< 遥控
    TAG_TYPE_AO,    ///< 遥设
};

/*!
 * \class  CPointI
 * \brief  概述 点信息类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CPointI
{
public:
    virtual ~CPointI(){}
    virtual bool OnValueChange(QVariant valueSet_) = 0;///< 响应变量什发生变化
    virtual CValueI *GetValue() = 0;///< 获取值
    virtual int GetDeviceAddress() = 0;///< 获取设备地址

};

#endif // CPOINTI_H
