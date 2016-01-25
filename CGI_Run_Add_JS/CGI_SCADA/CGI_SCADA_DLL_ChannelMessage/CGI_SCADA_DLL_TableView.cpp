#include "CGI_SCADA_DLL_TableView.h"

CGI_SCADA_DLL_TableView::CGI_SCADA_DLL_TableView(QWidget *parent) :
    QTableView(parent)
{
}

QModelIndexList CGI_SCADA_DLL_TableView::GetselectedIndexes()
{
    return this->selectedIndexes();
}
