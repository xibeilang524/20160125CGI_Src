/*!
 * \file  CItemDelegateReadOnly.cpp
 * \brief 概述
 *
 *详细概述
 *
 * \author zzy
 * \version 版本号
 * \date 2015/1/9
 *
 * \b 修改记录：
 * \li 2015/1/9
 *  添加注释
 */

#include "CItemDelegateReadOnly.h"

/*!
 \brief 构造函数

 \fn CItemDelegateReadOnly::CItemDelegateReadOnly
 \param parent 父类的指针
 \author zzy
 \date   2015/5/29
*/
CItemDelegateReadOnly::CItemDelegateReadOnly(QObject *parent) :
    QItemDelegate(parent)
{
}

/*!
 \brief 创建控件

 \fn CItemDelegateReadOnly::createEditor
 \param parent 无效
 \param option 无效
 \param index 无效
 \return QWidget 返回空指针
 \author zzy
 \date   2015/5/29
*/
QWidget *CItemDelegateReadOnly::createEditor(QWidget */*parent*/, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    return NULL;
}
