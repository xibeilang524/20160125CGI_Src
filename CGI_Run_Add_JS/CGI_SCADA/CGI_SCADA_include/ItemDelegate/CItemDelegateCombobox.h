/*!
 * \file  CItemDelegateCombobox.h
 * \brief 概述 CItemDelegateCombobox类的头文件
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

#ifndef CITEMDELEGATECOMBOBOX_H
#define CITEMDELEGATECOMBOBOX_H

#include <QItemDelegate>

/*!
 \brief Combobox自定义委托类

 \class CItemDelegateCombobox CItemDelegateCombobox.h "SCADA/ItemDelegate/CItemDelegateCombobox.h"
 \author zzy
 \date   2015/5/29
*/
class CItemDelegateCombobox : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CItemDelegateCombobox(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex	&index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const  QModelIndex &index) const;
    void setList(const QStringList &list);
signals:

public slots:
private:
    QStringList m_strList;

};

#endif // CITEMDELEGATECOMBOBOX_H
