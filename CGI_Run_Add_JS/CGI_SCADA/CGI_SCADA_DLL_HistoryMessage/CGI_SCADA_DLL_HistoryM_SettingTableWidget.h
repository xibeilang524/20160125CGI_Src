#ifndef CGI_SCADA_DLL_HISTORYM_SETTINGTABLEWIDGET_H
#define CGI_SCADA_DLL_HISTORYM_SETTINGTABLEWIDGET_H

#include <QTableWidget>
#include <QInputDialog>
#include <QTreeWidgetItem>
#include <QDomDocument>
#include <QDomElement>

class CGI_SCADA_DLL_HistoryM_SettingTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit CGI_SCADA_DLL_HistoryM_SettingTableWidget(QTreeWidgetItem *pParentTreeItem_,QWidget *parent = 0);
    bool SaveAction(QDomDocument &doc, QDomElement &parentElem);
    bool SetBuildTagMap(const QMap<QString, QStringList> &TagMap_);
    bool SetDBFile(QString strFileName_);
signals:
    void signal_ShowMessageDebug(int,QString);
private slots:
    void slot_customContextMenuRequested(QPoint);
    void slot_changeTest(int row, int col);
    void slot_DoubleClicked(QModelIndex index);

    void slot_AddTag();
    void slot_DeleteTag();
    void slot_CreateMuchTag();
    void slot_BatchChange();
    void slot_ReplaceChange();
private:
//    CGI_SCADA_DLL_HistoryM_SettingTableView *m_pMainWidget;
    QModelIndex m_CurrentModelIndex;
    QTreeWidgetItem *m_pParentTreeItem;

};

#endif // CGI_SCADA_DLL_HISTORYM_SETTINGTABLEWIDGET_H
