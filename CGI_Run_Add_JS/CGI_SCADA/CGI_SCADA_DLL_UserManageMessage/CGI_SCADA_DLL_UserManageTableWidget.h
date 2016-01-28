#ifndef CGI_SCADA_DLL_USERMANAGETABLEWIDGET_H
#define CGI_SCADA_DLL_USERMANAGETABLEWIDGET_H

#include <QTableWidget>

class CGI_SCADA_DLL_UserManageTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit CGI_SCADA_DLL_UserManageTableWidget(QWidget *parent = 0);

signals:

public slots:

private slots:
    void slot_customContextMenuRequested(QPoint point_);
    void slot_AddUser();
    void slot_DeleteUser();
    void slot_InquireLog();
    void slot_SaveChange();
private:
    QModelIndex m_CurrentModelIndex;

};

#endif // CGI_SCADA_DLL_USERMANAGETABLEWIDGET_H
