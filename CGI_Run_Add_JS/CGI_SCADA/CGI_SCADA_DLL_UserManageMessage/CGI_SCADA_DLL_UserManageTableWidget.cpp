#include "CGI_SCADA_DLL_UserManageTableWidget.h"
#include <QMenu>
#include <QAction>
#include <QDebug>

CGI_SCADA_DLL_UserManageTableWidget::CGI_SCADA_DLL_UserManageTableWidget(QWidget *parent) :
    QTableWidget(parent)
{////< 3 4 5列都是checkbox  7 8 9 列都是按钮
//    this->setToolTip("右键添加点删除点等功能");
    this->setColumnCount(7);
    this->setRowCount(0);

    QStringList strDisplayList;
    strDisplayList<<"用户名"<<"描述"<<"密码"<<"查询权限"<<"修改权限"<<"备份权限"<<"最后登录时间"<<"查询日志"<<"保存修改"<<"删除用户";
    this->setHorizontalHeaderLabels(strDisplayList);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slot_customContextMenuRequested(QPoint)));

//    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(slot_changeTest(int, int)));
//    connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_DoubleClicked(QModelIndex)));
}
void CGI_SCADA_DLL_UserManageTableWidget::slot_customContextMenuRequested(QPoint point_)
{
    m_CurrentModelIndex = this->indexAt(point_);
    QMenu *pMenu = new QMenu();
    QAction *AddUser = new QAction(tr("新建用户"),pMenu);
    pMenu->addAction(AddUser);
    connect(AddUser,SIGNAL(triggered()),this,SLOT(slot_AddUser()));
    QAction *DeleteUser = new QAction(tr("删除用户"),pMenu);
    pMenu->addAction(DeleteUser);
    connect(DeleteUser,SIGNAL(triggered()),this,SLOT(slot_DeleteUser()));
    QAction *InquireLog = new QAction(tr("查询日志"),pMenu);
    pMenu->addAction(InquireLog);
    connect(InquireLog,SIGNAL(triggered()),this,SLOT(slot_InquireLog()));
    QAction *SaveChange = new QAction(tr("保存修改"),pMenu);
    pMenu->addAction(SaveChange);
    connect(SaveChange,SIGNAL(triggered()),this,SLOT(slot_SaveChange()));

    pMenu->exec(QCursor::pos());

//    delete m_pMenu;
    pMenu->deleteLater();
    return ;

}
void CGI_SCADA_DLL_UserManageTableWidget::slot_AddUser()
{
    qDebug()<<__func__<<__LINE__<<this->rowCount();

    if (m_CurrentModelIndex.row() >= 0)///<zzy 2015/1/23 修改:能够指定位置添加
    {
        this->insertRow(m_CurrentModelIndex.row()+1);
        QTableWidgetItem *InquirecheckBox = new QTableWidgetItem("查询");
        InquirecheckBox->setCheckState(Qt::Checked);
        QTableWidgetItem *Change = new QTableWidgetItem("修改");
        Change->setCheckState(Qt::Checked);

        QTableWidgetItem *BackUp = new QTableWidgetItem("备份");
        BackUp->setCheckState(Qt::Checked);
        this ->setItem(m_CurrentModelIndex.row()+1, 3, InquirecheckBox);
        this ->setItem(m_CurrentModelIndex.row()+1, 4, Change);
        this ->setItem(m_CurrentModelIndex.row()+1, 5, BackUp);
    }else
    {
        this->insertRow(this->rowCount());

        QTableWidgetItem *InquirecheckBox = new QTableWidgetItem("查询");
        InquirecheckBox->setCheckState(Qt::Checked);
        QTableWidgetItem *Change = new QTableWidgetItem("修改");
        Change->setCheckState(Qt::Checked);

        QTableWidgetItem *BackUp = new QTableWidgetItem("备份");
        BackUp->setCheckState(Qt::Checked);
        this->setItem(this->rowCount()-1, 3, InquirecheckBox);
        this->setItem(this->rowCount()-1, 4, Change);
        this->setItem(this->rowCount()-1, 5, BackUp);
    }
}
void CGI_SCADA_DLL_UserManageTableWidget::slot_DeleteUser()
{
    QMap<int,QModelIndex> ColumnMap;
    for (int i = 0; i < this->selectedIndexes().count(); ++i)
    {
        qDebug()<<selectedIndexes().at(i).data();
        ColumnMap.insert(selectedIndexes().at(i).column(),selectedIndexes().at(i));
    }
    if (selectedIndexes().count() > 0)
    {
        int removeRowCount = selectedIndexes().count()/ColumnMap.count();
        this->model()->removeRows(selectedIndexes().at(0).row(),removeRowCount);
    }
}
/*!
 \brief 查询日志

 \fn CGI_SCADA_DLL_UserManageTableWidget::slot_InquireLog
*/
void CGI_SCADA_DLL_UserManageTableWidget::slot_InquireLog()
{

}

/*!
 \brief 保存修改

 \fn CGI_SCADA_DLL_UserManageTableWidget::slot_SaveChange
*/
void CGI_SCADA_DLL_UserManageTableWidget::slot_SaveChange()
{

}
