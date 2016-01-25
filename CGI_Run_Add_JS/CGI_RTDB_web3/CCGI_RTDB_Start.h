#ifndef CCGI_RTDB_START_H
#define CCGI_RTDB_START_H

#include <QObject>
#include <QLibrary>
#include "../PortInclude/CRTDBSharedMemoryLibI.h"

class CCGI_RTDB_Start : public QObject
{
    Q_OBJECT
public:
    explicit CCGI_RTDB_Start(QObject *parent = 0);


    bool Open();
    bool SetValue(int nID_, QVariant varValue_);///< 设置值
    QList<CSharedMemoryPoint> GetValue(QList<int> nIDList_);///< 获取值
    CSharedMemoryPoint GetValue(int nID_);
    CSharedMemoryPoint GetValue(QString strRTDBName_);
signals:

public slots:
private:
    bool m_nIsConnect;
    CRTDBSharedMemoryLibI *m_pRTDBSharedMemoryLibI;///< 库中导出的类
    QString m_LibName;///< 库文件的路径
    QLibrary mylib;///< 库文件

};

#endif // CCGI_RTDB_START_H
