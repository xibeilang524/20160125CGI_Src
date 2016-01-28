#include "CGI_SCADA_DLL_ChannelMessageItem.h"
#include <QLabel>
#include "../CGI_SCADA_include/XmlParser/CXmlParser.h"
#include "CGI_SCADA_define.h"


/*!
 \brief 库文件导出接口函数

 \fn CreateDriver
 \param parent QObject对象指针
 \return CGI_SCADA_DLL_I 返回创建的驱动指针
*/
CGI_SCADA_DLL_I * CreateDriver(QObject *parent)
{
    QStringList strings;
    strings << "通道信息";
    qDebug()<<__func__<<strings;
    CGI_SCADA_DLL_ChannelMessageItem *testProtocol = new CGI_SCADA_DLL_ChannelMessageItem(strings,CGI_SCADA_DLL_I::TreeItemType_ChanneM,parent);
    qDebug()<<testProtocol->text(0);
    return  testProtocol;
}

CGI_SCADA_DLL_ChannelMessageItem::CGI_SCADA_DLL_ChannelMessageItem(const QStringList &strings, int type, QObject *parent):
    CGI_SCADA_DLL_TreeItemBase(strings,type,parent)
{
    QString str = QString("aaaaaaaaaaaa cha%1").arg(QString::number(type));
    m_pLabel = new QLabel(str);
    CGI_SCADA_DeviceConfig::InitManufactureMap();
}

CGI_SCADA_DLL_ChannelMessageItem::~CGI_SCADA_DLL_ChannelMessageItem()
{
    qDebug()<<__func__<<__FILE__<<__LINE__<<"start"<<this->text(0);
    qDebug()<<__func__<<__FILE__<<__LINE__<<"end"<<this->text(0);
}

/*!
 \brief 获取树节点的顺序

 \fn CGI_SCADA_DLL_ChannelMessageItem::GetItemNumber
 \return int 这个号码代表的是树节点的顺序，这个号码在所有节点驱动中是唯一的
*/
int CGI_SCADA_DLL_ChannelMessageItem::GetItemNumber()
{
    return 0;
}

QWidget *CGI_SCADA_DLL_ChannelMessageItem::GetWidget(QTreeWidgetItem */*pItem_*/)
{
//    QWidget *pwidget = new QWidget;
//    return pwidget;
//    return m_pLabel;
    return NULL;
}

void CGI_SCADA_DLL_ChannelMessageItem::GetPopUpMenu()
{
    qDebug()<<__func__<<__LINE__<<"CGI_SCADA_define"<<CGI_SCADA_define::s_baCookie;
    qDebug()<<__func__<<__LINE__<<this->childCount();
    QMenu *SwitchMenu = new QMenu();
    //->添加网络节点
//    AddColChannel->setIcon(QIcon(":/ico/networkc.ico"));

    QAction *pAddCOM = new QAction(tr("添加COM"), SwitchMenu);
    connect(pAddCOM, SIGNAL(triggered()), this, SLOT(slot_AddCOM()));

    QAction *pAddCAN = new QAction(tr("添加CAN"), SwitchMenu);
    connect(pAddCAN, SIGNAL(triggered()), this, SLOT(slot_AddCAN()));

    QAction *pAddNET = new QAction(tr("添加NET"),SwitchMenu);
    connect(pAddNET,SIGNAL(triggered()),this,SLOT(slot_AddNET()));

    SwitchMenu->addAction(pAddCOM);
    SwitchMenu->addAction(pAddCAN);
    SwitchMenu->addAction(pAddNET);
    SwitchMenu->addSeparator();
    //-> 运行菜单
    SwitchMenu->exec(QCursor::pos());
//    delete SwitchMenu;//防止内存泄露
    SwitchMenu->deleteLater();
    qDebug()<<__func__<<__LINE__<<this->childCount();
    return ;
}

/*!
 \brief 保存函数

 \fn CGI_SCADA_DLL_ChannelMessageItem::SaveAction
 \return bool
*/
bool CGI_SCADA_DLL_ChannelMessageItem::SaveAction()
{
    QTreeWidgetItem *pItem = NULL;
    for (int nChildIndex = 0; nChildIndex < this->childCount(); ++nChildIndex)
    {
        pItem = this->child(nChildIndex);
        if (pItem->type() == CGI_SCADA_DLL_I::TreeItemType_Undefined)
            continue;
        else
        {
            if (pItem->isDisabled())
            {///< 未使能的不保存

            }else
            {
                ((CGI_SCADA_DLL_I*)child(nChildIndex))->SaveAction();
                emit signal_ShowMessageDebug(0,tr("通道：%1 保存成功").arg(pItem->text(0)));
            }
        }
    }
    qDebug()<<__func__<<__LINE__<<__FILE__<<this->text(0);
    return true;
}

bool CGI_SCADA_DLL_ChannelMessageItem::SetInitTagMap(QMap<QString, QStringList> &TagMap_)
{
    QTreeWidgetItem *pItem = NULL;
    for (int nChildIndex = 0; nChildIndex < this->childCount(); ++nChildIndex)
    {
        pItem = this->child(nChildIndex);
        if (pItem->type() == CGI_SCADA_DLL_I::TreeItemType_Undefined)
            continue;
        else
        {
            if (pItem->isDisabled())
            {///< 未使能的不保存

            }else
            {
                ((CGI_SCADA_DLL_I*)child(nChildIndex))->SetInitTagMap(TagMap_);
            }
        }
    }
    qDebug()<<__func__<<__LINE__<<__FILE__<<this->text(0);
    return true;
}

bool CGI_SCADA_DLL_ChannelMessageItem::SetBuildTagMap(const QMap<QString, QStringList> &TagMap_)
{
    QTreeWidgetItem *pItem = NULL;
    for (int nChildIndex = 0; nChildIndex < this->childCount(); ++nChildIndex)
    {
        pItem = this->child(nChildIndex);
        if (pItem->type() == CGI_SCADA_DLL_I::TreeItemType_Undefined)
            continue;
        else
        {
            if (pItem->isDisabled())
            {///< 未使能的不保存

            }else
            {
                ((CGI_SCADA_DLL_I*)child(nChildIndex))->SetBuildTagMap(TagMap_);
            }
        }
    }
    qDebug()<<__func__<<__LINE__<<__FILE__<<this->text(0);
    return true;
}

void CGI_SCADA_DLL_ChannelMessageItem::GetTreeWidgetItem(QTreeWidget *pTreeWidget_)
{
    qDebug()<<__func__<<__LINE__;
    for (int i = 0; i < this->childCount(); ++i)
    {
        QTreeWidgetItem *pItem = this->child(i);
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
        QTreeWidgetItem *pNewItem = new QTreeWidgetItem(pTreeWidget_,QStringList()<<pItem->text(0),pItem->type());
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
        pTreeWidget_->insertTopLevelItem(i,pNewItem);
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
        GetTreeWidgetItem(pItem,pNewItem);
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
    }
}

void CGI_SCADA_DLL_ChannelMessageItem::GetTreeWidgetItem(QTreeWidgetItem *pTreeItem_, QTreeWidgetItem *pNewTreeItem_)
{
    qDebug()<<__func__<<__LINE__;
    for (int i = 0; i < pTreeItem_->childCount(); ++i)
    {
        QTreeWidgetItem *pItem = pTreeItem_->child(i);
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
        QTreeWidgetItem *pNewItem = new QTreeWidgetItem(QStringList()<<pItem->text(0),pItem->type());
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
        pNewTreeItem_->addChild(pNewItem);
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
        GetTreeWidgetItem(pItem,pNewItem);
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
    }
}

void CGI_SCADA_DLL_ChannelMessageItem::slot_AddCOM()
{
    /// 弹出对话框 新建COM口
    bool ok;
    int nNumber = QInputDialog::getInt(NULL, tr("添加COM通道"),
                                     tr("COM:"), 1, 0, 100, 1, &ok);
    if (ok)
    {
        if (m_nComNumberList.contains(nNumber))
        {
            QMessageBox::warning(NULL, tr("添加COM通道"),
                                           tr("此COM通道已存在，\n"
                                              "请勿重新添加。"),
                                           QMessageBox::Ok,
                                           QMessageBox::Ok);
        }else {
            qDebug()<<__func__<<tr("%1%").arg(nNumber);
            AddChannel(nNumber,TreeItemType_ChanneM_COM);
//            m_nComNumberList.append(nNumber);
//            QStringList strings;
//            strings << tr("Com%1").arg(nNumber);
//            qDebug()<<__func__<<strings;
//            CGI_SCADA_DLL_TreeItemCOM *pItem = new CGI_SCADA_DLL_TreeItemCOM(nNumber,this,strings,TreeItemType_ChanneM_COM,this);
//            connect(pItem,SIGNAL(signal_DeleteChannl()),this,SLOT(slot_DeleteChannel()));
//            this->addChild(pItem);
//            qDebug()<<__func__;
        }
    }
}

void CGI_SCADA_DLL_ChannelMessageItem::slot_AddCAN()
{
    qDebug()<<__func__;
    bool ok;
    int nNumber = QInputDialog::getInt(NULL, tr("添加CAN通道"),
                                     tr("CAN:"), 1, 0, 100, 1, &ok);
    if (ok)
    {
        if (m_nCanNumberList.contains(nNumber))
        {
            QMessageBox::warning(NULL, tr("添加CAN通道"),
                                           tr("此CAN通道已存在，\n"
                                              "请勿重新添加。"),
                                           QMessageBox::Ok,
                                           QMessageBox::Ok);
        }else {
            qDebug()<<__func__<<tr("%1%").arg(nNumber);
            AddChannel(nNumber,TreeItemType_ChanneM_CAN);
//            m_nCanNumberList.append(nNumber);
//            QStringList strings;
//            strings << tr("Can%1").arg(nNumber);
//            qDebug()<<__func__<<strings;
//            CGI_SCADA_DLL_TreeItemCAN *pItem = new CGI_SCADA_DLL_TreeItemCAN(nNumber,this,strings,TreeItemType_ChanneM_CAN,this);
//            connect(pItem,SIGNAL(signal_DeleteChannl()),this,SLOT(slot_DeleteChannel()));
//            this->addChild(pItem);
//            qDebug()<<__func__;
        }
    }
}

void CGI_SCADA_DLL_ChannelMessageItem::AddChannel(int nNumber,CGI_SCADA_DLL_I::TreeItemType nType)
{
    switch (nType) {
    case TreeItemType_ChanneM_COM:
    {
        m_nComNumberList.append(nNumber);
        QStringList strings;
        strings << tr("Com%1").arg(nNumber);
        qDebug()<<__func__<<strings;
        CGI_SCADA_DLL_TreeItemCOM *pItem = new CGI_SCADA_DLL_TreeItemCOM(nNumber,this,strings,TreeItemType_ChanneM_COM,this);
        connect(pItem,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
        connect(pItem,SIGNAL(signal_DeleteChannl()),this,SLOT(slot_DeleteChannel()));
        pItem->setDisabled(false);
        this->addChild(pItem);
    }
        break;
    case TreeItemType_ChanneM_CAN:
    {
        m_nCanNumberList.append(nNumber);
        QStringList strings;
        strings << tr("Can%1").arg(nNumber);
        qDebug()<<__func__<<strings;
        CGI_SCADA_DLL_TreeItemCAN *pItem = new CGI_SCADA_DLL_TreeItemCAN(nNumber,this,strings,TreeItemType_ChanneM_CAN,this);
        connect(pItem,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
        connect(pItem,SIGNAL(signal_DeleteChannl()),this,SLOT(slot_DeleteChannel()));
        pItem->setDisabled(false);
        this->addChild(pItem);
    }
        break;
    case TreeItemType_ChanneM_NET:
    {
        m_nNetNumberList.append(nNumber);
        QStringList strings;
        strings << tr("Net%1").arg(nNumber);
        qDebug()<<__func__<<strings;
        CGI_SCADA_DLL_TreeItemNET *pItem = new CGI_SCADA_DLL_TreeItemNET(nNumber,this,strings,TreeItemType_ChanneM_NET,this);
        connect(pItem,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
        connect(pItem,SIGNAL(signal_DeleteChannl()),this,SLOT(slot_DeleteChannel()));
        pItem->setDisabled(false);
        this->addChild(pItem);
        qDebug()<<__func__;
    }
        break;
    default:
        break;
    }
}

void CGI_SCADA_DLL_ChannelMessageItem::slot_DeleteChannel()
{
    if (CGI_SCADA_DLL_TreeItemCOM* pTreeItem = dynamic_cast<CGI_SCADA_DLL_TreeItemCOM*>(sender()))
    {
        qDebug()<<__func__<<__FILE__<<__LINE__<<"start"<<this->text(0);
        int nNumber = pTreeItem->GetComNumber();
        m_nComNumberList.removeOne(nNumber);
        pTreeItem->deleteLater();
        qDebug()<<__func__<<__FILE__<<__LINE__<<"end"<<this->text(0);
    }else if (CGI_SCADA_DLL_TreeItemCAN* pTreeItem = dynamic_cast<CGI_SCADA_DLL_TreeItemCAN*>(sender()))
    {
        qDebug()<<__func__<<__FILE__<<__LINE__<<"start"<<this->text(0);
        int nNumber = pTreeItem->GetCanNumber();
        m_nCanNumberList.removeOne(nNumber);
        pTreeItem->deleteLater();
        qDebug()<<__func__<<__FILE__<<__LINE__<<"end"<<this->text(0);
    }else if (CGI_SCADA_DLL_TreeItemNET* pTreeItem = dynamic_cast<CGI_SCADA_DLL_TreeItemNET*>(sender()))
    {
        qDebug()<<__func__<<__FILE__<<__LINE__<<"start"<<this->text(0);
        int nNumber = pTreeItem->GetNetNumber();
        m_nNetNumberList.removeOne(nNumber);
        pTreeItem->deleteLater();
        qDebug()<<__func__<<__FILE__<<__LINE__<<"end"<<this->text(0);
    }
}

void CGI_SCADA_DLL_ChannelMessageItem::slot_AddNET()
{
    qDebug()<<__func__;
    bool ok;
    int nNumber = QInputDialog::getInt(NULL, tr("添加NET通道"),
                                     tr("Net:"), 1, 0, 100, 1, &ok);
    if (ok)
    {
        if (m_nNetNumberList.contains(nNumber))
        {
            QMessageBox::warning(NULL, tr("添加NET通道"),
                                           tr("此NET通道已存在，\n"
                                              "请勿重新添加。"),
                                           QMessageBox::Ok,
                                           QMessageBox::Ok);
        }else {
            AddChannel(nNumber,TreeItemType_ChanneM_NET);
            qDebug()<<__func__<<tr("%1%").arg(nNumber);
//            m_nNetNumberList.append(nNumber);
//            QStringList strings;
//            strings << tr("Net%1").arg(nNumber);
//            qDebug()<<__func__<<strings;
//            CGI_SCADA_DLL_TreeItemNET *pItem = new CGI_SCADA_DLL_TreeItemNET(nNumber,this,strings,TreeItemType_ChanneM_NET,this);
//            connect(pItem,SIGNAL(signal_DeleteChannl()),this,SLOT(slot_DeleteChannel()));
//            this->addChild(pItem);
//            qDebug()<<__func__;
        }
    }
}

/*!
 \brief 打开工程函数

 \fn CGI_SCADA_DLL_ChannelMessageItem::slot_OpenProject
 \param strProjectPath
*/
void CGI_SCADA_DLL_ChannelMessageItem::slot_OpenProject(QString strProjectPath)
{
    qDebug()<<__func__<<__FILE__<<__LINE__;
    CGI_SCADA_DLL_TreeItemBase::slot_OpenProject(strProjectPath);
    ///< 打开工程入口函数
    for (int nChildIndex = 0; nChildIndex < this->childCount(); ++nChildIndex)
    {
        ((CGI_SCADA_DLL_I*)this->child(nChildIndex))->deleteLater();
    }
    m_ComFileList.clear();
    m_CANFileList.clear();
    m_NetFileList.clear();
    m_nComNumberList.clear();
    m_nCanNumberList.clear();
    m_nNetNumberList.clear();
    FindFile(strProjectPath);
}
void CGI_SCADA_DLL_ChannelMessageItem::FindFile(QString strPathName_)
{
    QString strPath = strPathName_;
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
//            FindFile(fileInfo.filePath());
        }else{
            fileName = fileInfo.fileName();
            if (fileName.left(3) == "Com")
            {
                m_ComFileList.append(fileInfo.absoluteFilePath());
                QString str = fileInfo.fileName();
                QString tmp;
                for(int j = 0; j < str.length(); j++)
                {
                    if (str[j] >= '0' && str[j] <= '9')
                    {
                        tmp.append(str[j]);
                    }
                }
                m_nComNumberList.append(tmp.toInt());
                QStringList strings;
                strings << tr("Com%1").arg(tmp);
                qDebug()<<__func__<<strings;
                CGI_SCADA_DLL_TreeItemCOM *pItem = new CGI_SCADA_DLL_TreeItemCOM(tmp.toInt(),this,strings,TreeItemType_ChanneM_COM,this);
                connect(pItem,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
                connect(pItem,SIGNAL(signal_DeleteChannl()),this,SLOT(slot_DeleteChannel()));
                pItem->setDisabled(false);
                this->addChild(pItem);
                pItem->SetChannelFile(fileInfo.filePath());

            }else if (fileName.left(3) == "Can")
            {
                m_NetFileList.append(fileInfo.absoluteFilePath());
                QString str = fileInfo.fileName();
                QString tmp;
                for(int j = 0; j < str.length(); j++)
                {
                    if (str[j] >= '0' && str[j] <= '9')
                    {
                        tmp.append(str[j]);
                    }
                }
                m_nCanNumberList.append(tmp.toInt());
                QStringList strings;
                strings << tr("Can%1").arg(tmp);
                qDebug()<<__func__<<strings;
                CGI_SCADA_DLL_TreeItemCAN *pItem = new CGI_SCADA_DLL_TreeItemCAN(tmp.toInt(),this,strings,TreeItemType_ChanneM_CAN,this);
                connect(pItem,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
                connect(pItem,SIGNAL(signal_DeleteChannl()),this,SLOT(slot_DeleteChannel()));
                pItem->setDisabled(false);
                this->addChild(pItem);
                pItem->SetChannelFile(fileInfo.filePath());
            }else if (fileName.left(3) == "Net")
            {
                m_CANFileList.append(fileInfo.absoluteFilePath());
                QString str = fileInfo.fileName();
                QString tmp;
                for(int j = 0; j < str.length(); j++)
                {
                    if (str[j] >= '0' && str[j] <= '9')
                    {
                        tmp.append(str[j]);
                    }
                }
                m_nNetNumberList.append(tmp.toInt());
                QStringList strings;
                strings <<tr("Net%1").arg(tmp);
                CGI_SCADA_DLL_TreeItemNET *pItem = new CGI_SCADA_DLL_TreeItemNET(tmp.toInt(),this,strings,TreeItemType_ChanneM_NET,this);
                connect(pItem,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
                connect(pItem,SIGNAL(signal_DeleteChannl()),this,SLOT(slot_DeleteChannel()));
                pItem->setDisabled(false);
                this->addChild(pItem);
                pItem->SetChannelFile(fileInfo.filePath());
            }
            qDebug()<<(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName(),10).arg(fileInfo.path()))<<fileInfo.absoluteFilePath();
        }
        i++;
    }while(i<list.size());
}
