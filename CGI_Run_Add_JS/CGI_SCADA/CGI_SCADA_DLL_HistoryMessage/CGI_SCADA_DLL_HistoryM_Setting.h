#ifndef CGI_SCADA_DLL_HISTORYM_SETTING_H
#define CGI_SCADA_DLL_HISTORYM_SETTING_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include <QTableWidget>
#include "CGI_SCADA_DLL_HistoryM_SettingTableWidget.h"

class CGI_SCADA_DLL_HistoryM_Setting : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    explicit CGI_SCADA_DLL_HistoryM_Setting(const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_HistoryM_Setting();
    QWidget *GetWidget(QTreeWidgetItem *pItem_);
    bool SaveAction();
    bool SetBuildTagMap(const QMap<QString, QStringList> &TagMap_);
    bool SetChannelFile(QString strFileName_);
signals:

public slots:
private slots:

private:
    CGI_SCADA_DLL_HistoryM_SettingTableWidget *m_pTableWidget;

};

#endif // CGI_SCADA_DLL_HISTORYM_SETTING_H
