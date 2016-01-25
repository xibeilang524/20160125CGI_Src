#include "CGI_SCADA_DLL_HistoryMessageItem.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>

/*!
 \brief 库文件导出接口函数

 \fn CreateDriver
 \param parent QObject对象指针
 \return CGI_SCADA_DLL_I 返回创建的驱动指针
*/
CGI_SCADA_DLL_I * CreateDriver(QObject *parent)
{
    QStringList strings;
    strings << "历史数据";
    qDebug()<<__func__<<"历史数据";
    CGI_SCADA_DLL_HistoryMessageItem *testProtocol = new CGI_SCADA_DLL_HistoryMessageItem(strings,CGI_SCADA_DLL_I::TreeItemType_HistorM,parent);
    qDebug()<<testProtocol->text(0);
    return  testProtocol;
}

CGI_SCADA_DLL_HistoryMessageItem::CGI_SCADA_DLL_HistoryMessageItem(const QStringList &strings, int type, QObject *parent) :
    CGI_SCADA_DLL_TreeItemBase(strings,type,parent)
{
    QString str = QString("aaaaaaaaaaaa his%1").arg(QString::number(type));
    m_pLabel = new QLabel(str);

    m_pCGI_SCADA_DLL_HistoryM_Setting = new CGI_SCADA_DLL_HistoryM_Setting(QStringList()<<"设置",TreeItemType_HistorM_HistorySettings,this);
    connect(m_pCGI_SCADA_DLL_HistoryM_Setting,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
    this->addChild(m_pCGI_SCADA_DLL_HistoryM_Setting);

    m_pCGI_SCADA_DLL_TreeItemHistorySelect = new CGI_SCADA_DLL_TreeItemHistorySelect(this,QStringList()<<"查询",TreeItemType_HistorM_HistorySelect,this);
    connect(m_pCGI_SCADA_DLL_TreeItemHistorySelect,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));
    this->addChild(m_pCGI_SCADA_DLL_TreeItemHistorySelect);
}

/*!
 \brief 获取树节点的顺序

 \fn CGI_SCADA_DLL_HistoryMessageItem::GetItemNumber
 \return int 这个号码代表的是树节点的顺序，这个号码在所有节点驱动中是唯一的
*/
int CGI_SCADA_DLL_HistoryMessageItem::GetItemNumber()
{
    return 2;
}

QWidget *CGI_SCADA_DLL_HistoryMessageItem::GetWidget(QTreeWidgetItem *pItem_)
{
//    QWidget *pwidget = new QWidget;
//    return pwidget;
    return (QWidget*)m_pLabel;
}

bool CGI_SCADA_DLL_HistoryMessageItem::SaveAction()
{
    m_pCGI_SCADA_DLL_HistoryM_Setting->SaveAction();
    emit signal_ShowMessageDebug(0,QString("通道：历史数据 保存成功"));
    return true;
}

bool CGI_SCADA_DLL_HistoryMessageItem::SetBuildTagMap(const QMap<QString, QStringList> &TagMap_)
{
    m_pCGI_SCADA_DLL_HistoryM_Setting->SetBuildTagMap(TagMap_);
    return true;
}

void CGI_SCADA_DLL_HistoryMessageItem::slot_OpenProject(QString strProjectPath)
{
    qDebug()<<__func__<<__FILE__<<__LINE__;
    CGI_SCADA_DLL_TreeItemBase::slot_OpenProject(strProjectPath);
    m_DBFileList.clear();
    FindFile(strProjectPath);
}
void CGI_SCADA_DLL_HistoryMessageItem::FindFile(QString strPathName_)
{
    QString strPath = strPathName_;
    qDebug()<<strPath;
    QDir dir(strPath);
    if (!dir.exists())
        return ;
    dir.setFilter(QDir::Dirs|QDir::Files);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int i=0;
    QString fileName;
    do{
        QFileInfo fileInfo = list.at(i);
        if((fileInfo.fileName()==".")|(fileInfo.fileName()==".."))
        {
            i++;
            continue;
        }
        bool bisDir=fileInfo.isDir();
        if(bisDir)
        {
            qDebug()<<(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName(),10).arg(fileInfo.path()))<<fileInfo.absoluteFilePath();
//            FindFile(fileInfo.filePath());
        }else{
            fileName = fileInfo.fileName();
            if (fileName == "DB.xml")
            {
                m_DBFileList.append(fileInfo.absoluteFilePath());
                m_pCGI_SCADA_DLL_HistoryM_Setting->SetChannelFile(fileInfo.absoluteFilePath());
            }
            qDebug()<<(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName(),10).arg(fileInfo.path()))<<fileInfo.absoluteFilePath();
        }
        i++;
    }while(i<list.size());
}
