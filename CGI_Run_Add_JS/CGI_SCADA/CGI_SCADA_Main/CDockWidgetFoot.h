/*!
 * \file  CDockWidgetFoot.h
 * \brief 概述 CDockWidgetFoot类的头文件
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

#ifndef CDOCKWIDGETFOOT_H
#define CDOCKWIDGETFOOT_H

#include <QDockWidget>

class CMainWindow;
class CTextBrowserDebug;

/*!
 \brief 主界面下方用于打印信息的DockWidget类

 \class CDockWidgetFoot CDockWidgetFoot.h "SCADA/DockWidget/CDockWidgetFoot.h"
 \author zzy
 \date   2015/5/29
*/
class CDockWidgetFoot : public QDockWidget
{
    Q_OBJECT
public:
    explicit CDockWidgetFoot(QWidget *parent = 0);
    CDockWidgetFoot(CMainWindow *parent = 0);
    ~CDockWidgetFoot();
    CTextBrowserDebug *GetTextBrowserDebug() const ;

signals:
    void signal_ClearDebug();
    void signal_ShowMessageDebug(int,QString);

private slots:
    void slot_ShowMessageDebug(int,QString);
private:
    void SetInitWidget();
private:
    CMainWindow *m_pMainWimdows;
    CTextBrowserDebug *m_pTextBrowserDebug;

};

#endif // CDOCKWIDGETFOOT_H
