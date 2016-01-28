/*!
 * \file
 * \brief   概述
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/2/4
 *
 * \b       修改记录：
 * \li      2015/2/4
 *  添加注释
 */

#include "CDatabaseProtocolDatabase.h"
#include "../PortInclude/XmlParser/CXmlParser.h"
#include "../PortInclude/CRTDBI.h"
#include "../PortInclude/CTagI.h"
#include "../PortInclude/cvaluebase.h"
#include "../PortInclude/CPointI.h"
#include <QDebug>
//#include <QDomDocument>
#include <QDomElement>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QFile>
#include <QTimer>
#include <QDir>
//#include <QDateTime>

//! 供运行程序调用的函数
CDatabaseProtocolI * CreateDriver()
{
    /*!
    * \brief 功能概述 库文件导出接口函数
    * \param 参数描述
    * \return 返回值描述
    * \author zzy
    * \date 2015/2/4
    */
    CDatabaseProtocolDatabase *testProtocol = new CDatabaseProtocolDatabase();
    return  testProtocol;
}
CDatabaseProtocolDatabase::CDatabaseProtocolDatabase(QObject */*parent*/) :
    CDatabaseProtocolI()
{
    connect(this,SIGNAL(signal_TagChange(CPointBase*,QVariant)),this,SLOT(slot_TagChange(CPointBase*,QVariant)));
    m_bIsFirst = true;
}

CDatabaseProtocolDatabase::~CDatabaseProtocolDatabase()
{
    CloseConnection();
}

bool CDatabaseProtocolDatabase::OnCreateChannel(const QString strFileName_, CRTDBI *pRealDatabase,int nNumber)
{
    qDebug()<<"shujuku qudong ";
    qDebug()<<strFileName_;
    m_strFileName = strFileName_;
    m_pRTDB = pRealDatabase;
    m_nNumber = nNumber;
//    pPoint = new Point;
//    QDomDocument doc;

    /// @note 备份日志文件
    QString backup("/DBBackup");
    QDir dir(backup);
    if(!dir.exists())
    {
        dir.mkdir(backup);
    }
    m_strDatabaseFileName = QString("test.db");
    QString BackupDBName = QString("%1.db").arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
    QFile file(m_strDatabaseFileName);
    if(file.exists())
    {
        QFile::copy(m_strDatabaseFileName,QString("%1/%2").arg(backup).arg(BackupDBName));
//        qDebug()<<dir.currentPath()<<1111;
    }
#if 0
    QDir dir("Backup");
    if(!dir.exists())
    {
        dir.mkdir(QString("%1/Backup").arg(dir.currentPath()));
    }
//    m_strDatabaseFileName = QString("%1.db").arg(QDateTime::currentDateTime().toString("yyyy_MM"));
    m_strDatabaseFileName = QString("test.db");
    QString BackupDBName = QString("%1.db").arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
    QFile file(m_strDatabaseFileName);
    if(file.exists())
    {
        QFile::copy(m_strDatabaseFileName,QString("%1/Backup/%2").arg(dir.currentPath()).arg(BackupDBName));
//        qDebug()<<dir.currentPath()<<1111;
    }
#endif

    if (!CXmlParser::readFile(m_strFileName,m_strDoc))
        return false;
    if (!CGI_InfoInit(m_strDoc))
        return false;
    if (!CGI_OpenSqliteDatabase())
        return false;
    /// @note 删除并创建表
    if(!CGI_IndexTableInit(m_strDoc))
    {
//        qDebug()<<"Can't create IndexTable";
        return false;
    }
    if(!CGI_InfoTableInit())
    {
        return false;
    }
    QTimer::singleShot(3*1000,this,SLOT(MyStartTimer()));/// @note 3秒钟之后启动定时器
    return true;
}

bool CDatabaseProtocolDatabase::OnTagChange(CPointBase *pPointBase_, QVariant VarSet_)
{
    emit signal_TagChange(pPointBase_,VarSet_);
    return true;
}

void CDatabaseProtocolDatabase::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nTimerID)
    {
        CGI_OnWriteDatabase();
    }
}

bool CDatabaseProtocolDatabase::CGI_InfoInit(const QDomDocument &/*doc*/)
{
//    QDomNodeList PointType = doc.elementsByTagName("DatabaseType");
//    if (DatabaseType.count()<=0)
//        return false;
//    QDomElement DatabaseTypeElem = DatabaseType.at(0).toElement();
////    QString strDatabaseType = DatabaseTypeElem.attribute("DatabaseType");
//    m_nSavePeriod = DatabaseTypeElem.attribute("SavePeriod").toInt();
//    if (m_nSavePeriod <= 0)
//        return false;

    return true;
}

bool CDatabaseProtocolDatabase::CGI_OpenSqliteDatabase()
{
    m_strTagIndexTableName = QString("TagIndexTable");
    if (m_strTagIndexTableName.isEmpty())
        return false;

    if(QSqlDatabase::contains("qt_sql_default_connection")) {
        qDebug()<<"------------------44444444444443333";
        m_SqlDatabase = QSqlDatabase::database("qt_sql_default_connection");
        qDebug()<<"------------------55555555555555555";
    }
    else {
        qDebug()<<"================-------------------------11111111111";
        m_SqlDatabase = QSqlDatabase::addDatabase("QSQLITE",QString(m_nNumber));
        qDebug()<<"================-------------------------22222222222";
    }

    m_SqlDatabase.setDatabaseName(m_strDatabaseFileName);
    QFile file(m_strDatabaseFileName);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        file.close();
    }else
    {
        QString strDebug = QString("New CreateDB %1").arg(m_strDatabaseFileName);
        qDebug()<<strDebug;
        file.open(QIODevice::ReadWrite);
        file.close();
    }
    if(!m_SqlDatabase.open()){
        qCritical("Can't open database: %s(%s)",
                  m_SqlDatabase.lastError().text().toLocal8Bit().data(),
                  qt_error_string().toLocal8Bit().data());
        qDebug()<<m_SqlDatabase.lastError().text()<<qt_error_string();

        return false;
    }

    return true;
}

bool CDatabaseProtocolDatabase::CGI_InfoTableInit()
{
    QSqlQuery query(m_SqlDatabase);
    QString queryString = QString("drop table ValueTable");
    if(!query.exec(queryString))
    {
        qDebug()<<"Don't have ( ValueTable ) table.";
    }
    queryString = QString("create table ValueTable(ID int,Value varchar(20),AcpTime datetime,primary key(ID,AcpTime));");
    if (!query.exec(queryString))
    {
        qDebug()<<"Create ( ValueTable ) table failed.";
        return false;
    }
    return true;
}

void CDatabaseProtocolDatabase::CGI_InitPointMap(const QDomDocument &doc)
{
    QDomNodeList PointNodeList = doc.elementsByTagName("Point");
    int nPointIndex = 0;
    for (int i = 0; i < PointNodeList.count(); ++i)
    {
        QDomElement PointElement = PointNodeList.at(i).toElement();
        CTagI *pTag = m_pRTDB->FindTag(PointElement.attribute("Link"));
        if (pTag)
        {
            CPointBase *pPoint = new CPointBase;

            pPoint->m_nPeriodicStorage = PointElement.attribute("PeriodicStorage").toUInt();
            pPoint->m_strRTDBName = PointElement.attribute("Link");
            pPoint->m_nPointIndex = nPointIndex++;
            pPoint->m_pTag = pTag;
            if (PointElement.attribute("ChangeStorage") == "true")
            {
                pPoint->m_nStorageMode = PointStorageMode_ChangeStorage;
                pPoint->m_pProtocol = this;
                if (pPoint->m_pTag)
                {
                    pPoint->m_pTag->SetNotify(pPoint);///设置转发点
                }
            }else if (PointElement.attribute("ChangeStorage") == "false")
            {
                pPoint->m_nStorageMode = PointStorageMode_PeriodicStorage;
            }else
            {
                pPoint->m_nStorageMode = PointStorageMode_PeriodicStorage;
            }
            CGI_AddPoint(pPoint);
        }
    }
}

bool CDatabaseProtocolDatabase::CGI_AddPoint(CPointBase *pPoint)
{
    m_IndexPointMap.insert(pPoint->m_nPointIndex,pPoint);
    if (m_PointNodeMap.contains(pPoint->m_nPeriodicStorage))
    {
        m_PointNodeMap.value(pPoint->m_nPeriodicStorage)->m_PointList.append(pPoint);
    }else
    {
        PointNode *pPointNode = new PointNode;
        pPointNode->m_PointList.append(pPoint);
        pPointNode->m_nPeriodicStorage = pPoint->m_nPeriodicStorage;
        pPointNode->m_nStorageMoment = GetSysAbsTime();

        m_PointNodeMap.insert(pPoint->m_nPeriodicStorage,pPointNode);
    }
    return true;
}

/*!
 * \brief  功能概述 获取绝对秒
 * \param  参数描述 无
 * \return 返回值描述 返回绝对秒
 * \author zzy
 * \date   2015/5/27
 */
int CDatabaseProtocolDatabase::GetSysAbsTime()
{
    ///获取绝对秒
    int nTime = QDateTime::currentDateTime().secsTo(QDateTime(QDate(1970,1,1),QTime(0,0,0,0)));
    if(nTime < 0)
    {
        nTime = -nTime;
    }
    return nTime;
}

void CDatabaseProtocolDatabase::slot_TagChange(CPointBase *pPointBase_, QVariant VarSet_)
{
    QMap<int,CPointBase*>::iterator PointIterator;
    for (PointIterator = m_IndexPointMap.begin(); PointIterator != m_IndexPointMap.end(); ++PointIterator)
    {
        if(PointIterator.value()->m_strRTDBName == pPointBase_->m_strRTDBName)
        {
            float SrcValue =PointIterator.value()->GetValue()->GetVarValue().toFloat();
            if (SrcValue < 0)
                SrcValue = -SrcValue;

            float NewValue = VarSet_.toFloat();
            if (NewValue < 0)
                NewValue = -NewValue;
//            float ChangePercentage = PointIterator.value()->m_fChangePercentage;
            float ChangePercentage = 0.05;
            if (ChangePercentage < 0)
                ChangePercentage = -ChangePercentage;
            if ( ((SrcValue>NewValue)?(SrcValue-NewValue):(NewValue-SrcValue)) > (SrcValue*ChangePercentage))
            {
                qDebug()<<"插入一个变化"<<pPointBase_->m_strRTDBName;
                QSqlQuery query(m_SqlDatabase);
                QString queryString;
                PointIterator.value()->GetValue()->SetVarValue(VarSet_);
                CTagI *pTempTagI = PointIterator.value()->m_pTag;
                if (pTempTagI->GetPointCollection())
                {
                    if (pTempTagI->GetPointCollection()->GetValue())
                    {
//                        queryString = QString("insert into %1 values(%2,'%3','%4');")
//                                .arg(m_strTagHDValueName)
//                                .arg(PointIterator.key())
//                                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
//                                .arg(pTempTagI->GetPointCollection()->GetValue()->GetVarValue().toString());
//                        ///向数据库写入的时间是写入数据库时的时间
//                        if(query.exec(queryString)){
//                            qDebug()<<"添加成功"<<m_strFileName;
//                        }else{
//                            qDebug()<<"添加失败"<<m_strFileName<<pPointBase_->m_strRTDBName;
//                        }
                        QString strValue;
//                        if (pTempTagI->GetPointCollection())
//                        {
//                            strValue = pTempTagI->GetProjectValue()->GetVarValue().toString();
//                        }
                        strValue = VarSet_.toString();
                        QString m_strTagHDValueName;
                        m_strTagHDValueName = "ValueTable";
                        queryString = QString("insert into %1 values(%2,'%3','%4');")
                                .arg(m_strTagHDValueName)
                                .arg(PointIterator.value()->m_nPointIndex)
                                .arg(strValue)
                                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
                        ///向数据库写入的时间是写入数据库时的时间
                        if(query.exec(queryString)){
                            qDebug()<<"add succeed"<<queryString<<m_strFileName;
                        }else{
                            qDebug()<<"add failed"<<queryString<<m_strFileName;
                        }
                    }
                }
            }else
            {
                return;
            }
        }
    }
}

bool CDatabaseProtocolDatabase::CGI_IndexTableInit(const QDomDocument &doc)
{
    CGI_InitPointMap(doc);/// @note 初始化点表的map

    /// @note 创建表
    QSqlQuery query(m_SqlDatabase);
    QString queryString = QString("drop table %1").arg(m_strTagIndexTableName);
    if(!query.exec(queryString))
    {
        QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
        qDebug()<<debug<<"Don't have ( "<<m_strTagIndexTableName<<" )table.";
    }
    queryString = QString("create table %1(ID int,TagName varchar(50),StorageMode int,PeriodicStorage int,primary key(ID));").arg(m_strTagIndexTableName);
    if (!query.exec(queryString))
    {
        QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
        qDebug()<<debug<<"Create \"TagIndex\" table is faild.";
        return false;
    }

    /// @note 初始化索引表
    QMap<int,CPointBase *>::iterator iterator;
    for (iterator = m_IndexPointMap.begin(); iterator != m_IndexPointMap.end(); ++iterator)
    {
        queryString = QString("insert into %1 values(%2,'%3',%4,%5);")
                .arg(m_strTagIndexTableName)
                .arg(iterator.key())
                .arg(iterator.value()->m_strRTDBName)
                .arg(iterator.value()->m_nStorageMode)
                .arg(iterator.value()->m_nPeriodicStorage);
        qDebug()<<"AAAAAAAAAAAAAAAAAA"<<queryString;
        while(!query.exec(queryString))
        {
            queryString = QString("insert into %1 values(%2,'%3',%4,%5);")
                    .arg(m_strTagIndexTableName)
                    .arg(iterator.key())
                    .arg(iterator.value()->m_strRTDBName)
                    .arg(iterator.value()->m_nStorageMode)
                    .arg(iterator.value()->m_nPeriodicStorage);
        }
    }
    return true;
}


bool CDatabaseProtocolDatabase::CGI_TablesInit()
{
    /// @note 初始化所有表
    /// @brief 已存在表不做处理，不存在则生成并初始化
    /// @author Yu

    QSqlQuery query(m_SqlDatabase);
    QString queryString;
    QStringList tableList = m_SqlDatabase.tables();
    if(!tableList.contains(m_strTagIndexTableName))
    {
        queryString = QString("create table %1(ID int,TagName varchar(50),StorageMode int,PeriodicStorage int,primary key(ID));").arg(m_strTagIndexTableName);
        if (!query.exec(queryString))
        {
            QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
            qDebug()<<debug<<"Create \"TagIndex\" table is faild.";
            return false;
        }
        /// @note 初始化索引表
        QMap<int,CPointBase *>::iterator iterator;
        for (iterator = m_IndexPointMap.begin(); iterator != m_IndexPointMap.end(); ++iterator)
        {
            queryString = QString("insert into %1 values(%2,'%3',%4,%5);")
                    .arg(m_strTagIndexTableName)
                    .arg(iterator.key())
                    .arg(iterator.value()->m_strRTDBName)
                    .arg(iterator.value()->m_nStorageMode)
                    .arg(iterator.value()->m_nPeriodicStorage);
            qDebug()<<"AAAAAAAAAAAAAAAAAA"<<queryString;
            while(!query.exec(queryString))
            {
                queryString = QString("insert into %1 values(%2,'%3',%4,%5);")
                        .arg(m_strTagIndexTableName)
                        .arg(iterator.key())
                        .arg(iterator.value()->m_strRTDBName)
                        .arg(iterator.value()->m_nStorageMode)
                        .arg(iterator.value()->m_nPeriodicStorage);
            }
        }
    }
    if(!tableList.contains("ValueTable"))
    {
        queryString = QString("create table ValueTable(ID int,Value varchar(20),AcpTime datetime,primary key(ID,AcpTime));");
        if (!query.exec(queryString))
        {
            qDebug()<<"Create ( ValueTable ) table failed.";
            return false;
        }
    }
    return true;
}

void CDatabaseProtocolDatabase::CloseConnection()
{
    m_SqlDatabase.close();
//    QSqlDatabase::database().close();
}

void CDatabaseProtocolDatabase::CGI_OnWriteDatabase()
{
    if (!m_SqlDatabase.isOpen())
    {
        if(!CGI_OpenSqliteDatabase())
        {
            qDebug()<<"Can't Open Database";
            return;
        }
    }

    if(!CGI_TablesInit())
    {
        qDebug()<<"Tables init error";
        return;
    }
    QSqlQuery query(m_SqlDatabase);
    QString queryString;
    if (m_bIsFirst)
    {
        m_bIsFirst = false;
        QMap<int,CPointBase *>::iterator iterator;
        for (iterator = m_IndexPointMap.begin(); iterator != m_IndexPointMap.end();++iterator)
        {
            CTagI *pTempTagI = iterator.value()->m_pTag;
            if (pTempTagI)
            {
                QString strValue;
                if (pTempTagI->GetPointCollection())
                {
                    strValue = pTempTagI->GetProjectValue()->GetVarValue().toString();
                }

                QString m_strTagHDValueName;
                    m_strTagHDValueName = "ValueTable";
                    queryString = QString("insert into %1 values(%2,'%3','%4');")
                            .arg(m_strTagHDValueName)
                            .arg(iterator.key())
                            .arg(strValue)
                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
                ///向数据库写入的时间是写入数据库时的时间
                if(query.exec(queryString)){
                    qDebug()<<"add succeed"<<queryString<<m_strFileName;
                }else{
                    qDebug()<<"add failed"<<queryString<<m_strFileName;
                }
            }
        }
    }else
    {
        QMap<unsigned int, PointNode* >::iterator iterator;
        for (iterator = m_PointNodeMap.begin(); iterator != m_PointNodeMap.end(); ++iterator)
        {
            if (GetSysAbsTime()-iterator.value()->m_nStorageMoment >= iterator.value()->m_nPeriodicStorage)
            {
                for (int i = 0; i < iterator.value()->m_PointList.count(); ++i)
                {
                    if (iterator.value()->m_PointList.at(i)->m_nStorageMode == PointStorageMode_ChangeStorage)
                    {/// 对于变化存储的不做处理
                        continue;
                    }
                    CTagI *pTempTagI = iterator.value()->m_PointList.at(i)->m_pTag;
                    if (pTempTagI)
                    {
                        QString strValue;
                        if (pTempTagI->GetPointCollection())
                        {
                            strValue = pTempTagI->GetProjectValue()->GetVarValue().toString();
                        }

                        QString m_strTagHDValueName;
                        m_strTagHDValueName = "ValueTable";
                        queryString = QString("insert into %1 values(%2,'%3','%4');")
                                .arg(m_strTagHDValueName)
                                .arg(iterator.value()->m_PointList.at(i)->m_nPointIndex)
                                .arg(strValue)
                                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
                        ///向数据库写入的时间是写入数据库时的时间
                        if(query.exec(queryString)){
                            qDebug()<<"add succeed"<<queryString<<m_strFileName;
                        }else{
                            qDebug()<<"add failed"<<queryString<<m_strFileName;
                        }
                    }
                    iterator.value()->m_nStorageMoment = GetSysAbsTime();
                }
            }
        }
    }


}
#if 0
void CDatabaseProtocolDatabase::CGI_OnWriteDatabase()
{
    if(!CGI_OpenSqliteDatabase())
    {
        qDebug()<<"Can't Open Database";
        return;
    }
    if(!CGI_TablesInit())
    {
        qDebug()<<"Tables init error";
        return;
    }

    ///写入数据库
    QSqlQuery query(m_SqlDatabase);
    QString queryString;
    QMap<int,CPointBase *>::iterator iterator;
    for (iterator = m_IndexPointMap.begin(); iterator != m_IndexPointMap.end();++iterator)
    {
        CTagI *pTempTagI = iterator.value()->m_pTag;
        if (pTempTagI)
        {
            QString strValue;
            if (pTempTagI->GetPointCollection())
            {
                strValue = pTempTagI->GetProjectValue()->GetVarValue().toString();
            }

            QString m_strTagHDValueName;
                m_strTagHDValueName = "ValueTable";
                queryString = QString("insert into %1 values(%2,'%3','%4');")
                        .arg(m_strTagHDValueName)
                        .arg(iterator.key())
                        .arg(strValue)
                        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            ///向数据库写入的时间是写入数据库时的时间
            if(query.exec(queryString)){
                qDebug()<<"add succeed"<<queryString<<m_strFileName;
            }else{
                qDebug()<<"add failed"<<queryString<<m_strFileName;
            }
        }
    }
}
#endif
/*!
 \brief 启动定时器

 \fn CDatabaseProtocolDatabase::MyStartTimer
*/
void CDatabaseProtocolDatabase::MyStartTimer()
{
    qDebug()<<"Qi Dong Ding Shi Qi";
    m_nTimerID = startTimer(100);
}
