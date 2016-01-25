#include "CPointNode.h"

CPointNode::CPointNode()
{
}

bool CPointNode::AddPoint(CPointDLT_645_07 *pPoint_)
{
    if (Device_Read_Map.contains(pPoint_->m_arrayDeviceAddr))
    {
        DI_DLT_645_07PointMap *pDIPointMap = Device_Read_Map.value(pPoint_->m_arrayDeviceAddr);
        if (pDIPointMap->contains(pPoint_->m_unDI))
        {
            DLT_645_07PointMap *pPointMap = pDIPointMap->value(pPoint_->m_unDI);
            if (pPointMap->contains(pPoint_->m_nStartByte))
            {
                return false;
            }else
            {
                pPointMap->insert(pPoint_->m_nStartByte,pPoint_);
                return true;
            }
        }else
        {
            DLT_645_07PointMap *pPointMap = new DLT_645_07PointMap;
            pPointMap->insert(pPoint_->m_nStartByte,pPoint_);

            pDIPointMap->insert(pPoint_->m_unDI,pPointMap);
            return true;
        }
    }else
    {
        DLT_645_07PointMap *pPointMap = new DLT_645_07PointMap;
        pPointMap->insert(pPoint_->m_nStartByte,pPoint_);

        DI_DLT_645_07PointMap *pDIPointMap = new DI_DLT_645_07PointMap;
        pDIPointMap->insert(pPoint_->m_unDI,pPointMap);

        Device_Read_Map.insert(pPoint_->m_arrayDeviceAddr,pDIPointMap);
        return true;
    }
    return true;
}

DLT_645_07PointMap *CPointNode::FindPointMap(QByteArray baDeviceID_, unsigned int unDI_)
{
    DLT_645_07PointMap *pPointMap = NULL;

    if (Device_Read_Map.contains(baDeviceID_))
    {
        DI_DLT_645_07PointMap *pDIPointMap = Device_Read_Map.value(baDeviceID_);
        if (pDIPointMap->contains(unDI_))
        {
            pPointMap = pDIPointMap->value(unDI_);
        }
    }
    return pPointMap;
}

//CPointNode::DLT_645_07PointMap *CPointNode::FindPoint(QByteArray baDeviceID_, unsigned int unDI_)
//{
//    DLT_645_07PointMap *pPointMap = NULL;

//    if (Device_Read_Map.contains(baDeviceID_))
//    {
//        DI_DLT_645_07PointMap pDIPointMap = Device_Read_Map.value(baDeviceID_);
//        if (pDIPointMap.contains(unDI_))
//        {
//            pPointMap = pDIPointMap.value(unDI_);
//        }
//    }
//    return pPointMap;
//}
