/*!
 * \file    CReplaceDialog.h
 * \brief   概述 CReplaceDialog类的头文件
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

#ifndef CREPLACEDIALOG_H
#define CREPLACEDIALOG_H

#include <QDialog>

namespace Ui {
class CReplaceDialog;
}

/*!
 \brief 替换对话框

 \class CReplaceDialog CReplaceDialog.h "SCADA/Dialog/CReplaceDialog.h"
 \author zzy
 \date   2015/5/29
*/
class CReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CReplaceDialog(QWidget *parent = 0);
    ~CReplaceDialog();
    QString GetBefore()const;
    QString GetAfter() const;

private:
    Ui::CReplaceDialog *ui;
};

#endif // CREPLACEDIALOG_H
