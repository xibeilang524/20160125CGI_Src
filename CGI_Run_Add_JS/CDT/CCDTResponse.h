#ifndef CCDTRESPONSE_H
#define CCDTRESPONSE_H

#include "CBaseFrameCDT.h"

class CCDTResponse : public CBaseFrameCDT
{
    Q_OBJECT
public:
    explicit CCDTResponse();

    BYTE GetWordOfInfo();
signals:

public slots:

};

#endif // CCDTRESPONSE_H
