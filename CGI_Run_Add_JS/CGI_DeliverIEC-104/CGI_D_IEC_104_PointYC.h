#ifndef CGI_D_IEC_104_POINTYC_H
#define CGI_D_IEC_104_POINTYC_H

#include "CGI_D_IEC_104_PointBase.h"

class CGI_D_IEC_104_PointYC : public CGI_D_IEC_104_PointBase
{
public:
    CGI_D_IEC_104_PointYC();
    int Get104Type();
    int m_n_IEC_104_YC_PointType;
};

#endif // CGI_D_IEC_104_POINTYC_H
