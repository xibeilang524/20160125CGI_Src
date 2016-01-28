/*!
 * \file  CTextBrowserDebug.cpp
 * \brief 概述 CTextBrowserDebug类的源文件
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

#include "CTextBrowserDebug.h"
#include <QAction>
#include <QMenu>

/*!
 \brief 构造函数,构造一个CTextBrowserBase类对象

 \fn CTextBrowserDebug::CTextBrowserDebug
 \param parent 父窗体的指针
 \author zzy
 \date   2015/5/29
*/
CTextBrowserDebug::CTextBrowserDebug(QWidget *parent) :
    QTextBrowser(parent)
{
    SetInit();
}

/*!
 \brief 打印信息函数

 \fn CTextBrowserDebug::PrintOutPutInfo
 \param str 需要打印的文字信息
 \param color 文字的颜色
 \author zzy
 \date   2015/5/29
*/
void CTextBrowserDebug::PrintOutPutInfo(const QString &str, const QColor &color)
{
    this->setTextBackgroundColor(color);
    this->setTextColor(Qt::black);
    this->append(str);
}

void CTextBrowserDebug::PrintOutPutInfo(const QString &str)
{
    this->setTextBackgroundColor(Qt::white);
    this->setTextColor(Qt::black);
    this->append(str);
}

/*!
 \brief 打印错误信息,调试信息一律为红色

 \fn CTextBrowserDebug::printDebugInfo
 \param str 需要打印的文字信息
 \author zzy
 \date   2015/5/29
*/
void CTextBrowserDebug::printDebugInfo(const QString &str)
{
    this->setTextBackgroundColor(QColor(255,255,255));
    this->setTextColor(QColor(255,0,0));
    this->append(str);
}

/*!
 \brief 打印错误信息,调试信息一律为红色

 \fn CTextBrowserDebug::printDebugInfo
 \param str 需要打印的文字信息
 \author zzy
 \date   2015/5/29
*/
void CTextBrowserDebug::printDebugInfo(const QString &str,QColor color)
{
    this->setTextBackgroundColor(QColor(255,255,255));
    this->setTextColor(color);
    this->append(str);
}
/*!
 \brief 右键弹出菜单事件函数

 \fn CTextBrowserDebug::contextMenuEvent
 \param event 无效
 \author zzy
 \date   2015/5/29
*/
void CTextBrowserDebug::contextMenuEvent(QContextMenuEvent *event)
{
    QCursor cur=this->cursor();
    m_pMenu->exec(cur.pos());
}

/*!
 \brief 初始化函数,初始化弹出菜单和动作连接

 \fn CTextBrowserDebug::SetInit
 \author zzy
 \date   2015/5/29
*/
void CTextBrowserDebug::SetInit()
{
    m_pMenu = new QMenu("右键菜单",this);

    QAction *pStopAction = new QAction("暂停刷新",0);
    pStopAction->setIcon(QIcon(":/images/close.png"));
    pStopAction->setStatusTip("暂停刷新报文");

    QAction *pStartAction = new QAction("开始刷新",0);
    pStartAction->setIcon(QIcon(":/images/savesyn.png"));
    pStartAction->setStatusTip("开始刷新报文");

    QAction *pClearAction = new QAction("清空所有",0);
    pClearAction->setIcon(QIcon(":/images/delete.png"));
    pClearAction->setStatusTip("清空所有报文");

    m_pMenu->addAction(pStopAction);
    m_pMenu->addAction(pStartAction);
    m_pMenu->addAction(pClearAction);

    connect(pStopAction,SIGNAL(triggered()),this,SLOT(slot_StopRefresh()));
    connect(pStartAction,SIGNAL(triggered()),this,SLOT(slot_StartRefresh()));
    connect(pClearAction,SIGNAL(triggered()),this,SLOT(slot_ClearAll()));

    this->setFontPointSize(11);///设置显示字体的大小
}

void CTextBrowserDebug::slot_ShowMessageDebug(int nType_, QString strMessage)
{
    switch (nType_) {
    case DebugType_Undefined:
        PrintOutPutInfo(strMessage);
        break;
    case DebugType_Debug:
        printDebugInfo(strMessage);
        break;
    default:
        break;
    }
}

/*!
 \brief 停止刷新

 \fn CTextBrowserDebug::slot_StopRefresh
*/
void CTextBrowserDebug::slot_StopRefresh()
{
    /*!
    * \brief 功能概述
    * \param 参数描述 无
    * \return 返回值描述 无
    * \author zzy
    * \date 2015/1/9
    */

}

/*!
 \brief 开始刷新

 \fn CTextBrowserDebug::slot_StartRefresh
*/
void CTextBrowserDebug::slot_StartRefresh()
{
    /*!
    * \brief 功能概述
    * \param 参数描述 无
    * \return 返回值描述 无
    * \author zzy
    * \date 2015/1/9
    */

}

/*!
 \brief 清除显示

 \fn CTextBrowserDebug::slot_ClearAll
*/
void CTextBrowserDebug::slot_ClearAll()
{
    /*!
    * \brief 功能概述
    * \param 参数描述 无
    * \return 返回值描述 无
    * \author zzy
    * \date 2015/1/9
    */
    this->clear();
}
