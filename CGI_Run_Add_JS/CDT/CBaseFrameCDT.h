#ifndef CBASEFRAMECDT_H
#define CBASEFRAMECDT_H

#include "../PortInclude/CBaseFrame.h"


class CBaseFrameCDT : public CBaseFrame
{
    Q_OBJECT
public:
    explicit CBaseFrameCDT();

    static BYTE MakeCRC_CDT(const void *pInfo, int nLen);
signals:

public slots:

};

#endif // CBASEFRAMECDT_H
