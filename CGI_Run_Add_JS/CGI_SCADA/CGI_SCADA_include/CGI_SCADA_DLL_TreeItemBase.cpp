#include "CGI_SCADA_DLL_TreeItemBase.h"
#include "CGI_SCADA_define.h"


QString CGI_SCADA_DLL_TreeItemBase::m_strProjectPaths;
CGI_SCADA_DLL_TreeItemBase::~CGI_SCADA_DLL_TreeItemBase()
{
    qDebug()<<__func__<<__FILE__<<__LINE__<<"start"<<this->text(0);
    qDebug()<<__func__<<__FILE__<<__LINE__<<"end"<<this->text(0);
}

void CGI_SCADA_DLL_TreeItemBase::SetIPAddress_Cookie(QString strIPAddress_, QByteArray baCookie_)
{
    CGI_SCADA_define::s_strIPAddress = strIPAddress_;
    CGI_SCADA_define::s_baCookie = baCookie_;
}

int CGI_SCADA_DLL_TreeItemBase::GetItemNumber()
{
    return -1;
}

QWidget *CGI_SCADA_DLL_TreeItemBase::GetWidget(QTreeWidgetItem */*pItem_*/)
{
    return NULL;
}

void CGI_SCADA_DLL_TreeItemBase::GetPopUpMenu()
{
    return ;
}

bool CGI_SCADA_DLL_TreeItemBase::SaveAction(QDomDocument &/*doc*/, QDomElement &/*parentElem*/)
{
    qDebug()<<__func__<<__LINE__<<__FILE__<<this->text(0);
    return false;
}

bool CGI_SCADA_DLL_TreeItemBase::SetChannelFile(QString /*strFileName_*/)
{
    qDebug()<<"----------------------------------------------------";
    return true;
}

bool CGI_SCADA_DLL_TreeItemBase::SetInitTagMap(QMap<QString, QStringList> &/*TagMap_*/)
{
    qDebug()<<__func__<<__LINE__<<this->text(0);
    return true;
}

bool CGI_SCADA_DLL_TreeItemBase::SetBuildTagMap(const QMap<QString, QStringList> &/*TagMap_*/)
{
    qDebug()<<__func__<<__LINE__<<this->text(0);
    return true;
}

void CGI_SCADA_DLL_TreeItemBase::slot_OpenProject(QString strProjectPath)
{
    qDebug()<<__func__<<__FILE__<<__LINE__<<strProjectPath;
    CGI_SCADA_DLL_TreeItemBase::m_strProjectPaths = strProjectPath;
}

bool CGI_SCADA_DLL_TreeItemBase::SaveAction()
{
    qDebug()<<__func__<<__LINE__<<__FILE__<<this->text(0);
    return false;
}
