#include "CGI_D_IEC_104_PointNode.h"
#include <QDebug>

bool CGI_D_IEC_104_PublicAddress_Node::AddPoint(CGI_D_IEC_104_PointBase *pPoint_)
{
    if (m_PointMap.contains(pPoint_->m_nPointNumber))
    {
        return false;
    }

    bool addResult = false;

    switch (pPoint_->GetPointType()) {
    case TAG_TYPE_DI://遥信
    {
        switch (pPoint_->Get104Type()) {
        case M_SP_NA_1:
            YX_M_SP_NA_1_PointMap.insert(pPoint_->m_nPointNumber,(CGI_D_IEC_104_PointYX *)pPoint_);
            m_PointMap.insert(pPoint_->m_nPointNumber,pPoint_);
            addResult = true;
            break;
        case M_DP_NA_1:
            YX_M_DP_NA_1_PointMap.insert(pPoint_->m_nPointNumber,(CGI_D_IEC_104_PointYX *)pPoint_);
            m_PointMap.insert(pPoint_->m_nPointNumber,pPoint_);
            addResult = true;
            break;
        default:
            break;
        }
    }
        break;
    case TAG_TYPE_AI://遥测
    {
        switch (pPoint_->Get104Type()) {
        case M_ME_NA_1:
            YC_M_ME_NA_1_PointMap.insert(pPoint_->m_nPointNumber,(CGI_D_IEC_104_PointYC *)pPoint_);
            m_PointMap.insert(pPoint_->m_nPointNumber,pPoint_);
            addResult = true;
            break;
        case M_ME_NB_1:
            YC_M_ME_NB_1_PointMap.insert(pPoint_->m_nPointNumber,(CGI_D_IEC_104_PointYC *)pPoint_);
            m_PointMap.insert(pPoint_->m_nPointNumber,pPoint_);
            addResult = true;
            break;
        case M_ME_NC_1:
            YC_M_ME_NC_1_PointMap.insert(pPoint_->m_nPointNumber,(CGI_D_IEC_104_PointYC *)pPoint_);
            m_PointMap.insert(pPoint_->m_nPointNumber,pPoint_);
            addResult = true;
            break;
        case M_ME_ND_1:
            YC_M_ME_ND_1_PointMap.insert(pPoint_->m_nPointNumber,(CGI_D_IEC_104_PointYC *)pPoint_);
            m_PointMap.insert(pPoint_->m_nPointNumber,pPoint_);
            addResult = true;
            break;
        default:
            break;
        }
    }
        break;
    case TAG_TYPE_MEM:
        YM_M_IT_NA_1_PointMap.insert(pPoint_->m_nPointNumber,(CGI_D_IEC_104_PointYM *)pPoint_);
        m_PointMap.insert(pPoint_->m_nPointNumber,pPoint_);
        addResult = true;
        break;
    case TAG_TYPE_DO://遥控
        YK_PointMap.insert(pPoint_->m_nPointNumber,(CGI_D_IEC_104_PointYK *)pPoint_);
        m_PointMap.insert(pPoint_->m_nPointNumber,pPoint_);
        addResult = true;
        break;
    case TAG_TYPE_AO://遥设
        YS_PointMap.insert(pPoint_->m_nPointNumber,(CGI_D_IEC_104_PointYS *)pPoint_);
        m_PointMap.insert(pPoint_->m_nPointNumber,pPoint_);
        addResult = true;
        break;
    default:
        qDebug()<<"Tag Type error!!!"<<pPoint_->m_nTagType;
        Q_ASSERT(false);
        break;
    }
    return addResult;
}

QMap<int, CGI_D_IEC_104_PointYX *> *CGI_D_IEC_104_PublicAddress_Node::Get_YX_PointMap(ASDU101_TYPE nType)
{
    Q_ASSERT( (nType == M_SP_NA_1) || (nType == M_DP_NA_1) );
    switch (nType) {
    case M_SP_NA_1:
        return &YX_M_SP_NA_1_PointMap;
    case M_DP_NA_1:
        return &YX_M_DP_NA_1_PointMap;
    default:
        return NULL;
    }
}

QMap<int, CGI_D_IEC_104_PointYC *> *CGI_D_IEC_104_PublicAddress_Node::Get_YC_PointMap(ASDU101_TYPE nType)
{
    Q_ASSERT( (nType == M_ME_NA_1) || (nType == M_ME_NB_1) || (nType == M_ME_NC_1) || (nType == M_ME_ND_1) );
    switch (nType) {
    case M_ME_NA_1:
        return &YC_M_ME_NA_1_PointMap;
    case M_ME_NB_1:
        return &YC_M_ME_NB_1_PointMap;
    case M_ME_NC_1:
        return &YC_M_ME_NC_1_PointMap;
    case M_ME_ND_1:
        return &YC_M_ME_ND_1_PointMap;
    default:
        return NULL;
    }
}

QMap<int, CGI_D_IEC_104_PointYM *> *CGI_D_IEC_104_PublicAddress_Node::Get_YM_PointMap(ASDU101_TYPE nType)
{
    Q_ASSERT(nType == M_IT_NA_1);
    switch (nType) {
    case M_IT_NA_1:
        return &YM_M_IT_NA_1_PointMap;
    default:
        return NULL;
    }
}

QMap<int, CGI_D_IEC_104_PointYK *> *CGI_D_IEC_104_PublicAddress_Node::Get_YK_PointMap()
{
    return &YK_PointMap;
}

QMap<int, CGI_D_IEC_104_PointYS *> *CGI_D_IEC_104_PublicAddress_Node::Get_YS_PointMap()
{
    return &YS_PointMap;
}

QMap<int, CGI_D_IEC_104_PointBase *> *CGI_D_IEC_104_PublicAddress_Node::Get_AllPointMap()
{
    return &m_PointMap;
}


CGI_D_IEC_104_PointNode::CGI_D_IEC_104_PointNode(QObject *parent) :
    QObject(parent)
{
}

bool CGI_D_IEC_104_PointNode::AddPoint(CGI_D_IEC_104_PointBase *pPoint_)
{
    if (m_CommandAddressPointNode.contains(pPoint_->m_nPublicAddress))
    {
        CGI_D_IEC_104_PublicAddress_Node *pPublicAddress_Node = m_CommandAddressPointNode.value(pPoint_->m_nPublicAddress);
        if (pPublicAddress_Node == NULL)
        {
            return false;
        }else
        {
            return pPublicAddress_Node->AddPoint(pPoint_);
        }
    }else
    {
        CGI_D_IEC_104_PublicAddress_Node *pPublicAddress_Node = new CGI_D_IEC_104_PublicAddress_Node;
        m_CommandAddressPointNode.insert(pPoint_->m_nPublicAddress,pPublicAddress_Node);
        return pPublicAddress_Node->AddPoint(pPoint_);
    }
}

QMap<int, CGI_D_IEC_104_PublicAddress_Node *> *CGI_D_IEC_104_PointNode::GetCommandAddress_PointNode()
{
    return &m_CommandAddressPointNode;
}

