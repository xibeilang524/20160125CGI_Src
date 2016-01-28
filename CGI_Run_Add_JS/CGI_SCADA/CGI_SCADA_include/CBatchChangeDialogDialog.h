/*!
 * \file    CBatchChangeDialogDialog.h
 * \brief   概述 CBatchChangeDialogDialog类的头文件
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

#ifndef CBATCHCHANGEDIALOGDIALOG_H
#define CBATCHCHANGEDIALOGDIALOG_H

#include <QDialog>
//#define
/*!
 * \enum  RadioButton_TYPE
 * \brief 概述 批量修改Dialog的状态
 * \author zzy
 * \date 2015/1/8
 * 详细概述
 */
enum RadioButton_TYPE{
    RadioButton = 0,///< 未定义模式
    RadioButtonSame,///< 相同值模式
    RadioButtonProgressively,///< 递增值模式
    RadioButtonDecrease,///< 递减值模式
};
namespace Ui {
class CBatchChangeDialogDialog;
}
/*!
 * \class CBatchChangeDialogDialog
 * \brief 概述 对表格中被选择的Item进行批量修改的对话框类
 * \author zzy
 * \date 2015/1/8
 * 详细概述
 */
class CBatchChangeDialogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CBatchChangeDialogDialog(QWidget *parent = 0);
    ~CBatchChangeDialogDialog();
    QString GetBaseValue()const;///< 获取基值
    QString GetInterval()const;///< 获取间隔值
    int GetRadioCondition()const;///< 获取radio的状态

private:
    Ui::CBatchChangeDialogDialog *ui;
};

#endif // CBATCHCHANGEDIALOGDIALOG_H
