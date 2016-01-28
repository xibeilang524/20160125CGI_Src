#include "CProtocolCDT.h"
#include "../PortInclude/CRTDBI.h"
#include "../PortInclude/CTagI.h"
#include "../PortInclude/cvaluebase.h"
#include "../PortInclude/XmlParser/CXmlParser.h"
#include "CDT.h"
#include <QCoreApplication>

CBufferPool g_bufferSyncWord;//同步字


/*!
 \brief 库文件导出接口函数

 \fn CreateDriver
 \param parent QObject对象指针
 \return CProtocolI 返回创建的驱动指针
*/
CProtocolI * CreateDriver(QObject *parent)
{
    CProtocolCDT *testProtocol = new CProtocolCDT(parent);
    return  testProtocol;
}


CProtocolCDT::CProtocolCDT(QObject */*parent*/)
{
}

bool CProtocolCDT::OnCreateChannel(const QString strChannelFileName_, CRTDBI *pDatabaseReturn)
{
    SetPROTOCOL_TYPE(Collect_Type);/// 设置驱动类型 分为转发和采集驱动
    if (!CProtocolBase::OnCreateChannel(strChannelFileName_,pDatabaseReturn))
        return false;
    QDomDocument ChannelDoc;

    if (!CXmlParser::readFile(strChannelFileName_,ChannelDoc))
        return false;

    QDomElement docElem = ChannelDoc.documentElement();
    if (docElem.childNodes().count() != 5)///<zzy 2015/1/14 修改:5个节点
        return false;
//    QDomElement Channel    = docElem.childNodes().at(0).toElement();
//    QDomElement Top        = docElem.childNodes().at(1).toElement();
    QDomElement Protocol   = docElem.childNodes().at(2).toElement();
//    QDomElement MainPort   = docElem.childNodes().at(3).toElement();
    QDomElement DeviceList = docElem.childNodes().at(4).toElement();
    if (Protocol.attribute("tongbuziSelect") == "EB 90")
    {
        BYTE szPre [] ={0xeb,0x90,0xeb,0x90,0xeb,0x90};
        g_bufferSyncWord.SetData(szPre,6);
    }else if (Protocol.attribute("tongbuziSelect") == "D7 09")
    {
        BYTE szPre [] ={0xD7,0x09,0xD7,0x09,0xD7,0x09};
        g_bufferSyncWord.SetData(szPre,6);
    }else
    {
        BYTE szPre [] ={0xeb,0x90,0xeb,0x90,0xeb,0x90};
        g_bufferSyncWord.SetData(szPre,6);
    }

    InitPointMap(DeviceList);

    return true;
}

bool CProtocolCDT::OnCreateChannel(const QDomElement &ChannelElement_, ChannelType nChannelType_, int nComOrCanNumber_, CRTDBI *pDatabaseReturn_)
{
    SetPROTOCOL_TYPE(Collect_Type);/// 设置驱动类型 分为转发和采集驱动
    if (!CProtocolBase::OnCreateChannel(ChannelElement_,nChannelType_,nComOrCanNumber_,pDatabaseReturn_))
        return false;

    if (ChannelElement_.elementsByTagName("Protocol").isEmpty())
        return false;
    if (ChannelElement_.elementsByTagName("DeviceList").isEmpty())
        return false;
    QDomElement Protocol   = ChannelElement_.elementsByTagName("Protocol").at(0).toElement();
    QDomElement DeviceList = ChannelElement_.elementsByTagName("DeviceList").at(0).toElement();
//    <Option value="0" name="D7 09"/>
//    <Option value="1" name="EB 90"/>
    if (Protocol.attribute("tongbuziSelect") == "1")
    {
        BYTE szPre [] ={0xeb,0x90,0xeb,0x90,0xeb,0x90};
        g_bufferSyncWord.SetData(szPre,6);
    }else if (Protocol.attribute("tongbuziSelect") == "0")
    {
        BYTE szPre [] ={0xD7,0x09,0xD7,0x09,0xD7,0x09};
        g_bufferSyncWord.SetData(szPre,6);
    }else
    {
        BYTE szPre [] ={0xeb,0x90,0xeb,0x90,0xeb,0x90};
        g_bufferSyncWord.SetData(szPre,6);
    }

    CGI_InitPointMap(DeviceList);
    return true;
}
/*!
 \brief 初始化点表map

 \fn CProtocolCDT::InitPointMap
 \param DeviceList
*/
void CProtocolCDT::InitPointMap(const QDomElement &DeviceListElement_)
{
    QDomNodeList DeviceList = DeviceListElement_.childNodes();
    for (int i = 0; i <DeviceList.count(); ++i)
    {
        QDomElement DeviceElement = DeviceList.at(i).toElement();
        if (DeviceElement.elementsByTagName("Attribute").count() < 1)
        {
            Q_ASSERT(false);
        }
        int DeviceAddress = DeviceElement.elementsByTagName("Attribute").at(0).toElement().attribute("Address").toInt();
        m_nTargetStationAddress = DeviceAddress;
        QDomNodeList TypeNodeList = DeviceElement.elementsByTagName("Type");
        for (int nType = 0; nType < TypeNodeList.count(); ++nType)
        {
            QDomElement TypeElem = TypeNodeList.at(nType).toElement();
            if (TypeElem.attribute("Name") == "YX"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    InitPointMessage(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_DI,m_nTargetStationAddress);
                }
            }else if (TypeElem.attribute("Name") == "YC"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    InitPointMessage(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_AI,m_nTargetStationAddress);
                }
            }else if (TypeElem.attribute("Name") == "YM"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    InitPointMessage(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_MEM,m_nTargetStationAddress);
                }
            }else if (TypeElem.attribute("Name") == "YK"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    InitPointMessage(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_DO,m_nTargetStationAddress);
                }
            }else if (TypeElem.attribute("Name") == "YS"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    InitPointMessage(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_AO,m_nTargetStationAddress);
                }
            }else
            {
                Q_ASSERT(false);
            }
        }
    }
}

void CProtocolCDT::CGI_InitPointMap(const QDomElement &DeviceListElement_)
{
    QDomNodeList DeviceList = DeviceListElement_.childNodes();
    for (int i = 0; i <DeviceList.count(); ++i)
    {
        QDomElement DeviceElement = DeviceList.at(i).toElement();
        if (DeviceElement.elementsByTagName("Attribute").count() < 1)
        {
            Q_ASSERT(false);
        }
        int DeviceAddress = DeviceElement.elementsByTagName("Attribute").at(0).toElement().attribute("Device_Address").toInt();
        m_nTargetStationAddress = DeviceAddress;
        QDomNodeList TypeNodeList = DeviceElement.elementsByTagName("Type");
        for (int nType = 0; nType < TypeNodeList.count(); ++nType)
        {
            QDomElement TypeElem = TypeNodeList.at(nType).toElement();
            if (TypeElem.attribute("Name") == "YX"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InitPointMessage(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_DI,m_nTargetStationAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointCDT *pPoint = new CPointCDT;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YX_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YX_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YX_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YX_Link_Strings");
                    pPoint->m_fKValue = PointElem_.attribute("Device_YX_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YX_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }
                    int nTagType_ = TAG_TYPE_DI;
                    pPoint->m_nTagType = nTagType_;/// 点的类型


                    pPoint->m_nSrcStationAdd = m_nTargetStationAddress;
                    pPoint->m_nPointNO = PointElem_.attribute("Device_YX_PointNO_Strings").toInt();
                    pPoint->m_nTargetStationAdd = PointElem_.attribute("Device_YX_TargetStationAddress_Strings").toInt();
                    switch (nTagType_) {
                    case TAG_TYPE_DI:
                    {
                        pPoint->m_nStartFunctionalGroup = 0xF0;
                    }
                        break;
                    case TAG_TYPE_AI:
                    {
                        if (PointElem_.attribute("Device_YC_PointType_Strings") == "遥测")
                        {
                            pPoint->m_nStartFunctionalGroup = 0x00;
                        }else if (PointElem_.attribute("Device_YC_PointType_Strings") == "总加遥测")
                        {
                            pPoint->m_nStartFunctionalGroup = 0x86;
                        }else
                        {
                            pPoint->m_nStartFunctionalGroup = 0x00;
                        }
                    }
                        break;
                    case TAG_TYPE_MEM:
                    {
                        pPoint->m_nStartFunctionalGroup = 0xA0;
                    }
                        break;
                    case TAG_TYPE_DO:
                    {
                        return ;
                    }
                        break;
                    case TAG_TYPE_AO:
                    {
                        return ;
                    }
                        break;
                    default:
                        qDebug()<<"Don't have TAG_TYPE_DI."<<nTagType_;
                        Q_ASSERT(false);
                        return ;
                        break;
                    }
                    m_PointNodeCDT.AddPoint(pPoint);
                }
            }else if (TypeElem.attribute("Name") == "YC"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InitPointMessage(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_AI,m_nTargetStationAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointCDT *pPoint = new CPointCDT;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YC_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YC_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YC_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YC_Link_Strings");
                    pPoint->m_fKValue = PointElem_.attribute("Device_YC_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YC_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }
                    int nTagType_ = TAG_TYPE_AI;
                    pPoint->m_nTagType = nTagType_;/// 点的类型


                    pPoint->m_nSrcStationAdd = m_nTargetStationAddress;
                    pPoint->m_nPointNO = PointElem_.attribute("Device_YC_PointNO_Strings").toInt();
                    pPoint->m_nTargetStationAdd = PointElem_.attribute("Device_YC_TargetStationAddress_Strings").toInt();
                    switch (nTagType_) {
                    case TAG_TYPE_DI:
                    {
                        pPoint->m_nStartFunctionalGroup = 0xF0;
                    }
                        break;
                    case TAG_TYPE_AI:
                    {
                        if (PointElem_.attribute("Device_YC_PointType_Strings") == "遥测")
                        {
                            pPoint->m_nStartFunctionalGroup = 0x00;
                        }else if (PointElem_.attribute("Device_YC_PointType_Strings") == "总加遥测")
                        {
                            pPoint->m_nStartFunctionalGroup = 0x86;
                        }else
                        {
                            pPoint->m_nStartFunctionalGroup = 0x00;
                        }
                    }
                        break;
                    case TAG_TYPE_MEM:
                    {
                        pPoint->m_nStartFunctionalGroup = 0xA0;
                    }
                        break;
                    case TAG_TYPE_DO:
                    {
                        return ;
                    }
                        break;
                    case TAG_TYPE_AO:
                    {
                        return ;
                    }
                        break;
                    default:
                        qDebug()<<"Don't have TAG_TYPE_DI."<<nTagType_;
                        Q_ASSERT(false);
                        return ;
                        break;
                    }
                    m_PointNodeCDT.AddPoint(pPoint);
                }
            }else if (TypeElem.attribute("Name") == "YM"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InitPointMessage(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_MEM,m_nTargetStationAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointCDT *pPoint = new CPointCDT;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YM_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YM_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YM_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YM_Link_Strings");
                    pPoint->m_fKValue = PointElem_.attribute("Device_YM_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YM_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }
                    int nTagType_ = TAG_TYPE_MEM;
                    pPoint->m_nTagType = nTagType_;/// 点的类型


                    pPoint->m_nSrcStationAdd = m_nTargetStationAddress;
                    pPoint->m_nPointNO = PointElem_.attribute("Device_YM_PointNO_Strings").toInt();
                    pPoint->m_nTargetStationAdd = PointElem_.attribute("Device_YM_TargetStationAddress_Strings").toInt();
                    switch (nTagType_) {
                    case TAG_TYPE_DI:
                    {
                        pPoint->m_nStartFunctionalGroup = 0xF0;
                    }
                        break;
                    case TAG_TYPE_AI:
                    {
                        if (PointElem_.attribute("Device_YC_PointType_Strings") == "遥测")
                        {
                            pPoint->m_nStartFunctionalGroup = 0x00;
                        }else if (PointElem_.attribute("Device_YC_PointType_Strings") == "总加遥测")
                        {
                            pPoint->m_nStartFunctionalGroup = 0x86;
                        }else
                        {
                            pPoint->m_nStartFunctionalGroup = 0x00;
                        }
                    }
                        break;
                    case TAG_TYPE_MEM:
                    {
                        pPoint->m_nStartFunctionalGroup = 0xA0;
                    }
                        break;
                    case TAG_TYPE_DO:
                    {
                        return ;
                    }
                        break;
                    case TAG_TYPE_AO:
                    {
                        return ;
                    }
                        break;
                    default:
                        qDebug()<<"Don't have TAG_TYPE_DI."<<nTagType_;
                        Q_ASSERT(false);
                        return ;
                        break;
                    }
                    m_PointNodeCDT.AddPoint(pPoint);
                }
            }else if (TypeElem.attribute("Name") == "YK"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InitPointMessage(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_DO,m_nTargetStationAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointCDT *pPoint = new CPointCDT;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YK_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YK_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YK_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YK_Link_Strings");
                    pPoint->m_fKValue = PointElem_.attribute("Device_YK_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YK_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }
                    int nTagType_ = TAG_TYPE_DO;
                    pPoint->m_nTagType = nTagType_;/// 点的类型


                    pPoint->m_nSrcStationAdd = m_nTargetStationAddress;
                    pPoint->m_nPointNO = PointElem_.attribute("Device_YK_PointNO_Strings").toInt();
                    pPoint->m_nTargetStationAdd = PointElem_.attribute("Device_YK_TargetStationAddress_Strings").toInt();
                    switch (nTagType_) {
                    case TAG_TYPE_DI:
                    {
                        pPoint->m_nStartFunctionalGroup = 0xF0;
                    }
                        break;
                    case TAG_TYPE_AI:
                    {
                        if (PointElem_.attribute("Device_YC_PointType_Strings") == "遥测")
                        {
                            pPoint->m_nStartFunctionalGroup = 0x00;
                        }else if (PointElem_.attribute("Device_YC_PointType_Strings") == "总加遥测")
                        {
                            pPoint->m_nStartFunctionalGroup = 0x86;
                        }else
                        {
                            pPoint->m_nStartFunctionalGroup = 0x00;
                        }
                    }
                        break;
                    case TAG_TYPE_MEM:
                    {
                        pPoint->m_nStartFunctionalGroup = 0xA0;
                    }
                        break;
                    case TAG_TYPE_DO:
                    {
                        return ;
                    }
                        break;
                    case TAG_TYPE_AO:
                    {
                        return ;
                    }
                        break;
                    default:
                        qDebug()<<"Don't have TAG_TYPE_DI."<<nTagType_;
                        Q_ASSERT(false);
                        return ;
                        break;
                    }
                    m_PointNodeCDT.AddPoint(pPoint);
                }
            }else if (TypeElem.attribute("Name") == "YS"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InitPointMessage(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_AO,m_nTargetStationAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointCDT *pPoint = new CPointCDT;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YS_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YS_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YS_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YS_Link_Strings");
                    pPoint->m_fKValue = PointElem_.attribute("Device_YS_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YS_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }
                    int nTagType_ = TAG_TYPE_AO;
                    pPoint->m_nTagType = nTagType_;/// 点的类型


                    pPoint->m_nSrcStationAdd = m_nTargetStationAddress;
                    pPoint->m_nPointNO = PointElem_.attribute("Device_YS_PointNO_Strings").toInt();
                    pPoint->m_nTargetStationAdd = PointElem_.attribute("Device_YS_TargetStationAddress_Strings").toInt();
                    switch (nTagType_) {
                    case TAG_TYPE_DI:
                    {
                        pPoint->m_nStartFunctionalGroup = 0xF0;
                    }
                        break;
                    case TAG_TYPE_AI:
                    {
                        if (PointElem_.attribute("Device_YC_PointType_Strings") == "遥测")
                        {
                            pPoint->m_nStartFunctionalGroup = 0x00;
                        }else if (PointElem_.attribute("Device_YC_PointType_Strings") == "总加遥测")
                        {
                            pPoint->m_nStartFunctionalGroup = 0x86;
                        }else
                        {
                            pPoint->m_nStartFunctionalGroup = 0x00;
                        }
                    }
                        break;
                    case TAG_TYPE_MEM:
                    {
                        pPoint->m_nStartFunctionalGroup = 0xA0;
                    }
                        break;
                    case TAG_TYPE_DO:
                    {
                        return ;
                    }
                        break;
                    case TAG_TYPE_AO:
                    {
                        return ;
                    }
                        break;
                    default:
                        qDebug()<<"Don't have TAG_TYPE_DI."<<nTagType_;
                        Q_ASSERT(false);
                        return ;
                        break;
                    }
                    m_PointNodeCDT.AddPoint(pPoint);
                }
            }else
            {
                Q_ASSERT(false);
            }
        }
    }
}
void CProtocolCDT::InitPointMessage(const QDomElement &PointElem_,TAG_TYPE nTagType_,int nSrcStationAddress_)
{
    CPointCDT *pPoint = new CPointCDT;
    pPoint->m_strPointName = PointElem_.attribute("Name");
    pPoint->m_strPointComment = PointElem_.attribute("Desc");
    pPoint->m_nID = PointElem_.attribute("SN").toInt();
    pPoint->m_strRTDBName = PointElem_.attribute("Link");
    pPoint->m_fKValue = PointElem_.attribute("KValue").toFloat();
    pPoint->m_fBaseValue = PointElem_.attribute("BaseValue").toFloat();
    pPoint->m_pProtocol = this;
    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
    if (pPoint->m_pTag)
    {
        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
    }
    pPoint->m_nTagType = nTagType_;/// 点的类型


    pPoint->m_nSrcStationAdd = nSrcStationAddress_;
    pPoint->m_nPointNO = PointElem_.attribute("PointNO").toInt();
    pPoint->m_nTargetStationAdd = PointElem_.attribute("TargetStationAddress").toInt();
    switch (nTagType_) {
    case TAG_TYPE_DI:
    {
        pPoint->m_nStartFunctionalGroup = 0xF0;
    }
        break;
    case TAG_TYPE_AI:
    {
        if (PointElem_.attribute("PointType") == "遥测")
        {
            pPoint->m_nStartFunctionalGroup = 0x00;
        }else if (PointElem_.attribute("PointType") == "总加遥测")
        {
            pPoint->m_nStartFunctionalGroup = 0x86;
        }else
        {
            pPoint->m_nStartFunctionalGroup = 0x00;
        }
    }
        break;
    case TAG_TYPE_MEM:
    {
        pPoint->m_nStartFunctionalGroup = 0xA0;
    }
        break;
    case TAG_TYPE_DO:
    {
        return ;
    }
        break;
    case TAG_TYPE_AO:
    {
        return ;
    }
        break;
    default:
        qDebug()<<"Don't have TAG_TYPE_DI."<<nTagType_;
        Q_ASSERT(false);
        return ;
        break;
    }
    m_PointNodeCDT.AddPoint(pPoint);
}

CPointCDT *CProtocolCDT::FindPoint(int nSrcStationAdd_, int nStart_, int nPoint)
{
    return m_PointNodeCDT.FindPoint(nSrcStationAdd_, nStart_, nPoint);
}
bool CProtocolCDT::OnTagChange(CPointBase */*pPointBase_*/, QVariant /*VarSet_*/)
{
    return true;
}

void CProtocolCDT::ProcessRespond(QByteArray data_)
{
    m_bufferReceive.AppendData(data_.data(),data_.count());
    QByteArray array((char *)m_bufferReceive.GetBuffer(0),m_bufferReceive.GetInfoSize());
    qDebug()<<"array.toHex()"<<array.toHex();
    if (m_bufferReceive.GetInfoSize() < MinFrameLength)
        return ;

    for(int nStart=0; nStart<=m_bufferReceive.GetInfoSize() - MinFrameLength; ++nStart)
    {
        int nFrameLength = IsAFrame(m_bufferReceive.GetBuffer(nStart),m_bufferReceive.GetInfoSize()-nStart);
        if(nFrameLength)
        {
            m_CDTResponse.SetData(m_bufferReceive.GetBuffer(nStart),nFrameLength);
            m_bufferReceive.Fifo(nStart+nFrameLength);
            ProcessResponse(m_CDTResponse);
            nStart=-1;//重新再来
        }
    }
    if(m_bufferReceive.GetInfoSize()>MAX_FRAME)
    {
        m_bufferReceive.SetInfoSize(0);
//        Q_ASSERT(false);
    }
}
/*!
 \brief 判断数据帧是否有效
        说明:	//从机地址->功能码->CRC
 \fn CProtocolCDT::IsAFrame
 \param pInfo 报文
 \param nInfoLength 报文长度
 \return int 成功返回正确报文长度  错误返回0
*/
int  CProtocolCDT::IsAFrame(BYTE *pInfo,int nInfoLength)
{
    if(nInfoLength<12)
        return 0;

    if(g_bufferSyncWord.Compare(0,pInfo,6) != 0)//前导字符不正确
        return 0;


    if(!IsValidInfoWord(pInfo+6))//控制字正确
        return 0;

    CCDTResponse response;
//    response.SetData(pInfo,nInfoLength);
    response.RefBuffer(pInfo,nInfoLength);

    BYTE nWord=response.GetWordOfInfo();
    int nFrameLength = 12+ nWord*6;
    if(nInfoLength < nFrameLength)//长度正确
        return 0;

    for(int i=12;i<nFrameLength ;i+=6)
    {
        if(!IsValidInfoWord(pInfo+i))
            return 0;
    }
    return nFrameLength;
}
/*!
 \brief 判定pInfo 的帧校验是否正确

 \fn CProtocolCDT::IsValidInfoWord
 \param pInfo
 \return bool
*/
bool CProtocolCDT::IsValidInfoWord(const void *pInfo)
{
    Q_ASSERT(pInfo != NULL);
    //	ASSERT(CCdt91Query::MakeCRC_CDT(pInfo,6)==12);

    return CBaseFrameCDT::MakeCRC_CDT(pInfo,6)==12;
}

/*!
 \brief

 \fn CProtocolCDT::ProcessResponse
 \param response_
*/
void CProtocolCDT::ProcessResponse(CCDTResponse &response_)
{
    QByteArray receive((char *)response_.GetBuffer(0),response_.GetInfoSize());
//    receive.toHex().toUpper().count();
    QString frame = receive.toHex().toUpper();
    int nCount = frame.count();
    for (int i=0,j=0; i < nCount; i+=12,++j)
    {
        frame.insert(i+j,' ');
    }
    ShowMessage(SHOWMESSAGE_TYPE_RECEIVE,frame);
    if(response_.GetBit(6,7))
    {
        ProcessFrameExtend(response_);//处理扩展帧
        return ;
    }
    BYTE nInfoWord=response_.GetWordOfInfo();

    BYTE nSrcStationAddress = response_.GetUIntValue(9,1);
    CBaseFrame  frameInfoWord;//一个信息字
    for(int i=0;i<nInfoWord;i++)
    {
        frameInfoWord.RefBuffer(response_.GetBuffer(12+i*6),6);
        BYTE nFunctionCode=frameInfoWord[0];//功能码
        BYTE nLittle = 0x0;
        BYTE nBig    = 0x7f;
//        if ( (((BYTE)0x0) <= nFunctionCode) && (nFunctionCode <= ((BYTE)0x7f)) )//遥测信息字
//            ProcessRMInfoWord(frameInfoWord,nSrcStationAddress);
        if ( (nLittle <= nFunctionCode) && (nFunctionCode <= nBig) )//遥测信息字
        ProcessRMInfoWord(frameInfoWord,nSrcStationAddress);

        if ((0x86 <=nFunctionCode)&&(nFunctionCode <=0x89))//总加遥测
            ProcessTMInfoWord(frameInfoWord,nSrcStationAddress);

        nLittle = 0xf0;
        nBig    = 0xff;
//        if ( (0xf0 <=nFunctionCode) && (nFunctionCode <=0xff) )//遥信信息字
//            ProcessRSInfoWord(frameInfoWord,nSrcStationAddress);
        if ( (nLittle <=nFunctionCode) && (nFunctionCode <=nBig) )//遥信信息字
            ProcessRSInfoWord(frameInfoWord,nSrcStationAddress);


        if ((0xa0 <=nFunctionCode)&&(nFunctionCode <=0xdf))//电能脉冲计数值
            ProcessRPInfoWord(frameInfoWord,nSrcStationAddress);


        if  ((0x8d <= nFunctionCode)&&(nFunctionCode <=0x92))//水位信息
            ProcessWLInfoWord(frameInfoWord,nSrcStationAddress);


        if  (nFunctionCode == 0x8a)//频率信息
            ProcessFrequencyInfoWord(frameInfoWord,nSrcStationAddress);


        if (nFunctionCode ==  0x80 )//事件顺序记录信息//&& frameInfoWord[6]==  0x81
        {
            frameInfoWord.SetData(response_.GetBuffer(12+i*6),12);
            Q_ASSERT(frameInfoWord[6] == 0x81);
            i++;//soe为双信息字
            ProcessSoeInfoWord(frameInfoWord,nSrcStationAddress);
        }

        if(nFunctionCode == 0x84)//时钟怊唤返回//&& frameInfoWord[6]==  0x85
        {
            frameInfoWord.SetData(response_.GetBuffer(12+i*6),12);
            Q_ASSERT(frameInfoWord[6] == 0x85);
            i++;//时钟返回为双信息字
            ProcessTimeBackInfoWord(frameInfoWord,nSrcStationAddress);
        }

        if (nFunctionCode ==  0xec)//子站工作状态信息字
            ProcessSTInfoWord(frameInfoWord,nSrcStationAddress);

        if(nFunctionCode ==  0xe1 || nFunctionCode == 0xe5)//遥控、遥调返校
            ProcessRRInfoWord(frameInfoWord,nSrcStationAddress);
    }
}

/*!
 \brief 处理扩展类型的数据帧

 \fn CProtocolCDT::ProcessFrameExtend
 \param response_ 接收到的帧
*/
void CProtocolCDT::ProcessFrameExtend(CCDTResponse &response_)
{
    Q_ASSERT(response_.GetBit(6,7)==1);
    response_.GetBit(6,7);
    return;
}

/*!
 \brief 处理遥测信息字，获取遥测量 RemoteMeasure

 \fn CProtocolCDT::ProcessRMInfoWord
 \param frameInfoWord 信息字
*/
void CProtocolCDT::ProcessRMInfoWord(CBaseFrame &frameInfoWord, int n_SrcStationAddress_)
{
    FRAME_RM_TYPE *pValue;
    BYTE nFunctionCode=frameInfoWord[0];//功能码
    for(int i=0; i<2;i++)
    {
        int nPoint = nFunctionCode*2+i;
        pValue=(FRAME_RM_TYPE *)frameInfoWord.GetBuffer(1+i*2);
        CPointCDT *pPoint = FindPoint(n_SrcStationAddress_,0,nPoint);
//        qDebug()<<"遥测信息nPoint:"<<nPoint<<"value:"<<pValue->VALUE<<pPoint;
        if (pPoint == NULL)
            continue;
        if(!pValue->OVERFALL && !pValue->INVALID)
        {

            float fValue = (pValue->VALUE+pPoint->m_fBaseValue)*pPoint->m_fKValue;
//            qDebug()<<fValue<<pValue->VALUE<<pPoint->m_strPointName<<pPoint->m_fBaseValue<<pPoint->m_fKValue;
            QVariant VarProject_(fValue);
            QVariant VarOriginal_(pValue->VALUE);
            pPoint->SetValue(pPoint,VarProject_,VarOriginal_);
        }
    }
}

/*!
 \brief 获取总加遥测量 TotalMeasure

 \fn CProtocolCDT::ProcessTMInfoWord
 \param frameInfoWord 信息字
*/
void CProtocolCDT::ProcessTMInfoWord(CBaseFrame &frameInfoWord, int n_SrcStationAddress_)
{
    BYTE nFunctionCode = frameInfoWord[0];//功能码
    for(int i=0; i<2; i++)
    {
        int nPoint = (nFunctionCode-0x86)*2+i;
        CPointCDT *pPoint = FindPoint(n_SrcStationAddress_, 0x86, nPoint);
//        qDebug()<<"总加遥测量nPoint:"<<nPoint<<"value:"<<frameInfoWord.GetUIntValue(1+i*2,2)<<pPoint;
        if (pPoint == NULL)
            continue;

        float fValue = (frameInfoWord.GetUIntValue(1+i*2,2)+pPoint->m_fBaseValue)*pPoint->m_fKValue;
//        qDebug()<<fValue<<frameInfoWord.GetUIntValue(1+i*2,2)<<pPoint->m_strPointName<<pPoint->m_fBaseValue<<pPoint->m_fKValue;
        QVariant VarProject_(fValue);
        QVariant VarOriginal_(frameInfoWord.GetUIntValue(1+i*2,2));
        pPoint->SetValue(pPoint,VarProject_,VarOriginal_);
    }
}

/*!
 \brief 获取遥信量  RemoteSignal

 \fn CProtocolCDT::ProcessRSInfoWord
 \param frameInfoWord 信息字
*/
void CProtocolCDT::ProcessRSInfoWord(CBaseFrame &frameInfoWord, int n_SrcStationAddress_)
{
    BYTE nFunctionCode=frameInfoWord[0];//功能码
    for(int i=1; i<5; i++)
    {
        for(int j=0; j<8; j++)
        {
            BYTE nOffset = (i-1)*8+j;
            int nPoint = (nFunctionCode-0XF0)*32+nOffset;
            CPointCDT *pPoint = FindPoint(n_SrcStationAddress_, 0xF0, nPoint);
//            qDebug()<<"遥信量nPoint:"<<nPoint<<"value:"<<frameInfoWord.GetBit(1,nOffset)<<pPoint;
            if (pPoint == NULL)
                continue;
//            qDebug()<<frameInfoWord.GetBit(1,nOffset)<<pPoint->m_strPointName;
            QVariant VarProject_(frameInfoWord.GetBit(1,nOffset));
            QVariant VarOriginal_(frameInfoWord.GetBit(1,nOffset));
            pPoint->SetValue(pPoint,VarProject_,VarOriginal_);
        }
    }
}

/*!
 \brief 获取遥脉量  RemotePulse

 \fn CProtocolCDT::ProcessRPInfoWord
 \param frameInfoWord 信息字
*/
void CProtocolCDT::ProcessRPInfoWord(CBaseFrame &frameInfoWord, int n_SrcStationAddress_)
{
    FRAME_RP_TYPE *pValue;
    pValue = (FRAME_RP_TYPE *)frameInfoWord.GetBuffer(1);
    BYTE nFunctionCode=frameInfoWord[0];//功能码
    int nPoint  = nFunctionCode-0XA0;
    CPointCDT *pPoint = FindPoint(n_SrcStationAddress_, 0xA0, nPoint);
//    qDebug()<<"遥脉量nPoint:"<<nPoint<<"value:"<<frameInfoWord.GetUIntValue(1,4)<<pPoint;
    if (pPoint == NULL)
        return;
//	IPoint * pTag = FindPoint(0XA0,nPoint);
//	if(pTag == NULL)
//		return;

    if(pValue->INVALID == 1)//无效
        return;
    if(pValue->BCD == 1)//bcd码
    {
        QVariant VarProject_(BCD2Int(pValue,3)/100.0);
        QVariant VarOriginal_(BCD2Int(pValue,3)/100.0);
        pPoint->SetValue(pPoint,VarProject_,VarOriginal_);
//		pTag->SetIntValue(BCD2Int(pValue,3)/100.0);
    }
    else//二进制码
    {
        QVariant VarProject_(pValue->VALUE);
        QVariant VarOriginal_(pValue->VALUE);
        pPoint->SetValue(pPoint,VarProject_,VarOriginal_);
//		pTag->SetIntValue(pValue->VALUE);
    }
}

/*!
 \brief 把BCD码表示的数转换成整数

 \fn CProtocolCDT::BCD2Int
 \param pInfo 内存地址(紧凑形式)
 \param nSize 字节个数
 \return unsigned int 返回实际结果
*/
unsigned int  CProtocolCDT::BCD2Int(const void *pInfo, size_t nSize)
{
    Q_ASSERT(pInfo != NULL);
    if(pInfo == NULL)
        return 0;
    Q_ASSERT(nSize < 6);
    Q_ASSERT(nSize > 0);//整数最大值为10位数

    unsigned int nTotal = 0;
    unsigned int nBase = 1;
    BYTE *pData = (BYTE *)pInfo;
    AnyType byte;
    //for(size_t i=0;i<nSize;i++)
    while (nSize--)
    {
        byte.m_nByte = *pData++;
        //	nTotal=nTotal*100+byte.m_nHigh*10+byte.m_nLow;  //错误:高低字节不正确
        //	nTotal=nTotal+(byte.m_nHigh*10+byte.m_nLow)*pow(10,i*2);//使用pow不好
        nTotal = nTotal + (byte.HIGH4 * 10+byte.LOW4) * nBase;
        nBase *= 100;
    }
    return nTotal;
}
/*!
 \brief 水位信息  WaterLevel

 \fn CProtocolCDT::ProcessWLInfoWord
 \param frameInfoWord 信息字
*/
void CProtocolCDT::ProcessWLInfoWord(CBaseFrame &/*frameInfoWord*/, int /*n_SrcStationAddress_*/)
{
//    BYTE nFunctionCode = frameInfoWord[0];//功能码
//    int nPoint = nFunctionCode - 0X8D;
//	IPoint * pTag =  FindPoint(0X8D,nPoint);
//	if(pTag == NULL)
//		return ;
//	pTag->SetIntValue(frameInfoWord.GetBcd(1,3)/100.0);
}

/*!
 \brief 获取SOE信息

 \fn CProtocolCDT::ProcessSoeInfoWord
 \param frameInfoWord
*/
void CProtocolCDT::ProcessSoeInfoWord(CBaseFrame &/*frameInfoWord*/, int /*n_SrcStationAddress_*/)
{
//        FRAME_SOE_TYPE *pValue;
//        pValue = (FRAME_SOE_TYPE *)frameInfoWord.GetBuffer(1);
}

/*!
 \brief 子站时钟返回

 \fn CProtocolCDT::ProcessTimeBackInfoWord
 \param frameInfoWord
*/
void CProtocolCDT::ProcessTimeBackInfoWord(CBaseFrame &frameInfoWord, int /*n_SrcStationAddress_*/)
{
    //子站时钟
    m_TS2 = frameInfoWord[3]+frameInfoWord[4]*60;//只校准秒
    //等待时间
    m_T0  = frameInfoWord[9]+frameInfoWord[10]*60;//只校准秒
}

/*!
 \brief 获取子站状态信息(变量字典中应该rtu状态的内部变量) SubSTStatus

 \fn CProtocolCDT::ProcessSTInfoWord
 \param frameInfoWord
*/
void CProtocolCDT::ProcessSTInfoWord(CBaseFrame &/*frameInfoWord*/, int /*n_SrcStationAddress_*/)
{
//    BYTE nFunctionCode = frameInfoWord[0];//功能码
//    for(int i=0; i<32; i++)
//    {
//        int nPoint = (nFunctionCode-0XEC)*32+i;
//        IPoint  * pTag = FindPoint(0XEC,nPoint);
//        pTag->SetIntValue(frameInfoWord.GetBit(1,i));
//    }
}

/*!
 \brief 遥控反校 遥调返校  RemoteCtrlReturn

 \fn CProtocolCDT::ProcessRRInfoWord
 \param frameInfoWord
*/
void CProtocolCDT::ProcessRRInfoWord(CBaseFrame &/*frameInfoWord*/ ,int /*n_SrcStationAddress_*/)
{
    //判断是否是二进制表示对象号
//    BYTE nFunctionCode = frameInfoWord[0];//功能码
//    if(m_pTagControl == NULL)
//        return;
//    if(frameInfoWord[1] == 0xff)
//    {
//        m_pTagControl->SetResult(false);
//    }
//    else
//    {
//        m_pTagControl->SetResult(true);
//    }
}

/*!
 \brief 获取频率量

 \fn CProtocolCDT::ProcessFrequencyInfoWord
 \param frameInfoWord
*/
void CProtocolCDT::ProcessFrequencyInfoWord(CBaseFrame &/*frameInfoWord*/, int /*n_SrcStationAddress_*/)
{
//    BYTE nFunctionCode=frameInfoWord[0];//功能码
//    for(int i=0; i<2; i++)
//    {
//        int nPoint = (nFunctionCode - 0X8A)*2+i;
//		IPoint *pTag=FindPoint(0X8A, nPoint);
//		if(pTag == NULL)
//			continue;
//		pTag->SetIntValue(frameInfoWord.GetBcd(1+i*2,2)/100.0);
//    }
}
//IPoint * CProtocolCDT::FindPoint( int _nStart,int _nPoint )
//{
//    //查找指定的点
//    for(int i=0;i<GetPointCount();i++)
//    {
//        IPoint *pPoint = GetPoint(i);
//        int *pParam = (int *)pPoint->GetPara();
//        int nStart= *pParam++;
//        int nPoint = *pParam++;
//        if(nStart == _nStart && nPoint == _nPoint)
//            return pPoint;
//    }
//    return NULL;
//}
void CProtocolCDT::slotConnect(bool /*isConnected_*/)
{

}
