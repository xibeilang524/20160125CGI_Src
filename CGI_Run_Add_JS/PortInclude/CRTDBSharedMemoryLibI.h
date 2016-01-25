#ifndef RTDBSHAREDMEMORYLIB_H
#define RTDBSHAREDMEMORYLIB_H


#include <QtCore/qglobal.h>

#if defined(RTDBSHAREDMEMORYLIB_LIBRARY)
#  define RTDBSHAREDMEMORYLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RTDBSHAREDMEMORYLIBSHARED_EXPORT Q_DECL_IMPORT
#endif
#include <QObject>
#include <QMap>
#include <QVariant>
#include <QVector>
#include "CSharedMemoryPoint.h"

class CSharedMemoryTag
{
public:
    CSharedMemoryTag(int nTagIndex_, int nTagType_, QString strTagRTDBName_){
        m_nTagIndex = nTagIndex_;
        m_nTagType = nTagType_;
        m_strTagRTDBName = strTagRTDBName_;
    }
    int m_nTagIndex;
    int m_nTagType;
    QString m_strTagRTDBName;
};

class RTDBSHAREDMEMORYLIBSHARED_EXPORT CRTDBSharedMemoryLibI : public QObject
{
    Q_OBJECT
public:
    virtual bool Create(QMap<int,CSharedMemoryTag *> *pRTDB_, QString strFileName_) = 0;///< 创建点表文件、创建共享内存
//    virtual bool ReadPointFile() = 0;///< 读取点表文件，初始化RTDB的map
    virtual bool Open() = 0;///< 连接上共享内存
    virtual bool SetValue(int nID_, QVariant varValue_) = 0;///< 设置值
    virtual QList<CSharedMemoryPoint> GetValue(QList<int> nIDList_) = 0;///< 获取值
    virtual CSharedMemoryPoint GetValue(int nID_) = 0;
    virtual CSharedMemoryPoint GetValue(QString strRTDBName_) = 0;
};
extern "C" RTDBSHAREDMEMORYLIBSHARED_EXPORT CRTDBSharedMemoryLibI * CreateDriver(QObject *parent);

#endif // RTDBSHAREDMEMORYLIB_H
