#ifndef CGI_SCADA_DLL_TREEITEMCOM_H
#define CGI_SCADA_DLL_TREEITEMCOM_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "CGI_SCADA_DLL_TreeItemDevice.h"
#include "UI_COMCAN_Attribute.h"
#include "CGI_SCADA_DeviceConfig.h"

class CGI_SCADA_DLL_TreeItemCOM : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    CGI_SCADA_DLL_TreeItemCOM(int nComNumber_,QTreeWidgetItem *parentItem, const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_TreeItemCOM();
    bool SetChannelFile(QString strFileName_);
    void GetPopUpMenu();
    int GetComNumber() const;
    bool SaveAction();
    bool SetInitTagMap(QMap<QString, QStringList> &TagMap_);
    bool SetBuildTagMap(const QMap<QString, QStringList> &TagMap_);
    QWidget *GetWidget(QTreeWidgetItem *pItem_);
signals:
    void signal_ProtocolTypeChange(int);

private:
    UI_COMCAN_Attribute *m_pMainWidget;
//    CGI_SCADA_DLL_TreeItemComCanAttribute *m_pAttributeItem;
    int m_nComNumber;///< 通道号
    ProtocolType m_nProtocolType;
private slots:
    void slot_AddDevice();
    void slot_DeleteChannel();
    void slot_ProtocolTypeChange(int nProtocolType_);
    void slot_SetEnable(bool);
};

#endif // CGI_SCADA_DLL_TREEITEMCOM_H
