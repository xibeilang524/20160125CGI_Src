/*!
 * \file  CItemDelegateEdit.h
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

#ifndef CITEMDELEGATEEDIT_H
#define CITEMDELEGATEEDIT_H

#include <QItemDelegate>
#include <QString>

/*!
 \brief 编辑框自定义委托类

 \class CItemDelegateEdit CItemDelegateEdit.h "SCADA/ItemDelegate/CItemDelegateEdit.h"
 \author zzy
 \date   2015/5/29
*/
class CItemDelegateEdit : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CItemDelegateEdit(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
signals:

public slots:

};

#endif // CITEMDELEGATEEDIT_H
