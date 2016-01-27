#ifndef CGI_SCADA_DLL_TREEITEMNETLINK_H
#define CGI_SCADA_DLL_TREEITEMNETLINK_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "CGI_SCADA_DLL_TreeItemDevice.h"
#include "UI_COMCAN_Attribute.h"
#include "CGI_SCADA_DeviceConfig.h"
#include <QProcess>

class CGI_SCADA_DLL_TreeItemNetLink : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    CGI_SCADA_DLL_TreeItemNetLink(QTreeWidgetItem *parentItem, const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_TreeItemNetLink();
    QWidget *GetWidget(QTreeWidgetItem *pItem_);
    void GetPopUpMenu();
    bool SaveAction(QDomDocument &doc, QDomElement &parentElem);
    bool SetInitTagMap(QMap<QString, QStringList> &TagMap_);
    bool SetBuildTagMap(const QMap<QString, QStringList> &TagMap_);
    bool setLinkElem(const QDomElement &LinkElem_);
signals:
    void signal_ProtocolTypeChange(int);
    void signal_DeleteChannl();
public slots:

private slots:
    void slot_ChangeProtocolName(QString strProtocolName);
    void slot_ProtocolTypeChange(int nProtocolType_);
    void slot_AddDevice();
    void slot_DeleteChannel();
    void slot_ProtocolExplanation();
private:
    UI_COMCAN_Attribute *m_pMainWidget;
    ProtocolType m_nProtocolType;
    QString m_strProtocolName;
    QProcess m_Process;
};

#endif // CGI_SCADA_DLL_TREEITEMNETLINK_H
