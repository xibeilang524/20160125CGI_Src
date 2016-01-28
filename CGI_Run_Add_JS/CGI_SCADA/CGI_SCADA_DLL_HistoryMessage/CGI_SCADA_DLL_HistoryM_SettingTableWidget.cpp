#include "CGI_SCADA_DLL_HistoryM_SettingTableWidget.h"
#include <QMenu>
#include <QAction>
#include <QDebug>
#include "CBatchChangeDialogDialog.h"
#include "CReplaceDialog.h"
#include "../CGI_SCADA_DLL_ChannelMessage/UI_LinkNameDialog.h"
#include "XmlParser/CXmlParser.h"

CGI_SCADA_DLL_HistoryM_SettingTableWidget::CGI_SCADA_DLL_HistoryM_SettingTableWidget(QTreeWidgetItem *pParentTreeItem_,QWidget *parent) :
    QTableWidget(parent),
    m_pParentTreeItem(pParentTreeItem_)
{
    this->setToolTip("右键添加点删除点等功能");
    this->setColumnCount(3);
//    m_pTableWidget->setRowCount(5);

    QStringList strDisplayList;
    strDisplayList<<"变量名称"<<"变化存储"<<"存储周期";
    this->setHorizontalHeaderLabels(strDisplayList);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slot_customContextMenuRequested(QPoint)));

    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(slot_changeTest(int, int)));
    connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_DoubleClicked(QModelIndex)));

}

bool CGI_SCADA_DLL_HistoryM_SettingTableWidget::SaveAction(QDomDocument &doc, QDomElement &parentElem)
{
    QStringList LinkNameList;///< 作用是去除重复的点
    for (int i = 0; i < this->rowCount(); ++i)
    {
        QString LinkName = this->item(i,0)->text();
        if (LinkNameList.contains(LinkName))
        {

        }else
        {
            LinkNameList.append(LinkName);
            QDomElement PointElem = doc.createElement("Point");
            parentElem.appendChild(PointElem);
            PointElem.setAttribute("Link",LinkName);
            if (this->item(i, 1)->checkState() == Qt::Checked)
            {
                PointElem.setAttribute("ChangeStorage","true");
            }else
            {
                PointElem.setAttribute("ChangeStorage","false");
            }
            PointElem.setAttribute("PeriodicStorage",this->item(i,2)->text());
        }
    }

    return true;
}

bool CGI_SCADA_DLL_HistoryM_SettingTableWidget::SetBuildTagMap(const QMap<QString, QStringList> &TagMap_)
{
    for (int i = 0; i < this->model()->rowCount(); ++i)
    {
        if (TagMap_.contains(this->model()->index(i,GUANLIANBIANLIANGMING_COLUMN,QModelIndex()).data().toString()))
        {

        }else
        {
            QString strDebugMessage = QString("数据库通道中没有此点:%1 第(%2)行").arg(this->model()->index(i,GUANLIANBIANLIANGMING_COLUMN,QModelIndex()).data().toString()).arg(QString::number(i+1));
            qDebug()<<__func__<<__LINE__<<strDebugMessage;
            emit signal_ShowMessageDebug(1,strDebugMessage);
//            ShowMessageDebug();///< 没有此点
        }
    }
}

bool CGI_SCADA_DLL_HistoryM_SettingTableWidget::SetDBFile(QString strFileName_)
{
    qDebug()<<__func__<<__LINE__;
    this->model()->removeRows(0,this->model()->rowCount());
    QDomDocument doc;
    if (CXmlParser::readFile(strFileName_,doc))
    {
        QDomNodeList PointNodeList = doc.elementsByTagName("Point");
        for (int nPointIndex = 0; nPointIndex < PointNodeList.count(); ++nPointIndex)
        {
            QDomElement PointElem = PointNodeList.at(nPointIndex).toElement();
            QTableWidgetItem *pLinkItem = new QTableWidgetItem(PointElem.attribute("Link"));
            QTableWidgetItem *checkBox = new QTableWidgetItem();
            checkBox->setText("变化存储");
            if (PointElem.attribute("ChangeStorage") == "true")
            {
                checkBox->setCheckState(Qt::Checked);
            }else
            {
                checkBox->setCheckState(Qt::Unchecked);
            }
            QTableWidgetItem *pPeriodicStorageItem = new QTableWidgetItem(PointElem.attribute("PeriodicStorage"));
            this->insertRow(this->rowCount());
            this->setItem(this->rowCount()-1, 0, pLinkItem);
            this->setItem(this->rowCount()-1, 1, checkBox);
            this->setItem(this->rowCount()-1, 2, pPeriodicStorageItem);
        }
    }else
    {
        return false;
    }
    return true;
}
void CGI_SCADA_DLL_HistoryM_SettingTableWidget::slot_customContextMenuRequested(QPoint point_)
{
    m_CurrentModelIndex = this->indexAt(point_);
    QMenu *pMenu = new QMenu();
    QAction *AddTag = new QAction(tr("新建单个点"),pMenu);
    pMenu->addAction(AddTag);
    connect(AddTag,SIGNAL(triggered()),this,SLOT(slot_AddTag()));
    QAction *DeleteTag = new QAction(tr("删除点"),pMenu);
    pMenu->addAction(DeleteTag);
    connect(DeleteTag,SIGNAL(triggered()),this,SLOT(slot_DeleteTag()));
    QAction *CreateMuchTag = new QAction(tr("新建多个点"),pMenu);
    pMenu->addAction(CreateMuchTag);
    connect(CreateMuchTag,SIGNAL(triggered()),this,SLOT(slot_CreateMuchTag()));
    QAction *batchChange = new QAction(tr("批量修改"),pMenu);
    pMenu->addAction(batchChange);
    connect(batchChange,SIGNAL(triggered()),this,SLOT(slot_BatchChange()));
    QAction *replaceChange = new QAction(tr("替换"),pMenu);
    pMenu->addAction(replaceChange);
    connect(replaceChange,SIGNAL(triggered()),this,SLOT(slot_ReplaceChange()));

    pMenu->exec(QCursor::pos());

//    delete m_pMenu;
    pMenu->deleteLater();
    return ;

}

/*!
 \brief Checked控件的状态发生变化会触发此槽函数

 \fn CGI_SCADA_DLL_HistoryM_SettingTableWidget::slot_changeTest
 \param row
 \param col
*/
void CGI_SCADA_DLL_HistoryM_SettingTableWidget::slot_changeTest(int row, int col)
{
    qDebug()<<__func__<<__LINE__<<this->item(row, col)->checkState();
    if(this->item(row, col)->checkState() == Qt::Checked) //选中
    {
        qDebug()<<__func__<<__LINE__<<"check";
    }else
    {
        qDebug()<<__func__<<__LINE__<<"uncheck";
    }
}

void CGI_SCADA_DLL_HistoryM_SettingTableWidget::slot_DoubleClicked(QModelIndex index)
{
    qDebug()<<__func__<<__LINE__<<index.column()<<index.row();
    int columnNum = index.column();
    if (columnNum == 0)
    {
        qDebug()<<__func__<<__LINE__<< index.data()<<"双击第一列"<<"弹点表选择框";
        qDebug()<<__func__<<__LINE__<<m_pParentTreeItem->treeWidget();
        UI_LinkNameDialog dialog((QTreeWidgetItem *)m_pParentTreeItem);
        int ret = dialog.exec();
        qDebug()<<ret<<QDialogButtonBox::Ok<<QDialogButtonBox::Cancel;
        if (ret)
        {
            qDebug()<<__func__<<__LINE__<< index.data()<<"双击第一列"<<"弹点表选择框"<<"OK";
            this->model()->setData(index,QVariant(dialog.GetLinkName()));
        }else
        {
            qDebug()<<__func__<<__LINE__<< index.data()<<"双击第一列"<<"弹点表选择框"<<"Cancel";
        }
    }
}

void CGI_SCADA_DLL_HistoryM_SettingTableWidget::slot_AddTag()
{
    qDebug()<<__func__<<__LINE__<<this->rowCount();

    if (m_CurrentModelIndex.row() >= 0)///<zzy 2015/1/23 修改:能够指定位置添加
    {
        this->insertRow(m_CurrentModelIndex.row()+1);

        QTableWidgetItem *pLinkItem = new QTableWidgetItem();
        QTableWidgetItem *pPeriodicStorageItem = new QTableWidgetItem();
        QTableWidgetItem *checkBox = new QTableWidgetItem();
        checkBox->setText("变化存储");
        checkBox->setCheckState(Qt::Checked);
        this ->setItem(m_CurrentModelIndex.row()+1, 0, pLinkItem);
        this ->setItem(m_CurrentModelIndex.row()+1, 1, checkBox);
        this ->setItem(m_CurrentModelIndex.row()+1, 2, pPeriodicStorageItem);
    }else
    {
        this->insertRow(this->rowCount());

        QTableWidgetItem *pLinkItem = new QTableWidgetItem();
        QTableWidgetItem *pPeriodicStorageItem = new QTableWidgetItem();
        QTableWidgetItem *checkBox = new QTableWidgetItem();
        checkBox->setCheckState(Qt::Checked);
        checkBox->setText("变化存储");
        this->setItem(this->rowCount()-1, 0, pLinkItem);
        this->setItem(this->rowCount()-1, 1, checkBox);
        this->setItem(this->rowCount()-1, 2, pPeriodicStorageItem);
    }
}

void CGI_SCADA_DLL_HistoryM_SettingTableWidget::slot_DeleteTag()
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

void CGI_SCADA_DLL_HistoryM_SettingTableWidget::slot_CreateMuchTag()
{
    bool ok;
    int nNumber = QInputDialog::getInt(NULL, tr("批量添加点"),
                                     tr("添加点个数:"), 1, 0, 1000, 1, &ok);
    if (ok)
    {
        for (int i = 0; i < nNumber; ++i)
        {
            slot_AddTag();
        }
    }
}

void CGI_SCADA_DLL_HistoryM_SettingTableWidget::slot_BatchChange()
{
    CBatchChangeDialogDialog dlg;
    int result = dlg.exec();
    if(result)
    {
//        qDebug()<<"确定";
        switch (dlg.GetRadioCondition()) {
        case RadioButtonSame:///相同值模式
            for (int i = 0; i < this->selectedIndexes().count(); ++i)
            {
                this->model()->setData(selectedIndexes().at(i),dlg.GetBaseValue());
            }
            break;
        case RadioButtonProgressively:///递增值模式
        {
            int nBaseValue = dlg.GetBaseValue().toInt();
            int nInterval = dlg.GetInterval().toInt();
            for (int i = 0; i < this->selectedIndexes().count(); ++i)
            {
                this->model()->setData(selectedIndexes().at(i),QVariant(nBaseValue));
                nBaseValue += nInterval;
            }
        }
            break;
        case RadioButtonDecrease:///递减值模式
        {
            int nBaseValue = dlg.GetBaseValue().toInt();
            int nInterval = dlg.GetInterval().toInt();
            for (int i = 0; i < this->selectedIndexes().count(); ++i)
            {
                this->model()->setData(selectedIndexes().at(i),QVariant(nBaseValue));
                nBaseValue -= nInterval;
            }
        }
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }else
    {
//        qDebug()<<"取消";
    }
}
// 获取checkbox列的checked状态
//for (int i = 0; i < this->rowCount(); ++i)
//{
//    qDebug()<<__func__<<__LINE__<<this->item(i, 1)->checkState();
//}

void CGI_SCADA_DLL_HistoryM_SettingTableWidget::slot_ReplaceChange()
{
    CReplaceDialog dialog;
    if (dialog.exec())
    {
        for (int i = 0; i < this->selectedIndexes().count(); ++i)
        {
            QString strDisplay;
            strDisplay = this->model()->data(selectedIndexes().at(i)).toString();
            strDisplay.replace(dialog.GetBefore(),dialog.GetAfter(),Qt::CaseSensitive);
            this->model()->setData(selectedIndexes().at(i),strDisplay);
//            qDebug()<<strDisplay;
        }
    }else
    {

    }
}
