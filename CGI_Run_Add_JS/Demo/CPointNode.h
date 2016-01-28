#ifndef CPOINTNODE_H
#define CPOINTNODE_H

#include <QMap>
#include "CPointDemoBase.h"

class CDemoPointNode
{
public:
    CDemoPointNode()
    {
        m_nPeriodicStorage = 0;
        m_nStorageMoment = 0;
    }
    int m_nPeriodicStorage;///< 改变周期
    int m_nStorageMoment;///< 改变时刻
    QList<CPointDemoBase* > m_PointList;
};
class CPointNode
{
public:
    CPointNode();
    bool AddPoint(CPointDemoBase *pPoint_);
    QMap<unsigned int ,CDemoPointNode *> m_PointNodeMap; /// unsigned int 为点的周期
};

#endif // CPOINTNODE_H
