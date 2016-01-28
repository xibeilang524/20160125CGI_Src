#ifndef UI_NETATTRIBUTE_H
#define UI_NETATTRIBUTE_H

#include <QWidget>
#include "CGI_SCADA_DLL_define.h"
#include "CGI_SCADA_DLL_TableView.h"

namespace Ui {
class UI_NetAttribute;
}

class UI_NetAttribute : public QWidget
{
    Q_OBJECT

public:
    explicit UI_NetAttribute(QWidget *parent = 0);
    ~UI_NetAttribute();

    bool SaveAction(QDomDocument &doc, QDomElement &parentElem);
private slots:
    void slot_RightClick(QPoint point_);

    void on_pushButton_AddIPAddr_clicked();

    void slot_DeleteTag();
private:
    Ui::UI_NetAttribute *ui;
    QStandardItemModel *m_pModel;
    QModelIndex m_CurrentModelIndex;
    CGI_SCADA_DLL_TableView *m_pTableView;

};

#endif // UI_NETATTRIBUTE_H
