/*!
 * \file    CTagI.h
 * \brief   概述 CTagI类的头文件
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

#ifndef CTAGI_H
#define CTAGI_H

class QString;
class CValueI;
class CPointI;
#include <QVariant>
#include "CValueI.h"
#include "typedef.h"

/*!
 * \class  CTagI
 * \brief  概述 变量的基类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CTagI
{
public:
    virtual QString GetName() = 0;      ///< 获取点名称
    virtual QString GetDesc() = 0 ;     ///< 获取点描述
    virtual int GetTagID() = 0;         ///< 获取点id
    virtual int SetTagID(int ID_) = 0;  ///< 设置点ID
    virtual int GetType() = 0;          ///< 获取点的类型 YX YC YM YK YS

//  工程值处理函数
    virtual void SetProjectDataType(DATA_TYPE nDataType_) = 0;///< 设置工程值类型 该值类型是取自实时数据库中
    virtual DATA_TYPE GetProjectDataType() = 0;///< 变量工程值类型 该值类型是取自实时数据库中
    virtual CValueI *GetProjectValue() = 0;///< 获取工程值
//  采集点的设置
    virtual void SetCollection(CPointI *pPoint_) = 0;///< 设置采集量
    virtual void DisCollection(CPointI *pPoint_) = 0;///< 取消采集
//  转发点的设置
    virtual void SetNotify(CPointI *pPoint_) = 0;///< 设置转发
    virtual void DisNotify(CPointI *pPoint_) = 0;///< 取消转发

    virtual CPointI *GetPointCollection() = 0;///< 获取采集点信息
    virtual void SetValue(CPointI* pPoint_,QVariant VarProject_,QVariant VarOriginal_) = 0;///< 设置值
//    virtual const QDateTime GetTagChangeDateTime()const = 0;///< 获取点的值改变时的时间
//    virtual const QDateTime GetSetValueDateTime()const = 0;///< 获取调用setValue函数的时间
//    virtual const QVariant GetColor()const = 0;///< 获取显示的颜色
};

#endif // CTAGI_H
