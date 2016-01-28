/*!
 * \file    CRTDBBase.cpp
 * \brief   概述 CRTDBBase类的源文件
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

#include "crtdbbase.h"
#include <QString>
#include <QDebug>
#include "../PortInclude/CTagI.h"

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/22
 */
CRTDBBase::CRTDBBase()
{
}

/*!
 * \brief  功能概述 查找指定的名称的变量
 * \param  参数描述 strTagName_是变量的全局名
 * \return 返回值描述 如果查找到则返回点的指针，否则返回NULL
 * \author zzy
 * \date   2015/5/22
 */
CTagI *CRTDBBase::FindTag(const QString strTagName_)
{
    CTagI *result = m_mapTag.value(strTagName_);
    return result;
}

/*!
 * \brief  功能概述 向实时库map中添加变量
 * \param  参数描述 strTagName_是点的全局名称，pTag_是点的指针
 * \return 返回值描述 成功返回true，否则返回false
 * \author zzy
 * \date   2015/5/22
 */
bool CRTDBBase::AddTag(QString strTagName_, CTagI *pTag_)
{
    QMutexLocker lock(&m_mutex);
    if(!m_mapTag.contains(strTagName_))
    {
        m_mapTag.insert(strTagName_, pTag_);
        return true;
    }else
    {
        qDebug()<<__func__<<" Add Point Failed"<<strTagName_<<pTag_->GetType();
        return false;
    }
}

/*!
 * \brief  功能概述 获取实时库map的指针
 * \param  参数描述 无
 * \return 返回值描述 返回实时库map的指针
 * \author zzy
 * \date   2015/5/22
 */
CRTDBBase::TAG_MAP *CRTDBBase::GetTAG_MAP()
{
    return &m_mapTag;
}
