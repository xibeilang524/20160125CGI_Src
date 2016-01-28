/*!
 * \file  CItemDelegateEdit.cpp
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

#include "CItemDelegateEdit.h"
#include <QLineEdit>

/*!
 \brief 构造函数

 \fn CItemDelegateEdit::CItemDelegateEdit
 \param parent 父类的指针
 \author zzy
 \date   2015/5/29
*/
CItemDelegateEdit::CItemDelegateEdit(QObject *parent) :
    QItemDelegate(parent)
{
}

/*!
 \brief 创建QLineEdit控件

 \fn CItemDelegateEdit::createEditor
 \param parent 父窗体指针
 \param option 无效
 \param index 无效
 \return QWidget 返回窗体的指针
 \author zzy
 \date   2015/5/29
*/
QWidget *CItemDelegateEdit::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QLineEdit *editor = new QLineEdit(parent);
//    editor->setEnabled(false);
    editor->installEventFilter(const_cast<CItemDelegateEdit*>(this));
    return editor;
}

/*!
 \brief 设置自定义委托显示的文本

 \fn CItemDelegateEdit::setEditorData
 \param editor 自定义委托的窗体指针
 \param index 当前表格点中的索引
 \author zzy
 \date   2015/5/29
*/
void CItemDelegateEdit::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.model()->data(index).toString();

    QLineEdit *lineEdit = static_cast<QLineEdit *>(editor);
    lineEdit->setText(str);
}

/*!
 \brief 设置当前点中的节点的文本

 \fn CItemDelegateEdit::setModelData
 \param editor 自定义委托的窗体指针
 \param model 表格的model
 \param index 当前点中的索引
 \author zzy
 \date   2015/5/29
*/
void CItemDelegateEdit::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit *>(editor);
    model->setData(index, QVariant(lineEdit->text()));
}

/*!
 \brief 设置控件的大小和坐标

 \fn CItemDelegateEdit::updateEditorGeometry
 \param editor 编辑框的指针
 \param option 无效
 \param index 无效
 \author zzy
 \date   2015/5/29
*/
void CItemDelegateEdit::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

