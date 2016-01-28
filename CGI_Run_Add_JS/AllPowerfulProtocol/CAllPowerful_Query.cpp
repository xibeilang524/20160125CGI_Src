#include "CAllPowerful_Query.h"

CAllPowerful_Query::CAllPowerful_Query()
{
    m_nMinResponseFrameLength = 0;
    m_baAllPowerfulSendFrame.clear();
}

/*!
 \brief 添加点，并且需要计算出最小的返回报文长度

 \fn CAllPowerful_Query::AddPoint
 \param pPoint_
 \return bool
*/
bool CAllPowerful_Query::AddPoint(CAllPowerful_Point *pPoint_)
{
    if (pPoint_->m_nByteCount >= 5000)
    {
        int nStartBit = (pPoint_->m_nByteCount%5000)/100;
        int nBitCount = (pPoint_->m_nByteCount%100);
        int nByteCount = (nStartBit+nBitCount+8)%8;
        if (pPoint_->m_nStartByte + nByteCount > m_nMinResponseFrameLength)
        {
            m_nMinResponseFrameLength = pPoint_->m_nStartByte+nByteCount;
        }
    }else if ( (pPoint_->m_nByteCount>=10) && (pPoint_->m_nByteCount<=17) )
    {
//        pPoint_->m_nByteCount = pPoint_->m_nByteCount%10+10;
        if (pPoint_->m_nStartByte > m_nMinResponseFrameLength)
        {
            m_nMinResponseFrameLength = pPoint_->m_nByteCount;
        }
    }else if ( (pPoint_->m_nByteCount >=1) && (pPoint_->m_nByteCount<=4) )
    {
        if (pPoint_->m_nByteCount+pPoint_->m_nByteCount > m_nMinResponseFrameLength)
        {
            m_nMinResponseFrameLength = pPoint_->m_nByteCount+pPoint_->m_nByteCount;
        }
    }else
    {/// 参数不正确，添加点失败
        return false;
    }
    if (m_AllPowerful_PointMAP.contains(pPoint_->m_nStartByte))
    {
        ByteCount_PointMAP *pByteCount_PointMAP = m_AllPowerful_PointMAP.value(pPoint_->m_nStartByte);
        if (pByteCount_PointMAP == NULL)
        {
            pByteCount_PointMAP = new ByteCount_PointMAP;
            pByteCount_PointMAP->insert(pPoint_->m_nByteCount,pPoint_);
            m_AllPowerful_PointMAP.insert(pPoint_->m_nStartByte,pByteCount_PointMAP);
        }else
        {
            pByteCount_PointMAP->insert(pPoint_->m_nByteCount,pPoint_);
            m_AllPowerful_PointMAP.insert(pPoint_->m_nStartByte,pByteCount_PointMAP);
        }
    }else
    {
        ByteCount_PointMAP *pByteCount_PointMAP = new ByteCount_PointMAP;
        pByteCount_PointMAP->insert(pPoint_->m_nByteCount,pPoint_);
        m_AllPowerful_PointMAP.insert(pPoint_->m_nStartByte,pByteCount_PointMAP);
    }
    return true;
}
QByteArray CAllPowerful_Query::GetbaSendFrame() const
{
    return m_baAllPowerfulSendFrame;
}

void CAllPowerful_Query::SetBaSendFrame(const QByteArray &baSendFrame)
{
    m_baAllPowerfulSendFrame = baSendFrame;
}

int CAllPowerful_Query::GetMinResponseFrameLength()
{
    return m_nMinResponseFrameLength;
}

QMap<int, CAllPowerful_Query::ByteCount_PointMAP *> *CAllPowerful_Query::Get_pAllPowerful_PointMAP()
{
    return &m_AllPowerful_PointMAP;
}

