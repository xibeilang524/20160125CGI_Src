#include "CHDBRead.h"
#include <QDir>

CHDBRead::CHDBRead(QObject *parent) :
    QObject(parent)
{
    m_strTagIndexTable = QString("TagIndexTable");
    m_strValueTable = QString("ValueTable");

}

bool CHDBRead::OpenSqliteDatabase()
{
//    printf("CurrentPath=%s\n",QDir::currentPath().toStdString().data());
    if(QSqlDatabase::contains("qt_sql_default_connection")) {
        qDebug()<<"------------------44444444444443333";
        m_SqlDatabase = QSqlDatabase::database("qt_sql_default_connection");
        qDebug()<<"------------------55555555555555555";
    }
    else {
        qDebug()<<"================-------------------------11111111111";
        m_SqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
        qDebug()<<"================-------------------------22222222222";
    }

    QFile file;
    m_strDatabaseFileName = QString("test.db");
    file.setFileName(m_strDatabaseFileName);
    if (file.open(QIODevice::ReadOnly| QFile::Text))
    {
//        printf("m_strDatabaseFileName1=%s\n",m_strDatabaseFileName.toStdString().data());
        file.close();
    }else
    {
        m_strDatabaseFileName = QString("cgi-bin/test.db");
//        printf("m_strDatabaseFileName2=%s\n",m_strDatabaseFileName.toStdString().data());
        file.setFileName(m_strDatabaseFileName);
        if (file.open(QIODevice::ReadOnly| QFile::Text))
        {
            file.close();
        }else
        {
            QString strDebug = QString("New CreateDB %1").arg(m_strDatabaseFileName);
            qDebug()<<strDebug;
            return false;
        }
    }

    m_SqlDatabase.setDatabaseName(m_strDatabaseFileName);
    if(!m_SqlDatabase.open()){
        qCritical("Can't open database: %s(%s)",
                  m_SqlDatabase.lastError().text().toLocal8Bit().data(),
                  qt_error_string().toLocal8Bit().data());
        qDebug()<<m_SqlDatabase.lastError().text()<<qt_error_string();

        return false;
    }

    return true;

}
// --operate=select&value=COM2.pz@1&starttime=2015-11-01 00:00:00&endtime=2015-11-21 00:00:00--
// ==COM2.pz@1==2015-11-01 00:00:00==2015-11-21 00:00:00
// Open DB success.
// PointSrc= 0 - COM2.pz@1
// nTagID =0Value_idx =1 AcpTime_idx ={value:[]}
const QString CHDBRead::GetValue(const QString &strLinkName_, QDateTime dtStartTime_, QDateTime dtStopTime_)
{
    QString result;
    int nTagID = 0;
    QSqlQuery query(m_SqlDatabase);
    QString queryString;
    queryString = QString("select ID from %1 WHERE %2.TagName='%3';")
            .arg(m_strTagIndexTable)
            .arg(m_strTagIndexTable)
            .arg(strLinkName_);

    if(query.exec(queryString)){
        int id_idx;
        id_idx = query.record().indexOf("ID");

        while (query.next()){
            nTagID = query.value(id_idx).toString().toUInt();
        }
    }
//    printf("nTagID =%d",nTagID);
    qDebug()<<"strLinkName_"<<strLinkName_<<nTagID;
//    result.append(strLinkName_).append("+");
//return result;

    queryString = QString("SELECT AcpTime,Value FROM %1 where ID=%2 AND %3.AcpTime>'%4' AND %5.AcpTime<'%6';")
            .arg(m_strValueTable)
            .arg(nTagID)
            .arg(m_strValueTable)
            .arg(dtStartTime_.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(m_strValueTable)
            .arg(dtStopTime_.toString("yyyy-MM-dd hh:mm:ss"));
//    qDebug()<<queryString;
    if(query.exec(queryString)){
        int Value_idx, AcpTime_idx;

        Value_idx = query.record().indexOf("Value");
        AcpTime_idx = query.record().indexOf("AcpTime");
//        printf("Value_idx =%d AcpTime_idx =%d" ,Value_idx,AcpTime_idx);

        while (query.next()){
            QString strValue = query.value(Value_idx).toString();
            QString strAcpTime = query.value(AcpTime_idx).toString();

            result.append(strLinkName_).append(",");
            result.append(strAcpTime).append(",").append(strValue).append(",");
//            printf("resultAAAAA=%s",result.toStdString().data());
        }
    }
//    result.append(",");
    result.remove(result.count()-1,1);
    return result;


//    qDebug()<<queryString;

//    if(query.exec(queryString)){
//        int id_idx, name_idx, department_idx, credit_idx;

//        name_idx = query.record().indexOf("name");
//        id_idx = query.record().indexOf("ID");
//        department_idx = query.record().indexOf("dept_name");
//        credit_idx = query.record().indexOf("tot_cred");

//        while (query.next()){
//            QString name = query.value(name_idx).toString();
//            QString id = query.value(id_idx).toString();
//            QString dept = query.value(department_idx).toString();
//            int cred = query.value(credit_idx).toInt();

//            Student st(id, name, dept, cred);
//            list.append(st);
//            ok = true;
//        }
//    }

}
