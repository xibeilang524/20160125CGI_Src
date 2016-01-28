/*!
 * \file  CItemDelegateAll.cpp
 * \brief 概述 CItemDelegateAll类的源文件
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

#include "CItemDelegateAll.h"
#include "CItemDelegateCombobox.h"
#include "CItemDelegateEdit.h"
#include "CItemDelegateLabel.h"
#include "CItemDelegateReadOnly.h"

/*!
 \brief 构造函数

 \fn CItemDelegateAll::CItemDelegateAll
 \author zzy
 \date   2015/5/29
*/
CItemDelegateAll::CItemDelegateAll()
{
}

/*!
 \brief 创建并返回相应的自定义委托函数

 \fn CItemDelegateAll::GetItemDelegate
 \param strItemDelegateTypeName_ 自定义委托的类型名
 \param strList_ 如果类型为combobox此参数才有用
 \return CItemDelegateBase 返回CItemDelegateBase指针
 \author zzy
 \date   2015/5/29
*/
QItemDelegate *CItemDelegateAll::GetItemDelegate(const QString &strItemDelegateTypeName_,const QStringList &strList_)
{
//    <Parameter Name="Desc" Display="变量描述" EditType="Edit" InitValue="变量%1"/>
//    <Parameter Name="SubType" Display="变量子类型" EditType="select" InitValue="1">
//    <Parameter Name="CreateTime" Display="创建时间" EditType="display" InitValue="2014/12/21">
//    <Parameter Name="AssociationName" Display="关联变量名" EditType="dialog" InitValue="">
    if ("select" == strItemDelegateTypeName_)
    {
        CItemDelegateCombobox *combobox = new CItemDelegateCombobox;
        combobox->setList(strList_);
        return combobox;
    }else if ("text" == strItemDelegateTypeName_)
    {
        return (new CItemDelegateEdit);
    }else if ("display" == strItemDelegateTypeName_)
    {
        return (new CItemDelegateLabel);
    }else if ("dialog" == strItemDelegateTypeName_)
    {
        return (new CItemDelegateReadOnly);
    }else if ("link" == strItemDelegateTypeName_)
    {
        return (new CItemDelegateLabel);
    }else {
        return (new QItemDelegate);
    }
}
