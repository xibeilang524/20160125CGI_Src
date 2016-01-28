#ifndef CPOINTNODE_H
#define CPOINTNODE_H

#include <QMap>
#include "CPointDLT_645_07.h"

typedef QMap<int, CPointDLT_645_07 *> DLT_645_07PointMap;///< int 为开始字节
typedef QMap<unsigned int,DLT_645_07PointMap *> DI_DLT_645_07PointMap;///< unsigned int 为DI0~DI3
class CPointNode
{
public:
    CPointNode();
    bool AddPoint(CPointDLT_645_07 *pPoint_);
    DLT_645_07PointMap *FindPointMap(QByteArray baDeviceID_, unsigned int unDI_);
    QMap<QByteArray,DI_DLT_645_07PointMap *> Device_Read_Map;///< 读点
};

#endif // CPOINTNODE_H
