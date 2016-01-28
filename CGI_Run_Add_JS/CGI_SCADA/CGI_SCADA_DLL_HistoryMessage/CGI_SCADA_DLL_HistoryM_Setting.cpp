#include "CGI_SCADA_DLL_HistoryM_Setting.h"
#include "XmlParser/CXmlParser.h"

CGI_SCADA_DLL_HistoryM_Setting::CGI_SCADA_DLL_HistoryM_Setting(const QStringList &strings, int type, QObject *parent) :
    CGI_SCADA_DLL_TreeItemBase(strings,type,parent)
{
//    m_pMainWidget = new CGI_SCADA_DLL_HistoryM_SettingTableView();


    m_pTableWidget = new CGI_SCADA_DLL_HistoryM_SettingTableWidget(this);
    connect(m_pTableWidget,SIGNAL(signal_ShowMessageDebug(int,QString)),this,SIGNAL(signal_ShowMessageDebug(int,QString)));

}

CGI_SCADA_DLL_HistoryM_Setting::~CGI_SCADA_DLL_HistoryM_Setting()
{
    if (m_pTableWidget != NULL)
    {
        m_pTableWidget->deleteLater();
        m_pTableWidget = NULL;
    }
}

QWidget *CGI_SCADA_DLL_HistoryM_Setting::GetWidget(QTreeWidgetItem */*pItem_*/)
{
    return m_pTableWidget;
}

bool CGI_SCADA_DLL_HistoryM_Setting::SaveAction()
{
    QDomDocument DBdoc;
    QDomElement RootElem = DBdoc.createElement(tr("Root"));
    DBdoc.appendChild(RootElem);

    QDomElement DBElem = DBdoc.createElement("DB");
    RootElem.appendChild(DBElem);

    m_pTableWidget->SaveAction(DBdoc,DBElem);


    QString strFileName;
    strFileName = "temp_project/DB.xml";
    CXmlParser::writeFile(strFileName,DBdoc);
}

bool CGI_SCADA_DLL_HistoryM_Setting::SetBuildTagMap(const QMap<QString, QStringList> &TagMap_)
{
    m_pTableWidget->SetBuildTagMap(TagMap_);
    return true;
}

bool CGI_SCADA_DLL_HistoryM_Setting::SetChannelFile(QString strFileName_)
{
    m_pTableWidget->SetDBFile(strFileName_);
    return true;
}


