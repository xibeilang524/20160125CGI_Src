#include "CGI_SCADA_DLL_TreeItemNET.h"
#include "../CGI_SCADA_include/XmlParser/CXmlParser.h"


CGI_SCADA_DLL_TreeItemNET::CGI_SCADA_DLL_TreeItemNET(int nNetNumber_, QTreeWidgetItem *parentItem, const QStringList &strings, int type, QObject *parent) :
    CGI_SCADA_DLL_TreeItemBase(parentItem,strings,type,parent),
    m_nNetNumber(nNetNumber_)
{
//    m_pAttributeItem = new CGI_SCADA_DLL_TreeItemComAttribute(this,QStringList()<<"属性",TreeItemType_ChanneM_COM_Attribute,this);
//    connect(m_pAttributeItem,SIGNAL(signal_ProtocolTypeChange(int)),this,SLOT(slot_ProtocolTypeChange(int)));
//    connect(m_pAttributeItem,SIGNAL(signal_ProtocolTypeChange(int)),this,SIGNAL(signal_ProtocolTypeChange(int)));
//    this->addChild(m_pAttributeItem);
    this->setDisabled(true);
    m_pNetAttribute = new UI_NetAttribute;
}

CGI_SCADA_DLL_TreeItemNET::~CGI_SCADA_DLL_TreeItemNET()
{
    if (m_pNetAttribute != NULL)
    {
        m_pNetAttribute->deleteLater();
//        delete m_pNetAttribute;
        m_pNetAttribute = NULL;
    }
}

QWidget *CGI_SCADA_DLL_TreeItemNET::GetWidget(QTreeWidgetItem */*pItem_*/)
{
    return m_pNetAttribute;
}

void CGI_SCADA_DLL_TreeItemNET::GetPopUpMenu()
{
    QMenu *SwitchMenu = new QMenu();

    QAction *pAddLink = new QAction(tr("新建连接"), SwitchMenu);
    connect(pAddLink, SIGNAL(triggered()), this, SLOT(slot_AddLink()));

    QAction *pDeleteChannel = new QAction(tr("删除通道"), SwitchMenu);
    connect(pDeleteChannel, SIGNAL(triggered()), this, SLOT(slot_DeleteChannel()));

    QAction *pSetEnable = new QAction(tr("使能通道"), SwitchMenu);
    pSetEnable->setCheckable(true);
    if (this->isDisabled())
    {
        pSetEnable->setChecked(false);
    }else
    {
        pSetEnable->setChecked(true);
    }
//    this->isDisabled();
    connect(pSetEnable, SIGNAL(triggered(bool)), this, SLOT(slot_SetEnable(bool)));

    SwitchMenu->addAction(pAddLink);
    SwitchMenu->addAction(pDeleteChannel);
    SwitchMenu->addAction(pSetEnable);
    SwitchMenu->addSeparator();
    SwitchMenu->exec(QCursor::pos());
//    delete SwitchMenu;//防止内存泄露
    SwitchMenu->deleteLater();

    qDebug()<<__func__<<__LINE__<<__FILE__<<this->childCount();
    return ;
}

int CGI_SCADA_DLL_TreeItemNET::GetNetNumber()
{
    return m_nNetNumber;
}

bool CGI_SCADA_DLL_TreeItemNET::SaveAction()
{
    QDomDocument NetConfigFile;
    QDomElement ProRoot = NetConfigFile.createElement(tr("Root"));
    for (int nNetLinkIndex = 0; nNetLinkIndex < this->childCount(); ++nNetLinkIndex)
    {
        if (this->child(nNetLinkIndex)->type() == TreeItemType_Undefined)
            continue;
        else
        {
            ((CGI_SCADA_DLL_I *)this->child(nNetLinkIndex))->SaveAction(NetConfigFile,ProRoot);
        }
    }

    NetConfigFile.appendChild(ProRoot);
    QString strFileName;
//    qDebug()<<__func__<<__FILE__<<__LINE__<<m_strProjectPaths;
//    if (CGI_SCADA_DLL_TreeItemBase::m_strProjectPaths.isEmpty())
//    {
//        strFileName = "project/"+this->text(0)+"_.xml";
//    }else
//    {
//        strFileName = CGI_SCADA_DLL_TreeItemBase::m_strProjectPaths+"/"+this->text(0)+"_.xml";
//    }
    strFileName = "temp_project/"+this->text(0)+"_.xml";
    CXmlParser::writeFile(strFileName,NetConfigFile);
    return true;
}

bool CGI_SCADA_DLL_TreeItemNET::SetInitTagMap(QMap<QString, QStringList> &TagMap_)
{
    for (int nNetLinkIndex = 0; nNetLinkIndex < this->childCount(); ++nNetLinkIndex)
    {
        if (this->child(nNetLinkIndex)->type() == TreeItemType_Undefined)
            continue;
        else
        {
            ((CGI_SCADA_DLL_I *)this->child(nNetLinkIndex))->SetInitTagMap(TagMap_);
        }
    }
}

bool CGI_SCADA_DLL_TreeItemNET::SetBuildTagMap(const QMap<QString, QStringList> &TagMap_)
{
    for (int nNetLinkIndex = 0; nNetLinkIndex < this->childCount(); ++nNetLinkIndex)
    {
        if (this->child(nNetLinkIndex)->type() == TreeItemType_Undefined)
            continue;
        else
        {
            ((CGI_SCADA_DLL_I *)this->child(nNetLinkIndex))->SetBuildTagMap(TagMap_);
        }
    }
}
void CGI_SCADA_DLL_TreeItemNET::slot_AddLink()
{
    bool ok;
    QString strNewLinkName = QInputDialog::getText(NULL, tr("新建连接"),
                                         tr("连接名字:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !strNewLinkName.isEmpty())
    {
        qDebug()<<strNewLinkName;
        QStringList strings;
        strings << strNewLinkName;
        qDebug()<<__func__<<strings;
        CGI_SCADA_DLL_TreeItemNetLink *pItem = new CGI_SCADA_DLL_TreeItemNetLink(this,strings,TreeItemType_ChanneM_NET_Link_Attribute,this);
        connect(pItem,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
//        connect(pItem,SIGNAL(signal_DeleteChannl()),this,SLOT(slot_DeleteNetLink()));
        this->addChild(pItem);
        qDebug()<<__func__;
    }
}
void CGI_SCADA_DLL_TreeItemNET::slot_DeleteChannel()
{
    qDebug()<<__func__;
    int ret = QMessageBox::warning(NULL, tr("删除通道"),
                                             tr("即将删除此通道，\n"
                                                "确定删除此通道吗？"),
                                             QMessageBox::Cancel|QMessageBox::Ok,
                                             QMessageBox::Cancel);
    if (ret == QMessageBox::Ok)
    {
        emit signal_DeleteChannl();
    }
}

void CGI_SCADA_DLL_TreeItemNET::slot_SetEnable(bool enable)
{
    qDebug()<<__func__<<__FILE__<<__LINE__<<enable;
//    if (QAction* pAction = dynamic_cast<QAction*>(sender()))
//    {
        if (enable)
        {
            this->setDisabled(false);
        }else
        {
            this->setDisabled(true);
        }
//    }
}
bool CGI_SCADA_DLL_TreeItemNET::SetChannelFile(QString strFileName_)
{
    qDebug()<<__func__<<__LINE__<<__FILE__<<strFileName_;
    QDomDocument doc;
    if (!CXmlParser::readFile(strFileName_,doc))
    {
        return false;
    }
    QDomNodeList LinkNodeList = doc.elementsByTagName("Link");
    for (int nLinkIndex = 0; nLinkIndex < LinkNodeList.count(); ++nLinkIndex)
    {
        QDomElement LinkElem = LinkNodeList.at(nLinkIndex).toElement();
//        qDebug()<<strNewLinkName;
        QStringList strings;
        strings << LinkElem.attribute("Name");
        qDebug()<<__func__<<strings;
        CGI_SCADA_DLL_TreeItemNetLink *pItem = new CGI_SCADA_DLL_TreeItemNetLink(this,strings,TreeItemType_ChanneM_NET_Link_Attribute,this);
        connect(pItem,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
//        connect(pItem,SIGNAL(signal_DeleteChannl()),this,SLOT(slot_DeleteNetLink()));
        this->addChild(pItem);
        qDebug()<<__func__;
        pItem->setLinkElem(LinkElem);
    }

    return true;
}
