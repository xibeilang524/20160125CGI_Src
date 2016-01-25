/*!
 * \file    CBatchChangeDialogDialog.cpp
 * \brief   概述 CBatchChangeDialogDialog类的源文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/1/8
 *
 * \b       修改记录：
 * \li      2015/1/8
 *  添加注释
 */

#include "CBatchChangeDialogDialog.h"
#include "ui_CBatchChangeDialogDialog.h"
#include <QPushButton>

/*!
 * \brief 功能概述 构造函数 构造一个对话框,构造一个Ui界面
 * \param 参数描述 parent是父窗体指针
 * \return 返回值描述 无
 * \author zzy
 * \date 2015/1/8
 */
CBatchChangeDialogDialog::CBatchChangeDialogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBatchChangeDialogDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("批量修改");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
}

/*!
 * \brief 功能概述 析构函数
 * \param 参数描述
 * \return 返回值描述
 * \author zzy
 * \date 2015/1/8
 */
CBatchChangeDialogDialog::~CBatchChangeDialogDialog()
{
    delete ui;
}

/*!
 * \brief 功能概述 获取基值
 * \param 参数描述 无
 * \return 返回值描述 返回基值
 * \author zzy
 * \date 2015/1/8
 */
QString CBatchChangeDialogDialog::GetBaseValue() const
{
    return ui->lineEditBaseValue->text();
}

/*!
 * \brief 功能概述 获取间隔值
 * \param 参数描述 无
 * \return 返回值描述 返回间隔值
 * \author zzy
 * \date 2015/1/8
 */
QString CBatchChangeDialogDialog::GetInterval() const
{
    return ui->lineEditInterval->text();
}

/*!
 * \brief 功能概述 获取radio的状态
 * \param 参数描述 无
 * \return 返回值描述 返回radio的状态
 * \author zzy
 * \date 2015/1/8
 */
int CBatchChangeDialogDialog::GetRadioCondition() const
{
    int result = RadioButton;
    if (ui->radioButtonSame->isChecked())
    {
        result = RadioButtonSame;
    }else if (ui->radioButtonProgressively->isChecked())
    {
        result = RadioButtonProgressively;
    }else {
        result = RadioButtonDecrease;
    }
    return result;
}
