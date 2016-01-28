#ifndef CGI_SCADA_DLL_TREEITEMWEBUI_H
#define CGI_SCADA_DLL_TREEITEMWEBUI_H

#include "../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h"

class CGI_SCADA_DLL_TreeItemWebUI : public CGI_SCADA_DLL_TreeItemBase
{
    Q_OBJECT
public:
    explicit CGI_SCADA_DLL_TreeItemWebUI(const QStringList &strings, int type = Type,QObject *parent = 0);

signals:

public slots:

};

#endif // CGI_SCADA_DLL_TREEITEMWEBUI_H
