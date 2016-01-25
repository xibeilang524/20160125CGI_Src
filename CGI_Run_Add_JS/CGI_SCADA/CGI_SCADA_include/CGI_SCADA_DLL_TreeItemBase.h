#ifndef CGI_SCADA_DLL_TREEITEMBASE_H
#define CGI_SCADA_DLL_TREEITEMBASE_H

#include "CGI_SCADA_DLL_I.h"

class CGI_SCADA_DLL_TreeItemBase : public CGI_SCADA_DLL_I
{
    Q_OBJECT
public:
    CGI_SCADA_DLL_TreeItemBase(int type = Type,QObject *parent = 0)
        :CGI_SCADA_DLL_I(type,parent){}
    CGI_SCADA_DLL_TreeItemBase(const QStringList &strings, int type = Type,QObject *parent = 0)
        :CGI_SCADA_DLL_I(strings,type,parent){}
    CGI_SCADA_DLL_TreeItemBase(QTreeWidget *view, int type = Type,QObject *parent = 0)
        :CGI_SCADA_DLL_I(view,type,parent){}
    CGI_SCADA_DLL_TreeItemBase(QTreeWidget *view, const QStringList &strings, int type = Type,QObject *parent = 0)
        :CGI_SCADA_DLL_I(view,strings,type,parent){}
    CGI_SCADA_DLL_TreeItemBase(QTreeWidget *view, QTreeWidgetItem *after, int type = Type,QObject *parent = 0)
        :CGI_SCADA_DLL_I(view,after,type,parent){}
    CGI_SCADA_DLL_TreeItemBase(QTreeWidgetItem *parentItem, int type = Type,QObject *parent = 0)
        :CGI_SCADA_DLL_I(parentItem,type,parent){}
    CGI_SCADA_DLL_TreeItemBase(QTreeWidgetItem *parentItem, const QStringList &strings, int type = Type,QObject *parent = 0)
        :CGI_SCADA_DLL_I(parentItem,strings,type,parent){}
    CGI_SCADA_DLL_TreeItemBase(QTreeWidgetItem *parentItem, QTreeWidgetItem *after, int type = Type,QObject *parent = 0)
        :CGI_SCADA_DLL_I(parentItem,after,type,parent){}
    CGI_SCADA_DLL_TreeItemBase(const QTreeWidgetItem &other,QObject *parent = 0)
        :CGI_SCADA_DLL_I(other,parent){}
    virtual ~CGI_SCADA_DLL_TreeItemBase();
    void SetIPAddress_Cookie(QString strIPAddress_,QByteArray baCookie_);
    int GetItemNumber();
    QWidget *GetWidget(QTreeWidgetItem *pItem_);
    void GetPopUpMenu();
    bool SaveAction();
    bool SaveAction(QDomDocument &doc, QDomElement &parentElem);
    bool SetChannelFile(QString strFileName_);
    bool SetInitTagMap(QMap<QString,QStringList> &TagMap_);
    bool SetBuildTagMap(const QMap<QString, QStringList> &TagMap_);
signals:
    void signal_DeleteChannl();
public slots:
    void slot_OpenProject(QString strProjectPath);
public:
    static QString m_strProjectPaths;///< 未用到

};

#endif // CGI_SCADA_DLL_TREEITEMBASE_H
