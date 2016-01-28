/*!
 * \file
 * \brief   概述 CReplaceDialog类的源文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/29
 *
 * \b       修改记录：
 * \li      2015/5/29
 *
 *  添加注释
 */

#include "CReplaceDialog.h"
#include "ui_CReplaceDialog.h"
#include <QPushButton>

/*!
 \brief 构造函数

 \fn CReplaceDialog::CReplaceDialog
 \param parent 父窗体的指针
 \author zzy
 \date   2015/5/29
*/
CReplaceDialog::CReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CReplaceDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("替换对话框");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
}

/*!
 \brief 析构函数

 \fn CReplaceDialog::~CReplaceDialog
 \author zzy
 \date   2015/5/29
*/
CReplaceDialog::~CReplaceDialog()
{
    delete ui;
}

/*!
 \brief 获取目标值

 \fn CReplaceDialog::GetBefore
 \return QString 返回目标值
 \author zzy
 \date   2015/5/29
*/
QString CReplaceDialog::GetBefore() const
{
    return ui->lineEdit->text();
}

/*!
 \brief 获取替换为值

 \fn CReplaceDialog::GetAfter
 \return QString 返回替换为值
 \author zzy
 \date   2015/5/29
*/
QString CReplaceDialog::GetAfter() const
{
    return ui->lineEdit_2->text();
}
