#ifndef CDATABASEPROTOCOLDATABASE_H
#define CDATABASEPROTOCOLDATABASE_H

#include "../PortInclude/CDatabaseProtocolI.h"
#include <QSqlDatabase>
#include <QMap>
#include <QDomDocument>
#include <QDateTime>
#include <QTimerEvent>
#include "CPointBase.h"

//class QDomDocument;
class CTagI;
enum PointStorageMode
{
    PointStorageMode_ChangeStorage=1,///< 变化存储
    PointStorageMode_PeriodicStorage ///< 周期存储
};

class Point
{
public:
    int m_nPointIndex;///< 索引号
    int m_nPeriodicStorage;///< 存储周期
    int m_nStorageMode;///< 存储模式
    int m_nPointType;
    CTagI *m_pTag;
    QString m_strLinkName;
};

class PointNode
{
public:
    int m_nPeriodicStorage;///< 存储周期
    int m_nStorageMoment;///< 存储时刻
    QList<CPointBase* > m_PointList;
};

struct DatabaseInfo{
    QString Value;
    QDateTime AcpTime;
};

/*!
 * \class
 * \brief  概述
 * \author zzy
 * \date   2015/2/4
 * 详细概述
 */
class CDatabaseProtocolDatabase : public CDatabaseProtocolI
{
    Q_OBJECT
public:
    explicit CDatabaseProtocolDatabase(QObject *parent = 0);
    ~CDatabaseProtocolDatabase();

    bool OnCreateChannel(const QString strFileName_, CRTDBI *pRealDatabase, int nNumber);
    bool OnTagChange(CPointBase *pPointBase_, QVariant VarSet_);
    void timerEvent(QTimerEvent *event);
    int GetSysAbsTime();

signals:
    void signal_TagChange(CPointBase *pPointBase_, QVariant VarSet_);
private slots:
    void slot_TagChange(CPointBase *pPointBase_, QVariant VarSet_);
private:
    void CloseConnection();
    bool CGI_InfoInit(const QDomDocument &doc);
//    bool Open_SQLite(QDomDocument &doc);
//    bool Open_SQLServer(QDomDocument &doc);
    void CGI_InitPointMap(const QDomDocument &doc);
    bool CGI_OpenSqliteDatabase();
    bool CGI_InfoTableInit();
    bool CGI_IndexTableInit(const QDomDocument &doc);
    bool CGI_TablesInit();

private:
    QString m_strFileName;
    CRTDBI *m_pRTDB;
    int m_nTimerID;
    QString m_strTagIndexTableName; ///< 点索引表
//    QString m_strTagHDValueName;    /// @note 点历史数据表

    QString m_strDatabaseFileName;     ///< db文件的名字 只有SQLite和Access数据库有
    QDomDocument m_strDoc;

    QSqlDatabase m_SqlDatabase;
    int m_nNumber;/// 用于区分数据库的标志
    QMap<int,CPointBase *> m_IndexPointMap;///< int 为索引
    QMap<unsigned int, PointNode* > m_PointNodeMap;
    bool CGI_AddPoint(CPointBase *pPoint);
//    QMap<int,QString> m_PointTypeMap;
//    QMap<int,DatabaseInfo *> m_DatabaseInfoMap;
    bool m_bIsFirst;///< 第一次将所有变量的值都写入数据库

private slots:
    void CGI_OnWriteDatabase();
    void MyStartTimer();
};

#endif // CDATABASEPROTOCOLDATABASE_H
