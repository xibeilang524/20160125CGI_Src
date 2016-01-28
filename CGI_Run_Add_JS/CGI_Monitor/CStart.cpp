#include "CStart.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include "Src/cmyfunction.h"

CStart::CStart(QObject *parent) :
    QObject(parent)
{
    FindFile();
    m_pInitRTDB = new CInitRTDB(&m_ComFileList,&m_NetFileList,&m_CANFileList);
    m_pInitRTDB->InitRealTimeDB();

}

void CStart::FindFile()
{
    qDebug()<<QDir::currentPath()+"../project";
    QString strPath = QDir::currentPath().remove("cgi-bin")+"project";
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
            if (fileName.left(3) == "Com")
            {
                m_ComFileList.append(fileInfo.absoluteFilePath());
            }else if (fileName.left(3) == "Net")
            {
                m_NetFileList.append(fileInfo.absoluteFilePath());
            }else if (fileName.left(3) == "Can")
            {
                m_CANFileList.append(fileInfo.absoluteFilePath());
            }else if (fileName == "Information_.xml")
            {
                InitMonitorIP(fileInfo.absoluteFilePath());
            }
            qDebug()<<(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName(),10).arg(fileInfo.path()))<<fileInfo.absoluteFilePath();
        }
        i++;
    }while(i<list.size());

    qDebug()<<"m_ComFileList"<<m_ComFileList;
    qDebug()<<"m_NetFileList"<<m_NetFileList;
    qDebug()<<"m_CANFileList"<<m_CANFileList;
}
void CStart::InitMonitorIP(QString strFileName_Informtion_)
{
    QDomDocument doc;
    if (CXmlParser::readFile(strFileName_Informtion_,doc))
    {
        if (!doc.elementsByTagName("Monitor").isEmpty())
        {
            QDomElement MonitorElement = doc.elementsByTagName("Monitor").at(0).toElement();
            CMyFunction::MonitorIPAddress = MonitorElement.attribute("MonitorIPAddress");
            qDebug()<<__func__<<"CMyFunction::MonitorIPAddress:"<<CMyFunction::MonitorIPAddress ;
        }
    }
}
const QStringList CStart::GetFileList(ChannelType nFileListType_) const
{
    switch (nFileListType_) {
    case ChannelType_COM:
        return m_ComFileList;
        break;
    case ChannelType_NET:
        return m_NetFileList;
        break;
    case ChannelType_CAN:
        return m_CANFileList;
        break;
    default:
        return QStringList();
        break;
    }
}
