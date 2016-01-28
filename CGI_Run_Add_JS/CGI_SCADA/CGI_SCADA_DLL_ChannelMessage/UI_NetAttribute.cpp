#include "UI_NetAttribute.h"
#include "ui_UI_NetAttribute.h"

UI_NetAttribute::UI_NetAttribute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UI_NetAttribute),
    m_pModel(new QStandardItemModel)
{
    ui->setupUi(this);

    m_pTableView = new CGI_SCADA_DLL_TableView(ui->groupBox);
    m_pTableView->setModel(m_pModel);
    ui->gridLayout_4->addWidget(m_pTableView,0,1,1,1);

    m_pTableView->setContextMenuPolicy(Qt::CustomContextMenu);//允许弹菜单
    connect(m_pTableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slot_RightClick(QPoint)));
    QStringList strDisplayList;
    strDisplayList<<"IP地址"<<"子网掩码";
    m_pModel->setHorizontalHeaderLabels(strDisplayList);
    m_pTableView->setColumnWidth(0,150);
    m_pTableView->setColumnWidth(1,150);

//    QList<QStandardItem *> Items;
//    QStandardItem *item = new QStandardItem();
//    item->setData(QVariant("192.168.100.100"),Qt::DisplayRole);
//    Items.append(item);
//    item = new QStandardItem();
//    item->setData(QVariant("255.255.255.255"),Qt::DisplayRole);
//    Items.append(item);
//    m_pModel->insertRow(m_pModel->rowCount(),Items);
//    m_pTableView->resizeColumnsToContents();///设置宽度自动适应
//    m_pTableView->resizeRowsToContents();///设置高度自动适应
    QRegExp ipRegExp = QRegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){4}");
    QRegExpValidator *ipRegExpValidator = new QRegExpValidator(ipRegExp, this);
    ui->lineEdit_IPAddr->setValidator(ipRegExpValidator);
    ui->lineEdit_IPAddr->setInputMask("000.000.000.000");

    ui->lineEdit_IPMask->setValidator(ipRegExpValidator);
    ui->lineEdit_IPMask->setInputMask("000.000.000.000");
    ui->lineEdit_IPAddr->setFocus();
}

UI_NetAttribute::~UI_NetAttribute()
{
    delete ui;
}
void UI_NetAttribute::slot_RightClick(QPoint point_)
{
    QMenu *m_pMenu = new QMenu();
//    QAction *AddTag = new QAction(tr("新建单个点"),m_pMenu);
//    m_pMenu->addAction(AddTag);
//    connect(AddTag,SIGNAL(triggered()),this,SLOT(slot_AddTag()));
    QAction *DeleteTag = new QAction(tr("删除IP地址"),m_pMenu);
    m_pMenu->addAction(DeleteTag);
    connect(DeleteTag,SIGNAL(triggered()),this,SLOT(slot_DeleteTag()));
//    QAction *CreateMuchTag = new QAction(tr("新建多个点"),m_pMenu);
//    m_pMenu->addAction(CreateMuchTag);
//    connect(CreateMuchTag,SIGNAL(triggered()),this,SLOT(slot_CreateMuchTag()));
//    QAction *batchChange = new QAction(tr("批量修改"),m_pMenu);
//    m_pMenu->addAction(batchChange);
//    connect(batchChange,SIGNAL(triggered()),this,SLOT(slot_BatchChange()));
//    QAction *replaceChange = new QAction(tr("替换"),m_pMenu);
//    m_pMenu->addAction(replaceChange);
//    connect(replaceChange,SIGNAL(triggered()),this,SLOT(slot_ReplaceChange()));

    m_CurrentModelIndex = m_pTableView->indexAt(point_);
    m_pMenu->exec(QCursor::pos());

//    delete m_pMenu;
    m_pMenu->deleteLater();
}
bool UI_NetAttribute::SaveAction(QDomDocument &/*doc*/, QDomElement &/*parentElem*/)
{
    return true;
}
void UI_NetAttribute::on_pushButton_AddIPAddr_clicked()
{
    QList<QStandardItem *> Items;
    QStandardItem *item = new QStandardItem();
    item->setData(QVariant(ui->lineEdit_IPAddr->text()),Qt::DisplayRole);
    Items.append(item);
    item = new QStandardItem();
    item->setData(QVariant(ui->lineEdit_IPMask->text()),Qt::DisplayRole);
    Items.append(item);
    m_pModel->insertRow(m_pModel->rowCount(),Items);
}
void UI_NetAttribute::slot_DeleteTag()
{
    QMap<int,QModelIndex> ColumnMap;
    for (int i = 0; i < m_pTableView->GetselectedIndexes().count(); ++i)
    {
//        qDebug()<<selectedIndexes().at(i).data();
        ColumnMap.insert(m_pTableView->GetselectedIndexes().at(i).column(),m_pTableView->GetselectedIndexes().at(i));
    }
    if (m_pTableView->GetselectedIndexes().count() > 0)
    {
        int removeRowCount = m_pTableView->GetselectedIndexes().count()/ColumnMap.count();
        m_pModel->removeRows(m_pTableView->GetselectedIndexes().at(0).row(),removeRowCount);
    }
}
