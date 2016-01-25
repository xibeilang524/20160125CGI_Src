#ifndef CGI_SCADA_DLL_TREEITEMDEVICE_H
#define CGI_SCADA_DLL_TREEITEMDEVICE_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "CGI_SCADA_DLL_define.h"
#include "UI_DeviceAttribute.h"
#include "CGI_SCADA_DLL_DeviceTableView.h"
#include <QStringListModel>

class CGI_SCADA_DLL_TreeItemDevice : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    CGI_SCADA_DLL_TreeItemDevice(ProtocolType nProtocolType_,Device device_,QTreeWidgetItem *parentItem, const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_TreeItemDevice();
    void GetPopUpMenu();
    QWidget *GetWidget(QTreeWidgetItem *pItem_);
    bool SaveAction(QDomDocument &doc, QDomElement &parentElem);
    bool SetInitTagMap(QMap<QString, QStringList> &TagMap_);
    bool SetBuildTagMap(const QMap<QString, QStringList> &TagMap_);
    bool SetDeviceElem(const QDomElement &DeviceElem);
    QStringListModel *GetListModel(QStringListModel *pStringListModel);
signals:
    void signal_ProtocolTypeChange(int);
public slots:
private slots:
    void slot_UploadDevice();
    void slot_DeleteDevice();
//    void slot_ProtocolTypeChange(int nProtocolType_);
private:
//    ProtocolType m_nProtocolType;
    ProtocolType m_nProtocolType;
    Device m_Device;
    QTabWidget *m_pTabWidget;
    UI_DeviceAttribute *m_pDeviceAttribute;
    QList<CGI_SCADA_DLL_DeviceTableView*> m_TableViewList;
    QMap<QString ,CGI_SCADA_DLL_DeviceTableView*> m_TableViewMap;///< QString为节点的Name属性值

};

#endif // CGI_SCADA_DLL_TREEITEMDEVICE_H
