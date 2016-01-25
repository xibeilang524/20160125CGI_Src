#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSplitter>
#include "Src/cmyfunction.h"
#include "Src/crtdbbase.h"
#include "Src/CPRTMapBase.h"
#include "Src/CPRTVBase.h"
#include "../PortInclude/CTagI.h"
#include "Src/ctagbase.h"
#include "../PortInclude/CPointI.h"
extern CRTDBBase g_RealTimeDB;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pStart(new CStart),
    m_nDataFlushTimerID(0),
    m_pCurrentItem(NULL)
{
    ui->setupUi(this);
    m_ComFileList = m_pStart->GetFileList(ChannelType_COM);
    m_NetFileList = m_pStart->GetFileList(ChannelType_NET);
    m_CANFileList = m_pStart->GetFileList(ChannelType_CAN);

    this->setWindowTitle("CGI 监视");


    SetInitWidget();///初始化主窗口的widget
    m_nDataFlushTimerID = startTimer(500);
}

MainWindow::~MainWindow()
{
    qDebug()<<__func__<<"xigou";
    if (m_ThreadList.count())
    {
        for (int i = 0; i < m_ThreadList.count(); ++i)
        {
            m_ThreadList.at(i)->quit();
        }
    }
    delete ui;
}

void MainWindow::SetInitWidget()
{

    m_pDockWidget = new QDockWidget("导航器",this);

    m_pTreeWidget = new QTreeWidget;
    m_pTreeWidget->headerItem()->setText(0,"Channel");
    m_pTreeWidget->setAlternatingRowColors(true);
    m_pTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);//允许弹菜单
    connect(m_pTreeWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(Slot_TreeRightClick(QPoint)));
    connect(m_pTreeWidget,SIGNAL(itemPressed(QTreeWidgetItem*,int)),this,SLOT(Slot_itemPressed(QTreeWidgetItem*,int)));

    m_pDockWidget->setWidget(m_pTreeWidget);

    QSplitter *mainspltr = new QSplitter(Qt::Horizontal,this);

    m_pTabWidget = new QTabWidget(this);
    m_pTreeView_Pro = new QTreeView(m_pTabWidget);
    m_pTab_YX = new QTableView(m_pTabWidget);
    m_pTab_YC = new QTableView(m_pTabWidget);
    m_pTab_YM = new QTableView(m_pTabWidget);
    m_pTab_YK = new QTableView(m_pTabWidget);
    m_pTab_YS = new QTableView(m_pTabWidget);
    m_pTabWidget->addTab(m_pTreeView_Pro,"属性");
    m_pTabWidget->addTab(m_pTab_YX,"遥信");
    m_pTabWidget->addTab(m_pTab_YC,"遥测");
    m_pTabWidget->addTab(m_pTab_YM,"遥脉");
    m_pTabWidget->addTab(m_pTab_YK,"遥控");
    m_pTabWidget->addTab(m_pTab_YS,"遥设");

    mainspltr->addWidget(m_pDockWidget);
//    mainspltr->addWidget(m_pTreeWidget);
    mainspltr->addWidget(m_pTabWidget);
    mainspltr->setStretchFactor(0,1);
    mainspltr->setStretchFactor(1,3);
    setCentralWidget(mainspltr);



    int ChannelNumber = 0;
    if (!m_ComFileList.isEmpty())
    {
        for (int i = 0; i < m_ComFileList.count(); ++i)
        {
            QDomDocument doc;
            if (CXmlParser::readFile(m_ComFileList.at(i),doc))
            {
                QFileInfo fileInfo(m_ComFileList.at(i));
                QString str = fileInfo.fileName();
                QString tmp;
                for(int j = 0; j < str.length(); j++)
                {
                    if (str[j] >= '0' && str[j] <= '9')
                    {
                        tmp.append(str[j]);
                    }
                }
                QDomNodeList RootNodeList =  doc.elementsByTagName("Root");
                for (int RootCount = 0; RootCount < RootNodeList.count(); ++RootCount)
                {
                    CTreeWidgetItem *pTreeWidgetItem = new CTreeWidgetItem(ChannelNumber++,str,RootNodeList.at(RootCount).toElement(),m_pTreeWidget,TreeWidgetItem_Channel);
                    qDebug()<<"Com Count"<<RootCount;
//                    m_ChannelElement.insert(ChannelNumber++,RootNodeList.at(RootCount).toElement());
//                    CChannelThread_CGI *pThread = new CChannelThread_CGI(ChannelNumber++,ChannelType_COM,tmp.toInt(),this);
//                    pThread->SetDomElement(RootNodeList.at(RootCount).toElement());
//                    pThread->start();
                }
            }
        }
    }
    if (!m_NetFileList.isEmpty())
    {
        for (int i = 0; i < m_NetFileList.count(); ++i)
        {
            QFileInfo fileInfo(m_NetFileList.at(i));
            QString str = fileInfo.fileName();
            QTreeWidgetItem *pItem = new QTreeWidgetItem(m_pTreeWidget,QStringList()<<str,TreeWidgetItem_File);
            pItem->setIcon(0,QIcon("://Image/ico/channel.ico"));
            QDomDocument doc;
            if (CXmlParser::readFile(m_NetFileList.at(i),doc))
            {
                QDomNodeList LinkNodeList =  doc.elementsByTagName("Link");
                for (int LinkCount = 0; LinkCount < LinkNodeList.count(); ++LinkCount)
                {
                    CTreeWidgetItem *pTreeWidgetItem = new CTreeWidgetItem(ChannelNumber++,LinkNodeList.at(LinkCount).toElement(),pItem,TreeWidgetItem_Channel);
//                    m_ChannelElement.insert(ChannelNumber++,LinkNodeList.at(LinkCount).toElement());
                    qDebug()<<"LinkCount"<<LinkCount;
//                    CChannelThread_CGI *pThread = new CChannelThread_CGI(ChannelNumber++,ChannelType_NET,this);
//                    pThread->SetDomElement(LinkNodeList.at(LinkCount).toElement());
//                    pThread->start();
                }
            }
        }
    }
    {
        CChannelThread_CGI *pThread = new CChannelThread_CGI(ChannelNumber++,ChannelType_DeliverMonitor104,this);
        pThread->InitChannel();
        pThread->SetMonitorIPAddress(CMyFunction::MonitorIPAddress);
        pThread->start();
        m_ThreadList.append(pThread);
    }
    m_pTreeWidget->expandAll();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nDataFlushTimerID)
    {
        if (m_pTabWidget->currentIndex() != 0)
        {
            switch (m_pTabWidget->currentIndex()) {
            case 1:/// 遥信
            {
                QStandardItemModel *pModel =  (QStandardItemModel *)m_pTab_YX->model();
                if (pModel != NULL)
                {
                    int nColumnCount = pModel->columnCount();
                    if (nColumnCount > 3)
                    {
                        for (int i = 0; i < pModel->rowCount(); ++i)
                        {
                            QString strLink = pModel->item(i,TagLinkColumn)->text();
                            CTagI *pTempTagI = g_RealTimeDB.FindTag(strLink);
                            if (pTempTagI)
                            {
                                if (pTempTagI->GetPointCollection())
                                {
                                    if (pTempTagI->GetPointCollection()->GetValue())
                                    {
                //                        qDebug()<<"遥信:"<<pTempTagI->GetPointCollection()->GetValue()->GetVarValue().toString()<<TagName;
                                        pModel->setData(pModel->index(i, TagOriginalColumn, QModelIndex()),pTempTagI->GetPointCollection()->GetValue()->GetVarValue().toString().toFloat());
                                    }
                                }
                                pModel->setData(pModel->index(i, TagProjectColumn, QModelIndex()),pTempTagI->GetProjectValue()->GetVarValue().toString().toFloat());
                            }
                            qDebug()<<"strLink"<<i<<strLink;
                        }
                    }
                }
            }
                break;
            case 2:/// 遥测
            {
                QStandardItemModel *pModel =  (QStandardItemModel *)m_pTab_YC->model();
                if (pModel != NULL)
                {
                    int nColumnCount = pModel->columnCount();
                    if (nColumnCount > 3)
                    {
                        for (int i = 0; i < pModel->rowCount(); ++i)
                        {
                            QString strLink = pModel->item(i,TagLinkColumn)->text();
                            CTagI *pTempTagI = g_RealTimeDB.FindTag(strLink);
                            if (pTempTagI)
                            {
                                if (pTempTagI->GetPointCollection())
                                {
                                    if (pTempTagI->GetPointCollection()->GetValue())
                                    {
                //                        qDebug()<<"遥信:"<<pTempTagI->GetPointCollection()->GetValue()->GetVarValue().toString()<<TagName;
                                        pModel->setData(pModel->index(i, TagOriginalColumn, QModelIndex()),pTempTagI->GetPointCollection()->GetValue()->GetVarValue().toString().toFloat());
                                    }
                                }
                                pModel->setData(pModel->index(i, TagProjectColumn, QModelIndex()),pTempTagI->GetProjectValue()->GetVarValue().toString().toFloat());
                            }
                            qDebug()<<"strLink"<<i<<strLink;
                        }
                    }
                }
            }
                break;
            case 3:/// 遥脉
            {
                QStandardItemModel *pModel =  (QStandardItemModel *)m_pTab_YM->model();
                if (pModel != NULL)
                {
                    int nColumnCount = pModel->columnCount();
                    if (nColumnCount > 3)
                    {
                        for (int i = 0; i < pModel->rowCount(); ++i)
                        {
                            QString strLink = pModel->item(i,TagLinkColumn)->text();
                            CTagI *pTempTagI = g_RealTimeDB.FindTag(strLink);
                            if (pTempTagI)
                            {
                                if (pTempTagI->GetPointCollection())
                                {
                                    if (pTempTagI->GetPointCollection()->GetValue())
                                    {
                //                        qDebug()<<"遥信:"<<pTempTagI->GetPointCollection()->GetValue()->GetVarValue().toString()<<TagName;
                                        pModel->setData(pModel->index(i, TagOriginalColumn, QModelIndex()),pTempTagI->GetPointCollection()->GetValue()->GetVarValue().toString().toFloat());
                                    }
                                }
                                pModel->setData(pModel->index(i, TagProjectColumn, QModelIndex()),pTempTagI->GetProjectValue()->GetVarValue().toString().toFloat());
                            }
                            qDebug()<<"strLink"<<i<<strLink;
                        }
                    }
                }
            }
                break;
            case 4:/// 遥控
            {
                QStandardItemModel *pModel =  (QStandardItemModel *)m_pTab_YK->model();
                if (pModel != NULL)
                {
                    int nColumnCount = pModel->columnCount();
                    if (nColumnCount > 3)
                    {
                        for (int i = 0; i < pModel->rowCount(); ++i)
                        {
                            QString strLink = pModel->item(i,TagLinkColumn)->text();
                            CTagI *pTempTagI = g_RealTimeDB.FindTag(strLink);
                            if (pTempTagI)
                            {
                                if (pTempTagI->GetPointCollection())
                                {
                                    if (pTempTagI->GetPointCollection()->GetValue())
                                    {
                //                        qDebug()<<"遥信:"<<pTempTagI->GetPointCollection()->GetValue()->GetVarValue().toString()<<TagName;
                                        pModel->setData(pModel->index(i, TagOriginalColumn, QModelIndex()),pTempTagI->GetPointCollection()->GetValue()->GetVarValue().toString().toFloat());
                                    }
                                }
                                pModel->setData(pModel->index(i, TagProjectColumn, QModelIndex()),pTempTagI->GetProjectValue()->GetVarValue().toString().toFloat());
                            }
                            qDebug()<<"strLink"<<i<<strLink;
                        }
                    }
                }
            }
                break;
            case 5:/// 遥设
            {
                QStandardItemModel *pModel =  (QStandardItemModel *)m_pTab_YS->model();
                if (pModel != NULL)
                {
                    int nColumnCount = pModel->columnCount();
                    if (nColumnCount > 3)
                    {
                        for (int i = 0; i < pModel->rowCount(); ++i)
                        {
                            QString strLink = pModel->item(i,TagLinkColumn)->text();
                            CTagI *pTempTagI = g_RealTimeDB.FindTag(strLink);
                            if (pTempTagI)
                            {
                                if (pTempTagI->GetPointCollection())
                                {
                                    if (pTempTagI->GetPointCollection()->GetValue())
                                    {
                //                        qDebug()<<"遥信:"<<pTempTagI->GetPointCollection()->GetValue()->GetVarValue().toString()<<TagName;
                                        pModel->setData(pModel->index(i, TagOriginalColumn, QModelIndex()),pTempTagI->GetPointCollection()->GetValue()->GetVarValue().toString().toFloat());
                                    }
                                }
                                pModel->setData(pModel->index(i, TagProjectColumn, QModelIndex()),pTempTagI->GetProjectValue()->GetVarValue().toString().toFloat());
                            }
                            qDebug()<<"strLink"<<i<<strLink;
                        }
                    }
                }
            }
                break;
            default:
                break;
            }
        }
    }
}

void MainWindow::Slot_TreeRightClick(QPoint point_)
{
    QTreeWidgetItem *pItem = m_pTreeWidget->itemAt(point_);/// 设置当前的树节点
     if (pItem == NULL)
     {
 //        Q_ASSERT(false);
         return;
     }
     m_pCurrentItem = pItem;
     switch (m_pTabWidget->currentIndex()) {
     case 0:/// 属性
         m_pTreeView_Pro->setModel(((CTreeWidgetItem*)pItem)->GetModel(ChannelMessage_TAG_TYPE_PRO));
         break;
     case 1:/// 遥信
         m_pTreeView_Pro->setModel(((CTreeWidgetItem*)pItem)->GetModel(ChannelMessage_TAG_TYPE_DI));
         break;
     case 2:/// 遥测
         m_pTreeView_Pro->setModel(((CTreeWidgetItem*)pItem)->GetModel(ChannelMessage_TAG_TYPE_AI));
         break;
     case 3:/// 遥脉
         m_pTreeView_Pro->setModel(((CTreeWidgetItem*)pItem)->GetModel(ChannelMessage_TAG_TYPE_MEM));
         break;
     case 4:/// 遥控
         m_pTreeView_Pro->setModel(((CTreeWidgetItem*)pItem)->GetModel(ChannelMessage_TAG_TYPE_DO));
         break;
     case 5:/// 遥设
         m_pTreeView_Pro->setModel(((CTreeWidgetItem*)pItem)->GetModel(ChannelMessage_TAG_TYPE_AO));
         break;
     default:
         break;
     }
     qDebug()<<"Pitem->type"<<pItem->type();
     switch (pItem->type()) {
     case TreeWidgetItem_Channel:
     {
         QMenu *pMenu = new QMenu(this);
         //->添加网络节点

         QAction *pDisplayFrame = new QAction(tr("显示报文"), pMenu);
         connect(pDisplayFrame, SIGNAL(triggered()), this, SLOT(slot_DisplayFrame()));

         pMenu->addAction(pDisplayFrame);
         pMenu->addSeparator();
         //-> 运行菜单
         pMenu->exec(QCursor::pos());
         delete pMenu;//防止内存泄露
     }
         break;
     case TreeWidgetItem_File:
     case TreeWidgetItem_Device:
         return;
         break;
     default:
         break;
     }
}

void MainWindow::slot_DisplayFrame()
{
    int nChannelNumber;
    switch(m_pCurrentItem->type())
    {
    case TreeWidgetItem_Channel:
    {
        nChannelNumber = ((CTreeWidgetItem *)m_pCurrentItem)->GetChannelNumber();
        qDebug()<<m_pCurrentItem->text(0)<<"1111"<<nChannelNumber;
        CMyTextBrowserSocket *pTextBrowserSocket = new CMyTextBrowserSocket;
        m_CMyTextBrowserSocketList.push_back(pTextBrowserSocket);
        connect(pTextBrowserSocket,SIGNAL(signalClose(CMyTextBrowserSocket *)),this,SLOT(slot_CloseTextBrowserSocket(CMyTextBrowserSocket *)));
        pTextBrowserSocket->SetChannelMessage(nChannelNumber,m_pCurrentItem->text(0));
        pTextBrowserSocket->Start();
    }
        break;
    default:
        break;
    }
}
void MainWindow::slot_CloseTextBrowserSocket(CMyTextBrowserSocket *pTextBrowserSocket)
{
    for (int i = 0; i < m_CMyTextBrowserSocketList.count(); ++i)
    {
        if (m_CMyTextBrowserSocketList.at(i) == pTextBrowserSocket)
        {
            m_CMyTextBrowserSocketList.removeAt(i);
            delete pTextBrowserSocket;
        }
    }
}
void MainWindow::Slot_itemPressed(QTreeWidgetItem *item, int column)
{
    m_pCurrentItem = item;
    qDebug()<<"QTreeWidgetItem type"<<item->type();
    switch (item->type()) {
    case TreeWidgetItem_File:
        qDebug()<<"QTreeWidgetItem type"<<"TreeWidgetItem_File"<<((CTreeWidgetItem*)item)->GetChannelNumber();
        break;
    case TreeWidgetItem_Channel:
        qDebug()<<"QTreeWidgetItem type"<<"TreeWidgetItem_Channel"<<((CTreeWidgetItem*)item)->GetChannelNumber();
        break;
    case TreeWidgetItem_Device:
        qDebug()<<"QTreeWidgetItem type"<<"TreeWidgetItem_Device"<<((CTreeWidgetItem*)item)->GetChannelNumber();
        break;
    default:
        break;
    }
    if (item->type() == TreeWidgetItem_File)
    {
        m_pTreeView_Pro->setModel(NULL);
        m_pTab_YX->setModel(NULL);
        m_pTab_YC->setModel(NULL);
        m_pTab_YM->setModel(NULL);
        m_pTab_YK->setModel(NULL);
        m_pTab_YS->setModel(NULL);
    }else
    {
        m_pTreeView_Pro->setModel(((CTreeWidgetItem*)item)->GetModel(ChannelMessage_TAG_TYPE_PRO));
        m_pTab_YX->setModel(((CTreeWidgetItem*)item)->GetModel(ChannelMessage_TAG_TYPE_DI));
        m_pTab_YC->setModel(((CTreeWidgetItem*)item)->GetModel(ChannelMessage_TAG_TYPE_AI));
        m_pTab_YM->setModel(((CTreeWidgetItem*)item)->GetModel(ChannelMessage_TAG_TYPE_MEM));
        m_pTab_YK->setModel(((CTreeWidgetItem*)item)->GetModel(ChannelMessage_TAG_TYPE_DO));
        m_pTab_YS->setModel(((CTreeWidgetItem*)item)->GetModel(ChannelMessage_TAG_TYPE_AO));
    }
}
