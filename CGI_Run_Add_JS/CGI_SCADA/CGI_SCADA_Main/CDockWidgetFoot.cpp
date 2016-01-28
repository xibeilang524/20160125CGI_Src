/*!
 * \file  CDockWidgetFoot.cpp
 * \brief 概述 CDockWidgetFoot类的源文件
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

#include "CDockWidgetFoot.h"
#include "CMainWindow.h"
#include "CTextBrowserDebug.h"
#include <QDebug>
/*!
 \brief 构造函数,构造一个CDockWidgetBase对象

 \fn CDockWidgetFoot::CDockWidgetFoot
 \param parent 父窗体的指针
 \author zzy
 \date   2015/5/29
*/
CDockWidgetFoot::CDockWidgetFoot(QWidget *parent) :
    QDockWidget(parent)
{
    SetInitWidget();
}

/*!
 \brief 构造函数,构造一个CDockWidgetBase对象,初始化m_pMainWimdows指针的指向

 \fn CDockWidgetFoot::CDockWidgetFoot
 \param parent 父窗体的指针
 \author zzy
 \date   2015/5/29
*/
CDockWidgetFoot::CDockWidgetFoot(CMainWindow *parent) :
    QDockWidget(parent),
    m_pMainWimdows(parent)
{
    SetInitWidget();
}

/*!
 \brief 析构函数

 \fn CDockWidgetFoot::~CDockWidgetFoot
 \author zzy
 \date   2015/5/29
*/
CDockWidgetFoot::~CDockWidgetFoot()
{
}

/*!
 \brief 获取打印信息控件的指针

 \fn CDockWidgetFoot::GetTextBrowserDebug
 \return CTextBrowserDebug 返回m_pTextBrowserDebug指针
 \author zzy
 \date   2015/5/29
*/
CTextBrowserDebug *CDockWidgetFoot::GetTextBrowserDebug() const
{
    return m_pTextBrowserDebug;
}

void CDockWidgetFoot::slot_ShowMessageDebug(int, QString)
{
    if (this->isHidden())
    {
        this->show();
    }
}

/*!
 \brief 初始化调试信息栏

 \fn CDockWidgetFoot::SetInitWidget
 \author zzy
 \date   2015/5/29
*/
void CDockWidgetFoot::SetInitWidget()
{
    connect(this,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SLOT(slot_ShowMessageDebug(int,QString)));
    this->setWindowTitle("调试信息栏");
    m_pMainWimdows->addDockWidget(Qt::BottomDockWidgetArea,this);
    m_pTextBrowserDebug = new CTextBrowserDebug(this);
    connect(this,SIGNAL(signal_ShowMessageDebug(int,QString)),m_pTextBrowserDebug,SLOT(slot_ShowMessageDebug(int,QString)));
    connect(this,SIGNAL(signal_ClearDebug()),m_pTextBrowserDebug,SLOT(slot_ClearAll()));
    this->setWidget(m_pTextBrowserDebug);
}
