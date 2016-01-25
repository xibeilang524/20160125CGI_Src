#ifndef CGI_SCADA_DLL_TREEITEMPROPERTY_INFORMATION_H
#define CGI_SCADA_DLL_TREEITEMPROPERTY_INFORMATION_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"
#include "UI_Property_information.h"

class CGI_SCADA_DLL_TreeItemProperty_information : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    CGI_SCADA_DLL_TreeItemProperty_information(QTreeWidgetItem *parentTreeItem_,const QStringList &strings, int type = Type,QObject *parent = 0);
    ~CGI_SCADA_DLL_TreeItemProperty_information();
    QWidget *GetWidget(QTreeWidgetItem *);

signals:

public slots:
private:
    UI_Property_information *m_pWidget;

};

#endif // CGI_SCADA_DLL_TREEITEMPROPERTY_INFORMATION_H
