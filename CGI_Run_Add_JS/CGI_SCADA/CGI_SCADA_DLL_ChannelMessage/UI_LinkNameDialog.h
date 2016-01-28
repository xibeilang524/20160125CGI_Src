#ifndef UI_LINKNAMEDIALOG_H
#define UI_LINKNAMEDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "CGI_SCADA_DLL_define.h"
#include "CGI_SCADA_DLL_ChannelMessageItem.h"

namespace Ui {
class UI_LinkNameDialog;
}

class UI_LinkNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UI_LinkNameDialog(QTreeWidgetItem *pParentTreeItem_,QWidget *parent = 0);
    ~UI_LinkNameDialog();
    QString GetLinkName();
private slots:
    void slot_TreeLeftClicked(QTreeWidgetItem *pItem, int);
    void slot_pressed(QModelIndex index);

private:
    Ui::UI_LinkNameDialog *ui;
    QTreeWidgetItem *m_pParentTreeWidgetItem;
    QTreeWidget *m_pMainTreeWidget;
    CGI_SCADA_DLL_ChannelMessageItem *m_pCGI_SCADA_DLL_ChannelMessageItem;
    void GetTreeWidgetItem(QTreeWidget *pTreeWidget_);
    void GetTreeWidgetItem(QTreeWidgetItem *pTreeItem_, QTreeWidgetItem *pNewTreeItem_);
    QMap<QTreeWidgetItem *,QTreeWidgetItem*> m_TreeWidgetItemCorrespond;///< 关联对话框中的树节点与通道中主界面之间关联的map
    QStringListModel *m_pStringListModel;
};

#endif // UI_LINKNAMEDIALOG_H
