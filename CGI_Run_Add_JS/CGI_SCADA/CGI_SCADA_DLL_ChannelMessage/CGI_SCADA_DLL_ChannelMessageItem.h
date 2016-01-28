#ifndef CGI_SCADA_DLL_CHANNELMESSAGEITEM_H
#define CGI_SCADA_DLL_CHANNELMESSAGEITEM_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "CGI_SCADA_DLL_TreeItemCOM.h"
#include "CGI_SCADA_DLL_TreeItemCAN.h"
#include "CGI_SCADA_DLL_TreeItemNET.h"
#include "CGI_SCADA_DeviceConfig.h"

class CGI_SCADA_DLL_ChannelMessageItem : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    explicit CGI_SCADA_DLL_ChannelMessageItem(const QStringList &strings, int type = Type,QObject *parent = 0);
    virtual ~CGI_SCADA_DLL_ChannelMessageItem();
    int GetItemNumber();
    QWidget *GetWidget(QTreeWidgetItem *pItem_);
    void GetPopUpMenu();
    bool SaveAction();
    bool SetInitTagMap(QMap<QString, QStringList> &TagMap_);
    bool SetBuildTagMap(const QMap<QString, QStringList> &TagMap_);
    void GetTreeWidgetItem(QTreeWidget *pTreeWidget_);
    void GetTreeWidgetItem(QTreeWidgetItem *pTreeItem_,QTreeWidgetItem *pNewTreeItem_);

signals:

public slots:
    void slot_OpenProject(QString strProjectPath);
private slots:
    void slot_AddCOM();
    void slot_AddCAN();
    void slot_AddNET();
    void slot_DeleteChannel();
private:

    QLabel *m_pLabel;
    QList<int> m_nComNumberList;
    QList<int> m_nCanNumberList;
    QList<int> m_nNetNumberList;

private:
    QStringList m_ComFileList;
    QStringList m_NetFileList;
    QStringList m_CANFileList;
    void FindFile(QString strPathName_);
    void AddChannel(int nNumber, CGI_SCADA_DLL_I::TreeItemType nType);
};

#endif // CGI_SCADA_DLL_CHANNELMESSAGEITEM_H
