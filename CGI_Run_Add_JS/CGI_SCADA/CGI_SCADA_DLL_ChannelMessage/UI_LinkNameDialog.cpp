#include "UI_LinkNameDialog.h"
#include "ui_UI_LinkNameDialog.h"
#include <QStringListModel>

UI_LinkNameDialog::UI_LinkNameDialog(QTreeWidgetItem *pParentTreeItem_,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UI_LinkNameDialog),
    m_pParentTreeWidgetItem(pParentTreeItem_)
{
    ui->setupUi(this);
    this->setWindowTitle("关联点名");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");

    m_pCGI_SCADA_DLL_ChannelMessageItem = NULL;
    m_pMainTreeWidget = m_pParentTreeWidgetItem->treeWidget();
    connect(ui->treeWidget, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(slot_TreeLeftClicked(QTreeWidgetItem *, int)));
    connect(ui->listView,SIGNAL(pressed(QModelIndex)),this,SLOT(slot_pressed(QModelIndex)));

    qDebug()<<m_pMainTreeWidget;
    ///< 生成配置文件，生成在temp目录中
    qDebug()<<__func__<<__LINE__;
    for (int nTreeItemCount = 0; nTreeItemCount < m_pMainTreeWidget->topLevelItemCount(); ++nTreeItemCount)
    {
        qDebug()<<__func__<<__LINE__<<m_pMainTreeWidget->topLevelItem(nTreeItemCount)->text(0);
        if (m_pMainTreeWidget->topLevelItem(nTreeItemCount)->type() == CGI_SCADA_DLL_I::TreeItemType_ChanneM)
        {
//            continue;
            qDebug()<<__func__<<__LINE__;
            m_pCGI_SCADA_DLL_ChannelMessageItem = (CGI_SCADA_DLL_ChannelMessageItem *)(m_pMainTreeWidget->topLevelItem(nTreeItemCount));
            break;
        }
    }
    qDebug()<<__func__<<__LINE__;
    if (m_pCGI_SCADA_DLL_ChannelMessageItem == NULL)
    {

    }else
    {
        this->GetTreeWidgetItem(ui->treeWidget);
    }
    ui->treeWidget->expandAll();
}

UI_LinkNameDialog::~UI_LinkNameDialog()
{
    delete ui;
}

QString UI_LinkNameDialog::GetLinkName()
{
    return ui->lineEdit->text();
}

void UI_LinkNameDialog::slot_TreeLeftClicked(QTreeWidgetItem *pItem, int)
{
    QTreeWidgetItem *pCurrentItem = (QTreeWidgetItem *)pItem;/// 设置当前的树节点
    if (pCurrentItem == NULL)
    {
        return;
    }
    qDebug()<<__func__<<__LINE__<<pCurrentItem->type()<<pCurrentItem->text(0);
    if (pCurrentItem->type() == CGI_SCADA_DLL_I::TreeItemType_Undefined)
    {
        return;
    }
    CGI_SCADA_DLL_TreeItemDevice *pCGI_SCADA_DLL_TreeItemDevice = (CGI_SCADA_DLL_TreeItemDevice*)m_TreeWidgetItemCorrespond.key(pItem);
    switch (pItem->type()) {
    case CGI_SCADA_DLL_I::TreeItemType_ChanneM_COM_Device:
        qDebug()<<__func__<<__LINE__<<"ComDevice";
        m_pStringListModel = new QStringListModel;
        ui->listView->setModel(pCGI_SCADA_DLL_TreeItemDevice->GetListModel(m_pStringListModel));

        break;
    case CGI_SCADA_DLL_I::TreeItemType_ChanneM_CAN_Device:
        qDebug()<<__func__<<__LINE__<<"CanDevice";
        m_pStringListModel = new QStringListModel;
        ui->listView->setModel(pCGI_SCADA_DLL_TreeItemDevice->GetListModel(m_pStringListModel));

        break;
    case CGI_SCADA_DLL_I::TreeItemType_ChanneM_NET_Link_Device:
        qDebug()<<__func__<<__LINE__<<"NetLinkDevice";
        m_pStringListModel = new QStringListModel;
        ui->listView->setModel(pCGI_SCADA_DLL_TreeItemDevice->GetListModel(m_pStringListModel));

        break;
    default:
        break;
    }
}

void UI_LinkNameDialog::slot_pressed(QModelIndex index)
{
    ui->lineEdit->setText(index.data().toString());
}
void UI_LinkNameDialog::GetTreeWidgetItem(QTreeWidget *pTreeWidget_)
{
    qDebug()<<__func__<<__LINE__;
    for (int i = 0; i < m_pCGI_SCADA_DLL_ChannelMessageItem->childCount(); ++i)
    {
        QTreeWidgetItem *pItem = m_pCGI_SCADA_DLL_ChannelMessageItem->child(i);
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
        QTreeWidgetItem *pNewItem = new QTreeWidgetItem(pTreeWidget_,QStringList()<<pItem->text(0),pItem->type());
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
        m_TreeWidgetItemCorrespond.insert(pItem,pNewItem);
        pTreeWidget_->insertTopLevelItem(i,pNewItem);
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
        GetTreeWidgetItem(pItem,pNewItem);
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
    }
}

void UI_LinkNameDialog::GetTreeWidgetItem(QTreeWidgetItem *pTreeItem_, QTreeWidgetItem *pNewTreeItem_)
{
    qDebug()<<__func__<<__LINE__;
    for (int i = 0; i < pTreeItem_->childCount(); ++i)
    {
        QTreeWidgetItem *pItem = pTreeItem_->child(i);
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
        QTreeWidgetItem *pNewItem = new QTreeWidgetItem(QStringList()<<pItem->text(0),pItem->type());
        m_TreeWidgetItemCorrespond.insert(pItem,pNewItem);
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
        pNewTreeItem_->addChild(pNewItem);
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
        GetTreeWidgetItem(pItem,pNewItem);
        qDebug()<<__func__<<__LINE__<<pItem->text(0);
    }
}
