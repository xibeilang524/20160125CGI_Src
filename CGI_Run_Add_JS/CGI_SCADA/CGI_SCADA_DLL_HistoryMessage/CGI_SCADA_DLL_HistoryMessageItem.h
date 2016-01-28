#ifndef CGI_SCADA_DLL_HISTORYMESSAGEITEM_H
#define CGI_SCADA_DLL_HISTORYMESSAGEITEM_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "CGI_SCADA_DLL_HistoryM_Setting.h"
#include "CGI_SCADA_DLL_TreeItemHistorySelect.h"

class CGI_SCADA_DLL_HistoryMessageItem : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    explicit CGI_SCADA_DLL_HistoryMessageItem(const QStringList &strings, int type = Type,QObject *parent = 0);
    int GetItemNumber();
    QWidget *GetWidget(QTreeWidgetItem *pItem_);
    bool SaveAction();
    bool SetBuildTagMap(const QMap<QString, QStringList> &TagMap_);


signals:

public slots:
    void slot_OpenProject(QString strProjectPath);
private:
    QStringList m_DBFileList;
    QLabel *m_pLabel;
    CGI_SCADA_DLL_HistoryM_Setting      *m_pCGI_SCADA_DLL_HistoryM_Setting;
    CGI_SCADA_DLL_TreeItemHistorySelect *m_pCGI_SCADA_DLL_TreeItemHistorySelect;

    void FindFile(QString strPathName_);
};

#endif // CGI_SCADA_DLL_HISTORYMESSAGEITEM_H
