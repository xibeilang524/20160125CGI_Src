/*!
 * \file    CMyTextBrowser.h
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

#ifndef CMYTEXTBROWSER_H
#define CMYTEXTBROWSER_H

#include <QTextBrowser>

class QMenu;
class QAction;

/*!
 * \class  CMyTextBrowser
 * \brief  概述 报文显示窗口
 * \author zzy
 * \date   2015/1/16
 * 详细概述
 */
class CMyTextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit CMyTextBrowser(QTextBrowser *parent = 0);
    /// add zhang
    void closeEvent(QCloseEvent *);
    ///
    ///
public:
    QMenu* pMenu;
    void contextMenuEvent(QContextMenuEvent* event);
    bool getPauseFlag()const { return bPauseFlag ;}
    //////输出显示信息///////
    virtual void printDebugInfo(QString str);
    virtual void PrintOutPutInfo(QString str, QColor color);
    virtual void PrintRXFrame(QString str, QColor color);
    virtual void PrintTXFrame(QString str, QColor color);

signals:
    void SignalSaveFrame(QString str);
    void SignalSignFrame();/// @note 特殊报文显示信号

public slots:
    void OnStopRefresh();
    void OnStartRefresh();
    void OnClearAll();
    void OnSaveFrame(bool bSaveFrame);
    void OnSignFrame();/// @note 特殊报文显示槽函数
protected:
    QAction* pStopAction;
    QAction* pStartAction;
    QAction* pClearAction;
    QAction* pSaveFrame;
    QAction* m_pSignFrame;
    bool bPauseFlag;
    bool m_bTextColor;
//    int nClearCounter;
    bool m_bSaveMsgFlag;
    bool m_bSaveFrame;

};

#endif // CMYTEXTBROWSER_H
