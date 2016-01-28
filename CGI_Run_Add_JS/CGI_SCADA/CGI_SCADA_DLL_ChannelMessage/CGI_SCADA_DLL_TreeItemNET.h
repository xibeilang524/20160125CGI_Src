#ifndef CGI_SCADA_DLL_TREEITEMNET_H
#define CGI_SCADA_DLL_TREEITEMNET_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
//#include "CGI_SCADA_DLL_TreeItemComCanAttribute.h"
#include "CGI_SCADA_DLL_TreeItemDevice.h"
#include "UI_NetAttribute.h"
#include "CGI_SCADA_DLL_define.h"
#include "CGI_SCADA_DLL_TreeItemNetLink.h"

class CGI_SCADA_DLL_TreeItemNET : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    CGI_SCADA_DLL_TreeItemNET(int nNetNumber_,QTreeWidgetItem *parentItem, const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_TreeItemNET();
    QWidget *GetWidget(QTreeWidgetItem *pItem_);
    void GetPopUpMenu();
    int GetNetNumber();
    bool SaveAction();
    bool SetInitTagMap(QMap<QString, QStringList> &TagMap_);
    bool SetBuildTagMap(const QMap<QString, QStringList> &TagMap_);
    bool SetChannelFile(QString strFileName_);
signals:

public slots:
private slots:
    void slot_AddLink();
    void slot_DeleteChannel();
    void slot_SetEnable(bool enable);
private:
    int m_nNetNumber;
    UI_NetAttribute *m_pNetAttribute;

};

#endif // CGI_SCADA_DLL_TREEITEMNET_H
