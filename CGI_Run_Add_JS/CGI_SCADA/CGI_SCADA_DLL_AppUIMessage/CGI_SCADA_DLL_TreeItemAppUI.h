#ifndef CGI_SCADA_DLL_TREEITEMAPPUI_H
#define CGI_SCADA_DLL_TREEITEMAPPUI_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"

class CGI_SCADA_DLL_TreeItemAppUI : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    explicit CGI_SCADA_DLL_TreeItemAppUI(const QStringList &strings, int type = Type,QObject *parent = 0);

signals:

public slots:

};

#endif // CGI_SCADA_DLL_TREEITEMAPPUI_H
