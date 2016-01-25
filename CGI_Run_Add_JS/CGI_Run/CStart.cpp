#include "CStart.h"
#include <QDomDocument>
#include <QDomElement>
#include "../PortInclude/cvaluebase.h"
#include "../PortInclude/CPointI.h"
#include "../PortInclude/CValueI.h"
#include "Src/ctagbase.h"
#include "Src/cmyfunction.h"
#include <QDateTime>
#include <QDebug>
#include <QTest>
#include <QScriptEngine>
#include <QScriptValue>
#include "CJavaScript_Thread.h"


//CRTDBBase g_RealTimeDB;

CStart::CStart(QObject *parent) :
    QObject(parent),
    m_nTagID(0)
{

    m_pSharedMemoryTagMap = new QMap<int,CSharedMemoryTag *>;
    m_pRTDBSharedMemoryThread = new CRTDBSharedMemoryThread;
    CMyFunction::m_pRTDBSharedMemoryThread = m_pRTDBSharedMemoryThread;
    m_pRealTimeDB = new CRTDBBase;

    FindFile();
    qDebug()<<m_ComFileList<<'\n'<<m_NetFileList;
    InitRealTimeDB();


    InitChannel();
    m_nDisplayPointMessageTimerID = startTimer(10000);

    /// 运行脚本文件的启动节点的脚本
    qDebug()<<"jasc";
    Init_JavaScript_Start();

    ///
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

void CStart::timerEvent(QTimerEvent */*event*/)
{
#if 0
    if (event->timerId() == m_nDisplayPointMessageTimerID)
    {
//        QMap<QString,CTagI *> *pRTDBMap = g_RealTimeDB.GetTAG_MAP();
        QMap<QString,CTagI *> *pRTDBMap = m_pRealTimeDB->GetTAG_MAP();
        QMap<QString,CTagI *>::iterator iterator;

        qDebug()<<"\n--------------- --------------- Point value start ---------------------------------------";
        qDebug()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        for (iterator = pRTDBMap->begin(); iterator != pRTDBMap->end(); ++iterator)
        {
            CTagI *pTempTagI = iterator.value();
            if (pTempTagI)
            {
                if (pTempTagI->GetPointCollection())
                {
                    if (pTempTagI->GetPointCollection()->GetValue())
                    {
                        qDebug()<<"caiji"<<iterator.key()<<pTempTagI->GetType()<<pTempTagI->GetPointCollection()->GetValue()->GetVarValue().toFloat();
                    }
                }
            }
            qDebug()<<"gongc"<<iterator.key()<<pTempTagI->GetType()<<pTempTagI->GetProjectValue()->GetVarValue().toFloat();
        }
        qDebug()<<"--------------- --------------- Point value end -----------------------------------------\n";
    }
#endif
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
            }else if (fileName.left(2) == "DB")
            {
                m_DBFileList.append(fileInfo.absoluteFilePath());
            }else if (fileName.left(2) == "js")
            {
                m_JSFileList.append(fileInfo.absoluteFilePath());
            }
            qDebug()<<(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName(),10).arg(fileInfo.path()))<<fileInfo.absoluteFilePath();
        }
        i++;
    }while(i<list.size());
}

void CStart::InitRealTimeDB()
{
    ReadFilePoint(m_ComFileList);
    ReadFilePoint(m_NetFileList);
    ReadFilePoint(m_CANFileList);
}

void CStart::ReadFilePoint(QStringList &strFileNameList)
{
    if (!strFileNameList.isEmpty())
    {
        for (int i = 0; i < strFileNameList.count(); ++i)
        {
            ReadFilePoint(strFileNameList.at(i));
        }
    }
}

bool CStart::ReadFilePoint(QString strFileName)
{
//    enum TAG_TYPE
//    {
//        TAG_TYPE_AI = 1,///< 遥测
//        TAG_TYPE_AO,    ///< 遥设
//        TAG_TYPE_DI,    ///< 遥信
//        TAG_TYPE_DO,    ///< 遥控
//        TAG_TYPE_MEM,   ///< 遥脉
//    };
    QDomDocument doc;
    if (CXmlParser::readFile(strFileName,doc))
    {
        qDebug()<<"\n**********************************************************\n"<<strFileName;
        QDomNodeList TypeElemList = doc.elementsByTagName("Type");
        CTagBase *pTagBase = NULL;// = new CTagBase(TagName,TagDesc,nID,);
        CValueI *pValue;
        for (int i = 0; i < TypeElemList.count(); ++i)
        {
            QDomElement TypeElem = TypeElemList.at(i).toElement();
            qDebug()<<TypeElem.tagName()<<TypeElem.attribute("Name");
            QDomNodeList PointList = TypeElem.elementsByTagName("Point");
            qDebug()<<"*********"<<TypeElem.parentNode().toElement().attribute("Name")<<"***********";
            QString TypeElem_Name_Attribute = TypeElem.attribute("Name");
            for (int nPointCount = 0; nPointCount < PointList.count(); ++nPointCount)
            {
                QDomElement PointElem = PointList.at(nPointCount).toElement();
                QString strLinkName;
                pTagBase = NULL;
                if (TypeElem_Name_Attribute == "YX")
                {/*Device_YX_Link_Strings*/

                    qDebug()<<TypeElem.attribute("Name")<<PointElem.attribute("Device_YX_Link_Strings")<<PointElem.attribute("Device_YX_Name_Strings");
                    strLinkName = PointElem.attribute("Device_YX_Link_Strings");
                    QVariant variant(0);///<zzy 2015/1/12 修改
                    pValue = new CValueBase;
                    pValue->SetVarValue(variant);
                    pTagBase = new CTagBase(TAG_TYPE_DI,strLinkName
                                            ,PointElem.attribute("Device_YX_Desc_Strings")
                                            ,m_nTagID++
                                            ,DATA_TYPE_INT32U
                                            ,pValue);

                }else if (TypeElem_Name_Attribute == "YC")
                {
                    qDebug()<<TypeElem.attribute("Name")<<PointElem.attribute("Device_YC_Link_Strings")<<PointElem.attribute("Device_YC_Name_Strings");
                    strLinkName = PointElem.attribute("Device_YC_Link_Strings");
                    QVariant variant(0);///<zzy 2015/1/12 修改
                    pValue = new CValueBase;
                    pValue->SetVarValue(variant);
                    pTagBase = new CTagBase(TAG_TYPE_AI,strLinkName
                                            ,PointElem.attribute("Device_YC_Desc_Strings")
                                            ,m_nTagID++
                                            ,DATA_TYPE_INT32U
                                            ,pValue);
                }else if (TypeElem_Name_Attribute == "YM")
                {
                    qDebug()<<TypeElem.attribute("Name")<<PointElem.attribute("Device_YM_Link_Strings")<<PointElem.attribute("Device_YM_Name_Strings");
                    strLinkName = PointElem.attribute("Device_YM_Link_Strings");
                    QVariant variant(0);///<zzy 2015/1/12 修改
                    pValue = new CValueBase;
                    pValue->SetVarValue(variant);
                    pTagBase = new CTagBase(TAG_TYPE_MEM,strLinkName
                                            ,PointElem.attribute("Device_YM_Desc_Strings")
                                            ,m_nTagID++
                                            ,DATA_TYPE_INT32U
                                            ,pValue);
                }else if (TypeElem_Name_Attribute == "YK")
                {
                    qDebug()<<TypeElem.attribute("Name")<<PointElem.attribute("Device_YK_Link_Strings")<<PointElem.attribute("Device_YK_Name_Strings");
                    strLinkName = PointElem.attribute("Device_YK_Link_Strings");
                    QVariant variant(0);///<zzy 2015/1/12 修改
                    pValue = new CValueBase;
                    pValue->SetVarValue(variant);
                    pTagBase = new CTagBase(TAG_TYPE_DO,strLinkName
                                            ,PointElem.attribute("Device_YK_Desc_Strings")
                                            ,m_nTagID++
                                            ,DATA_TYPE_INT32U
                                            ,pValue);
                }else if (TypeElem_Name_Attribute == "YS")
                {
                    qDebug()<<TypeElem.attribute("Name")<<PointElem.attribute("Device_YS_Link_Strings")<<PointElem.attribute("Device_YS_Name_Strings");
                    strLinkName = PointElem.attribute("Device_YS_Link_Strings");
                    QVariant variant(0);///<zzy 2015/1/12 修改
                    pValue = new CValueBase;
                    pValue->SetVarValue(variant);
                    pTagBase = new CTagBase(TAG_TYPE_AO,strLinkName
                                            ,PointElem.attribute("Device_YS_Desc_Strings")
                                            ,m_nTagID++
                                            ,DATA_TYPE_INT32U
                                            ,pValue);
                }else
                {
                    continue;
                }
                if (m_pRealTimeDB->AddTag(strLinkName,pTagBase))
                {
                    qDebug()<<"111"<<QString::number(pTagBase->GetType())<<pTagBase->GetName();
                    CSharedMemoryTag *pSharedMemoryTag = new CSharedMemoryTag(m_nTagID,pTagBase->GetType(), pTagBase->GetName());
                    qDebug()<<"222"<<QString::number(pTagBase->GetType())<<pTagBase->GetName();
                    if(!m_pSharedMemoryTagMap->contains(m_nTagID-1))
                    {
                        qDebug()<<"333"<<QString::number(pTagBase->GetType())<<pTagBase->GetName();
                        m_pSharedMemoryTagMap->insert(m_nTagID-1, pSharedMemoryTag);
                    }
                    qDebug()<<"444"<<QString::number(pTagBase->GetType())<<pTagBase->GetName();
                }else
                {
                    m_nTagID--;
                }

            }
        }
    }
    return true;
}

bool CStart::InitChannel()
{
    int ChannelNumber = 0;
    if (!m_ComFileList.isEmpty())
    {
        for (int i = 0; i < m_ComFileList.count(); ++i)
        {
            QDomDocument doc;
            if (CXmlParser::readFile(m_ComFileList.at(i),doc))
            {
                QFileInfo fileInfo(m_ComFileList.at(i));
                QString str = fileInfo.fileName();
                QString tmp;
                for(int j = 0; j < str.length(); j++)
                {
                    if (str[j] >= '0' && str[j] <= '9')
                    {
                        tmp.append(str[j]);
                    }
                }
                QDomNodeList RootNodeList =  doc.elementsByTagName("Root");
                for (int RootCount = 0; RootCount < RootNodeList.count(); ++RootCount)
                {
                    CChannelThread_CGI *pThread = new CChannelThread_CGI(m_pRealTimeDB,ChannelNumber++,ChannelType_COM,tmp.toInt(),this);
                    pThread->SetDomElement(RootNodeList.at(RootCount).toElement());
                    m_ThreadList.append(pThread);
//                    pThread->start();
                }
            }
        }
    }
    if (!m_NetFileList.isEmpty())
    {
        for (int i = 0; i < m_NetFileList.count(); ++i)
        {
            QDomDocument doc;
            if (CXmlParser::readFile(m_NetFileList.at(i),doc))
            {
                QDomNodeList LinkNodeList =  doc.elementsByTagName("Link");
                for (int LinkCount = 0; LinkCount < LinkNodeList.count(); ++LinkCount)
                {
                    qDebug()<<"LinkCount"<<LinkCount;
                    CChannelThread_CGI *pThread = new CChannelThread_CGI(m_pRealTimeDB,ChannelNumber++,ChannelType_NET,this);
                    pThread->SetDomElement(LinkNodeList.at(LinkCount).toElement());
                    m_ThreadList.append(pThread);
//                    pThread->start();
                }
            }
        }
    }
    if (!m_CANFileList.isEmpty())
    {
        for (int i = 0; i < m_CANFileList.count(); ++i)
        {
            QDomDocument doc;
            if (CXmlParser::readFile(m_CANFileList.at(i),doc))
            {
                QFileInfo fileInfo(m_CANFileList.at(i));
                QString str = fileInfo.fileName();
                QString tmp;
                for(int j = 0; j < str.length(); j++)
                {
                    if (str[j] >= '0' && str[j] <= '9')
                    {
                        tmp.append(str[j]);
                    }
                }
                QDomNodeList RootNodeList =  doc.elementsByTagName("Root");
                for (int RootCount = 0; RootCount < RootNodeList.count(); ++RootCount)
                {
                    CChannelThread_CGI *pThread = new CChannelThread_CGI(m_pRealTimeDB,ChannelNumber++,ChannelType_CAN,tmp.toInt(),this);
                    pThread->SetDomElement(RootNodeList.at(RootCount).toElement());
                    m_ThreadList.append(pThread);
                }
            }
        }
    }
    if (!m_DBFileList.isEmpty())
    {
        for (int i = 0; i < m_DBFileList.count(); ++i)
        {
            QDomDocument doc;
            if (CXmlParser::readFile(m_DBFileList.at(i),doc))
            {
                CDatabaseThread *pThread = new CDatabaseThread(m_pRealTimeDB,m_DBFileList.at(i),i);
                m_ThreadList.append(pThread);
//                pThread->start();
            }

        }
    }
    if (!m_JSFileList.isEmpty())
    {
        for (int nFileCount = 0; nFileCount < m_JSFileList.count(); ++nFileCount)
        {
            QDomDocument doc;
            if (CXmlParser::readFile(m_JSFileList.at(nFileCount),doc))
            {
                ///< 循环
                qDebug()<<"XUN HUAN";
                QDomNodeList ControlCycle       = doc.elementsByTagName("ControlCycle");
                for (int nControlCount = 0; nControlCount < ControlCycle.count(); ++nControlCount)
                {
                    if (!ControlCycle.at(nControlCount).toElement().elementsByTagName("JavaScript").isEmpty())
                    {
                        ///< 创建新线程
                        QDomNodeList JavaScriptList = ControlCycle.at(nControlCount).toElement().elementsByTagName("JavaScript");
                        for (int nJavaScriptCount = 0; nJavaScriptCount < JavaScriptList.count(); ++nJavaScriptCount)
                        {
                            QDomElement Elem_JavaScript = JavaScriptList.at(nJavaScriptCount).toElement();
                            qDebug()<<"SCRIPT="<<Elem_JavaScript.attribute("Script");
                        }
//                        m_ThreadList.append(pThread);
                    }
                }
                ///< 定时
                qDebug()<<"DING SHI";
                QDomNodeList ControlTiming      = doc.elementsByTagName("ControlTiming");
                for (int nControlCount = 0; nControlCount < ControlTiming.count(); ++nControlCount)
                {
                    if (!ControlTiming.at(nControlCount).toElement().elementsByTagName("JavaScript").isEmpty())
                    {
                        ///< 创建新线程
                        QDomNodeList JavaScriptList = ControlTiming.at(nControlCount).toElement().elementsByTagName("JavaScript");
                        for (int nJavaScriptCount = 0; nJavaScriptCount < JavaScriptList.count(); ++nJavaScriptCount)
                        {
                            QDomElement Elem_JavaScript = JavaScriptList.at(nJavaScriptCount).toElement();
                            qDebug()<<"SCRIPT="<<Elem_JavaScript.attribute("Script");
                        }
//                        m_ThreadList.append(pThread);
                    }
                }
                ///< 数据改变
                qDebug()<<"SHUJUGAIBIAN";
                QDomNodeList ControlDateChange  = doc.elementsByTagName("ControlDateChange");
                for (int nControlCount = 0; nControlCount < ControlDateChange.count(); ++nControlCount)
                {
                    CJavaScript_Thread *pThread = new CJavaScript_Thread(m_pRealTimeDB,ControlDateChange.at(nControlCount).toElement(),CJavaScript_ProtocolI::Control_DateChange,this);
                    if (pThread->GetbIsValid())
                    {
                        m_ThreadList.append(pThread);
                    }
//                    if (!ControlDateChange.at(nControlCount).toElement().elementsByTagName("JavaScript").isEmpty())
//                    {
//                        ///< 创建新线程
//                        QDomNodeList JavaScriptList = ControlDateChange.at(nControlCount).toElement().elementsByTagName("JavaScript");
//                        for (int nJavaScriptCount = 0; nJavaScriptCount < JavaScriptList.count(); ++nJavaScriptCount)
//                        {
//                            QDomElement Elem_JavaScript = JavaScriptList.at(nJavaScriptCount).toElement();
//                            qDebug()<<"SCRIPT="<<Elem_JavaScript.attribute("Script");

//                        }
////                        m_ThreadList.append(pThread);
//                    }
                }
            }
        }
//        exit(0);
    }
    {
        ///<zzy 2015/7/5 修改:start Monitor转发线程
        CChannelThread_CGI *pThread = new CChannelThread_CGI(m_pRealTimeDB,ChannelNumber++,ChannelType_Monitor104,this);
        pThread->InitChannel();
        pThread->SetMonitorIPAddress(CMyFunction::MonitorIPAddress);
        m_ThreadList.append(pThread);
//        pThread->start();
        ///<zzy 2015/7/5 修改:end
    }
    {
        ///<zzy 2015/7/5 修改:
        m_pRTDBSharedMemoryThread->SetpRTDB(m_pSharedMemoryTagMap);
        m_ThreadList.append(m_pRTDBSharedMemoryThread);
//        m_pRTDBSharedMemoryThread->start();
        ///<zzy 2015/7/5 修改:end
    }
    for (int i = 0; i < m_ThreadList.count(); ++i)
    {
        qDebug()<<__func__<<__FILE__<<__LINE__<<"Open Thread"<<i;
        m_ThreadList.at(i)->start();
        QTest::qSleep(250);
    }
    return true;
}

/*!
 \brief 运行js.xml文件中的“启动”部分的脚本文件

 \fn CStart::Init_JavaScript_Start
*/
void CStart::Init_JavaScript_Start()
{
    qDebug()<<__func__<<__LINE__;
    if (!m_JSFileList.isEmpty())
    {
        qDebug()<<"is not empty";
        for (int nFileCount = 0; nFileCount < m_JSFileList.count(); ++nFileCount)
        {
            qDebug()<<"m_JSFileList===="<<m_JSFileList.at(nFileCount);
            QDomDocument doc;
            if (CXmlParser::readFile(m_JSFileList.at(nFileCount),doc))
            {
                QDomNodeList ControlStartList = doc.elementsByTagName("ControlStart");
                for (int nCtrlStart = 0; nCtrlStart < ControlStartList.count(); ++nCtrlStart)
                {
                    QDomNodeList JavaScriptList = ControlStartList.at(nCtrlStart).toElement().elementsByTagName("JavaScript");
                    for (int nJavaScCount = 0; nJavaScCount < JavaScriptList.count(); ++nJavaScCount)
                    {
                        QDomElement elemJavaScript = JavaScriptList.at(nJavaScCount).toElement();
                        JavaScript_ControlStart_JavaScript(elemJavaScript);
                    }
                }
            }else
            {
                qDebug()<<"open error "<<m_JSFileList.at(nFileCount);
            }
        }
    }else
    {
        qDebug()<<"is empty";
    }
}
bool CStart::JavaScript_ControlStart_JavaScript(const QDomElement &elemJavaScript_)
{
    QString strScript = elemJavaScript_.attribute("Script");
//    QString strDesc   = elemJavaScript_.attribute("Desc");
    QDomNodeList PointList = elemJavaScript_.elementsByTagName("Point");
    QScriptEngine engine;
    int nPointCount = PointList.count();
    for (int i = 0; i < nPointCount; ++i)
    {
        QString strLink = PointList.at(i).toElement().attribute("Link");
        QString strName = PointList.at(i).toElement().attribute("Name");
        CTagI *pTag = m_pRealTimeDB->FindTag(strLink);
        if (pTag)
        {
            engine.globalObject().setProperty(strName,pTag->GetProjectValue()->GetVarValue().toFloat());
        }else
        {
            return false;
        }
    }
    engine.evaluate(strScript);
    for (int i = 0; i < nPointCount; ++i)
    {
        QString strLink = PointList.at(i).toElement().attribute("Link");
        QString strName = PointList.at(i).toElement().attribute("Name");
        CTagI *pTag = m_pRealTimeDB->FindTag(strLink);
        if (pTag)
        {
            QVariant value = engine.globalObject().property(strName).toVariant();
            pTag->SetValue(0,value,value);
            qDebug()<<__func__<<__LINE__<<"StartJS="<<strLink<<value;
        }else
        {
            return false;
        }
    }
    return true;
}





