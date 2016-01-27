#ifndef CGI_D_IEC_104_POINTYK_H
#define CGI_D_IEC_104_POINTYK_H

#include "CGI_D_IEC_104_PointBase.h"

class CGI_D_IEC_104_PointYK : public CGI_D_IEC_104_PointBase
{
public:
    CGI_D_IEC_104_PointYK();
    enum{
        Point_YK_NoSelect = 0,
        Point_YK_Select = 1
    };
    unsigned int m_nSE;///< 0 未被选择 1 被选择
};

#endif // CGI_D_IEC_104_POINTYK_H
