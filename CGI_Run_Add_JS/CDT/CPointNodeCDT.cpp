#include "CPointNodeCDT.h"
#include <QDebug>

CPointNodeCDT::CPointNodeCDT()
{
}

/*!
 \brief 向点map中添加点

 \fn CPointNodeCDT::AddPoint
 \param pPointCDT
*/
void CPointNodeCDT::AddPoint(CPointCDT *pPointCDT)
{
    if (CDTPointNode.contains(pPointCDT->m_nSrcStationAdd))
    {
        CDTStartFunctionalGroupMap *pCDTStartFunctionalGroupMap = CDTPointNode.value(pPointCDT->m_nSrcStationAdd);
        if (pCDTStartFunctionalGroupMap->contains(pPointCDT->m_nStartFunctionalGroup))
        {
            CDTPointNOMap *pCDTPointNOMap = pCDTStartFunctionalGroupMap->value(pPointCDT->m_nStartFunctionalGroup);
            pCDTPointNOMap->insert(pPointCDT->m_nPointNO,pPointCDT);
        }else
        {
            CDTPointNOMap *pCDTPointNOMap = new CDTPointNOMap;
            pCDTPointNOMap->insert(pPointCDT->m_nPointNO,pPointCDT);
            pCDTStartFunctionalGroupMap->insert(pPointCDT->m_nStartFunctionalGroup,pCDTPointNOMap);
        }
    }else
    {
        CDTPointNOMap *pCDTPointNOMap = new CDTPointNOMap;
        pCDTPointNOMap->insert(pPointCDT->m_nPointNO,pPointCDT);

        CDTStartFunctionalGroupMap *pCDTStartFunctionalGroupMap = new CDTStartFunctionalGroupMap;
        pCDTStartFunctionalGroupMap->insert(pPointCDT->m_nStartFunctionalGroup,pCDTPointNOMap);

        CDTPointNode.insert(pPointCDT->m_nSrcStationAdd,pCDTStartFunctionalGroupMap);
    }
}

/*!
 \brief 从点map中取点

 \fn CPointNodeCDT::FindPoint
 \param nSrcStationAdd_ 点的源站地址
 \param nStart_ 点的起始功能码
 \param nPoint 点的点号
 \return CPointCDT 若找到此点则返回点的指针，反之返回NULL
*/
CPointCDT *CPointNodeCDT::FindPoint(int nSrcStationAdd_, int nStart_, int nPoint)
{
    CPointCDT *result = NULL;

    if (CDTPointNode.contains(nSrcStationAdd_))
    {
        CDTStartFunctionalGroupMap *pCDTStartFunctionalGroupMap = CDTPointNode.value(nSrcStationAdd_);
        if (pCDTStartFunctionalGroupMap->contains(nStart_))
        {
            CDTPointNOMap *pCDTPointNOMap = pCDTStartFunctionalGroupMap->value(nStart_);
            if (pCDTPointNOMap->contains(nPoint))
            {
                result = pCDTPointNOMap->value(nPoint);
            }
        }
    }

    return result;
}
