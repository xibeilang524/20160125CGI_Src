/*!
 * \file  CTextBrowserDebug.h
 * \brief 概述 CTextBrowserDebug类的头文件
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

#ifndef CTEXTBROWSERDEBUG_H
#define CTEXTBROWSERDEBUG_H

#include <QTextBrowser>

/*!
 \brief 显示Debug信息的类,用于显示文本信息的类

 \class CTextBrowserDebug CTextBrowserDebug.h "SCADA/TextBrowser/CTextBrowserDebug.h"
 \author zzy
 \date   2015/5/29
*/
class CTextBrowserDebug : public QTextBrowser
{
    Q_OBJECT
public:
    explicit CTextBrowserDebug(QWidget *parent = 0);
    void PrintOutPutInfo(const QString &str, const QColor &color);
    void PrintOutPutInfo(const QString &str);
    void printDebugInfo(const QString &str);
    void printDebugInfo(const QString &str, QColor color);

    void contextMenuEvent(QContextMenuEvent* event);
signals:

private:
    void SetInit();

public slots:
    void slot_ShowMessageDebug(int nType_,QString strMessage);
    void slot_StopRefresh();
    void slot_StartRefresh();
    void slot_ClearAll();
private:
    QMenu* m_pMenu;

    enum DebugType{
        DebugType_Undefined = 0,
        DebugType_Debug,
    };

};

#endif // CTEXTBROWSERDEBUG_H
