#ifndef CGI_SCADA_DLL_TREEITEMCAN_H
#define CGI_SCADA_DLL_TREEITEMCAN_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "CGI_SCADA_DLL_TreeItemDevice.h"
#include "UI_COMCAN_Attribute.h"
#include "CGI_SCADA_DeviceConfig.h"
#include <QProcess>

class CGI_SCADA_DLL_TreeItemCAN : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    CGI_SCADA_DLL_TreeItemCAN(int nComNumber_,QTreeWidgetItem *parentItem, const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_TreeItemCAN();
    bool SetChannelFile(QString strFileName_);
    void GetPopUpMenu();
    int GetCanNumber() const;
    bool SaveAction();
    bool SetInitTagMap(QMap<QString, QStringList> &TagMap_);
    bool SetBuildTagMap(const QMap<QString, QStringList> &TagMap_);
    QWidget *GetWidget(QTreeWidgetItem *pItem_);
signals:
    void signal_DeleteChannl();
    void signal_ProtocolTypeChange(int);

private:
    UI_COMCAN_Attribute *m_pMainWidget;
//    CGI_SCADA_DLL_TreeItemComCanAttribute *m_pAttributeItem;
    int m_nCanNumber;///< 通道号
    ProtocolType m_nProtocolType;
    QString m_strProtocolName;
    QProcess m_Process;
private slots:
    void slot_ChangeProtocolName(QString strProtocolName);
    void slot_AddDevice();
    void slot_DeleteChannel();
    void slot_ProtocolTypeChange(int nProtocolType_);

    void slot_SetEnable(bool enable);
    void slot_ProtocolExplanation();
};

#endif // CGI_SCADA_DLL_TREEITEMCAN_H
