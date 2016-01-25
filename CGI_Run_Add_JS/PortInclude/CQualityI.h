/*!
 * \file    CQualityI.h
 * \brief   概述 CQualityI类的头文件
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

#ifndef CQUALITYI_H
#define CQUALITYI_H
class QString;

/*!
 * \class  CQualityI
 * \brief  概述 变量的品质类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CQualityI
{
public:
    virtual int SetValue(int nValue_) = 0;  ///< 设置质量值
    virtual int GetValue() = 0;             ///< 取得质量值

    virtual QString GetValue() = 0;                         ///< 取得质量的字符描述
    virtual bool SetValue(const QString strQuality_) = 0;   ///< 使用字符串设置质量的值
};

#endif // CQUALITYI_H
