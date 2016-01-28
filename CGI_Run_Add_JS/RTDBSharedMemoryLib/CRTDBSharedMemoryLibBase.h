#ifndef CRTDBSHAREDMEMORYLIBBASE_H
#define CRTDBSHAREDMEMORYLIBBASE_H

#include "../PortInclude/CRTDBSharedMemoryLibI.h"
#include <QSharedMemory>

class CRTDBSharedMemoryLibBase : public CRTDBSharedMemoryLibI
{
    Q_OBJECT
public:
    explicit CRTDBSharedMemoryLibBase(QObject *parent = 0);
    bool Create(QMap<int, CSharedMemoryTag *> *pRTDB_, QString strFileName_);///< 创建点表文件、创建共享内存
    bool WritePointFile();///< 将点写入文件
    bool ReadPointFile();///< 读取点表文件，初始化RTDB的map
    bool Open();///< 连接上共享内存
    bool SetValue(int nID_, QVariant varValue_);///< 设置值
    QList<CSharedMemoryPoint> GetValue(QList<int> nIDList_);///< 获取值
    CSharedMemoryPoint GetValue(int nID_);
    CSharedMemoryPoint GetValue(QString strRTDBName_);

    QMap<int,CSharedMemoryTag *> *m_pRTDB;
    QSharedMemory m_SharedMemory;
    bool m_isSucceed;
signals:
    bool Signal_SetValue(int nID_, QVariant varValue_);///< 设置值

public slots:
    bool Slot_SetValue(int nID_, QVariant varValue_);///< 设置值
private:
    int m_nMaxTagIndex;/// 最大的点号
    int m_nSize;

};

#endif // CRTDBSHAREDMEMORYLIBBASE_H
