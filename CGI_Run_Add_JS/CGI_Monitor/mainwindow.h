#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CStart.h"
#include <QTabWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDockWidget>
#include <QTableView>
#include <QTreeView>
#include "CChannelMessage.h"
#include "CTreeWidgetItem.h"
#include "CDeviceModelMessage.h"
#include "CProtocolFileMessage.h"
#include "CChannelThread_CGI.h"
#include <QTimerEvent>
#include "CMyTextBrowserSocket.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    CStart *m_pStart;
    void SetInitWidget();///初始化主窗口
    void timerEvent(QTimerEvent *event);
    int m_nDataFlushTimerID;

protected slots:
    void slot_DisplayFrame();
    void slot_CloseTextBrowserSocket(CMyTextBrowserSocket *pTextBrowserSocket);
private slots:
    void Slot_TreeRightClick(QPoint point_);
    void Slot_itemPressed(QTreeWidgetItem *item, int column);
private:
    Ui::MainWindow *ui;

    QStringList m_ComFileList;
    QStringList m_NetFileList;
    QStringList m_CANFileList;

    QDockWidget *m_pDockWidget;///左侧DockWidget
    QTreeWidget *m_pTreeWidget;
    QTabWidget *m_pTabWidget;///右侧tab页
    QTreeView  *m_pTreeView_Pro;/// 属性页
    QTableView *m_pTab_YX;
    QTableView *m_pTab_YC;
    QTableView *m_pTab_YM;
    QTableView *m_pTab_YK;
    QTableView *m_pTab_YS;
    QList<CChannelThread_CGI* >  m_ThreadList;
    QTreeWidgetItem *m_pCurrentItem;
    QList<CMyTextBrowserSocket *> m_CMyTextBrowserSocketList;

};

#endif // MAINWINDOW_H
