#ifndef CSHAREDMEMORYPOINT_H
#define CSHAREDMEMORYPOINT_H
#include <QVariant>
class CSharedMemoryPoint
{
public:
    CSharedMemoryPoint()
    {
        m_isValid = false;
        m_nVariant = QVariant();
    }
    CSharedMemoryPoint(int nPointIndex_)
    {
        m_nPointIndex = nPointIndex_;
        m_isValid = false;
        m_nVariant = QVariant();
        m_strDateTime = QString();
    }

    CSharedMemoryPoint(int nPointIndex, QVariant Variant_, QString strDateTime,bool isValid)
    {
        m_isValid = isValid;
        m_nPointIndex = nPointIndex;
        m_nVariant = Variant_;
        m_strDateTime = strDateTime;
    }
    CSharedMemoryPoint(const CSharedMemoryPoint& SharedMemoryPoint)
    {
        m_isValid = SharedMemoryPoint.m_isValid;
        m_nPointIndex = SharedMemoryPoint.m_nPointIndex;
        m_nVariant = SharedMemoryPoint.m_nVariant;
        m_strDateTime = SharedMemoryPoint.m_strDateTime;
    }
    bool m_isValid;///< 是否有效
    int m_nPointIndex;///< 点的索引号
    QVariant m_nVariant;///< 点的值
    QString m_strDateTime;///< 时标
};

#endif // CSHAREDMEMORYPOINT_H
