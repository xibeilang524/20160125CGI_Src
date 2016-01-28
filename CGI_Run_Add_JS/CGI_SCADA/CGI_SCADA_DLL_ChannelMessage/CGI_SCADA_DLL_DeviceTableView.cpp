#include "CGI_SCADA_DLL_DeviceTableView.h"
#include <QHeaderView>
#include "UI_LinkNameDialog.h"

CGI_SCADA_DLL_DeviceTableView::CGI_SCADA_DLL_DeviceTableView(ProtocolType nProtocolType,QTreeWidgetItem *pParentTreeItem_, QString strLinkName_, Device_Type deviceType_, QWidget *parent) :
    QTableView(parent),
    m_pParentTreeItem(pParentTreeItem_),
    m_DeviceType(deviceType_),
    m_pModel(new QStandardItemModel),
    m_strLinkName(strLinkName_),
    m_nProtocolType(nProtocolType)
{
    qDebug()<<__func__<<__LINE__<<"nProtocolType"<<nProtocolType<<pParentTreeItem_->text(0);
    this->setModel(m_pModel);
    this->setSortingEnabled(true);///< 设置排序
    this->setContextMenuPolicy(Qt::CustomContextMenu);//允许弹菜单
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slot_RightClick(QPoint)));

    /// 添加排序功能
//    QHeaderView *headerGoods = this->horizontalHeader();
    //SortIndicator为水平标题栏文字旁边的三角指示器
//    headerGoods->setSortIndicator(0, Qt::AscendingOrder);
//    headerGoods->setSortIndicatorShown(true);
//    headerGoods->setSectionsClickable(true);
//    connect(headerGoods, SIGNAL(sectionClicked(int)), this, SLOT (sortByColumn(int)));
    SetInitType();
    connect(m_pModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(slot_ModelDataChanged(QModelIndex,QModelIndex)));
}

CGI_SCADA_DLL_DeviceTableView::~CGI_SCADA_DLL_DeviceTableView()
{
    qDebug()<<__func__<<__FILE__<<__LINE__<<"start";
    if (m_pModel != NULL)
    {
        m_pModel->deleteLater();
        m_pModel = NULL;
    }
    qDebug()<<__func__<<__FILE__<<__LINE__<<"start";
}

bool CGI_SCADA_DLL_DeviceTableView::SaveAction(QDomDocument &doc, QDomElement &parentElem)
{
    parentElem.setAttribute("Name",m_DeviceType.m_strValue);
    parentElem.setAttribute("Display",m_DeviceType.m_strTypeName);
    for (int i = 0; i < m_pModel->rowCount(); ++i)
    {
        QDomElement TagElem = doc.createElement("Point");
        for (int j = 0; j < m_DeviceType.m_PropertyList.count(); ++j)
        {
            TagElem.setAttribute(m_DeviceType.m_PropertyList.at(j).m_strvar,m_pModel->index(i,j,QModelIndex()).data().toString());
        }
        parentElem.appendChild(TagElem);
    }
    return true;
}

bool CGI_SCADA_DLL_DeviceTableView::SetInitTagMap(QMap<QString, QStringList> &TagMap_)
{
    for (int i = 0; i < m_pModel->rowCount(); ++i)
    {
        if (TagMap_.contains(m_pModel->index(i,GUANLIANBIANLIANGMING_COLUMN,QModelIndex()).data().toString()))
        {
            QString strDebugMessage = QString("采集通道:%1 点类型:%2 中已经包含此点:%3 第(%4)行").arg(m_strLinkName).arg(m_DeviceType.m_strTypeName).arg(m_pModel->index(i,GUANLIANBIANLIANGMING_COLUMN,QModelIndex()).data().toString()).arg(QString::number(i+1));
            qDebug()<<__func__<<__LINE__<<strDebugMessage;
            emit signal_ShowMessageDebug(1,strDebugMessage);
//            ShowMessageDebug();
        }else
        {
            QStringList strList;
            TagMap_.insert(m_pModel->index(i,GUANLIANBIANLIANGMING_COLUMN,QModelIndex()).data().toString(),strList);
        }
    }
    return true;
}

bool CGI_SCADA_DLL_DeviceTableView::SetBuildTagMap(const QMap<QString, QStringList> &TagMap_)
{
    for (int i = 0; i < m_pModel->rowCount(); ++i)
    {
        if (TagMap_.contains(m_pModel->index(i,GUANLIANBIANLIANGMING_COLUMN,QModelIndex()).data().toString()))
        {

        }else
        {
            QString strDebugMessage = QString("转发通道:%1 点类型:%2 中没有此点:%3 第(%4)行").arg(m_strLinkName).arg(m_DeviceType.m_strTypeName).arg(m_pModel->index(i,GUANLIANBIANLIANGMING_COLUMN,QModelIndex()).data().toString()).arg(QString::number(i+1));
            qDebug()<<__func__<<__LINE__<<strDebugMessage;
            emit signal_ShowMessageDebug(1,strDebugMessage);
//            ShowMessageDebug();///< 没有此点
        }
    }
    return true;
}

bool CGI_SCADA_DLL_DeviceTableView::SetTypeElem(const QDomElement &TypeElem_)
{
    QDomNodeList TagList = TypeElem_.elementsByTagName("Point");
    int nTagCount = TagList.count();
    m_pModel->setRowCount(nTagCount);
    for (int i = 0; i < nTagCount; ++i)
    {
        for (int j = 0; j < m_DeviceType.m_PropertyList.count(); ++j)
        {
            QString strValue = TagList.at(i).toElement().attribute(m_DeviceType.m_PropertyList.at(j).m_strvar);
            m_pModel->setData(m_pModel->index(i, j, QModelIndex()),strValue);
//            TagElem.setAttribute(m_DeviceType.m_PropertyList.at(j).m_strvar,m_pModel->index(i,j,QModelIndex()).data().toString());
        }
//        for (int j = 0; j < m_strDisplayList.count(); ++j)
//        {
//            QString strValue = TagList.at(i).toElement().attribute(m_strDisplayList.at(j).->value(j)->m_strName);
//            if (m_pParameterMap->value(j)->m_strName == "SN")///"序号"列做特殊处理 为了能够实现"序号"列按数字排序
//            {
//                if (strValue.isEmpty())
//                {
//                    m_pModel->setData(m_pModel->index(i, j, QModelIndex()),QVariant(m_pParameterMap->value(j)->m_strInitValue.toInt()));
//                }else
//                {
//                    m_pModel->setData(m_pModel->index(i, j, QModelIndex()),QVariant(strValue.toInt()));
//                }
//                continue;
//            }else
//            {
//                if (strValue.isEmpty())
//                {
//                    m_pModel->setData(m_pModel->index(i, j, QModelIndex()),m_pParameterMap->value(j)->m_strInitValue);
//                }else
//                {
//                    m_pModel->setData(m_pModel->index(i, j, QModelIndex()),strValue);
//                }
//            }
//        }
    }

    this->resizeColumnsToContents();///设置宽度自动适应
    this->resizeRowsToContents();///设置高度自动适应
    return true;
}

bool CGI_SCADA_DLL_DeviceTableView::SetInitStringListModel(QStringListModel *pStringListModel)
{
    int nstrListModelRowCount = pStringListModel->rowCount();
    pStringListModel->insertRows(nstrListModelRowCount,m_pModel->rowCount(),QModelIndex());
    for (int nCount = 0; nCount < m_pModel->rowCount(); ++nCount,++nstrListModelRowCount)
    {
//        pStringListModel->index(nstrListModelRowCount,0,QModelIndex())
        qDebug()<<__func__<<__LINE__<<m_pModel->index(nCount,0,QModelIndex()).data()<<nCount<<nstrListModelRowCount;
        pStringListModel->setData(pStringListModel->index(nstrListModelRowCount,0,QModelIndex()),m_pModel->index(nCount,0,QModelIndex()).data());

    }
    return true;
}

void CGI_SCADA_DLL_DeviceTableView::slot_ProtocolTypeChange(int nProtocolType_)
{
    m_nProtocolType = (ProtocolType)nProtocolType_;
}
void CGI_SCADA_DLL_DeviceTableView::SetInitType()
{
    m_pModel->setColumnCount(m_DeviceType.m_PropertyList.count());
    CItemDelegateAll cItemDelegate;
    for (int nColumnCount = 0; nColumnCount < m_DeviceType.m_PropertyList.count(); ++nColumnCount)
    {
        QStringList strList;
        Device_Type_Property *pDeviceTypeProperty = &(m_DeviceType.m_PropertyList[nColumnCount]);

        m_strDisplayList.push_back(m_DeviceType.m_PropertyList.at(nColumnCount).m_strName);
        if (pDeviceTypeProperty->m_StrEditType == "select")
        {
            strList = pDeviceTypeProperty->m_strListEnumNames;
        }else
        {
            strList.push_back(pDeviceTypeProperty->m_strInitValue);
        }
        setItemDelegateForColumn(nColumnCount,cItemDelegate.GetItemDelegate(m_DeviceType.m_PropertyList.at(nColumnCount).m_StrEditType,strList));
    }
    m_pModel->setHorizontalHeaderLabels(m_strDisplayList);

    this->resizeColumnsToContents();///设置宽度自动适应
    this->resizeRowsToContents();///设置高度自动适应

    connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_DoubleClicked(QModelIndex)));

}

/*!
 \brief 接收右键点击的槽函数

 \fn CTableViewDevice::slot_RightClick
 \param point_ 右键的坐标
 \author zzy
 \date   2015/5/29
*/
void CGI_SCADA_DLL_DeviceTableView::slot_RightClick(QPoint point_)
{
    QMenu *m_pMenu = new QMenu();
    QAction *AddTag = new QAction(tr("新建单个点"),m_pMenu);
    m_pMenu->addAction(AddTag);
    connect(AddTag,SIGNAL(triggered()),this,SLOT(slot_AddTag()));
    QAction *DeleteTag = new QAction(tr("删除点"),m_pMenu);
    m_pMenu->addAction(DeleteTag);
    connect(DeleteTag,SIGNAL(triggered()),this,SLOT(slot_DeleteTag()));
    QAction *CreateMuchTag = new QAction(tr("新建多个点"),m_pMenu);
    m_pMenu->addAction(CreateMuchTag);
    connect(CreateMuchTag,SIGNAL(triggered()),this,SLOT(slot_CreateMuchTag()));
    QAction *batchChange = new QAction(tr("批量修改"),m_pMenu);
    m_pMenu->addAction(batchChange);
    connect(batchChange,SIGNAL(triggered()),this,SLOT(slot_BatchChange()));
    QAction *replaceChange = new QAction(tr("替换"),m_pMenu);
    m_pMenu->addAction(replaceChange);
    connect(replaceChange,SIGNAL(triggered()),this,SLOT(slot_ReplaceChange()));

    m_CurrentModelIndex = this->indexAt(point_);
    m_pMenu->exec(QCursor::pos());

//    delete m_pMenu;
    m_pMenu->deleteLater();
}
/*!
 \brief 添加多个点

 \fn CGI_SCADA_DLL_DeviceTableView::slot_CreateMuchTag
*/
void CGI_SCADA_DLL_DeviceTableView::slot_CreateMuchTag()
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

/*!
 \brief 添加单个点

 \fn CGI_SCADA_DLL_DeviceTableView::slot_AddTag
*/
void CGI_SCADA_DLL_DeviceTableView::slot_AddTag()
{
    m_Items.clear();
    for (int i = 0; i < m_DeviceType.m_PropertyList.count(); ++i)
    {
        qDebug()<<__func__<<__LINE__<<m_DeviceType.m_PropertyList.at(i).m_strDisplayType<<m_DeviceType.m_PropertyList.at(i).m_strInitValue<<m_DeviceType.m_PropertyList.at(i).m_strName;
        QStandardItem *item = new QStandardItem();
        if (m_DeviceType.m_PropertyList.at(i).m_strDisplayType == "number")
        {
            item->setData(QVariant(m_DeviceType.m_PropertyList.at(i).m_strInitValue.toInt()),Qt::EditRole);
        }else
        {
            item->setData(QVariant(m_DeviceType.m_PropertyList.at(i).m_strInitValue),Qt::EditRole);
        }
        m_Items.append(item);
    }

    if (m_CurrentModelIndex.row() >= 0)///<zzy 2015/1/23 修改:能够指定位置添加
    {
        m_pModel->insertRow(m_CurrentModelIndex.row(),m_Items);
    }else
    {
        m_pModel->insertRow(m_pModel->rowCount(),m_Items);
    }
}

void CGI_SCADA_DLL_DeviceTableView::slot_DeleteTag()
{
    QMap<int,QModelIndex> ColumnMap;
    for (int i = 0; i < this->selectedIndexes().count(); ++i)
    {
//        qDebug()<<selectedIndexes().at(i).data();
        ColumnMap.insert(selectedIndexes().at(i).column(),selectedIndexes().at(i));
    }
    if (selectedIndexes().count() > 0)
    {
        int removeRowCount = selectedIndexes().count()/ColumnMap.count();
        m_pModel->removeRows(selectedIndexes().at(0).row(),removeRowCount);
    }
}
/*!
 \brief 替换

 \fn CGI_SCADA_DLL_DeviceTableView::slot_BatchChange
*/
void CGI_SCADA_DLL_DeviceTableView::slot_BatchChange()
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
                m_pModel->setData(selectedIndexes().at(i),dlg.GetBaseValue());
            }
            break;
        case RadioButtonProgressively:///递增值模式
        {
            int nBaseValue = dlg.GetBaseValue().toInt();
            int nInterval = dlg.GetInterval().toInt();
            for (int i = 0; i < this->selectedIndexes().count(); ++i)
            {
                m_pModel->setData(selectedIndexes().at(i),QVariant(nBaseValue));
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
                m_pModel->setData(selectedIndexes().at(i),QVariant(nBaseValue));
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

void CGI_SCADA_DLL_DeviceTableView::slot_ReplaceChange()
{
    CReplaceDialog dialog;
    if (dialog.exec())
    {
        for (int i = 0; i < this->selectedIndexes().count(); ++i)
        {
            QString strDisplay;
            strDisplay = m_pModel->data(selectedIndexes().at(i)).toString();
            strDisplay.replace(dialog.GetBefore(),dialog.GetAfter(),Qt::CaseSensitive);
            m_pModel->setData(selectedIndexes().at(i),strDisplay);
//            qDebug()<<strDisplay;
        }
    }else
    {

    }
}

void CGI_SCADA_DLL_DeviceTableView::slot_ModelDataChanged(QModelIndex topLeft, QModelIndex bottomRight)
{
    switch (m_nProtocolType) {
    case ProtocolType_Undefine:
    case ProtocolType_Collect:
        if (topLeft == bottomRight)
        {
            qDebug()<<__func__<<__LINE__<< topLeft.data()<<bottomRight.data();
            qDebug()<<topLeft.row()<<topLeft.column();
            if (topLeft.column() == BIANLIANGMING_COLUMN)
            {
                m_pModel->setData(m_pModel->index(topLeft.row(),GUANLIANBIANLIANGMING_COLUMN, QModelIndex()),QVariant(m_strLinkName+topLeft.data().toString()));

            }
        }else
        {
            qDebug()<<__func__<<__LINE__<< topLeft.data()<<bottomRight.data()<<"不相等";
        }
        break;
    case ProtocolType_Transform:
        break;
    default:
        if (topLeft == bottomRight)
        {
            qDebug()<<__func__<<__LINE__<< topLeft.data()<<bottomRight.data();
            qDebug()<<topLeft.row()<<topLeft.column();
            if (topLeft.column() == BIANLIANGMING_COLUMN)
            {
                m_pModel->setData(m_pModel->index(topLeft.row(),GUANLIANBIANLIANGMING_COLUMN, QModelIndex()),QVariant(m_strLinkName+topLeft.data().toString()));

            }
        }else
        {
            qDebug()<<__func__<<__LINE__<< topLeft.data()<<bottomRight.data()<<"不相等";
        }
        break;
    }
}

void CGI_SCADA_DLL_DeviceTableView::slot_DoubleClicked(QModelIndex index)
{
    int columnNum = index.column();
    switch (m_nProtocolType) {
    case ProtocolType_Undefine:

        break;
    case ProtocolType_Collect:

        break;
    case ProtocolType_Transform:
        if (columnNum == GUANLIANBIANLIANGMING_COLUMN)
        {
            qDebug()<<__func__<<__LINE__<< index.data()<<"双击第一列"<<"弹点表选择框";
            qDebug()<<__func__<<__LINE__<<m_pParentTreeItem->treeWidget();
            UI_LinkNameDialog dialog((QTreeWidgetItem *)m_pParentTreeItem);
            int ret = dialog.exec();
            qDebug()<<ret<<QDialogButtonBox::Ok<<QDialogButtonBox::Cancel;
            if (ret)
            {
                qDebug()<<__func__<<__LINE__<< index.data()<<"双击第一列"<<"弹点表选择框"<<"OK";
                m_pModel->setData(index,QVariant(dialog.GetLinkName()));
            }else
            {
                qDebug()<<__func__<<__LINE__<< index.data()<<"双击第一列"<<"弹点表选择框"<<"Cancel";

            }
        }
        break;
    default:
        break;
    }

}
