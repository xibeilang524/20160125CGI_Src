/*!
 * \file    CMyTextBrowser.cpp
 * \brief   概述 报文显示窗口
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/1/16
 *
 * \b       修改记录：
 * \li      2015/1/16
 *  添加注释
 */

#include "cmytextbrowser.h"
#include <QMenu>
#include <QAction>
#include <QDebug>
#include "Src/cmyfunction.h"
#include <QDateTime>

CMyTextBrowser::CMyTextBrowser(QTextBrowser *parent) :
    QTextBrowser(parent)
{
    /*!
    * \brief  功能概述 构造函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/1/16
    */
    this->document()->setMaximumBlockCount(1000);
    pMenu = new QMenu("右键菜单",this);
    pStopAction = new QAction("暂停刷新",0);
    pStopAction->setIcon(QIcon(":/images/close.png"));
    pStopAction->setStatusTip("暂停刷新报文");

    pStartAction = new QAction("开始刷新",0);
    pStartAction->setIcon(QIcon(":/images/savesyn.png"));
    pStartAction->setStatusTip("开始刷新报文");

    pClearAction = new QAction("清空所有",0);
    pClearAction->setIcon(QIcon(":/images/delete.png"));
    pClearAction->setStatusTip("清空所有报文");

    pSaveFrame = new QAction("保存报文",0);
//    pSaveFrame->setIconText(QIcon(""));
    pSaveFrame->setStatusTip("保存此通道的报文");
    pSaveFrame->setCheckable(true);

    m_pSignFrame = new QAction("标记报文",0);
    m_pSignFrame->setStatusTip("标记特殊报文");

    pMenu->addAction(pStopAction);
    pMenu->addAction(pStartAction);
    pMenu->addAction(pClearAction);
    pMenu->addAction(pSaveFrame);
    pMenu->addAction(m_pSignFrame);

    connect(pStopAction,SIGNAL(triggered()),this,SLOT(OnStopRefresh()));
    connect(pStartAction,SIGNAL(triggered()),this,SLOT(OnStartRefresh()));
    connect(pClearAction,SIGNAL(triggered()),this,SLOT(OnClearAll()));
    connect(pSaveFrame,SIGNAL(triggered(bool)),this,SLOT(OnSaveFrame(bool)));
    connect(m_pSignFrame,SIGNAL(triggered()),this,SLOT(OnSignFrame()));

    //初始状态
    //pStopAction->setDisabled(1);
    pStartAction->setDisabled(1);
    pClearAction->setDisabled(1);

    bPauseFlag = false;

    // 当 true 时，显示背景颜色
    // 当 false 时，字体带颜色
    m_bTextColor = false;
//    nClearCounter = 0;
    m_bSaveMsgFlag = true;
    m_bSaveFrame = false;

    setFontPointSize(11);///设置显示字体的大小
}

void CMyTextBrowser::closeEvent(QCloseEvent *)
{
    /*!
    * \brief  功能概述 关闭事件函数
    * \param  参数描述
    * \return 返回值描述 无
    * \author zzy
    * \date   2015/1/16
    */
    CMyFunction::IsShowTextBrowserWindow = false;
//    qDebug()<<"CMyFunction::IsShowTextBrowserWindow = false"<<"报文窗口的关闭事件函数";
}

void CMyTextBrowser::contextMenuEvent(QContextMenuEvent* event)
{
    /*!
    * \brief  功能概述 右键弹出菜单
    * \param  参数描述
    * \return 返回值描述 无
    * \author zzy
    * \date   2015/1/16
    */
   QCursor cur=this->cursor();
   pMenu->exec(cur.pos());
}

void CMyTextBrowser::printDebugInfo(QString str)
{
    /*!
    * \brief  功能概述 打印Debug信息
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/1/16
    */
    str.toUpper();/// @note 小写转大写
    if(getPauseFlag())/// @note 如果是暂定，则不刷文字及报文
        return;

    setTextBackgroundColor(QColor(255,255,255));/// @note 调试信息一律为红色
    setTextColor(QColor(255,0,0));
    append(str);
}
//
void CMyTextBrowser::PrintOutPutInfo(QString str, QColor color)
{
    /*!
    * \brief  功能概述 打印自定义颜色的信息
    * \param  参数描述 str打印的信息,color字颜色
    * \return 返回值描述 无
    * \author zzy
    * \date   2015/1/16
    */
    str.toUpper();/// @note 小写转大写
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz");
    if(getPauseFlag())/// @note 如果是暂定,则不刷文字及报文
       return;

    if(m_bTextColor)
    {
        setTextBackgroundColor(color);
        setTextColor(QColor(125,125,125));
        append(strDateTime);
//        setTextBackgroundColor(color);
        setTextColor(QColor(255,0,0));
        append(str);
    }
    else
    {
        setTextBackgroundColor(QColor(255,255,255));
        setTextColor(QColor(125,125,125));
        append(strDateTime);
        setTextColor(color);
        append(str);
    }
    if (m_bSaveFrame)
    {
        emit SignalSaveFrame(strDateTime+" "+str);
    }
}

void CMyTextBrowser::PrintRXFrame(QString str, QColor color)
{
    /*!
    * \brief  功能概述 打印接收报文
    * \param  参数描述 str打印的信息,color字颜色
    * \return 返回值描述 无
    * \author zzy
    * \date   2015/1/16
    */
    str.toUpper();///小写转大写
//    nClearCounter++;
//    if(nClearCounter > 512)
//    {
//        nClearCounter = 0;
//        clear();
//    }
    ////////保存报文////////
//    if(m_bSaveMsgFlag) SaveMsg(str,1);
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz");
    QString strForWord = "接收 <---: ";
    strForWord += str;
    if(getPauseFlag()) return;

     if(m_bTextColor)
     {
         setTextBackgroundColor(color);
         setTextColor(QColor(125,125,125));
         append(strDateTime);
         setTextColor(QColor(255,0,0));
         append(strForWord);
     }
     else
     {
         setTextBackgroundColor(QColor(255,255,255));
         setTextColor(QColor(125,125,125));
         append(strDateTime);
         setTextColor(color);
         append(strForWord);
     }
     if (m_bSaveFrame)
     {
         emit SignalSaveFrame(strDateTime+" "+strForWord);
     }

}

void CMyTextBrowser::PrintTXFrame(QString str, QColor color)
{
    /*!
    * \brief  功能概述 打印发送报文
    * \param  参数描述 str打印的信息,color字颜色
    * \return 返回值描述 无
    * \author zzy
    * \date   2015/1/16
    */
    str.toUpper();///小写转大写
//    nClearCounter++;
//    if(nClearCounter > 512)
//    {
//        nClearCounter = 0;
//        clear();
//    }
    ////////保存报文////////
//    if(m_bSaveMsgFlag) SaveMsg(str,2);
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz");
    QString strBackWord = "发送 --->: ";
    strBackWord += str;
    if(getPauseFlag()) return;

     if(m_bTextColor)
     {
         setTextBackgroundColor(color);
         setTextColor(QColor(125,125,125));
         append(strDateTime);
         setTextColor(QColor(255,0,0));
         append(strBackWord);
     }
     else
     {
         setTextBackgroundColor(QColor(255,255,255));
         setTextColor(QColor(125,125,125));
         append(strDateTime);
         setTextColor(color);
         append(strBackWord);
     }
     if (m_bSaveFrame)
     {
         emit SignalSaveFrame(strDateTime+" "+strBackWord);
     }
}

void CMyTextBrowser::OnStopRefresh()
{
    /*!
    * \brief  功能概述 暂停刷新
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/1/16
    */
    pStopAction->setDisabled(1);
    pStartAction->setEnabled(1);
    pClearAction->setEnabled(1);

    bPauseFlag = true;
}

void CMyTextBrowser::OnStartRefresh()
{
    /*!
    * \brief  功能概述 开始刷新
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/1/16
    */
    pStartAction->setDisabled(1);
    pClearAction->setEnabled(1);
    pStopAction->setEnabled(1);

    bPauseFlag = false;
}

void CMyTextBrowser::OnClearAll()
{
    /*!
    * \brief  功能概述 清空
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/1/16
    */
    pClearAction->setDisabled(1);
    pStopAction->setDisabled(1);
    pStartAction->setEnabled(1);

    bPauseFlag = true;
    this->clear();
}

void CMyTextBrowser::OnSaveFrame(bool bSaveFrame)
{
    m_bSaveFrame = bSaveFrame;
}

void CMyTextBrowser::OnSignFrame()
{
    /*!
    * \brief  功能概述 发送特殊报文显示信号
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    emit SignalSignFrame();
}
