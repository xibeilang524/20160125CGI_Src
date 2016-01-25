#ifndef CPOINTNODECDT_H
#define CPOINTNODECDT_H

#include <QMap>
#include "CPointCDT.h"


class CPointNodeCDT
{
public:
    CPointNodeCDT();
    void AddPoint(CPointCDT *pPointCDT);
    CPointCDT *FindPoint(int nSrcStationAdd_, int nStart_, int nPoint);
private:
    typedef QMap<int, CPointCDT*> CDTPointNOMap;///< int为点号，CPointCDT为点的指针
    typedef QMap<int, CDTPointNOMap*> CDTStartFunctionalGroupMap;///< int为点的起始功能码，CDTPointNOMap为点的map
    QMap<int, CDTStartFunctionalGroupMap*> CDTPointNode;///< int为源站地址，CDTPOINTMAP

};

#endif // CPOINTNODECDT_H
