/*!
 * \file    CRTDBBase.h
 * \brief   概述 CRTDBBase类的头文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/22
 *
 * \b       修改记录：
 * \li      2015/5/22
 *  添加注释
 */

#ifndef CRTDBBASE_H
#define CRTDBBASE_H

#include "../PortInclude/CRTDBI.h"
#include <QMap>
/*!
 * \class  CRTDBBase
 * \brief  概述 实时库map管理类
 * \author zzy
 * \date   2015/5/22
 * 详细概述
 */
class CRTDBBase : public CRTDBI
{
public:
    CRTDBBase();
public:
    virtual	CTagI * FindTag( const QString strTagName_);
    virtual bool AddTag(QString strTagName_, CTagI *pTag_);

    TAG_MAP *GetTAG_MAP();
    TAG_MAP m_mapTag;
};

#endif // CRTDBBASE_H
