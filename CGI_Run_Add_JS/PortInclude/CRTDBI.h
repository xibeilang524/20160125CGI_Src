/*!
 * \file    CRTDBI.h
 * \brief   概述 CRTDBI实时数据库纯虚基类的头文件
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

#ifndef CRTDBI_H
#define CRTDBI_H
class CTagI;
class QString;
#include <QMap>

/*!
 * \class  CRTDBI
 * \brief  概述 实时数据库纯虚基类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CRTDBI
{
public:
    virtual	CTagI * FindTag( const QString strTagName_) = 0;///< 查找指定的名称的变量

    typedef QMap<QString,CTagI *> TAG_MAP;
    virtual	TAG_MAP *GetTAG_MAP() = 0;///< Data Member
};

#endif // CRTDBI_H
