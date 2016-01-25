/*!
 * \file
 * \brief   概述
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/4/9
 *
 * \b       修改记录：
 * \li      2015/4/9
 *  添加注释
 */

#ifndef CSIGNFRAMEDIALOG_H
#define CSIGNFRAMEDIALOG_H

#include <QDialog>

namespace Ui {
class CSignFrameDialog;
}

/*!
 * \struct
 * \brief  概述
 * \author zzy
 * \date   2015/4/9
 * 详细概述
 */
struct SignFrame
{
    bool m_isCheck;
    QString m_strFrame;
    QColor m_Color;
};

/*!
 * \class
 * \brief  概述
 * \author zzy
 * \date   2015/4/9
 * 详细概述
 */
class CSignFrameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSignFrameDialog(QWidget *parent = 0);
    ~CSignFrameDialog();
    SignFrame *m_pFirstLine;
    SignFrame *m_pSecondLine;
    SignFrame *m_pThirdLine;
    SignFrame *m_pFourthLine;
    SignFrame *m_pFifthLine;
    SignFrame *m_pSixthLine;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_checkBox_clicked(bool checked);

    void on_checkBox_2_clicked(bool checked);

    void on_checkBox_3_clicked(bool checked);

    void on_checkBox_4_clicked(bool checked);

    void on_checkBox_5_clicked(bool checked);

    void on_checkBox_6_clicked(bool checked);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_lineEdit_5_textChanged(const QString &arg1);

    void on_lineEdit_6_textChanged(const QString &arg1);

private:
    Ui::CSignFrameDialog *ui;
//    QColor m_FirstLineColor;
//    QColor m_SecondLineColor;
//    QColor m_ThirdLineColor;
//    QColor m_FourthLineColor;
//    QColor m_FifthLineColor;
//    QColor m_SixthLineColor;

};

#endif // CSIGNFRAMEDIALOG_H
