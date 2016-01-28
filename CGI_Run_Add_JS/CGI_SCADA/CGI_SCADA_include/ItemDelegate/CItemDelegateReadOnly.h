/*!
 * \file  CItemDelegateReadOnly.h
 * \brief 概述 CItemDelegateReadOnly类的头文件
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

#ifndef CITEMDELEGATEREADONLY_H
#define CITEMDELEGATEREADONLY_H

#include <QItemDelegate>
#include <QString>

/*!
 \brief 只读的自定义委托类,只用于显示文本,不能编辑

 \class CItemDelegateReadOnly CItemDelegateReadOnly.h "SCADA/ItemDelegate/CItemDelegateReadOnly.h"
 \author zzy
 \date   2015/5/29
*/
class CItemDelegateReadOnly : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CItemDelegateReadOnly(QObject *parent = 0);
//    CItemDelegateReadOnly(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget*parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;


};

#endif // CITEMDELEGATEREADONLY_H
