/*!
 * \file    cprotocoliec_104.cpp
 * \brief   概述 CProtocolIEC_104类的源文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/26
 *
 * \b       修改记录：
 * \li      2015/5/26
 *  添加注释
 */

#include <QTimer>
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QDateTime>
//#include <windef.h>
#include <QDir>

#include "../PortInclude/CRTDBI.h"
#include "../PortInclude/CTagI.h"
#include "../PortInclude/cvaluebase.h"
#include "cprotocoliec_104.h"
#include "cpointiec_104.h"

/*!
 * \brief 功能概述 库文件导出接口函数
 * \param 参数描述 parent是QObject对象指针
 * \return 返回值描述 返回创建的驱动指针
 * \author zzy
 * \date 2015/5/26
 */
CProtocolI * CreateDriver(QObject *parent)
{
    CProtocolIEC_104 *testProtocol = new CProtocolIEC_104(parent);
    return  testProtocol;
}

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 parent父对象的指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
CProtocolIEC_104::CProtocolIEC_104(QObject */*parent*/)
{
    m_bTimerIsStart = false;
    m_nLinkCount = 0;///<zzy 2015/1/14 修改:

    m_bConnectOk = false;
    m_nReciveIdx = 0;
    m_nSendIdx = 0;

    ///默认值
    m_nK = 12;//可以不确认的最大i帧数目 发送方未被确认的I格式的APDU数目 K=12  m_nMaxUnconfirmIFrame
    m_nW = 8;//接收方最多收到未被确认的I格式的APDU数目 W=8
    m_nt0 = 30;//网络建立连接超时时间 t0<30s
    m_nt1 = 20;//发送或测试APDU的超时时间 t1<20s
    m_nt2 = 15;//接收方无数据报文时确认的超时时间 t2<15s
    m_nt3 = 5;//通道长期空闲时发送确认帧的超时时间 t3<25s  t3<5  10秒钟发送一帧测试报文

    m_nMaxSRValue=32767;//收发最大值
    m_bDataLink = false;
    m_nUnconfirmIFrame = 0;
}

/*!
 * \brief  功能概述 创建此通道
 * \param  参数描述 strChannelFileName_是通道文件绝对路径名，pDatabaseReturn是实时库指针
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CProtocolIEC_104::OnCreateChannel(const QString strChannelFileName_, CRTDBI *pDatabaseReturn)
{
    return true;
}

bool CProtocolIEC_104::OnCreateChannel(const QDomElement &ChannelElement_, ChannelType nChannelType_, int nComOrCanNumber_, CRTDBI *pDatabaseReturn_)
{
    SetPROTOCOL_TYPE(Collect_Type);
    if (!CProtocolBase::OnCreateChannel(ChannelElement_,nChannelType_,nComOrCanNumber_,pDatabaseReturn_))
        return false;

    if (ChannelElement_.elementsByTagName("Protocol").isEmpty())
        return false;
    if (ChannelElement_.elementsByTagName("DeviceList").isEmpty())
        return false;
    QDomElement Protocol   = ChannelElement_.elementsByTagName("Protocol").at(0).toElement();
    QDomElement DeviceList = ChannelElement_.elementsByTagName("DeviceList").at(0).toElement();


    g_param.strTransmitCauseLen = Protocol.attribute("TransmitCauseLen"); //传送原因地址长度
    g_param.strCommAddrLen = Protocol.attribute("CommAddrLen"); //公共地址长度
    g_param.strInfoAddrLen = Protocol.attribute("InfoAddrLen"); //信息体地址长度
    g_param.strTimeType = Protocol.attribute("TimeType");    //时标格式
    g_param.strTimingType = Protocol.attribute("TimingType");  //校时方式
    g_param.strCallingType = Protocol.attribute("CallingType"); //总召唤方式
//    g_param.strLinkTimeOut = Protocol.attribute("LinkTimeOut");///<zzy 2015/1/14 修改: //连接超时
    g_param.strLinkTimeOut = Protocol.attribute("t0");///<zzy 2015/1/14 修改: //连接超时
//    g_param.strSendFrameTimeOut = Protocol.attribute("SendFrameTimeOut");///<zzy 2015/1/14 修改: //发送或测试超时
    g_param.strSendFrameTimeOut = Protocol.attribute("t1"); //发送或测试超时
//    g_param.strNonDataTimeOut = Protocol.attribute("NonDataTimeOut");///<zzy 2015/1/14 修改:  //无数据超时
    g_param.strNonDataTimeOut = Protocol.attribute("t2");///<zzy 2015/1/14 修改:  //无数据超时
//    g_param.strSendSFrameTimeOut = Protocol.attribute("SendSFrameTimeOut");///<zzy 2015/1/14 修改: //S帧发送超时
    g_param.strSendSFrameTimeOut = Protocol.attribute("t3");///<zzy 2015/1/14 修改: //S帧发送超时
//    g_param.strMaxFrameInx = Protocol.attribute("MaxFrameInx");///<zzy 2015/1/14 修改: //最大不同接收序号
    g_param.strMaxFrameInx = Protocol.attribute("K");///<zzy 2015/1/14 修改: //最大不同接收序号
//    g_param.strSFrameConfirm = Protocol.attribute("SFrameConfirm");///<zzy 2015/1/14 修改: //最大S帧确认数
    g_param.strSFrameConfirm = Protocol.attribute("W");///<zzy 2015/1/14 修改: //最大S帧确认数
    g_param.strASDULen = Protocol.attribute("ASDULen"); //ASDU数据单元长度
    g_param.strKwhCjType = Protocol.attribute("KwhCjType"); //累积量采集方式

    CGI_InitPointMap(DeviceList);
    init104Param();
    return true;
}

/*!
 * \brief 功能概述 将点插入到相应的map中
 * \param 参数描述 PointElem_是文件中点的节点，nPointType是点的类型，nDeviceAddress是设备地址
 * \return 返回值描述 无
 * \author zzy
 * \date 2015/5/26
 */
void CProtocolIEC_104::InsertPoint(const QDomElement &PointElem_, int nPointType,int nDeviceAddress)
{
    CPointIEC_104 *pPoint = new CPointIEC_104;
    pPoint->m_strPointName = PointElem_.attribute("Name");
    pPoint->m_strPointComment = PointElem_.attribute("Desc");
    pPoint->m_nID = PointElem_.attribute("SN").toInt();
    pPoint->m_strRTDBName = PointElem_.attribute("Link");
    pPoint->m_nScanTime = PointElem_.attribute("ScanTime").toInt();
    pPoint->m_fKValue = PointElem_.attribute("KValue").toFloat();
    if (pPoint->m_fKValue == 0)
    {
        pPoint->m_fKValue = 1;
    }
    pPoint->m_fBaseValue = PointElem_.attribute("BaseValue").toFloat();
    pPoint->m_pProtocol = this;
    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
    pPoint->m_nDeviceAddr = nDeviceAddress;
    if (pPoint->m_pTag)
    {
        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
    }
    int nPointNumber = PointElem_.attribute("PointNumber").toInt();
    pPoint->m_nPointNumber = nPointNumber;
    pPoint->m_nPublicAddress = PointElem_.attribute("PublicAddress").toInt();
    pPoint->m_nTagType = nPointType;
qDebug()<<pPoint->m_strPointName<<nPointNumber;
    switch (nPointType) {
    case TAG_TYPE_DI:///遥信
        YX_PointMap.insert(nPointNumber,pPoint);
        break;
    case TAG_TYPE_AI:///遥测
        YC_PointMap.insert(nPointNumber,pPoint);
        break;
    case TAG_TYPE_MEM:///遥脉
        YM_PointMap.insert(nPointNumber,pPoint);
        break;
    case TAG_TYPE_DO:///遥控
        YK_PointMap.insert(nPointNumber,pPoint);
        break;
    case TAG_TYPE_AO:///遥设
        YS_PointMap.insert(nPointNumber,pPoint);
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}

void CProtocolIEC_104::CGI_InsertPoint(const QDomElement &PointElem_, int nPointType, int nDeviceAddress)
{
    CPointIEC_104 *pPoint = new CPointIEC_104;
    pPoint->m_strPointName = PointElem_.attribute("Name");
    pPoint->m_strPointComment = PointElem_.attribute("Desc");
    pPoint->m_nID = PointElem_.attribute("SN").toInt();
    pPoint->m_strRTDBName = PointElem_.attribute("Link");
    pPoint->m_nScanTime = PointElem_.attribute("ScanTime").toInt();
    pPoint->m_fKValue = PointElem_.attribute("KValue").toFloat();
    if (pPoint->m_fKValue == 0)
    {
        pPoint->m_fKValue = 1;
    }
    pPoint->m_fBaseValue = PointElem_.attribute("BaseValue").toFloat();
    pPoint->m_pProtocol = this;
    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
    pPoint->m_nDeviceAddr = nDeviceAddress;
    if (pPoint->m_pTag)
    {
        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
    }
    int nPointNumber = PointElem_.attribute("PointNumber").toInt();
    pPoint->m_nPointNumber = nPointNumber;
    pPoint->m_nPublicAddress = PointElem_.attribute("PublicAddress").toInt();
    pPoint->m_nTagType = nPointType;
qDebug()<<pPoint->m_strPointName<<nPointNumber;
    switch (nPointType) {
    case TAG_TYPE_DI:///遥信
        YX_PointMap.insert(nPointNumber,pPoint);
        break;
    case TAG_TYPE_AI:///遥测
        YC_PointMap.insert(nPointNumber,pPoint);
        break;
    case TAG_TYPE_MEM:///遥脉
        YM_PointMap.insert(nPointNumber,pPoint);
        break;
    case TAG_TYPE_DO:///遥控
        YK_PointMap.insert(nPointNumber,pPoint);
        break;
    case TAG_TYPE_AO:///遥设
        YS_PointMap.insert(nPointNumber,pPoint);
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}
/*!
 * \brief  功能概述 按照点的类型插入到相应的map中
 * \param  参数描述 DeviceListElement_是设备列表节点<DeviceList>
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
/*!
 \brief CGI 使用

 \fn CProtocolIEC_104::CGI_InitPointMap
 \param DeviceListElement_
*/
void CProtocolIEC_104::CGI_InitPointMap(const QDomElement &DeviceListElement_)
{
    //初始化点表Map
    QDomNodeList DeviceList = DeviceListElement_.childNodes();
    for (int i = 0; i <DeviceList.count(); ++i)
    {
        QDomElement DeviceElement = DeviceList.at(i).toElement();

//        int DeviceAddress = DeviceElement.attribute("Address").toInt();///<zzy 2015/1/14 修改:
        if (DeviceElement.elementsByTagName("Attribute").count() < 1)
        {
            Q_ASSERT(false);
        }
        int nDeviceAddress = DeviceElement.elementsByTagName("Attribute").at(0).toElement().attribute("Device_Address").toInt();
        m_nCommonAddress = nDeviceAddress;
        QDomNodeList TypeNodeList = DeviceElement.elementsByTagName("Type");
        for (int nType = 0; nType < TypeNodeList.count(); ++nType)
        {
            QDomElement TypeElem = TypeNodeList.at(nType).toElement();
            if (TypeElem.attribute("Name") == "YX"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_DI,nDeviceAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointIEC_104 *pPoint = new CPointIEC_104;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YX_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YX_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YX_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YX_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YX_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YX_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YX_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    pPoint->m_nDeviceAddr = nDeviceAddress;
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }
                    int nPointNumber = PointElem_.attribute("Device_YX_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YX_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_DI;
                    QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
                    qDebug()<<debug<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    if (!YX_PointMap.contains(nPointNumber))
                    {
                        YX_PointMap.insert(nPointNumber,pPoint);
                    }
                }
            }else if (TypeElem.attribute("Name") == "YC"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_AI,nDeviceAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointIEC_104 *pPoint = new CPointIEC_104;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YC_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YC_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YC_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YC_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YC_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YC_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YC_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    pPoint->m_nDeviceAddr = nDeviceAddress;
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }
                    int nPointNumber = PointElem_.attribute("Device_YC_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YC_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_AI;
                    QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
                    qDebug()<<debug<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    if (!YC_PointMap.contains(nPointNumber))
                    {
                        YC_PointMap.insert(nPointNumber,pPoint);
                    }
                }
            }else if (TypeElem.attribute("Name") == "YM"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_MEM,nDeviceAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointIEC_104 *pPoint = new CPointIEC_104;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YM_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YM_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YM_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YM_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YM_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YM_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YM_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    pPoint->m_nDeviceAddr = nDeviceAddress;
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }
                    int nPointNumber = PointElem_.attribute("Device_YM_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YM_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_MEM;
                    QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
                    qDebug()<<debug<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    if (!YM_PointMap.contains(nPointNumber))
                    {
                        YM_PointMap.insert(nPointNumber,pPoint);
                    }
                }
            }else if (TypeElem.attribute("Name") == "YK"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_DO,nDeviceAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointIEC_104 *pPoint = new CPointIEC_104;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YK_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YK_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YK_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YK_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YK_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YK_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YK_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    pPoint->m_nDeviceAddr = nDeviceAddress;
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }
                    int nPointNumber = PointElem_.attribute("Device_YK_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YK_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_DO;
                    QString str104Type = PointElem_.attribute("Device_YK_104type_Strings");
                    if (str104Type == "单位遥控")
                    {
                        pPoint->m_nYK_Type = C_SC_NA_1;
                    }else if (str104Type == "双位遥控")
                    {
                        pPoint->m_nYK_Type = C_DC_NA_1;
                    }else if (str104Type == "档位调节")
                    {
                        pPoint->m_nYK_Type = C_RC_NA_1;
                    }else
                    {
                        pPoint->m_nYK_Type = C_SC_NA_1;
                    }

                    pPoint->m_nOut_Type = PointElem_.attribute("Device_YK_104Out_type_Strings").toUInt();
                    if (pPoint->m_nOut_Type > 3)
                    {
                        pPoint->m_nOut_Type = 0;
                    }

                    qDebug()<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    if (!YK_PointMap.contains(nPointNumber))
                    {
                        YK_PointMap.insert(nPointNumber,pPoint);
                    }
                }
            }else if (TypeElem.attribute("Name") == "YS"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_AO,nDeviceAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointIEC_104 *pPoint = new CPointIEC_104;
                    pPoint->m_strPointName = PointElem_.attribute("Device_YS_Name_Strings");
                    pPoint->m_strPointComment = PointElem_.attribute("Device_YS_Desc_Strings");
                    pPoint->m_nID = PointElem_.attribute("Device_YS_SN_Strings").toInt();
                    pPoint->m_strRTDBName = PointElem_.attribute("Device_YS_Link_Strings");
                    pPoint->m_nScanTime = PointElem_.attribute("Device_YS_ScanTime_Strings").toInt();
                    pPoint->m_fKValue = PointElem_.attribute("Device_YS_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = PointElem_.attribute("Device_YS_BaseValue_Strings").toFloat();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    pPoint->m_nDeviceAddr = nDeviceAddress;
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }
                    int nPointNumber = PointElem_.attribute("Device_YS_RegisterAddress_Strings").toInt();
                    pPoint->m_nPointNumber = nPointNumber;
                    pPoint->m_nPublicAddress = PointElem_.attribute("Device_YS_PublicAddress_Strings").toInt();
                    pPoint->m_nTagType = TAG_TYPE_AO;

                    QString str104Type = PointElem_.attribute("Device_YS_104type_Strings");
                    if (str104Type == "归一化值")
                    {
                        pPoint->m_nYS_Type = C_SE_NA_1;
                    }else if (str104Type == "标度化值")
                    {
                        pPoint->m_nYS_Type = C_SE_NB_1;
                    }else if (str104Type == "短浮点值")
                    {
                        pPoint->m_nYS_Type = C_SE_NC_1;
                    }else if (str104Type == "多个设定值")
                    {
                        pPoint->m_nYS_Type = C_SE_ND_1;
                    }else
                    {
                        pPoint->m_nYS_Type = C_SE_NC_1;
                    }

                    qDebug()<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<nPointNumber;
                    if (!YS_PointMap.contains(nPointNumber))
                    {
                        YS_PointMap.insert(nPointNumber,pPoint);
                    }
                }
            }else{
                Q_ASSERT(false);
            }
        }
    }
}

/*!
 * \brief  功能概述 用于响应点的值发生变化的函数
 * \param  参数描述 pPointBase_是点的指针，VarSet_是点的值
 * \return 返回值描述 返回true
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolIEC_104::OnTagChange(CPointBase *pPointBase_, QVariant VarSet_)
{
    /// 转发遥控、遥设
    /// 生成遥控、遥设报文
    /// 转发驱动中不做处理
    switch (pPointBase_->m_nTagType) {
    case TAG_TYPE_AI://遥测
        break;
    case TAG_TYPE_AO://遥设
        emit signal_YS_Change(pPointBase_,VarSet_);
        break;
    case TAG_TYPE_DI://遥信
        break;
    case TAG_TYPE_DO://遥控
        emit signal_YK_Change(pPointBase_,VarSet_);
        break;
    case TAG_TYPE_MEM:

        break;
    default:
        break;
    }
    return true;
}

#if 0
/*!
 * \brief  功能概述 响应readyRead(QByteArray)信号，解析收到的报文函数
 * \param  参数描述 data是收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessRespond(QByteArray data)
{
    QByteArray ReceiveData = data;
    if (ReceiveData.isEmpty())
        return;
    if ((BYTE)ReceiveData.at(0) == 0x68)
    {
        while(!ReceiveData.isEmpty())
        {
            int nReceiveDataCount = ReceiveData.count();
            if (nReceiveDataCount < 6)
                return;
            int nReadFrameDateLen = (BYTE)ReceiveData.at(1);
            if (nReadFrameDateLen > nReceiveDataCount)
                return;
            QByteArray array = ReceiveData.left(nReadFrameDateLen+2);
            ReceiveData.remove(0,nReadFrameDateLen+2);
            DecodeFrame(array);
        }
    }
}
#else

/*!
 * \brief  功能概述 响应readyRead(QByteArray)信号，解析收到的报文函数
 * \param  参数描述 data是收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessRespond(QByteArray data)
{
    QByteArray ReceiveData = data;
    if (ReceiveData.isEmpty())
        return;
    m_bufferReceive.AppendData(data.data(),data.count());
    QMutexLocker locker(&m_mutexProcessData);
    for(int nStart=0;nStart<m_bufferReceive.GetInfoSize()-5;nStart++)
    {
        int nFrameLength = FindATrueFrame(m_bufferReceive.GetBuffer(nStart),m_bufferReceive.GetInfoSize()-nStart);

        if(nFrameLength>0)
        {
            m_IEC104Response.SetData((char *)m_bufferReceive.GetBuffer(nStart),nFrameLength);
            if (nStart == 0)
            {

            }else
            {
                ShowMessage(SHOWMESSAGE_TYPE_DEBUG,QByteArray((char *)m_bufferReceive.GetBuffer(),nStart).toHex().toUpper());
//                qDebug()<<"Error Frame:"<<QByteArray((char *)m_bufferReceive.GetBuffer(),nStart).toHex().toUpper();
            }
            m_bufferReceive.Fifo(nStart+nFrameLength);
            DecodeFrame(m_IEC104Response);
//            bReadFrame = true;
            nStart=-1;//重新再来
        }
    }
    if(m_bufferReceive.GetInfoSize()>MAX_FRAME)
    {
        qDebug()<<"Error Frame:"<<QByteArray((char *)m_bufferReceive.GetBuffer(),m_bufferReceive.GetInfoSize()).toHex().toUpper();
        ShowMessage(SHOWMESSAGE_TYPE_DEBUG,QByteArray((char *)m_bufferReceive.GetBuffer(),m_bufferReceive.GetInfoSize()).toHex().toUpper());
        m_bufferReceive.SetInfoSize(0);
    }

    ////////////////////////////////////
//    if ((BYTE)ReceiveData.at(0) == 0x68)
//    {
//        while(!ReceiveData.isEmpty())
//        {
//            int nReceiveDataCount = ReceiveData.count();
//            if (nReceiveDataCount < 6)
//                return;
//            int nReadFrameDateLen = (BYTE)ReceiveData.at(1);
//            if (nReadFrameDateLen > nReceiveDataCount)
//                return;
//            QByteArray array = ReceiveData.left(nReadFrameDateLen+2);
//            ReceiveData.remove(0,nReadFrameDateLen+2);
//            DecodeFrame(array);
//        }
//    }
}
#endif

/*!
 \brief 在接收数据缓冲区中找一帧正确的报文

 \fn ClientSocket::FindATrueFrame
 \param pInfo 开始地址
 \param nInfoLength 长度
 \return int 如果没有正确的帧则返回0，若有正确帧则返回正确帧的长度
*/
int CProtocolIEC_104::FindATrueFrame(BYTE *pInfo,int nInfoLength)
{
    if(nInfoLength<6)
        return 0;
    int nTrueFrameLength = 0;
    if (pInfo[0] != 0x68)
        return 0;
    int nFrameLength = pInfo[1];
    if (nFrameLength+2 > nInfoLength)
        return 0;
    nTrueFrameLength = nFrameLength + 2;
    return nTrueFrameLength;
}

/*!
 * \brief 功能概述 连接成功
 * \param 参数描述 isConnected_是连接标志
 * \return 返回值描述 无
 * \author zzy
 * \date 2015/5/26
 */
void CProtocolIEC_104::slotConnect(bool isConnected_)
{
    qDebug()<<"is connected"<<isConnected_;
    if (isConnected_)
    {
        InitIEC104();///成功连接时初始化104标志位
        m_bConnectOk = true;///
        qDebug()<<"连接成功";
        m_bTimerIsStart = true;
        m_nProtcolTimerId = startTimer(10);
    }else
    {
        m_bConnectOk = false;///
        if (m_bTimerIsStart)
        {
            killTimer(m_nProtcolTimerId);
            m_bTimerIsStart = false;
        }
    }
}

/*!
 \brief YS处理函数

 \fn CProtocolIEC_104::slot_YS_Change
 \param pPointBase_ 改变的点
 \param VarSet_ 改变的值
*/
void CProtocolIEC_104::slot_YS_Change(CPointBase *pPointBase_, QVariant VarSet_)
{
    if (pPointBase_->m_nTagType == TAG_TYPE_AO)
    {
        CPointIEC_104 *pPoint = (CPointIEC_104*)pPointBase_;
        switch (pPoint->m_nYS_Type) {
        case C_SE_NA_1:
        case C_SE_NB_1:
        case C_SE_NC_1:
        case C_SE_ND_1:
            m_IEC104Query.MakeQuery_I_YS(m_nSendIdx,m_nReciveIdx,m_nCommonAddress,pPoint->m_nYS_Type,pPoint->m_nPointNumber,VarSet_);
            SendFrame(m_IEC104Query);
            break;
        default:
            break;
        }
    }else
    {

    }
}

/*!
 \brief YK处理函数

 \fn CProtocolIEC_104::slot_YK_Change
 \param pPointBase_ aaaaaa
 \param VarSet_ aaaaaaaa
*/
void CProtocolIEC_104::slot_YK_Change(CPointBase *pPointBase_, QVariant VarSet_)
{

}

/*!
 * \brief  功能概述 解包函数
 * \param  参数描述 ReceiveData是接收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
//void CProtocolIEC_104::DecodeFrame(QByteArray &ReceiveData)
//{
//    m_nLinkCount = 0;///<zzy 2015/1/14 修改:
//    ShowMessage(SHOWMESSAGE_TYPE_RECEIVE,ReceiveData.toHex());
//    if (ReceiveData.count() < 6)
//        return;
//    n_mReadFrameDateLen = (BYTE)ReceiveData.at(1);
//    if ( (n_mReadFrameDateLen<4) ||(n_mReadFrameDateLen > 253))
//        return;
//    if((n_mReadFrameDateLen+2) != ReceiveData.count())
//        return ; //读取到的实际长度比帧内容不符合

//    m_IEC104Response.SetData(ReceiveData);
//    int nFrameType = m_IEC104Response.GetFrameType();
//    switch(nFrameType)
//    {
//    case IEC104_I_TYPE:
//        DecodeIFrame(m_IEC104Response);
//        break;
//    case IEC104_S_TYPE:
//        DecodeSFrame(m_IEC104Response);
//        break;
//    case IEC104_U_TYPE:
//        DecodeUFrame(m_IEC104Response);
//        break;
//    default:
//        break;
//    }
//}

void CProtocolIEC_104::DecodeFrame(CIEC104Response_new &response)
{
    m_nLinkCount = 0;///<zzy 2015/1/14 修改:
    ShowMessage(SHOWMESSAGE_TYPE_RECEIVE,QByteArray((char *)response.GetBuffer(),response.GetInfoSize()).toHex().toUpper());

//    response.Src_SetData(ReceiveData);
    int nFrameType = response.GetFrameType();
    switch(nFrameType)
    {
    case IEC104_I_TYPE:
        DecodeIFrame(response);
        break;
    case IEC104_S_TYPE:
        DecodeSFrame(response);
        break;
    case IEC104_U_TYPE:
        DecodeUFrame(response);
        break;
    default:
        break;
    }
}

/*!
 * \brief  功能概述 时间事件函数
 * \param  参数描述 event是时间事件
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nProtcolTimerId)
    {
        if (!m_bConnectOk)// 是否成功连接服务器 连接未成功前不走规约
            return;
//        if (!m_bDataLink)//协议链路
//        {
//            ConnectLink();///发送启动U帧 连接链路
//            return;
//        }
        Protocal_TX();
    }
}

/*!
 * \brief  功能概述 发送处理
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::Protocal_TX()
{//发送处理
//    if(n_mLastCallDataTime > 0)
//    {
//        if(ABSDEC(GetSysAbsTime(),n_mLastCallDataTime) > 30)
//            n_mLastCallDataTime = 0;
//    }
    // 最后的I帧一直未被确认，初始化链路
    if(n_mLastSendIFrameTime > 0)
    {
       if(ABS(GetSysAbsTime() - n_mLastSendIFrameTime) > m_nt1)
       {
           n_mCurrState = IEC104_RESET;
           n_mLastSendIFrameTime = 0;
       }
    }

    //超过T2没有收到I帧数据即发送S帧确认
    if(n_mLastReciveIFrameTime>0)
    {
        if(ABS(GetSysAbsTime() - n_mLastReciveIFrameTime) > m_nt2)
        {
            m_nAck = 1; //发送确认
            n_mLastReciveIFrameTime = 0;
        }
    }

    int nStopSendIFrame = 0;
    int nDifference =(MAX_RXTXSEQNUM + m_nSendIdx -n_mSub_ReciveIdx)% MAX_RXTXSEQNUM;
    if(nDifference >= m_nK)  //发送最多 m_nK 个 I帧，但未收到确认。
    {
        if(n_mSendIFrameFullTime <= 0)
        {
            n_mSendIFrameFullTime = GetSysAbsTime();
        }

        nStopSendIFrame = 1;
        if(ABSDEC(GetSysAbsTime(),n_mSendIFrameFullTime) > m_nt1)
        {
            n_mSendIFrameFullTime = 0;
            n_mCurrState = IEC104_RESET;
        }
    }
    else
    {
        n_mSendIFrameFullTime = 0;
    }
    //发送总召数据
    if(n_mCurrState == IEC104_IDLE)
    {
        if(!nStopSendIFrame)
        {
            //总召唤模式
            if(n_mCallDataMode == 1) //启动或重连时总召唤
            {
//                qDebug()<<"===========================================================";
                if(n_mCallAllDataFlag == 1)
                {
                    if(ProtCallAllData())
                    {
                       n_mLastCallDataTime = 0;
                    }
                    n_mCallAllDataFlag = 0;
                }
            }
            else //周期总召
            {
                if(ABS(n_mLastCallDataTime - GetSysAbsTime()) >= n_mCallDataCycle)
                {
//                    if(n_mCallAllDataFlag == 1)
//                    {
                        if(ProtCallAllData())
                        {
//                            qDebug()<<"11111111111111-------------------"<<n_mLastCallDataTime<<" "<<n_mCallDataCycle<<QTime::currentTime().toString("hh:mm:ss.zzz");
                           n_mLastCallDataTime = GetSysAbsTime();
                        }
//                        n_mCallAllDataFlag = 0;
//                    }
                }
            }
        }
    }
    //总召电度量
    if(n_mCurrState == IEC104_IDLE)
    {
        if(!nStopSendIFrame)
        {
            //总召唤模式
            if(n_mCallDataMode == 1) //启动或重连时
            {
                if(n_mCallKwhDataFalg == 1)
                {
                    if(ProtCallKwhData())
                    {
                       n_mLastCallDataTime = 0;
                    }
                    n_mCallKwhDataFalg = 0;
                }
            }
            else //周期总召
            {
                if(ABS(n_mLastCallDataTime - GetSysAbsTime()) >= n_mCallDataCycle)
                {
                    if(n_mCallKwhDataFalg == 1)
                    {
                        if(ProtCallKwhData())
                        {
                           n_mLastCallDataTime = GetSysAbsTime();
                        }
                        n_mCallKwhDataFalg = 0;
                    }
                }
            }
        }
    }

    CheckProcState();

    //收到后发送S帧确认
    if(m_nAck == 1)
    {
        SendOneSFrame();
    }
    return ;
}

/*!
 * \brief  功能概述 判断接收超时
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::receiveTimeOut()
{
    //判断接收超时
    // U帧超时判断
    if((GetSysAbsTime() - n_mLastTestFrameTime) > m_nt3)
    {
        if(n_mCurrState == IEC104_IDLE)
        {
            n_mLastTestFrameTime = GetSysAbsTime();
            n_mCurrState = IEC104_TESTLINK;
        }
    }

    //链路超时判断
    if((GetSysAbsTime() - n_mLastFrameTime) > m_nt0)
    {
        n_mCurrState = IEC104_RESET;
        n_mLastFrameTime = GetSysAbsTime();
    }
}

/*!
 * \brief  功能概述 发送总召唤
 * \param  参数描述 无
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolIEC_104::ProtCallAllData()
{
    if (m_IEC104Query.MakeQuery_IntrogenCall(m_nSendIdx,m_nReciveIdx,m_nCommonAddress))
    {
        SendFrame(m_IEC104Query);
        n_mCurrState = IEC104_WAITALLDATACONF;
        n_mLastTxTime = GetSysAbsTime();
        return true;
    }
    return false;
}

/*!
 * \brief  功能概述 发送点度量召唤
 * \param  参数描述 无
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolIEC_104::ProtCallKwhData()
{
    if(m_IEC104Query.MakeQuery_CallPlus(m_nSendIdx,m_nReciveIdx,m_nCommonAddress))
    {
        n_mCurrState = IEC104_WAITALLKWHCONF;
        n_mLastTxTime = GetSysAbsTime();
        return true;
    }
    return false;
}

/*!
 * \brief  功能概述 发送S帧
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::SendOneSFrame()
{
    if (m_IEC104Query.MakeQuery_S(m_nReciveIdx))
    {
        SendFrame(m_IEC104Query);
        m_nAck = 0;
        return;
    }
    return ;
}

/*!
 * \brief  功能概述 IEC-104通讯流程判断
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::CheckProcState()
{
    if(n_mCurrState == IEC104_RESET)
    {
        n_mCurrState = IEC104_STOPDT;
        n_mResetColseLinkFlag = 1;
    }

    switch (n_mCurrState) {
    case IEC104_RESETPROCESS: //复位进程
        n_mCurrState = IEC104_STOPDT;
        n_mResetColseLinkFlag = 1;
        break;
    case IEC104_STOPDT:
        init104Param();
        //发送U停止帧
        if (m_IEC104Query.MakeQuery_U_Command(false,true,false))
        {
            qDebug()<<"发送U停止帧";
            SendFrame(m_IEC104Query);
            n_mCurrState = IEC104_WAITSTOPCONF;
            n_mLastTxTime = GetSysAbsTime();
        }
        break;
    case IEC104_STARTDT:
        //init104Param();
        //发送U激活帧
        if (m_nLinkCount > 3)
        {
            m_nLinkCount = 0;
            if (m_pLink != NULL)
            {
                m_pLink->SetCloseLink();
            }
            qDebug()<<"主动断开链接";
        }
        if (m_IEC104Query.MakeQuery_U_Command(true,false,false))
        {
            m_nLinkCount++;
            SendFrame(m_IEC104Query);
            n_mCurrState = IEC104_WAITSTARTCONF;
            n_mLastTxTime = GetSysAbsTime();
        }
        n_mSyncTimeFlag = 1;
        n_mCallAllDataFlag = 1;
        n_mCallKwhDataFalg = 1;
        break;
    case IEC104_TESTLINK:
        if (m_IEC104Query.MakeQuery_U_Command(false,false,true))
        {
            SendFrame(m_IEC104Query);
            n_mCurrState = IEC104_WAITTESTLINKCONF;
            n_mLastTxTime = GetSysAbsTime();
        }
        break;
    case IEC104_TESTCON:
        if (m_IEC104Query.MakeQuery_U_Confirm(false,false,true))
        {

        }
        n_mCurrState = IEC104_IDLE;
        break;
    default:
        break;
    }

    if(ABS(GetSysAbsTime() - n_mLastTxTime) > 6)
    {
        switch(n_mCurrState)
        {
        case IEC104_IDLE:
            //MG ADD exter
            n_mCurrState = IEC104_TESTLINK; //2014-07-19
            break;
        case IEC104_RESET:
                break;
        case IEC104_WAITRSTPROCCONF:
            n_mResetColseLinkFlag = 1;
            n_mCurrState= IEC104_STOPDT;
            break;
        case IEC104_WAITSTARTCONF:
            n_mCurrState = IEC104_STARTDT;
            break;
        case IEC104_WAITSTOPCONF:
            n_mCurrState = IEC104_STARTDT;
            break;
        case IEC104_WAITALLDATACONF:   //2
        case IEC104_WAITALLDATA:       //5
            n_mCurrState = IEC104_IDLE;
            //m_AllDataFlag = 1;
            break;
        case IEC104_WAITTIMECONF:     //3
            n_mCurrState = IEC104_IDLE;
            //m_SyncTimeFlag = 1;
            break;
        case IEC104_WAITALLKWHCONF:   //4
        case IEC104_WAITALLKWH:       //6
            n_mCurrState = IEC104_IDLE;
            //m_AllKwhFlag = 1;
            break;
        default:
            n_mCurrState = IEC104_IDLE;
            break;
        }
    }
}

/*!
 * \brief  功能概述 初始化IEC-104驱动标志位
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::init104Param()
{
    m_bConfigInitParamOk = false; //未配置
    m_bResetFlag = false; //重置标识
    m_nK = 0;
    m_nW = 0;
    m_nt0 = 0;
    m_nt1 = 0;
    //m_nt1 = 15;
    m_nt2 = 0;
    m_nt3 = 0;
    n_mAPDULen = 0;

    //应用参数
    n_mCommonAddrLen = 0;
    n_mTransmitCauseLen = 0;
    n_mInformationAddrLen = 0;
    n_mTimeType = 0;
    //传输控制参数
    n_mCurrState = IEC104_STARTDT;
    m_nSendIdx = 0;
    m_nReciveIdx = 0;
    n_mSub_SendIdx = 0;
    n_mSub_ReciveIdx = 0;
    m_nAck = 0;

    n_mSyncTimeMode = 0;
    n_mSyncTimeCycle = 0;
    n_mCallDataMode = 0;
    n_mCallDataCycle = 0;

    //标识参数
    n_mSyncTimeFlag = 0;
    n_mCallAllDataFlag = 0;
    n_mCallKwhDataFalg = 0;

    //内部参数
    n_mLastTestFrameTime = GetSysAbsTime();
    n_mLastFrameTime = GetSysAbsTime();
    n_mSendIFrameFullTime = 0;
    n_mReadFrameDateLen = 0;
    n_mLastReciveIFrameTime = 0;
    n_mLastSendIFrameTime = 0;
    n_mLastCallDataTime = 0;
    n_mLastTimingTime = 0;
    n_mLastTxTime = 0;
    n_mResetColseLinkFlag = 0;

    //出问题，后加的 7-24
    setConfigProtocolInitParam(g_param);
    m_nYKProcStep = YS_UNKNOWN_STEP;
}

/*!
 * \brief  功能概述 设置IEC-104协议参数
 * \param  参数描述 para是IEC-104协议参数对象
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::setConfigProtocolInitParam(const AAALinkLayerConfigInfo &para)
{
    g_param = para;
    if(para.strTransmitCauseLen == "") //传输原因长度
        n_mTransmitCauseLen = 2;
    else
        n_mTransmitCauseLen = para.strTransmitCauseLen.toInt();
    if(para.strCommAddrLen == "")  //公共地址长度
        n_mCommonAddrLen = 2;
    else
        n_mCommonAddrLen = para.strCommAddrLen.toInt();
    if(para.strInfoAddrLen == "")  //信息体地址长度
        n_mInformationAddrLen = 3;
    else
        n_mInformationAddrLen = para.strInfoAddrLen.toInt();
    if(para.strTimeType == "")     //时标格式
        n_mTimeType = 1;  //CP56 Time2a
    else
    {
        if(para.strTimeType == "CP56 Time2a")
            n_mTimeType = 1;
        else
            n_mTimeType = 0;//CP24 Time2a
    }
    if(para.strTimingType == "OK") //启动时总召唤 或 周期召唤
        n_mSyncTimeMode = 1;
    else
    {
        n_mSyncTimeMode = 0;
        n_mSyncTimeCycle = para.strTimingType.toInt();
    }
    if(para.strCallingType == "OK") //启动时对时 或 周期对时
        n_mCallDataMode = 1;
    else
    {
        n_mCallDataMode = 0;
        n_mCallDataCycle = para.strCallingType.toInt();
    }
    if(para.strASDULen == "")  //ASDU长度
        n_mAPDULen = 253;
    else
        n_mAPDULen = para.strASDULen.toInt();
    if(para.strNonDataTimeOut == "") //接收方无数据报文时确认的超时时间
        m_nt2 = 10;
    else
        m_nt2 = para.strNonDataTimeOut.toInt();
    if(para.strMaxFrameInx == "")   //发送方未被确认的I帧APDU的最大数
        m_nK = 12;
    else
        m_nK = para.strMaxFrameInx.toInt();
    if(para.strSFrameConfirm == "")
        m_nW = 8;
    else
        m_nW = para.strSFrameConfirm.toInt();  //接收方最多接收到的未被确认的I格式的APDU数目
    if(para.strSendFrameTimeOut == "")
        m_nt1 = 15;
    else
        m_nt1 = para.strSendFrameTimeOut.toInt();
    if(para.strLinkTimeOut == "")    //网络建立连接超时时间
        m_nt0 = 30;
    else
        m_nt0 = para.strLinkTimeOut.toInt();
    if(para.strSendSFrameTimeOut == "")
        m_nt3 = 5;
    else
        m_nt3 = para.strSendSFrameTimeOut.toInt();

    m_bConfigInitParamOk = true; //配置完成
}

/*!
 * \brief  功能概述 发送启动U帧 连接链路
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ConnectLink()
{
    ///发送启动U帧 连接链路
    if (m_nLinkCount > 3)
    {
        if (m_pLink != NULL)
        {
            m_pLink->SetCloseLink();
        }
        qDebug()<<"主动断开链接";
    }
    if (m_IEC104Query.MakeQuery_U_Command(true,false,false))
    {
        m_nLinkCount++;///<zzy 2015/1/14 修改:
        n_mCurrState = IEC104_WAITSTARTCONF;
        n_mLastTxTime = GetSysAbsTime();
        qDebug()<<"发送启动帧"<<m_nLinkCount;
        SendFrame(m_IEC104Query);
    }
}

/*!
 * \brief  功能概述 发送报文函数
 * \param  参数描述 frameSend是需要下发的帧对象
 * \return 返回值描述 发送成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolIEC_104::SendFrame(CIEC104Query &frameSend)
{
    //作用:发送并显示所有报文
    if (m_pLink->write(frameSend.GetByteArrayFrame()))
    {
        ShowMessage(SHOWMESSAGE_TYPE_SEND,frameSend.GetByteArrayFrame().toHex());
        return true;
    }
    return false;
}

/*!
 * \brief  功能概述 处理 S帧
 * \param  参数描述 response是接收到的报文对象
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::DecodeSFrame(CIEC104Response_new &response)
{
    Q_ASSERT(response.GetFrameType() == IEC104_S_TYPE);
    //由于我们没有下发缓冲 所以不用

    WORD nAckNumber = response.GetReceiveFrameNo();

    if((m_nSendIdx-nAckNumber)>m_nK)
    {
        qDebug()<<"已经很多帧没有认可了";
        //已经很多帧没有认可了
    }
    if(nAckNumber>m_nSendIdx || (m_nSendIdx-nAckNumber)>m_nK)
    {
        if (m_pLink != NULL)
        {
            m_pLink->SetCloseLink();
        }
    }
    if(nAckNumber == m_nSendIdx) // 最后的I帧被确认
    {
        n_mLastSendIFrameTime = 0;
        n_mSub_ReciveIdx = nAckNumber % MAX_RXTXSEQNUM;
    }
    else
    {
        int nLastSendDecNo = (MAX_RXTXSEQNUM + m_nSendIdx - n_mSub_ReciveIdx)% MAX_RXTXSEQNUM;
        int nCurrRecDecNo = (MAX_RXTXSEQNUM +   nAckNumber - n_mSub_ReciveIdx)% MAX_RXTXSEQNUM;
        if(nCurrRecDecNo > nLastSendDecNo)
        {
            n_mCurrState = IEC104_RESET;
            return ;
        }

        n_mSub_ReciveIdx = nCurrRecDecNo % MAX_RXTXSEQNUM;
    }
}

/*!
 * \brief  功能概述 处理 I帧
 * \param  参数描述 response是接收到的报文对象
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::DecodeIFrame(CIEC104Response_new &response)
{
    n_mSub_SendIdx = response.GetSendFrameNo();

    //等待复位确认
    if(n_mCurrState != IEC104_WAITRSTPROCCONF)
    {
        int nCurrRecSeqNo = response.GetReceiveFrameNo();
        int nLastSendSeqNo = m_nSendIdx;
        if(nCurrRecSeqNo == nLastSendSeqNo) //最后的I帧被确认
        {
            n_mLastSendIFrameTime = 0;
            n_mSub_ReciveIdx = nCurrRecSeqNo % MAX_RXTXSEQNUM;
        }
        else
        {
            int nLastSendDecNo = (MAX_RXTXSEQNUM + nLastSendSeqNo - n_mSub_ReciveIdx)% MAX_RXTXSEQNUM;
            int nCurrRecDecNo = (MAX_RXTXSEQNUM + nCurrRecSeqNo - n_mSub_ReciveIdx)% MAX_RXTXSEQNUM;
            if(nCurrRecDecNo > nLastSendDecNo)
            {
                n_mCurrState = IEC104_RESET;
                return ;
            }

            n_mSub_ReciveIdx = nCurrRecDecNo% MAX_RXTXSEQNUM;
        }
        // 如果是复位或启动链路后第一个I帧则不检查接收序号
        if(n_mResetProcessWaitIFormatFlag || n_mStartDTWaitIFormatFlag)
        {
            m_nReciveIdx = n_mSub_SendIdx;
        }
        else
        {
            //如果子站发送序号不等于上轮接收序号，表明发送顺序错误，关闭连接
            if(m_nReciveIdx != n_mSub_SendIdx)
            {
                qDebug()<<"if(m_nReciveIdx != n_mSub_SendIdx)";
                n_mCurrState = IEC104_RESET;
                return ;
            }
        }
    }

    n_mStartDTWaitIFormatFlag = 0;
    n_mResetProcessWaitIFormatFlag = 0;

    m_nReciveIdx++;
    m_nReciveIdx %= MAX_RXTXSEQNUM;

    n_mLastReciveIFrameTime = GetSysAbsTime();
    m_nAck = 1; //发送S帧的标志

    ASDU101 *pAsdu = response.GetAsdu();
    if(pAsdu == NULL)
        return ;
    switch (pAsdu->m_nType)
    {
    case M_SP_NA_1:	//不带时标单点信息
        ProcessASDU101_Type001(*pAsdu);
        break;
    case M_SP_TA_1: //带时标单点信息
        Q_ASSERT(false);
        ProcessASDU101_Type002(*pAsdu);
        break;
    case M_DP_NA_1:		// 不带时标双点信息
        ProcessASDU101_Type003(*pAsdu);
        break;
    case M_DP_TA_1://带时标双点信息
        Q_ASSERT(false);
        ProcessASDU101_Type004(*pAsdu);
        break;
    case M_ST_NA_1:		// 步位置信息(变压器分接头信息)
        ProcessASDU101_Type005(*pAsdu);
        break;
    case M_ST_TA_1: //带时标步位置信息
        Q_ASSERT(false);
        ProcessASDU101_Type006(*pAsdu);
        break;
    case M_BO_NA_1:		// 子站远动终端状态
        Q_ASSERT(false);
        ProcessASDU101_Type007(*pAsdu);
        break;
    case M_BO_TA_1:		// 带时标的块数据
        Q_ASSERT(false);
//        ProcessASDU101_Type008(*pAsdu);
        break;
    case M_ME_NA_1:		// 品质测量值,无时标
        ProcessASDU101_Type009(*pAsdu);
        break;
    case M_ME_TA_1:		// 品质、事件、测量值
        Q_ASSERT(false);
//        ProcessASDU101_Type010(*pAsdu);
        break;
    case M_ME_NB_1:		// Scaled Value, Without Time Stamp
        ProcessASDU101_Type011(*pAsdu);
        break;
    case M_ME_TB_1:		// Scaled Value, With Time Stamp
        Q_ASSERT(false);
//        ProcessASDU101_Type012(*pAsdu);
        break;
    case M_ME_NC_1:		// Scaled Value, short floating Without Time Stamp
        ProcessASDU101_Type013(*pAsdu);
        break;
    case M_ME_TC_1:		// Scaled Value, short floating With Time Stamp
        Q_ASSERT(false);
//        ProcessASDU101_Type014(*pAsdu);
        break;
    case M_SP_TB_1:
        ProcessASDU101_Type030(*pAsdu);
        break;
    case M_DP_TB_1:
        ProcessASDU101_Type031(*pAsdu);
        break;
    case M_ST_TB_1:
        Q_ASSERT(false);
//        ProcessASDU101_Type032(*pAsdu);
        break;
    case M_BO_TB_1:
        Q_ASSERT(false);
//        ProcessASDU101_Type033(*pAsdu);
        break;
    case M_ME_TD_1:
        ProcessASDU101_Type034(*pAsdu);
        break;
    case M_ME_TE_1:
        ProcessASDU101_Type035(*pAsdu);
        break;
    case M_ME_TF_1:
        ProcessASDU101_Type036(*pAsdu);
        break;
    case M_IT_TB_1:
        ProcessASDU101_Type037(*pAsdu);
        break;
    case M_EP_TD_1:
        Q_ASSERT(false);
//        ProcessASDU101_Type038(*pAsdu);
        break;
    case M_EP_TE_1:
        Q_ASSERT(false);
//        ProcessASDU101_Type039(*pAsdu);
        break;
    case M_EP_TF_1:		// 带时标的继电保护装置成组输出电路信息事件
        Q_ASSERT(false);
//        ProcessASDU101_Type040(*pAsdu);
    case M_IT_NA_1:	// 电能脉冲计数
        ProcessASDU101_Type015(*pAsdu);
        break;
    case M_IT_TA_1:
        Q_ASSERT(false);
//        ProcessASDU101_Type016(*pAsdu);
        break;
    case M_EP_TA_1:		// 带时标的继电保护或重合闸设备单个事件
        Q_ASSERT(false);
//        ProcessASDU101_Type017(*pAsdu);
        break;
    case M_EP_TB_1:		// 带时标的继电保护装置成组启动事件
        Q_ASSERT(false);
//        ProcessASDU101_Type018(*pAsdu);
        break;
    case M_EP_TC_1:		// 带时标的继电保护装置成组输出电路信息事件
        Q_ASSERT(false);
//        ProcessASDU101_Type019(*pAsdu);
        break;
    case M_PS_NA_1:		// 具有状态变位检出的成组单点信息
        ProcessASDU101_Type020(*pAsdu);
        break;
    case M_ME_ND_1:		// 不带品质描述测量值
        ProcessASDU101_Type021(*pAsdu);
        break;
    case M_BD_NA_1:	// BDC码(水位值)
        ProcessASDU101_Type232(*pAsdu);
        break;
    case C_IC_NA_1://召唤命令返回
        break;
    case C_DC_NA_1://遥控返回
//        if(m_pTagControl != NULL)
//        {
//            if(pAsdu->m_reason == 7)//确定
//                m_pTagControl->SetResult(true);//SetComandReturn(0x010e,"命令成功");
//            else
//                m_pTagControl->SetResult(false);//SetComandReturn(0x010f,"命令失败");
//        }
        break;
    default:
//        TRACE("未处理类型:%d\n",pAsdu->m_nType);
        break;
    }
}

/*!
 * \brief  功能概述 解析单位遥信报文，带品质描述、不带时标
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type001(ASDU101 &asdu)
{
    //7.3.1.1类型标识1：M_SP_NA_1
    //不带时标的单点信息
    //// 品质测量值,无时标
    Q_ASSERT(asdu.m_nType == M_SP_NA_1);
    CTagI *pTag = NULL;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";
    int  nPointNumber = 0;
    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)//单个
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 SIQ
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_SIQ *pValue=(ASDU101_SIQ *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            if(pValue->IV)
                continue;
            pPoint = YX_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant(pValue->SPI);
//            QVariant OriginalVariant(pValue->SPI);
//            pTag->SetValue(ProjectVariant,OriginalVariant);
            pTag->SetValue(pPoint,ProjectVariant,ProjectVariant);
        }
    }
    else//连续
    {
        nPointNumber=asdu.GetPoint(pFrameStart);
        pFrameStart+=asdu.GetPointSize();
        for(int i=0;i<asdu.NUMBER;i++)
        {//SIQ
            ASDU101_SIQ *pValue=(ASDU101_SIQ *)pFrameStart;
//            qDebug()<<"有没有效？"<<*pFrameStart;
            pFrameStart+=pValue->GetSize();
            if(pValue->IV )
                continue;
//            qDebug()<<"有效"<<nPointNumber+i;
            pPoint = YX_PointMap.value(nPointNumber+i);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
//            qDebug()<<"有此点";
            QVariant ProjectVariant(pValue->SPI);
//            QVariant OriginalVariant(pValue->SPI);
//            pTag->SetValue(ProjectVariant,OriginalVariant);
            pTag->SetValue(pPoint,ProjectVariant,ProjectVariant);
        }
    }
}

/*!
 * \brief  功能概述 解析M_SP_TA_1报文
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type002(ASDU101 &asdu)
{
//	7.3.1.2 类型标识2：M_SP_TA_1
//		带时标的单点信息CP24Time2a

    Q_ASSERT(asdu.m_nType == M_SP_TA_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";
    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)//单个
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 SIQ CP24Time2a
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_SIQ *pValue=(ASDU101_SIQ *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_CP24Time2a *pTime=(ASDU101_CP24Time2a *)pFrameStart;
            pFrameStart+=pTime->GetSize();

            if(pValue->IV )
                continue;
            pPoint = YX_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant(pValue->SPI);
            pTag->SetValue(pPoint,ProjectVariant,ProjectVariant);
        }
    }
    else//连续
    {//协议不允许
        Q_ASSERT(false);
    }
}

/*!
 * \brief  功能概述 解析双位遥信报文，带品质描述、不带时标的报文
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type003(ASDU101 &asdu)
{
//	7.3.1.3类型标识3：M_DP_NA_1
//		不带时标的双点信息

    Q_ASSERT(asdu.m_nType == M_DP_NA_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";

    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)//单个
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 DIQ
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();


            ASDU101_DIQ *pValue=(ASDU101_DIQ *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            if(pValue->IV || pValue->DPI==0 || pValue->DPI==3)
                continue;
            pPoint = YX_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant(pValue->DPI==2);
            pTag->SetValue(pPoint,ProjectVariant,ProjectVariant);
        }
    }
    else//连续
    {
        nPointNumber=asdu.GetPoint(pFrameStart);
        pFrameStart+=asdu.GetPointSize();
        for(int i=0;i<asdu.NUMBER;i++)
        {//DIQ
            ASDU101_DIQ *pValue=(ASDU101_DIQ *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            if(pValue->IV || pValue->DPI==0 || pValue->DPI==3)
                continue;
            pPoint = YX_PointMap.value(nPointNumber+i);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant(pValue->DPI==2);
            pTag->SetValue(pPoint,ProjectVariant,ProjectVariant);
        }
    }
}

/*!
 * \brief  功能概述 解析M_DP_TA_1报文
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type004(ASDU101 &asdu)
{
    //7.3.1.4 类型标识4：M_DP_TA_1
//带时标的双点信息 CP24Time2a

    Q_ASSERT(asdu.m_nType == M_DP_TA_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";

    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 DIQ CP24Time2a
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_DIQ *pValue=(ASDU101_DIQ *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_CP24Time2a *pTime=(ASDU101_CP24Time2a *)pFrameStart;
            pFrameStart+=pTime->GetSize();

            if(pValue->IV || pValue->DPI==0 || pValue->DPI==3)
                continue;
            pPoint = YX_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant(pValue->DPI==2);
            pTag->SetValue(pPoint,ProjectVariant,ProjectVariant);

//			pTag=GetTag(asdu.GetCommonAddress(),nPointNumber);
//			if(pTag == NULL)
//				continue;
//			pTag->SetIntValue(pValue->DPI==2);
            //处理soe
// 			SYSTEMTIME timeSoe;
// 			GetLocalTime(&timeSoe);
// 			if(pTime->IV==0)
// 			{
// 				timeSoe.wMinute=pTime->MINUTE;
// 				timeSoe.wSecond=pTime->MILLISECOND/1000;
// 				timeSoe.wMilliseconds=pTime->MILLISECOND%1000;
// 			}
// 			GenerateSoe(timeSoe,pTag,pValue->DPI==2,nPoint);
//
        }
    }
    else
    {
        Q_ASSERT(false);
    }
}

/*!
 * \brief  功能概述 解析步位置信息(变压器分接头信息)报文
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type005(ASDU101 &asdu)
{
    // 步位置信息(变压器分接头信息)
    Q_ASSERT(asdu.m_nType == M_ST_NA_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";

    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0 && asdu.NUMBER==1)//只能有一个
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 VTI QDS
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_VTI *pValue=(ASDU101_VTI *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_QDS *pQds=(ASDU101_QDS *)pFrameStart;
            pFrameStart+=pQds->GetSize();

            if(pQds->IV|| pQds->OV)
                continue;
            pPoint = YC_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant(pValue->VALUE);
            pTag->SetValue(pPoint,ProjectVariant,ProjectVariant);

//            pTag=GetTag(asdu.GetCommonAddress(),nPointNumber);
//            if(pTag == NULL)
//                continue;
//            pTag->SetIntValue(pValue->VALUE);
        }
    }
    else
    {
        Q_ASSERT(false);
    }
}

/*!
 * \brief  功能概述 解析带时标的步位置信息(变压器分接头信息)报文
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type006(ASDU101 &asdu)
{
//7.3.1.6 类型标识6：M_ST_TA_1
    //带时标的步位置信息  CP24Time2a
    Q_ASSERT(asdu.m_nType == M_ST_TA_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";

    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0 && asdu.NUMBER==1)//只能有一个
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 VTI QDS CP24Time2a
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_VTI *pValue=(ASDU101_VTI *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_QDS *pQds=(ASDU101_QDS *)pFrameStart;
            pFrameStart+=pQds->GetSize();

            ASDU101_CP24Time2a *pTime=(ASDU101_CP24Time2a *)pFrameStart;
            pFrameStart+=pTime->GetSize();

            if(pQds->IV|| pQds->OV)
                continue;
            pPoint = YC_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant(pValue->VALUE);
            pTag->SetValue(pPoint,ProjectVariant,ProjectVariant);
//            pTag=GetTag(asdu.GetCommonAddress(),nPoint);
//            if(pTag == NULL)
//                continue;
//            pTag->SetIntValue(pValue->VALUE);

// 			//处理soe
// 			SYSTEMTIME timeSoe;
// 			GetLocalTime(&timeSoe);
// 			if(pTime->IV==0)
// 			{
// 				timeSoe.wMinute=pTime->MINUTE;
// 				timeSoe.wSecond=pTime->MILLISECOND/1000;
// 				timeSoe.wMilliseconds=pTime->MILLISECOND%1000;
// 			}
// 			CEventSoe soe;
// 			soe.m_nEventID=0;
// 			soe.m_timeSOE=timeSoe;
// 			soe.m_strDefaultComment.Format("分接头地址:%d, 状态:%d", nPoint,pValue->VALUE);
// 			AddEventSoe(soe);
        }
    }
    else
    {
        Q_ASSERT(false);
    }
}

/*!
 * \brief  功能概述 解析子站远动终端状态报文
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type007(ASDU101 &asdu)
{
// 可用于子站远动终端状态
    //按遥信方式解包　每个遥信量由其偏移决定
//	7.3.1.7类型标识7：M_BO_NA_1
//		32比特串
    Q_ASSERT(asdu.m_nType == M_BO_NA_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";

    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)
    {

        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 BSI QDS
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_BSI *pValue=(ASDU101_BSI *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_QDS *pQds=(ASDU101_QDS *)pFrameStart;
            pFrameStart+=pQds->GetSize();

            if(pQds->IV )
                continue;
            for(int nOffset=0;nOffset<32;nOffset++)
            {
                pPoint = YC_PointMap.value(nPointNumber);
                if (!pPoint)
                    continue;
                strLink = pPoint->m_strRTDBName;
                pTag = pPoint->GetTag();
                if (!pTag)
                    continue;
                QVariant ProjectVariant((uint)GET_BIT(pValue->BSI,nOffset));
                pTag->SetValue(pPoint,ProjectVariant,ProjectVariant);
//                pTag=GetTag(asdu.GetCommonAddress(),nPointNumber,nOffset);
//                if(pTag == NULL)
//                    continue;
//                pTag->SetIntValue(GET_BIT(pValue->BSI,nOffset));
            }
        }
    }
    else
    {
        Q_ASSERT(false);
    }
}

/*!
 * \brief  功能概述 解析归一化遥测值报文，带品质描述，不带时标
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type009(ASDU101 &asdu)
{
//	7.3.1.9类型标识9：M_ME_NA_1
    //		测量值, 规一化值

    //// 品质测量值,无时标
    Q_ASSERT(asdu.m_nType == M_ME_NA_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";

    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 NVA QDS
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();
            ASDU101_NVA *pValue=(ASDU101_NVA *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_QDS *pQds=(ASDU101_QDS *)pFrameStart;
            pFrameStart+=pQds->GetSize();

            if(pQds->IV|| pQds->OV)
                continue;
            pPoint = YC_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant( (pValue->GetValue()+pPoint->m_fBaseValue)*pPoint->m_fKValue );
            QVariant OriginalVariant(pValue->GetValue());
            pTag->SetValue(pPoint,ProjectVariant,OriginalVariant);
//            pTag=GetTag(asdu.GetCommonAddress(),nPointNumber);
//            if(pTag == NULL)
//                continue;
//            pTag->SetIntValue(pValue->GetValue());
        }
    }
    else
    {
        nPointNumber=asdu.GetPoint(pFrameStart);
        pFrameStart+=asdu.GetPointSize();
        for(int i=0;i<asdu.NUMBER;i++)
        {// NVA QDS
            ASDU101_NVA *pValue=(ASDU101_NVA *)pFrameStart;
//            qDebug()<<"有没有效？"<<nPointNumber+i<<*pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_QDS *pQds=(ASDU101_QDS *)pFrameStart;
            pFrameStart+=pQds->GetSize();

            if(pQds->IV|| pQds->OV)
                continue;
            pPoint = YC_PointMap.value(nPointNumber+i);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant( (pValue->GetValue()+pPoint->m_fBaseValue)*pPoint->m_fKValue );
            QVariant OriginalVariant(pValue->GetValue());
//            QVariant ProjectVariant(pValue->GetValue());
            pTag->SetValue(pPoint,ProjectVariant,OriginalVariant);
//            pTag=GetTag(asdu.GetCommonAddress(),nPoint+i);
//            if(pTag == NULL)
//                continue;
//            pTag->SetIntValue(pValue->GetValue());
        }
    }
}

/*!
 * \brief  功能概述 解析标度化遥测值报文，带品质描述，不带时标
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type011(ASDU101 &asdu)
{
//	7.3.1.11类型标识11：M_ME_NB_1
//		测量值, 标度化值
        // Scaled Value, Without Time Stamp// 测量值, 标度化值
    //// 品质测量值,无时标
    Q_ASSERT(asdu.m_nType == M_ME_NB_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";

    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 SVA QDS
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_SVA *pValue=(ASDU101_SVA *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_QDS *pQds=(ASDU101_QDS *)pFrameStart;
            pFrameStart+=pQds->GetSize();

            if(pQds->IV|| pQds->OV)
                continue;
            pPoint = YC_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant( (pValue->m_nValue+pPoint->m_fBaseValue)*pPoint->m_fKValue );
            QVariant OriginalVariant(pValue->m_nValue);
//            QVariant ProjectVariant(pValue->m_nValue);
            pTag->SetValue(pPoint,ProjectVariant,OriginalVariant);
//            pTag=GetTag(asdu.GetCommonAddress(),nPointNumber);
//            if(pTag == NULL)
//                continue;
//            pTag->SetIntValue(pValue->m_nValue);
        }
    }
    else
    {
        nPointNumber=asdu.GetPoint(pFrameStart);
        pFrameStart+=asdu.GetPointSize();
        for(int i=0;i<asdu.NUMBER;i++)
        {// SVA QDS
            ASDU101_SVA *pValue=(ASDU101_SVA *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_QDS *pQds=(ASDU101_QDS *)pFrameStart;
            pFrameStart+=pQds->GetSize();

            if(pQds->IV|| pQds->OV)
                continue;
            pPoint = YC_PointMap.value(nPointNumber+i);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant( (pValue->m_nValue+pPoint->m_fBaseValue)*pPoint->m_fKValue );
            QVariant OriginalVariant(pValue->m_nValue);
//            QVariant ProjectVariant(pValue->m_nValue);
            pTag->SetValue(pPoint,ProjectVariant,OriginalVariant);
//            pTag=GetTag(asdu.GetCommonAddress(),nPointNumber+i);
//            if(pTag == NULL)
//                continue;
//            pTag->SetIntValue(pValue->m_nValue);
        }
    }
}

/*!
 * \brief  功能概述 解析短浮点遥测值报文，带品质描述，不带时标
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type013(ASDU101 &asdu)
{
// Scaled Value, short floating Without Time Stamp
//	7.3.1.13类型标识13：M_ME_NC_1
//		测量值, 短浮点数
    Q_ASSERT(asdu.m_nType == M_ME_NC_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";

    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 R32 QDS
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_R32 *pValue=(ASDU101_R32 *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_QDS *pQds=(ASDU101_QDS *)pFrameStart;
            pFrameStart+=pQds->GetSize();

        //	if(pQds->IV|| pQds->OV)
        //		continue;
            pPoint = YC_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant( (pValue->m_nValue+pPoint->m_fBaseValue)*pPoint->m_fKValue );
            QVariant OriginalVariant(pValue->m_nValue);
//            QVariant ProjectVariant(pValue->m_nValue);
            pTag->SetValue(pPoint,ProjectVariant,OriginalVariant);

//            pTag=GetTag(asdu.GetCommonAddress(),nPointNumber);
//            if(pTag == NULL)
//                continue;
//            pTag->SetFloatValue(pValue->m_nValue);
        }
    }
    else
    {
        nPointNumber=asdu.GetPoint(pFrameStart);
        pFrameStart+=asdu.GetPointSize();
        for(int i=0;i<asdu.NUMBER;i++)
        {// R32 QDS
            ASDU101_R32 *pValue=(ASDU101_R32 *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_QDS *pQds=(ASDU101_QDS *)pFrameStart;
            pFrameStart+=pQds->GetSize();

        //	if(pQds->IV|| pQds->OV)
        //		continue;
            pPoint = YC_PointMap.value(nPointNumber+i);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant( (pValue->m_nValue+pPoint->m_fBaseValue)*pPoint->m_fKValue );
            QVariant OriginalVariant(pValue->m_nValue);
//            QVariant ProjectVariant(pValue->m_nValue);
            pTag->SetValue(pPoint,ProjectVariant,OriginalVariant);

//            pTag=GetTag(asdu.GetCommonAddress(),nPointNumber+i);
//            if(pTag == NULL)
//                continue;
//            pTag->SetFloatValue(pValue->m_nValue);
        }
    }
}

/*!
 * \brief  功能概述 解析累计量报文，带品质描述，不带时标
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type015(ASDU101 &asdu)
{
// 电能脉冲计数
//	7.3.1.15类型标识15：M_IT_NA_1
//		累计量
    Q_ASSERT(asdu.m_nType == M_IT_NA_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";

    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 BCR
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_BCR *pValue=(ASDU101_BCR *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            if(pValue->IV)
                continue;
            pPoint = YM_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant( ((uint)pValue->VALUE+pPoint->m_fBaseValue)*pPoint->m_fKValue );
            QVariant OriginalVariant((uint)pValue->VALUE);
//            QVariant ProjectVariant((uint)pValue->VALUE);
            pTag->SetValue(pPoint,ProjectVariant,OriginalVariant);

//            pTag=GetTag(asdu.GetCommonAddress(),nPointNumber);
//            if(pTag == NULL)
//                continue;
//            pTag->SetIntValue(pValue->VALUE);
        }
    }
    else
    {
        nPointNumber=asdu.GetPoint(pFrameStart);
        pFrameStart+=asdu.GetPointSize();
        for(int i=0;i<asdu.NUMBER;i++)
        {// BCR
            ASDU101_BCR *pValue=(ASDU101_BCR *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            if(pValue->IV)
                continue;
            pPoint = YM_PointMap.value(nPointNumber+i);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant( ((uint)pValue->VALUE+pPoint->m_fBaseValue)*pPoint->m_fKValue );
            QVariant OriginalVariant((uint)pValue->VALUE);
//            QVariant ProjectVariant((uint)pValue->VALUE);
            pTag->SetValue(pPoint,ProjectVariant,OriginalVariant);

//            pTag=GetTag(asdu.GetCommonAddress(),nPointNumber+i);
//            if(pTag == NULL)
//                continue;
//            pTag->SetIntValue(pValue->VALUE);

        }
    }
}
void CProtocolIEC_104::ProcessASDU101_Type020(ASDU101 &/*asdu*/)
{// 具有状态变位检出的成组单点信息  说明书？？？

}

/*!
 * \brief  功能概述 解析表度化遥测值报文，带品质描述，不带时标
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type021(ASDU101 &asdu)
{
// 不带品质描述测量值
    //7.3.1.21类型标识21：M_ME_ND_1
//	测量值, 不带品质描述词的规一化值

    Q_ASSERT(asdu.m_nType == M_ME_ND_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";
    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 NVA
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_NVA *pValue=(ASDU101_NVA *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            pPoint = YC_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant( (pValue->GetValue()+pPoint->m_fBaseValue)*pPoint->m_fKValue );
            QVariant OriginalVariant(pValue->GetValue());
//            QVariant ProjectVariant(pValue->GetValue());
            pTag->SetValue(pPoint,ProjectVariant,OriginalVariant);
//            QVariant ProjectVariant(pValue->GetValue());
//            pTag->SetValue(ProjectVariant,ProjectVariant);

//            pTag=GetTag(asdu.GetCommonAddress(),nPointNumber);
//            if(pTag == NULL)
//                continue;
//            pTag->SetIntValue(pValue->GetValue());
        }
    }
    else
    {
        nPointNumber=asdu.GetPoint(pFrameStart);
        pFrameStart+=asdu.GetPointSize();
        for(int i=0;i<asdu.NUMBER;i++)
        {// NVA
            ASDU101_NVA *pValue=(ASDU101_NVA *)pFrameStart;
            pFrameStart+=pValue->GetSize();
            pPoint = YC_PointMap.value(nPointNumber+i);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant( (pValue->GetValue()+pPoint->m_fBaseValue)*pPoint->m_fKValue );
            QVariant OriginalVariant(pValue->GetValue());
//            QVariant ProjectVariant(pValue->GetValue());
            pTag->SetValue(pPoint,ProjectVariant,OriginalVariant);
//            QVariant ProjectVariant(pValue->GetValue());
//            pTag->SetValue(ProjectVariant,ProjectVariant);

//            pTag=GetTag(asdu.GetCommonAddress(),nPointNumber+i);
//            if(pTag == NULL)
//                continue;
//            pTag->SetIntValue(pValue->GetValue());
        }
    }
}

/*!
 * \brief  功能概述 解析单位遥信（SOE）报文，带品质描述，带绝对时标
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type030(ASDU101 &asdu)
{
    //7.3.1.22类型标识30：M_SP_TB_1
    //带时标CP56Time2a的单点信息(图50)
    Q_ASSERT(asdu.m_nType == M_SP_TB_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";
    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 SIQ CP56Time2a
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_SIQ *pValue=(ASDU101_SIQ *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_CP56Time2a *pTime=(ASDU101_CP56Time2a *)pFrameStart;
            pFrameStart+=pTime->GetSize();

            if(pValue->IV )
                continue;
            pPoint = YX_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant(pValue->SPI);
            pTag->SetValue(pPoint,ProjectVariant,ProjectVariant);

//			pTag=GetTag(asdu.GetCommonAddress(),nPointNumber);
//			if(pTag == NULL)
//				continue;
//			pTag->SetIntValue(pValue->SPI);

// 			SYSTEMTIME timeSoe;
// 			GetLocalTime(&timeSoe);
// 			if(pTime->IV==0)  //我们的１０１转发可能不正确
// 			{
// 				timeSoe=pTime->GetTime();
// 			}
// 			GenerateSoe(timeSoe,pTag,pValue->SPI,nPoint);
        }
    }
    else
    {//协议不允许
        Q_ASSERT(false);
    }
}

/*!
 * \brief  功能概述 解析双位遥信（SOE）报文，带品质描述，带绝对时标
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type031(ASDU101 &asdu)
{
//	7.3.1.23类型标识31：M_DP_TB_1
//		带时标CP56Time2a的双点信息
    Q_ASSERT(asdu.m_nType == M_DP_TB_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";
    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 DIQ CP56Time2a
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_DIQ *pValue=(ASDU101_DIQ *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_CP56Time2a *pTime=(ASDU101_CP56Time2a *)pFrameStart;
            pFrameStart+=pTime->GetSize();

            if(pValue->IV || pValue->DPI==0 || pValue->DPI==3)
                continue;
            pPoint = YX_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant(pValue->DPI==2);
            pTag->SetValue(pPoint,ProjectVariant,ProjectVariant);

//            pTag=GetTag(asdu.GetCommonAddress(),nPointNumber);
//            if(pTag == NULL)
//                continue;
//            pTag->SetIntValue(pValue->DPI==2);
// 			//处理soe
// 			SYSTEMTIME timeSoe;
// 			GetLocalTime(&timeSoe);
// 			if(pTime->IV==0)
// 			{
// 				timeSoe=pTime->GetTime();
// 			}
// 			GenerateSoe(timeSoe,pTag,pValue->DPI==2,nPoint);

        }
    }
    else
    {
        Q_ASSERT(false);
    }
}

/*!
 * \brief  功能概述 解析归一化遥测值报文，带品质描述，带绝对时标
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type034(ASDU101 &asdu)
{
    //7.3.1.26 类型标34：M_ME_TD_1
//	测量值, 带时标CP56Time2a的规一化值
    Q_ASSERT(asdu.m_nType == M_ME_TD_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";
    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 NVA QDS CP56Time2a
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_NVA *pValue=(ASDU101_NVA *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_QDS *pQds=(ASDU101_QDS *)pFrameStart;
            pFrameStart+=pQds->GetSize();

            ASDU101_CP56Time2a *pTime=(ASDU101_CP56Time2a *)pFrameStart;
            pFrameStart+=pTime->GetSize();

            if(pQds->IV|| pQds->OV)
                continue;
            pPoint = YC_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant( (pValue->GetValue()+pPoint->m_fBaseValue)*pPoint->m_fKValue );
            QVariant OriginalVariant(pValue->GetValue());
//            QVariant ProjectVariant(pValue->GetValue());
            pTag->SetValue(pPoint,ProjectVariant,OriginalVariant);
//            QVariant ProjectVariant(pValue->GetValue());
//            pTag->SetValue(ProjectVariant,ProjectVariant);

//			pTag=GetTag(asdu.GetCommonAddress(),nPointNumber);
//			if(pTag == NULL)
//				continue;
//			pTag->SetIntValue(pValue->GetValue());
                //时标如何处理???
        }
    }
    else
    {//协议不允许
        Q_ASSERT(false);
    }
}

/*!
 * \brief  功能概述 解析标度化遥测值报文，带品质描述，带绝对时标
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type035(ASDU101 &asdu)
{
    //7.3.1.27 类型标35：M_ME_TE_1
    //测量值, 带时标CP56Time2a的标度化值
    Q_ASSERT(asdu.m_nType == M_ME_TE_1);
    Q_ASSERT(asdu.m_nType == M_ME_TD_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";
    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 SVA QDS CP56Time2a
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_SVA *pValue=(ASDU101_SVA *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_QDS *pQds=(ASDU101_QDS *)pFrameStart;
            pFrameStart+=pQds->GetSize();

            ASDU101_CP56Time2a *pTime=(ASDU101_CP56Time2a *)pFrameStart;
            pFrameStart+=pTime->GetSize();

            if(pQds->IV|| pQds->OV)
                continue;
            pPoint = YC_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant( (pValue->m_nValue+pPoint->m_fBaseValue)*pPoint->m_fKValue );
            QVariant OriginalVariant(pValue->m_nValue);
//            QVariant ProjectVariant(pValue->GetValue());
            pTag->SetValue(pPoint,ProjectVariant,OriginalVariant);
//            QVariant ProjectVariant(pValue->m_nValue);
//            pTag->SetValue(ProjectVariant,ProjectVariant);

//			pTag=GetTag(asdu.GetCommonAddress(),nPointNumber);
//			if(pTag == NULL)
//				continue;
//			pTag->SetIntValue(pValue->m_nValue);//处理时间？？？
        }
    }
    else
    {//协议不允许
        Q_ASSERT(false);
    }
}

/*!
 * \brief  功能概述 解析短浮点遥测值报文，带品质描述，带绝对时标
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type036(ASDU101 &asdu)
{
    //7.3.1.28 类型标36：M_ME_TF_1
//	测量值, 带时标CP56Time2a的短浮点数
    Q_ASSERT(asdu.m_nType == M_ME_TF_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";
    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 R32 QDS CP56Time2a
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_R32 *pValue=(ASDU101_R32 *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_QDS *pQds=(ASDU101_QDS *)pFrameStart;
            pFrameStart+=pQds->GetSize();

            ASDU101_CP56Time2a *pTime=(ASDU101_CP56Time2a *)pFrameStart;
            pFrameStart+=pTime->GetSize();

            if(pQds->IV|| pQds->OV)
                continue;
            pPoint = YC_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant( (pValue->m_nValue+pPoint->m_fBaseValue)*pPoint->m_fKValue );
            QVariant OriginalVariant(pValue->m_nValue);
//            QVariant ProjectVariant(pValue->GetValue());
            pTag->SetValue(pPoint,ProjectVariant,OriginalVariant);
//            QVariant ProjectVariant(pValue->m_nValue);
//            pTag->SetValue(ProjectVariant,ProjectVariant);

//			pTag=GetTag(asdu.GetCommonAddress(),nPointNumber);
//			if(pTag == NULL)
//				continue;
//			pTag->SetFloatValue(pValue->m_nValue);//处理时间？？？

        }
    }
    else
    {//协议不允许
        Q_ASSERT(false);
    }

}

/*!
 * \brief  功能概述 解析报文，带品质描述，带绝对时标
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type037(ASDU101 &asdu)
{
//	7.3.1.29 类型标37：M_IT_TB_1
//		带时标CP56Time2a的累计量

    Q_ASSERT(asdu.m_nType == M_IT_TB_1);
    CTagI *pTag = NULL;
    int  nPointNumber = 0;
    CPointIEC_104 *pPoint=NULL;
    QString strLink = "";
    BYTE *pFrameStart=asdu.GetInfoStart();
    if(asdu.SQ==0)
    {
        for(int i=0;i<asdu.NUMBER;i++)
        {//点号 BCR CP56Time2a
            nPointNumber=asdu.GetPoint(pFrameStart);
            pFrameStart+=asdu.GetPointSize();

            ASDU101_BCR *pValue=(ASDU101_BCR *)pFrameStart;
            pFrameStart+=pValue->GetSize();

            ASDU101_CP56Time2a *pTime=(ASDU101_CP56Time2a *)pFrameStart;
            pFrameStart+=pTime->GetSize();

            if(pValue->IV)
                continue;
            pPoint = YM_PointMap.value(nPointNumber);
            if (!pPoint)
                continue;
            strLink = pPoint->m_strRTDBName;
            pTag = pPoint->GetTag();
            if (!pTag)
                continue;
            QVariant ProjectVariant((uint)pValue->VALUE);
            pTag->SetValue(pPoint,ProjectVariant,ProjectVariant);
//            pTag=GetTag(asdu.GetCommonAddress(),nPointNumber);
//            if(pTag == NULL)
//                continue;
//            pTag->SetIntValue(pValue->VALUE);//处理时间？？？
        }
    }
    else
    {
        Q_ASSERT(false);
    }
}

/*!
 * \brief  功能概述 解析BDC码(水位值)报文，带品质描述，带绝对时标
 * \param  参数描述 asdu是报文中的ASDU
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ProcessASDU101_Type232(ASDU101 &/*asdu*/)
{
// BCD码(水位值)???说明书
//    Q_ASSERT(asdu.m_nType == M_BD_NA_1);
//    CTagI *pTag = NULL;
//    int  nPointNumber = 0;
//    CPointIEC_104 *pPoint=NULL;
//    QString strLink = "";
}

/*!
 * \brief  功能概述 解析U帧
 * \param  参数描述 response是接收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::DecodeUFrame(CIEC104Response_new &response)
{
    Q_ASSERT(response.GetFrameType() == IEC104_U_TYPE);
    if(response.IsStartFrame())
    {
        if(n_mCurrState == IEC104_WAITSTARTCONF)
        {
            m_nSendIdx = 0;
            m_nReciveIdx = 0;
            n_mCurrState = IEC104_IDLE;
            n_mStartDTWaitIFormatFlag = 1;
            m_bDataLink = true;
        }
//        m_nUnconfirmIFrame = 0;
//        m_timeTotal.Start(0);
    }
    if(response.IsStopFrame())
    {
        if(n_mCurrState == IEC104_WAITSTOPCONF)
        {
            m_nSendIdx = 0;
            m_nReciveIdx = 0;
            n_mCurrState = IEC104_STARTDT;
            m_bDataLink = false;
        }
    }
//    if(response.IsTestFrame())
//    {
//        CIEC104Query query;
//        query.MakeQuery_U_Confirm(false,false,true);
//        SendFrame(query);
//    }
    if (response.IsTestFrame())
    {
        if(n_mCurrState == IEC104_WAITTESTLINKCONF)
        {
            n_mCurrState = IEC104_IDLE;
        }
    }
}

/*!
 * \brief  功能概述 判断接收序号是否大于最大值
 * \param  参数描述 nValue接收序号
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::InCrease(WORD &nValue)
{
    nValue++;
    if(nValue>m_nMaxSRValue)
        nValue = 0;
}

/*!
 * \brief  功能概述 复位链路
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::ResetDataLink()
{
    ShowMessage(SHOWMESSAGE_TYPE_DEBUG,"复位用户链路");

    m_nReciveIdx = 0;
    m_nSendIdx = 0;
    m_bDataLink = false;
    m_nUnconfirmIFrame = 0;
    if (m_pLink != NULL)
    {
        m_pLink->SetCloseLink();
    }
//    if(m_pTcpSocket != NULL)
//    {
//        m_pTcpSocket->close();
//    }
}

/*!
 * \brief  功能概述 初始化IEC-104参数
 * \param  参数描述
 * \return 返回值描述
 * \author zzy
 * \date   2015/5/26
 */
void CProtocolIEC_104::InitIEC104()
{
    m_bConnectOk = false;
    m_bDataLink = false;
    m_nReciveIdx = 0;
    m_nSendIdx = 0;
    m_nUnconfirmIFrame = 0;
    init104Param();
}

