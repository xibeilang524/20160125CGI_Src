/*!
 * \file  CItemDelegateAll.h
 * \brief 概述 CItemDelegateAll类的头文件
 *
 *详细概述
 *
 * \author zzy
 * \version 版本号
 * \date 2015/1/8
 *
 * \b 修改记录：
 * \li 2015/1/8
 *  添加注释
 */

#ifndef CITEMDELEGATEALL_H
#define CITEMDELEGATEALL_H

#include <QString>
#include <QStringList>
#include <QItemDelegate>

/*!
 \brief 用于返回相应的自定义委托的类

 \class CItemDelegateAll CItemDelegateAll.h "SCADA/ItemDelegate/CItemDelegateAll.h"
 \author zzy
 \date   2015/5/29
*/
class CItemDelegateAll
{
public:
    CItemDelegateAll();
    QItemDelegate *GetItemDelegate(const QString &strItemDelegateTypeName_,const QStringList &strList_ = QStringList());
};

#endif // CITEMDELEGATEALL_H
