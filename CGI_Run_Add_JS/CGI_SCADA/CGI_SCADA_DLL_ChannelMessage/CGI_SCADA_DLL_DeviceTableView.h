#ifndef CGI_SCADA_DLL_DEVICETABLEVIEW_H
#define CGI_SCADA_DLL_DEVICETABLEVIEW_H

#include <QTableView>
#include "CGI_SCADA_DLL_define.h"
#include "CItemDelegateAll.h"
#include "CItemDelegateCombobox.h"
#include "CItemDelegateEdit.h"
#include "CItemDelegateLabel.h"
#include "CItemDelegateReadOnly.h"
#include "CBatchChangeDialogDialog.h"
#include "CReplaceDialog.h"
#include <QStringListModel>

class CGI_SCADA_DLL_DeviceTableView : public QTableView
{
    Q_OBJECT
public:
    explicit CGI_SCADA_DLL_DeviceTableView(ProtocolType nProtocolType_,QTreeWidgetItem *pParentTreeItem_,QString strLinkName_,Device_Type deviceType_,QWidget *parent = 0);
    ~CGI_SCADA_DLL_DeviceTableView();
    bool SaveAction(QDomDocument &doc, QDomElement &parentElem);
    bool SetInitTagMap(QMap<QString, QStringList> &TagMap_);
    bool SetBuildTagMap(const QMap<QString, QStringList> &TagMap_);
    bool SetTypeElem(const QDomElement &TypeElem_);
    bool SetInitStringListModel(QStringListModel *pStringListModel);
signals:
    void signal_ShowMessageDebug(int,QString);


public slots:
    void slot_ProtocolTypeChange(int nProtocolType_);
private slots:
    void slot_RightClick(QPoint point_);
    void slot_AddTag();
    void slot_DeleteTag();
    void slot_CreateMuchTag();
    void slot_BatchChange();
    void slot_ReplaceChange();
    void slot_ModelDataChanged(QModelIndex,QModelIndex);
    void slot_DoubleClicked(QModelIndex index);
private:
    QTreeWidgetItem *m_pParentTreeItem;
    QStandardItemModel *m_pModel;
    QString m_strLinkName;///< link的名字，例如COM1.Device@
    ProtocolType m_nProtocolType;
    Device_Type m_DeviceType;
    QStringList m_strDisplayList;

    QList<QStandardItem *>  m_Items;///< 添加点时才使用

    QModelIndex m_CurrentModelIndex;

    void SetInitType();
};

#endif // CGI_SCADA_DLL_DEVICETABLEVIEW_H
