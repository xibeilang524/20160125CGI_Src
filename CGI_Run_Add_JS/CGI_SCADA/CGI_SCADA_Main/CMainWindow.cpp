#include "CMainWindow.h"
#include "ui_CMainWindow.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include "CTreeItemThread.h"
#include <QTest>
#include <QFileDialog>
#include <QStandardPaths>
#include "../CGI_SCADA_include/CGI_SCADA_DLL_I.h"
#include "../CGI_SCADA_include/XmlParser/CXmlParser.h"
#include "UI_UpLoadDialog.h"
#include "UI_DownloadDialog.h"
#include "../CGI_SCADA_include/CGI_SCADA_define.h"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("CGI SCADA组态软件");
    m_pSplitter = NULL;
    m_pLeftDockWidget = NULL;
    m_pRightFrame = NULL;
    m_pTreeWidget = NULL;
    m_pRightLayout = NULL;
    m_pRightTabWidget = NULL;
    m_pDockWidgetFoot = NULL;
}

void CMainWindow::Init()
{
    m_pLeftDockWidget = new QDockWidget(QApplication::translate("SCADA", "北京微控工业网关", 0),this);

    QWidget *pWidget1 = new QWidget(this);
    pWidget1->resize( QSize( this->width()*0.8, this->height() ));
    QHBoxLayout *hBoxLayout = new QHBoxLayout(pWidget1);
//    m_pRightMainWindow = new QMainWindow(this);
//    m_pRightMainWindow->resize( QSize( this->width()*0.8, this->height() ));
//    m_pRightMainWindow->setStyleSheet("background-color: rgb(255, 255, 255);");

    m_pRightTabWidget = new QTabWidget;
    connect(m_pRightTabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(slot_tabCloseRequested(int)));
    connect(m_pRightTabWidget,SIGNAL(currentChanged(int)),this,SLOT(slot_RightTabWidgetCurrentChanged(int)));
    m_pRightTabWidget->setTabsClosable(true);
//    hBoxLayout->addWidget(m_pRightMainWindow);
    hBoxLayout->addWidget(m_pRightTabWidget);

//    m_pRightFrame = new QFrame(this);
//    qDebug()<<this->width()<<this->height();
//    m_pLeftDockWidget->resize(QSize( this->width()*0.2, this->height() ));
//    m_pRightFrame->resize( QSize( this->width()*0.8, this->height() ));
//    m_pRightLayout = new QHBoxLayout(m_pRightFrame);
//    m_pRightFrame->setLayout(m_pRightLayout);

    m_pSplitter = new QSplitter(Qt::Horizontal,this);
    m_pSplitter->addWidget(m_pLeftDockWidget);
//    m_pSplitter->addWidget(m_pRightFrame);
//    m_pSplitter->addWidget(pRightMainWindow);
    m_pSplitter->addWidget(pWidget1);
    this->setCentralWidget(m_pSplitter);

    QWidget *pWidget = new QWidget(this);
    m_pLeftDockWidget->setWidget(pWidget);

    QGridLayout *pGridLayout = new QGridLayout(pWidget);

    m_pTreeWidget = new QTreeWidget(pWidget);
    m_pTreeWidget->headerItem()->setText(0,"工程管理");
    m_pTreeWidget->setAlternatingRowColors(true);
    m_pTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);//允许弹菜单
    connect(m_pTreeWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slot_TreeRightClick(QPoint)));
    //使用itemPressed信号左键右键都会触发 比使用itemClicked效果好
    connect(m_pTreeWidget, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(slot_TreeLeftClicked(QTreeWidgetItem *, int)));

    pGridLayout->addWidget(m_pTreeWidget,0,0,1,1);
    m_pDockWidgetFoot = new CDockWidgetFoot(this);
    connect(this,SIGNAL(signal_ShowMessageDebug(int,QString)),m_pDockWidgetFoot,SIGNAL(signal_ShowMessageDebug(int,QString)));
    connect(this,SIGNAL(signal_ClearDebug()),m_pDockWidgetFoot,SIGNAL(signal_ClearDebug()));
    m_strProjectPath = QDir::currentPath()+"/project";
    //    emit signal_OpenProject(m_strProjectPath);
}

void CMainWindow::SetTreeItemDllNameList(QStringList strDllNameList_)
{
    qDebug()<<__func__<<strDllNameList_;
    int nTreeItemCount = 0;
    for (int i = 0; i < strDllNameList_.count();i +=2)
    {
        qDebug()<<__func__<<__LINE__<<strDllNameList_.at(i)<<strDllNameList_.at(i+1)<<(2*i + 1)<<strDllNameList_.count();
//        QTreeWidgetItem *pitem = new QTreeWidgetItem(m_pTreeWidget,QStringList()<<"Error:No",CGI_SCADA_DLL_I::TreeItemType_Undefined);
        qDebug()<<__func__<<__LINE__<<strDllNameList_.at(i)<<strDllNameList_.at(i+1);
//        m_pTreeWidget->insertTopLevelItem(0, pitem);
        qDebug()<<__func__<<__LINE__<<strDllNameList_.at(i)<<strDllNameList_.at(i+1);
        CTreeItemThread *pThread = new CTreeItemThread(this);
        qDebug()<<__func__<<__LINE__<<strDllNameList_.at(i)<<strDllNameList_.at(i+1);
        connect(this,SIGNAL(signal_OpenProject(QString)),pThread,SIGNAL(signal_OpenProject(QString)));
        connect(pThread,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
        qDebug()<<__func__<<__LINE__<<strDllNameList_.at(i)<<strDllNameList_.at(i+1);
        pThread->SetLibraryName(strDllNameList_.at(i));
        qDebug()<<__func__<<__LINE__<<strDllNameList_.at(i)<<strDllNameList_.at(i+1);
//        m_ThreadList.append(pThread);
        qDebug()<<__func__<<__LINE__<<strDllNameList_.at(i)<<strDllNameList_.at(i+1);
        if (pThread->SetStart())
        {
            CGI_SCADA_DLL_I *pitem = (CGI_SCADA_DLL_I*)(pThread->GetTreeItem());
            qDebug()<<__func__<<__LINE__<<strDllNameList_.at(i)<<strDllNameList_.at(i+1);
//            m_pTreeWidget->addTopLevelItem();
            m_pTreeWidget->insertTopLevelItem(nTreeItemCount++,pitem);
        }
    }
}

CMainWindow::~CMainWindow()
{
    qDebug()<<__func__<<__FILE__<<__LINE__<<"start m_pTreeWidget->clear()";
    if (m_pTreeWidget != NULL)
    {
        m_pTreeWidget->clear();
    }
    qDebug()<<__func__<<__FILE__<<__LINE__<<"start m_pRightTabWidget->clear()";
//    if (m_pRightTabWidget != NULL)
//    {
//        m_pRightTabWidget->clear();
//    }
    qDebug()<<__func__<<__FILE__<<__LINE__<<"start m_pRightTabWidget->clear() end";
    qDebug()<<__func__<<__FILE__<<__LINE__<<"start 2";
    delete ui;
    qDebug()<<__func__<<__FILE__<<__LINE__<<"end";
}

void CMainWindow::slot_TreeRightClick(QPoint point_)
{
    qDebug()<<"s_baCookie="<<CGI_SCADA_define::s_baCookie;
    QTreeWidgetItem *m_currentItem = m_pTreeWidget->itemAt(point_);/// 设置当前的书节点
    if (m_currentItem == NULL)
    {
        return;
    }
    if (m_currentItem->type() == CGI_SCADA_DLL_I::TreeItemType_Undefined)
    {
        return;
    }
    qDebug()<<m_currentItem->type()<<m_currentItem->text(0);

    ((CGI_SCADA_DLL_I*)m_currentItem)->GetPopUpMenu();

}

void CMainWindow::slot_TreeLeftClicked(QTreeWidgetItem *pItem, int)
{
    QTreeWidgetItem *m_currentItem = (QTreeWidgetItem *)pItem;/// 设置当前的树节点
    if (m_currentItem == NULL)
    {
        Q_ASSERT(false);
        return;
    }
    qDebug()<<__func__<<m_currentItem->type()<<m_currentItem->text(0);
    if (m_currentItem->type() == CGI_SCADA_DLL_I::TreeItemType_Undefined)
    {
        return;
    }
    QWidget *pWidget = ((CGI_SCADA_DLL_I*)pItem)->GetWidget(pItem);
    if (pWidget == NULL)
        return ;

    if (m_pRightTabWidget->indexOf(pWidget) >= 0)
    {
        qDebug()<<__func__<<__LINE__<<m_pRightTabWidget->indexOf(pWidget);
//        m_pRightTabWidget->indexOf(pWidget);
        m_pRightTabWidget->setCurrentWidget(pWidget);

    }else
    {
        qDebug()<<__func__<<__LINE__;
        m_pRightTabWidget->addTab(pWidget,pItem->text(0));
        m_TreeItem_WidgetMap.insert(m_currentItem,pWidget);
        m_pRightTabWidget->setCurrentWidget(pWidget);
    }
}

void CMainWindow::slot_tabCloseRequested(int index_)
{
    m_pRightTabWidget->removeTab(index_);
}

void CMainWindow::slot_RightTabWidgetCurrentChanged(int index_)
{
    QWidget *pWidget = m_pRightTabWidget->widget(index_);
    if (m_TreeItem_WidgetMap.key(pWidget) != NULL)
    {
        QList<QTreeWidgetItem *> selectedItemList = m_pTreeWidget->selectedItems();
        for (int i = 0; i < selectedItemList.count(); ++i)
        {
            selectedItemList.at(i)->setSelected(false);
        }
        m_TreeItem_WidgetMap.key(pWidget)->setSelected(true);
    }

}
void CMainWindow::DeletePathFile(QString strFilePath)
{
    QString strPath = strFilePath;
    qDebug()<<strPath;
    QDir dir(strPath);
    if (!dir.exists())
        return ;
    dir.setFilter(QDir::Dirs|QDir::Files);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int i=0;
    QString fileName;
    do{
        QFileInfo fileInfo = list.at(i);
        if((fileInfo.fileName()==".")|(fileInfo.fileName()==".."))
        {
            i++;
            continue;
        }
        bool bisDir=fileInfo.isDir();
        if(bisDir)
        {
            qDebug()<<(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName(),10).arg(fileInfo.path()))<<fileInfo.absoluteFilePath();
            DeletePathFile(fileInfo.filePath());
            dir.rmdir(fileInfo.filePath());
        }else{
            fileName = fileInfo.fileName();
            dir.remove(fileInfo.fileName());
            qDebug()<<(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName(),10).arg(fileInfo.path()))<<fileInfo.absoluteFilePath();
        }
        i++;
    }while(i<list.size());
}

/*!
 \brief 先清理temp_project中的文件，然后将保存的文件保存在temp_project目录中，然后再将目标目录清理干净，最后将temp_project目录中的文件copy到目标目录中

 \fn CMainWindow::on_actionSave_triggered
*/
void CMainWindow::on_actionSave_triggered()
{
    emit signal_ClearDebug();
    emit signal_ShowMessageDebug(0,QString("开始保存... %1").arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")));
    ///< 清理掉temp_project目录中的文件
    QDir tempDir("temp_project");
    if (tempDir.exists())
    {
        qDebug()<<__func__<<__LINE__<<"有";
        DeletePathFile(QDir::currentPath()+"/temp_project");
    }else
    {
        qDebug()<<__func__<<__LINE__<<"无";
        tempDir.mkdir(QDir::currentPath()+"/temp_project");
    }
//    qDebug()<<__func__<<__LINE__<<__FILE__<<m_pTreeWidget->topLevelItemCount();

    ///< 生成配置文件，生成在temp目录中
    for (int nTreeItemCount = 0; nTreeItemCount < m_pTreeWidget->topLevelItemCount(); ++nTreeItemCount)
    {
        if (m_pTreeWidget->topLevelItem(nTreeItemCount)->type() == CGI_SCADA_DLL_I::TreeItemType_Undefined)
        {
            continue;
        }else
        {
            ((CGI_SCADA_DLL_I*)m_pTreeWidget->topLevelItem(nTreeItemCount))->SaveAction();
        }
    }

    DeletePathFile(m_strProjectPath);///< 清理掉目标目录中的文件

    /// 将temp_project目录中的文件复制到目标目录
    if (!tempDir.exists())
        return ;
    tempDir.setFilter(QDir::Dirs|QDir::Files);
    tempDir.setSorting(QDir::DirsFirst);
    QFileInfoList list = tempDir.entryInfoList();
    int i=0;
    do{
        QFileInfo fileInfo = list.at(i);
        if((fileInfo.fileName()==".")|(fileInfo.fileName()==".."))
        {
            i++;
            continue;
        }
        bool bisDir=fileInfo.isDir();
        if(bisDir)
        {
            qDebug()<<(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName(),10).arg(fileInfo.path()))<<fileInfo.absoluteFilePath();
        }else{
            qDebug()<<(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName(),10).arg(fileInfo.path()))<<fileInfo.absoluteFilePath()<<fileInfo.baseName();
            QFile::copy(fileInfo.absoluteFilePath(),QString("%1/%2").arg(m_strProjectPath).arg(fileInfo.fileName()));
        }
        i++;
    }while(i<list.size());
    emit signal_ShowMessageDebug(0,QString("保存完毕"));
}

/*!
 \brief 新建工程目录

 \fn CMainWindow::on_actionNew_triggered
*/
void CMainWindow::on_actionNew_triggered()
{
    QFileDialog fileDialog;
    QString strProFilename = fileDialog.getSaveFileName(this,
               tr("Save file"),
            QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), tr(""),0,QFileDialog::ShowDirsOnly);
    if (strProFilename.isEmpty())
    {
        return;
    }else
    {
        qDebug()<<strProFilename;
        QDir dir(strProFilename);
        dir.mkdir(strProFilename);
        m_strProjectPath = strProFilename;
        emit signal_OpenProject(m_strProjectPath);
        this->setWindowTitle(m_strProjectPath);
    }
}

void CMainWindow::on_actionOpen_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly);
    if (dir.isEmpty())
    {
        return;
    }else
    {
        qDebug()<<dir;
        m_strProjectPath = dir;
        emit signal_OpenProject(m_strProjectPath);
        this->setWindowTitle(m_strProjectPath);
    }

}

/*!
 \brief 压缩槽函数

 \fn CMainWindow::on_actionCompress_triggered
*/
void CMainWindow::on_actionCompress_triggered()
{
    QDir dir = QDir::current();
    dir.remove("project.tar");
    dir.remove("project.tar.gz");

    system("7z.exe a project.tar project");
    system("7z.exe a project.tar.gz project.tar");
}

/*!
 \brief 解压缩槽函数

 \fn CMainWindow::on_actionExtract_triggered
*/
void CMainWindow::on_actionExtract_triggered()
{
    DeletePathFile("project");
    QDir dir = QDir::current();
    dir.remove("project.tar");
    system("7z.exe x project.tar.gz -y");
    system("7z.exe x project.tar -y");
}

/*!
 \brief 上传

 \fn CMainWindow::on_actionUpload_triggered
*/
void CMainWindow::on_actionUpload_triggered()
{
    UI_UpLoadDialog dialog;
    dialog.exec();
}

/*!
 \brief 下载

 \fn CMainWindow::on_actionDownload_triggered
*/
void CMainWindow::on_actionDownload_triggered()
{
    UI_DownloadDialog dialog;
    dialog.exec();
}

/*!
 \brief 编译，校验

 \fn CMainWindow::on_actionBuild_triggered
*/
void CMainWindow::on_actionBuild_triggered()
{
    emit signal_ShowMessageDebug(0,tr("开始编译... %1").arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")));
    m_TagMap.clear();
    for (int nTreeItemCount = 0; nTreeItemCount < m_pTreeWidget->topLevelItemCount(); ++nTreeItemCount)
    {
        if (m_pTreeWidget->topLevelItem(nTreeItemCount)->type() == CGI_SCADA_DLL_I::TreeItemType_Undefined)
        {
            continue;
        }else
        {
            ((CGI_SCADA_DLL_I*)m_pTreeWidget->topLevelItem(nTreeItemCount))->SetInitTagMap(m_TagMap);
        }
    }
    for (int nTreeItemCount = 0; nTreeItemCount < m_pTreeWidget->topLevelItemCount(); ++nTreeItemCount)
    {
        if (m_pTreeWidget->topLevelItem(nTreeItemCount)->type() == CGI_SCADA_DLL_I::TreeItemType_Undefined)
        {
            continue;
        }else
        {
            ((CGI_SCADA_DLL_I*)m_pTreeWidget->topLevelItem(nTreeItemCount))->SetBuildTagMap(m_TagMap);
        }
    }
    emit signal_ShowMessageDebug(0,tr("编译结束"));
}
