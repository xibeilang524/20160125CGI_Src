#ifndef CGI_SCADA_DLL_TABLEVIEW_H
#define CGI_SCADA_DLL_TABLEVIEW_H

#include <QTableView>

class CGI_SCADA_DLL_TableView : public QTableView
{
    Q_OBJECT
public:
    explicit CGI_SCADA_DLL_TableView(QWidget *parent = 0);
    QModelIndexList GetselectedIndexes();
signals:

public slots:

};

#endif // CGI_SCADA_DLL_TABLEVIEW_H
