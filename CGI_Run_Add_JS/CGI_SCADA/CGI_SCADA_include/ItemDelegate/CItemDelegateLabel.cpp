/*!
 * \file  CItemDelegateLabel.cpp
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

#include "CItemDelegateLabel.h"
#include <QLabel>

/*!
 \brief 构造函数

 \fn CItemDelegateLabel::CItemDelegateLabel
 \param parent 父类的指针
 \author zzy
 \date   2015/5/29
*/
CItemDelegateLabel::CItemDelegateLabel(QObject *parent) :
    QItemDelegate(parent)
{
}

/*!
 \brief 创建QLabel控件

 \fn CItemDelegateLabel::createEditor
 \param parent 父窗体的指针
 \param option 无效
 \param index 无效
 \return QWidget 返回QLabel指针
 \author zzy
 \date   2015/5/29
*/
QWidget *CItemDelegateLabel::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QLabel *label = new QLabel(parent);
    label->installEventFilter(const_cast<CItemDelegateLabel*>(this));
    return label;
}

/*!
 \brief 设置自定义委托显示的文本

 \fn CItemDelegateLabel::setEditorData
 \param editor 自定义委托的窗体指针
 \param index 当前表格点中的索引
 \author zzy
 \date   2015/5/29
*/
void CItemDelegateLabel::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.model()->data(index).toString();

    QLabel *label = static_cast<QLabel *>(editor);
    label->setText(str);
}

/*!
 \brief 设置当前点中的节点的文本

 \fn CItemDelegateLabel::setModelData
 \param editor 自定义委托的窗体指针
 \param model 表格的model
 \param index 当前点中的索引
 \author zzy
 \date   2015/5/29
*/
void CItemDelegateLabel::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLabel *label = static_cast<QLabel *>(editor);
    model->setData(index, QVariant(label->text()));
}

/*!
 \brief 设置控件的大小和坐标

 \fn CItemDelegateLabel::updateEditorGeometry
 \param editor 编辑框的指针
 \param option 无效
 \param index 无效
 \author zzy
 \date   2015/5/29
*/
void CItemDelegateLabel::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    /*!
    * \brief 功能概述
    * \param 参数描述 editor
    * \return 返回值描述 无
    * \author zzy
    * \date 2015/1/8
    */
    QRect rect(option.rect);
    rect.moveRight(4);
//    editor->setGeometry(option.rect);
    editor->setGeometry(rect);
}
