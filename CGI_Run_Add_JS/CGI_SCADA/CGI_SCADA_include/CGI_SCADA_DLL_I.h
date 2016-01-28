#ifndef CGI_SCADA_DLL_I_H
#define CGI_SCADA_DLL_I_H

#include <QtCore/qglobal.h>

#if defined(CGI_SCADA_DLL_LIBRARY)
#  define CGI_SCADA_DLLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CGI_SCADA_DLLSHARED_EXPORT Q_DECL_IMPORT
#endif
#include <QObject>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QDomDocument>
#include <QDomElement>

class CGI_SCADA_DLLSHARED_EXPORT CGI_SCADA_DLL_I : public QObject,public QTreeWidgetItem
{
    Q_OBJECT
public:
    enum TreeItemType
    {
        TreeItemType_Undefined = 0,
        TreeItemType_DeviceM,                   ///< 设备信息节点
        TreeItemType_DeviceM_operating_status,
        TreeItemType_DeviceM_hardware_model,
        TreeItemType_DeviceM_software_version,
        TreeItemType_DeviceM_current_users,
        TreeItemType_DeviceM_equipment_time,
        TreeItemType_DeviceM_current_project,
        TreeItemType_DeviceM_property_information,

        TreeItemType_ChanneM,
        TreeItemType_ChanneM_COM,
        TreeItemType_ChanneM_COM_Attribute,
        TreeItemType_ChanneM_COM_Frame,
        TreeItemType_ChanneM_COM_Device,
        TreeItemType_ChanneM_CAN,
        TreeItemType_ChanneM_CAN_Attribute,
        TreeItemType_ChanneM_CAN_Frame,
        TreeItemType_ChanneM_CAN_Device,
        TreeItemType_ChanneM_NET,
        TreeItemType_ChanneM_NET_Attribute,
        TreeItemType_ChanneM_NET_Link,
        TreeItemType_ChanneM_NET_Link_Attribute,
        TreeItemType_ChanneM_NET_Link_Frame,
        TreeItemType_ChanneM_NET_Link_Device,

        TreeItemType_HistorM,
        TreeItemType_HistorM_HistorySettings,
        TreeItemType_HistorM_HistorySelect,

        TreeItemType_ControM,

        TreeItemType_Alarm_M,

        TreeItemType_Web_UIM,
        TreeItemType_App_UIM,

        TreeItemType_UserMaM,

        TreeItemType_About_M
    };

    CGI_SCADA_DLL_I(int type = Type,QObject *parent = 0)
        :QTreeWidgetItem(type),QObject(parent){}
    CGI_SCADA_DLL_I(const QStringList &strings, int type = Type,QObject *parent = 0)
        :QTreeWidgetItem(strings,type),QObject(parent){}
    CGI_SCADA_DLL_I(QTreeWidget *view, int type = Type,QObject *parent = 0)
        :QTreeWidgetItem(view,type),QObject(parent){}
    CGI_SCADA_DLL_I(QTreeWidget *view, const QStringList &strings, int type = Type,QObject *parent = 0)
        :QTreeWidgetItem(view,strings,type),QObject(parent){}
    CGI_SCADA_DLL_I(QTreeWidget *view, QTreeWidgetItem *after, int type = Type,QObject *parent = 0)
        :QTreeWidgetItem(view,after,type),QObject(parent){}
    CGI_SCADA_DLL_I(QTreeWidgetItem *parentItem, int type = Type,QObject *parent = 0)
        :QTreeWidgetItem(parentItem,type),QObject(parent){}
    CGI_SCADA_DLL_I(QTreeWidgetItem *parentItem, const QStringList &strings, int type = Type,QObject *parent = 0)
        :QTreeWidgetItem(parentItem,strings,type),QObject(parent){}
    CGI_SCADA_DLL_I(QTreeWidgetItem *parentItem, QTreeWidgetItem *after, int type = Type,QObject *parent = 0)
        :QTreeWidgetItem(parentItem,after,type),QObject(parent){}
    CGI_SCADA_DLL_I(const QTreeWidgetItem &other,QObject *parent = 0)
        :QTreeWidgetItem(other),QObject(parent){}

    virtual ~CGI_SCADA_DLL_I(){
        qDebug()<<__func__<<__FILE__<<__LINE__<<"start"<<this->text(0);
        qDebug()<<__func__<<__FILE__<<__LINE__<<"end"<<this->text(0);
    }
    virtual void SetIPAddress_Cookie(QString strIPAddress_,QByteArray baCookie_) = 0;
    virtual int GetItemNumber() = 0;
    virtual QWidget *GetWidget(QTreeWidgetItem *pItem_) = 0;
    virtual void GetPopUpMenu() = 0;
    virtual bool SaveAction() = 0;
    virtual bool SaveAction(QDomDocument &doc, QDomElement &parentElem) = 0;
    virtual bool SetChannelFile(QString strFileName_) = 0;///< 在打开工程时各个通道用到这个函数 com、can、net通道节点
    virtual bool SetInitTagMap(QMap<QString,QStringList> &TagMap_) = 0;
    virtual bool SetBuildTagMap(const QMap<QString,QStringList> &TagMap_) = 0;
signals:
    void signal_ShowMessageDebug(int nType_,QString strMessage);
//    void signal_ProjectPathChange(QString strProjectPath);
public slots:
    virtual void slot_OpenProject(QString strProjectPath) = 0;
public:

//    void Signal_ChangeWidget(QWidget *pWidget);

};
extern "C" CGI_SCADA_DLLSHARED_EXPORT CGI_SCADA_DLL_I * CreateDriver(QObject *parent);
//class CGI_SCADA_DLLSHARED_EXPORT CGI_SCADA_DLL_TreeItemI : public QTreeWidgetItem,public CGI_SCADA_DLL_I
//{
//    Q_OBJECT
//public:
//    CGI_SCADA_DLL_TreeItemI(const QStringList &strings,QObject *parent):QTreeWidgetItem(strings),CGI_SCADA_DLL_I(parent){}
//    virtual ~CGI_SCADA_DLL_TreeItemI(){}

//};
#endif // CGI_SCADA_DLL_I_H
