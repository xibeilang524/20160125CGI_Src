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

#include "CSignFrameDialog.h"
#include "ui_CSignFrameDialog.h"
#include <QColorDialog>
#include <QDebug>
#include <QPalette>

CSignFrameDialog::CSignFrameDialog(QWidget *parent) :
    QDialog(parent),
    m_pFirstLine(new SignFrame),
    m_pSecondLine(new SignFrame),
    m_pThirdLine(new SignFrame),
    m_pFourthLine(new SignFrame),
    m_pFifthLine(new SignFrame),
    m_pSixthLine(new SignFrame),
    ui(new Ui::CSignFrameDialog)
{
    /*!
    * \brief  功能概述 构造函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    ui->setupUi(this);
    m_pFirstLine->m_Color = Qt::black;
    m_pSecondLine->m_Color = Qt::red;
    m_pThirdLine->m_Color = Qt::darkRed;
    m_pFourthLine->m_Color = Qt::cyan;
    m_pFifthLine->m_Color = Qt::yellow;
    m_pSixthLine->m_Color = Qt::gray;

    m_pFirstLine->m_isCheck = false;
    m_pSecondLine->m_isCheck = false;
    m_pThirdLine->m_isCheck = false;
    m_pFourthLine->m_isCheck = false;
    m_pFifthLine->m_isCheck = false;
    m_pSixthLine->m_isCheck = false;

    m_pFirstLine->m_strFrame = ui->lineEdit->text();
    m_pSecondLine->m_strFrame = ui->lineEdit_2->text();
    m_pThirdLine->m_strFrame = ui->lineEdit_3->text();
    m_pFourthLine->m_strFrame = ui->lineEdit_4->text();
    m_pFifthLine->m_strFrame = ui->lineEdit_5->text();
    m_pSixthLine->m_strFrame = ui->lineEdit_6->text();

    {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text,m_pFirstLine->m_Color);
        ui->lineEdit->setText(ui->lineEdit->text().toUpper());
        ui->lineEdit->setPalette(pal);
    }
    {
        QPalette pal = ui->lineEdit_2->palette();
        pal.setColor(QPalette::Text,m_pSecondLine->m_Color);
        ui->lineEdit_2->setText(ui->lineEdit_2->text().toUpper());
        ui->lineEdit_2->setPalette(pal);
    }
    {
        QPalette pal = ui->lineEdit_3->palette();
        pal.setColor(QPalette::Text,m_pThirdLine->m_Color);
        ui->lineEdit_3->setText(ui->lineEdit_3->text().toUpper());
        ui->lineEdit_3->setPalette(pal);
    }
    {
        QPalette pal = ui->lineEdit_4->palette();
        pal.setColor(QPalette::Text,m_pFourthLine->m_Color);
        ui->lineEdit_4->setText(ui->lineEdit_4->text().toUpper());
        ui->lineEdit_4->setPalette(pal);
    }
    {
        QPalette pal = ui->lineEdit_5->palette();
        pal.setColor(QPalette::Text,m_pFifthLine->m_Color);
        ui->lineEdit_5->setText(ui->lineEdit_5->text().toUpper());
        ui->lineEdit_5->setPalette(pal);
    }
    {
        QPalette pal = ui->lineEdit_6->palette();
        pal.setColor(QPalette::Text,m_pSixthLine->m_Color);
        ui->lineEdit_6->setText(ui->lineEdit_6->text().toUpper());
        ui->lineEdit_6->setPalette(pal);
    }
//    m_FirstLineColor = Qt::black;
//    m_SecondLineColor = Qt::red;
//    m_ThirdLineColor = Qt::darkRed;
//    m_FourthLineColor = Qt::cyan;
//    m_FifthLineColor = Qt::yellow;
//    m_SixthLineColor = Qt::gray;


    ui->lineEdit->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->lineEdit_2->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->lineEdit_4->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->lineEdit_5->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->lineEdit_6->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
}

CSignFrameDialog::~CSignFrameDialog()
{
    /*!
    * \brief  功能概述 析构函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    delete ui;
}

void CSignFrameDialog::on_pushButton_clicked()
{
    /*!
    * \brief  功能概述 显示字体颜色槽函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    QColorDialog dialog;
    dialog.setCurrentColor(m_pFirstLine->m_Color);
    if (dialog.exec())
    {
        qDebug()<<dialog.currentColor();

        m_pFirstLine->m_Color = dialog.currentColor();
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text,m_pFirstLine->m_Color);
        ui->lineEdit->setText(ui->lineEdit->text().toUpper());
        ui->lineEdit->setPalette(pal);
    }
}

void CSignFrameDialog::on_pushButton_2_clicked()
{
    /*!
    * \brief  功能概述 显示字体颜色槽函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    QColorDialog dialog;
    dialog.setCurrentColor(m_pSecondLine->m_Color);
    if (dialog.exec())
    {
        qDebug()<<dialog.currentColor();

        m_pSecondLine->m_Color = dialog.currentColor();
        QPalette pal = ui->lineEdit_2->palette();
        pal.setColor(QPalette::Text,m_pSecondLine->m_Color);
        ui->lineEdit_2->setText(ui->lineEdit_2->text().toUpper());
        ui->lineEdit_2->setPalette(pal);
    }
}

void CSignFrameDialog::on_pushButton_3_clicked()
{
    /*!
    * \brief  功能概述 显示字体颜色槽函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    QColorDialog dialog;
    dialog.setCurrentColor(m_pThirdLine->m_Color);
    if (dialog.exec())
    {
        qDebug()<<dialog.currentColor();

        m_pThirdLine->m_Color = dialog.currentColor();
        QPalette pal = ui->lineEdit_3->palette();
        pal.setColor(QPalette::Text,m_pThirdLine->m_Color);
        ui->lineEdit_3->setText(ui->lineEdit_3->text().toUpper());
        ui->lineEdit_3->setPalette(pal);
    }
}

void CSignFrameDialog::on_pushButton_4_clicked()
{
    /*!
    * \brief  功能概述 显示字体颜色槽函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    QColorDialog dialog;
    dialog.setCurrentColor(m_pFourthLine->m_Color);
    if (dialog.exec())
    {
        qDebug()<<dialog.currentColor();

        m_pFourthLine->m_Color = dialog.currentColor();
        QPalette pal = ui->lineEdit_4->palette();
        pal.setColor(QPalette::Text,m_pFourthLine->m_Color);
        ui->lineEdit_4->setText(ui->lineEdit_4->text().toUpper());
        ui->lineEdit_4->setPalette(pal);
    }
}

void CSignFrameDialog::on_pushButton_5_clicked()
{
    /*!
    * \brief  功能概述 显示字体颜色槽函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    QColorDialog dialog;
    dialog.setCurrentColor(m_pFifthLine->m_Color);
    if (dialog.exec())
    {
        qDebug()<<dialog.currentColor();

        m_pFifthLine->m_Color = dialog.currentColor();
        QPalette pal = ui->lineEdit_5->palette();
        pal.setColor(QPalette::Text,m_pFifthLine->m_Color);
        ui->lineEdit_5->setText(ui->lineEdit_5->text().toUpper());
        ui->lineEdit_5->setPalette(pal);
    }
}

void CSignFrameDialog::on_pushButton_6_clicked()
{
    /*!
    * \brief  功能概述 显示字体颜色槽函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    QColorDialog dialog;
    dialog.setCurrentColor(m_pSixthLine->m_Color);
    if (dialog.exec())
    {
        qDebug()<<dialog.currentColor();

        m_pSixthLine->m_Color = dialog.currentColor();
        QPalette pal = ui->lineEdit_6->palette();
        pal.setColor(QPalette::Text,m_pSixthLine->m_Color);
        ui->lineEdit_6->setText(ui->lineEdit_6->text().toUpper());
        ui->lineEdit_6->setPalette(pal);
    }
}

void CSignFrameDialog::on_checkBox_clicked(bool checked)
{
    if (checked)
    {
        m_pFirstLine->m_strFrame = ui->lineEdit->text();
        m_pFirstLine->m_isCheck = true;
        ui->lineEdit->setEnabled(true);
        ui->pushButton->setEnabled(true);
    }else
    {
        m_pFirstLine->m_isCheck = false;
        ui->lineEdit->setEnabled(false);
        ui->pushButton->setEnabled(false);
    }
}

void CSignFrameDialog::on_checkBox_2_clicked(bool checked)
{
    if (checked)
    {
        m_pSecondLine->m_strFrame = ui->lineEdit_2->text();
        m_pSecondLine->m_isCheck = true;
        ui->lineEdit_2->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
    }else
    {
        m_pSecondLine->m_isCheck = false;
        ui->lineEdit_2->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
    }
}

void CSignFrameDialog::on_checkBox_3_clicked(bool checked)
{
    if (checked)
    {
        m_pThirdLine->m_strFrame = ui->lineEdit_3->text();
        m_pThirdLine->m_isCheck = true;
        ui->lineEdit_3->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
    }else
    {
        m_pThirdLine->m_isCheck = false;
        ui->lineEdit_3->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
    }
}

void CSignFrameDialog::on_checkBox_4_clicked(bool checked)
{
    if (checked)
    {
        m_pFourthLine->m_strFrame = ui->lineEdit_4->text();
        m_pFourthLine->m_isCheck = true;
        ui->lineEdit_4->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
    }else
    {
        m_pFourthLine->m_isCheck = false;
        ui->lineEdit_4->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
    }
}

void CSignFrameDialog::on_checkBox_5_clicked(bool checked)
{
    if (checked)
    {
        m_pFifthLine->m_strFrame = ui->lineEdit_5->text();
        m_pFifthLine->m_isCheck = true;
        ui->lineEdit_5->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
    }else
    {
        m_pFifthLine->m_isCheck = false;
        ui->lineEdit_5->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
    }
}

void CSignFrameDialog::on_checkBox_6_clicked(bool checked)
{
    if (checked)
    {
        m_pSixthLine->m_strFrame = ui->lineEdit_6->text();
        m_pSixthLine->m_isCheck = true;
        ui->lineEdit_6->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
    }else
    {
        m_pSixthLine->m_isCheck = false;
        ui->lineEdit_6->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
    }
}

void CSignFrameDialog::on_lineEdit_textChanged(const QString &arg1)
{
    m_pFirstLine->m_strFrame = arg1;
}

void CSignFrameDialog::on_lineEdit_2_textChanged(const QString &arg1)
{
    m_pSecondLine->m_strFrame = arg1;
}

void CSignFrameDialog::on_lineEdit_3_textChanged(const QString &arg1)
{
    m_pThirdLine->m_strFrame = arg1;

}

void CSignFrameDialog::on_lineEdit_4_textChanged(const QString &arg1)
{
    m_pFourthLine->m_strFrame = arg1;

}

void CSignFrameDialog::on_lineEdit_5_textChanged(const QString &arg1)
{
    m_pFifthLine->m_strFrame = arg1;

}

void CSignFrameDialog::on_lineEdit_6_textChanged(const QString &arg1)
{
    m_pSixthLine->m_strFrame = arg1;

}
