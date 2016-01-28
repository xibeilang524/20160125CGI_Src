#ifndef CSTART_H
#define CSTART_H

#include <QObject>

#include <QDebug>
#include <QTimerEvent>
#include <QTimer>


class CStart : public QObject
{
    Q_OBJECT
public:
    explicit CStart(QObject *parent = 0);
    void SetStart();
    void timerEvent(QTimerEvent *event);
    int GetSysAbsTime();
    int m_nTimerID;
    int m_nChongQiShiJian;
    int m_nStartTime;
    QTimer m_Timer;
signals:

private slots:
};

#endif // CSTART_H
