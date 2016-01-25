/*!
 * \file  CItemDelegateLabel.h
 * \brief 概述 CItemDelegateLabel类的头文件
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

#ifndef CITEMDELEGATELABEL_H
#define CITEMDELEGATELABEL_H

#include <QItemDelegate>

/*!
 \brief Label自定义委托类

 \class CItemDelegateLabel CItemDelegateLabel.h "SCADA/ItemDelegate/CItemDelegateLabel.h"
 \author zzy
 \date   2015/5/29
*/
class CItemDelegateLabel : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CItemDelegateLabel(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif // CITEMDELEGATELABEL_H
