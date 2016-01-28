#ifndef CHDBREAD_H
#define CHDBREAD_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QFile>
#include <QTimer>
#include <QDir>
#include <QDebug>
#include <stdio.h>
#include <QDateTime>

class CHDBRead : public QObject
{
    Q_OBJECT
public:
    explicit CHDBRead(QObject *parent = 0);
    bool OpenSqliteDatabase();
    const QString GetValue(const QString &strLinkName_,QDateTime dtStartTime_, QDateTime dtStopTime_);
signals:

public slots:
private:
    QSqlDatabase m_SqlDatabase;
    QString m_strDatabaseFileName;

    QString m_strTagIndexTable;
    QString m_strValueTable;

};

#endif // CHDBREAD_H
