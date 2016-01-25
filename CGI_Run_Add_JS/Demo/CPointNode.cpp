#include "CPointNode.h"

CPointNode::CPointNode()
{
}
bool CPointNode::AddPoint(CPointDemoBase *pPoint_)
{
    CDemoPointNode *pPointNode = m_PointNodeMap.value(pPoint_->m_nScanTime);
    if (NULL == pPointNode)
    {
        pPointNode = new CDemoPointNode;
        pPointNode->m_nPeriodicStorage = pPoint_->m_nScanTime;
        pPointNode->m_nStorageMoment = 0;
        pPointNode->m_PointList.append(pPoint_);
        m_PointNodeMap.insert(pPoint_->m_nScanTime,pPointNode);
    }else
    {
        pPointNode->m_PointList.append(pPoint_);
    }
    return true;
}
