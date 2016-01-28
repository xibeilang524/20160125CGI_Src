#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QSplitter>
#include <QFrame>
#include <QTreeWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include "CDockWidgetFoot.h"


namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();
    void Init();
    void SetTreeItemDllNameList(QStringList strDllNameList_);
signals:
    void signal_ClearDebug();
    void signal_ShowMessageDebug(int,QString);
    void signal_OpenProject(QString strProjectPath);
protected slots:
private slots:
    void slot_TreeRightClick(QPoint point_);
    void slot_TreeLeftClicked(QTreeWidgetItem *pItem, int);
    void slot_tabCloseRequested(int index_);
    void slot_RightTabWidgetCurrentChanged(int index_);
    void on_actionSave_triggered();
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionCompress_triggered();
    void on_actionExtract_triggered();
    void on_actionUpload_triggered();
    void on_actionDownload_triggered();

    void on_actionBuild_triggered();

    void on_actionReadMe_triggered();

private:

private:
    Ui::CMainWindow *ui;
    QSplitter *m_pSplitter;
    QDockWidget *m_pLeftDockWidget;
//    QMainWindow *m_pRightMainWindow;
    QFrame *m_pRightFrame;
    QTreeWidget *m_pTreeWidget;
    QStringList m_DllFileList;
    QList<QThread *> m_ThreadList;
    QHBoxLayout *m_pRightLayout;
    QTabWidget *m_pRightTabWidget;
    QMap<QTreeWidgetItem *, QWidget *> m_TreeItem_WidgetMap;
    QString m_strProjectPath;
    CDockWidgetFoot *m_pDockWidgetFoot;

    void DeletePathFile(QString strFilePath);
    QMap<QString,QStringList> m_TagMap;///< 用于校验
};

#endif // CMAINWINDOW_H
