#ifndef CGI_D_IEC_104_POINTYX_H
#define CGI_D_IEC_104_POINTYX_H

#include "CGI_D_IEC_104_PointBase.h"

class CGI_D_IEC_104_PointYX : public CGI_D_IEC_104_PointBase
{
public:
    CGI_D_IEC_104_PointYX();
    int Get104Type();
    int m_n_IEC_104_YX_PointType;
};

#endif // CGI_D_IEC_104_POINTYX_H
