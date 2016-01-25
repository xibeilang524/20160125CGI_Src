/*!
 * \file  CItemDelegateCombobox.cpp
 * \brief 概述 CItemDelegateCombobox类的源文件
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

#include "CItemDelegateCombobox.h"
#include <QComboBox>

/*!
 \brief 构造函数,构造CItemDelegateBase对象

 \fn CItemDelegateCombobox::CItemDelegateCombobox
 \param parent 父对象的指针
 \author zzy
 \date   2015/5/29
*/
CItemDelegateCombobox::CItemDelegateCombobox(QObject *parent) :
    QItemDelegate(parent)
{
}

/*!
 \brief 创建combobox控件

 \fn CItemDelegateCombobox::createEditor
 \param parent 父窗体指针
 \param option 无效
 \param index 无效
 \return QWidget 返回窗体的指针
 \author zzy
 \date   2015/5/29
*/
QWidget *CItemDelegateCombobox::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QComboBox *editor = new QComboBox(parent);
//    editor->setEnabled(false);
    editor->addItems(m_strList);
//    editor->setCurrentIndex(1);
//    editor->installEventFilter(const_cast<CItemDelegateCombobox*>(this));
    return editor;

}

/*!
 \brief 设置自定义委托显示的值

 \fn CItemDelegateCombobox::setEditorData
 \param editor 当前显示控件的指针
 \param index 当前表格索引
 \author zzy
 \date   2015/5/29
*/
void CItemDelegateCombobox::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str =index.model()->data(index, Qt::EditRole).toString();

    QComboBox *combobox = static_cast<QComboBox*>(editor);
    int i=combobox->findText(str);
    combobox->setCurrentIndex(i);
}

/*!
 \brief 设置点中当前索引的值

 \fn CItemDelegateCombobox::setModelData
 \param editor 当前显示控件的指针
 \param model 当前的model
 \param index 当前表格索引
 \author zzy
 \date   2015/5/29
*/
void CItemDelegateCombobox::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *combobox = static_cast<QComboBox*>(editor);
    QString str = combobox->currentText();
    model->setData(index,str, Qt::EditRole);
}

/*!
 \brief 设置控件的大小和坐标

 \fn CItemDelegateCombobox::updateEditorGeometry
 \param editor 编辑框的指针
 \param option 无效
 \param index 无效
 \author zzy
 \date   2015/5/29
*/
void CItemDelegateCombobox::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

/*!
 \brief 设置m_strList的值

 \fn CItemDelegateCombobox::setList
 \param list combobox显示的值
 \author zzy
 \date   2015/5/29
*/
void CItemDelegateCombobox::setList(const QStringList &list)
{
    m_strList = list;
}
