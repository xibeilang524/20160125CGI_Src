/*!
 * \file    cdelivermodbustcp.cpp
 * \brief   概述 CProtocolDeliverModbusTcp类的源文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/25
 *
 * \b       修改记录：
 * \li      2015/5/25
 *  添加注释
 */

#include "../PortInclude/cvaluebase.h"
#include "../PortInclude/CRTDBI.h"
#include "../PortInclude/CTagI.h"
#include "cdelivermodbustcp.h"
#include "cpointbasemodbus.h"

#include <QDomDocument>
#include <QDebug>
#include <QFile>
#include <QThread>

/*!
 * \brief 功能概述 库文件导出接口函数
 * \param 参数描述 parent是QObject对象指针
 * \return 返回值描述 返回创建的驱动指针
 * \author zzy
 * \date 2015/1/14
 */
CProtocolI * CreateDriver(QObject *parent)
{
    ///库文件导出函数
    CProtocolDeliverModbusTcp *testProtocol = new CProtocolDeliverModbusTcp(parent);
    return  testProtocol;
}

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 parent是父对象的指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
CProtocolDeliverModbusTcp::CProtocolDeliverModbusTcp(QObject */*parent*/)
{
    qDebug()<<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
}

/*!
 * \brief  功能概述 创建此通道
 * \param  参数描述 strChannelFileName_是通道文件绝对路径名，pDatabaseReturn是实时库指针
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CProtocolDeliverModbusTcp::OnCreateChannel(const QString strChannelFileName_, CRTDBI *pDatabaseReturn)
{
//    qDebug()<<"bool CProtocolDeliverModbusTcp::OnCreateChannel"<<QThread::currentThreadId();
//    qDebug()<<"-------------adsfasdfas1";
    SetPROTOCOL_TYPE(Collect_Type);
    if (!CProtocolBase::OnCreateChannel(strChannelFileName_,pDatabaseReturn))
    {
//        qDebug()<<"-------------adsfasdfas2";

        return false;
    }
//    qDebug()<<"-------------adsfasdfas3";
    QFile file(strChannelFileName_);
    QDomDocument ChannelDoc;
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return false;
    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!ChannelDoc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        qDebug()<<strChannelFileName_<<"XML File Error Message:"<<errorStr<<" Error Line:"<<errorLine<<" Error Column:"<<errorColumn;
        return false;
    }
    file.close();

    QDomElement docElem = ChannelDoc.documentElement();
    if (docElem.childNodes().count() != 5)///<zzy 2015/1/14 修改:5个节点
        return false;
//    QDomElement Channel    = docElem.childNodes().at(0).toElement();
//    QDomElement Top        = docElem.childNodes().at(1).toElement();
//    QDomElement Protocol   = docElem.childNodes().at(2).toElement();
//    QDomElement MainPort   = docElem.childNodes().at(3).toElement();
    QDomElement DeviceList = docElem.childNodes().at(4).toElement();
    SetFunctionMap(DeviceList);///初始化点表Map

//    TCPConnect(MainPort);
    return true;
}

bool CProtocolDeliverModbusTcp::OnCreateChannel(const QDomElement &ChannelElement_, ChannelType nChannelType_, int nComOrCanNumber_, CRTDBI *pDatabaseReturn_)
{
//    qDebug()<<"11111111111111111*******************************************************************************";
    SetPROTOCOL_TYPE(Transform_Type);
    if (!CProtocolBase::OnCreateChannel(ChannelElement_,nChannelType_,nComOrCanNumber_,pDatabaseReturn_))
        return false;

    if (ChannelElement_.elementsByTagName("DeviceList").isEmpty())
        return false;
    QDomElement DeviceList = ChannelElement_.elementsByTagName("DeviceList").at(0).toElement();
    CGI_SetFunctionMap(DeviceList);///初始化点表Map

//    qDebug()<<"2222222222222222*******************************************************************************";
    return true;
}

/*!
 * \brief  功能概述 用于响应点的值发生变化的函数
 * \param pPointBase_ 是点的指针
 * \param VarSet_ 是点的值
 * \return 返回值描述 返回true
 * \author zzy
 * \date   2015/5/25
 */
bool CProtocolDeliverModbusTcp::OnTagChange(CPointBase */*pPointBase_*/, QVariant /*VarSet_*/)
{
    /// 转发遥控、遥设
    /// 生成遥控、遥设报文
    /// 转发驱动中不做处理
//    qDebug()<<"bool CProtocolDeliverModbusTcp::OnTagChange("<<QThread::currentThreadId();

    return true;

}
#if 0
/*!
 * \brief  功能概述 响应readyRead(QByteArray)信号，解析收到的报文函数
 * \param  参数描述 data是收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CProtocolDeliverModbusTcp::ProcessRespond(QByteArray data)
{
    QByteArray datas = data;
    if (!datas.isEmpty())
    {
        m_frameQUery = datas;
        ///处理接收到的报文,组织需要返回的报文并发送组织好的报文
        ShowMessage(1,datas.toHex());///显示接收的报文

        if (datas.count() < 12)
        {
            return;
        }
        datas.remove(0,6);

        QByteArray Respond;
        if (MakeRespond(datas,Respond))
        {
            SendFrame(Respond);
        }

    }else
    {
        return;
    }
}
#endif

/*!
 * \brief  功能概述 响应readyRead(QByteArray)信号，解析收到的报文函数
 * \param  参数描述 data是收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CProtocolDeliverModbusTcp::ProcessRespond(QByteArray data)
{
    QMutexLocker lock(&m_Mutex);
    qDebug()<<"==================================="<<data.toHex();
    m_bufferReceive.AppendData(data.data(),data.count());
    ProcessRespond();
    return;
#if 0
    QByteArray datas = data;
    if (!datas.isEmpty())
    {
        m_frameQUery = datas;
        ///处理接收到的报文,组织需要返回的报文并发送组织好的报文
        ShowMessage(1,datas.toHex());///显示接收的报文

        if (datas.count() < 12)
        {
            return;
        }
        datas.remove(0,6);

        QByteArray Respond;
        if (MakeRespond(datas,Respond))
        {
            SendFrame(Respond);
        }

    }else
    {
        return;
    }
#endif
}
void CProtocolDeliverModbusTcp::ProcessRespond()
{
    if (m_bufferReceive.GetInfoSize() < MinFrameLength)
        return ;
    for(int nStart=0; nStart<=m_bufferReceive.GetInfoSize() - MinFrameLength; ++nStart)
    {
        int nFrameLength = IsAFrame(m_bufferReceive.GetBuffer(nStart),m_bufferReceive.GetInfoSize()-nStart);
        if(nFrameLength)
        {
            QByteArray frame;
            if (nStart != 0)
            {
                frame = QByteArray((char*)m_bufferReceive.GetBuffer(0),nStart);
                ShowMessage(SHOWMESSAGE_TYPE_DEBUG,frame.toHex().toUpper());
            }
            frame = QByteArray((char*)m_bufferReceive.GetBuffer(nStart),nFrameLength);
            ShowMessage(SHOWMESSAGE_TYPE_RECEIVE,frame.toHex().toUpper());
            m_bufferReceive.Fifo(nStart+nFrameLength);
            ProcessRespond_Buffer(frame);
            nStart=-1;//重新再来
        }
    }
    if(m_bufferReceive.GetInfoSize()>MAX_FRAME)
    {
        m_bufferReceive.SetInfoSize(0);
//        Q_ASSERT(false);
    }
}
void CProtocolDeliverModbusTcp::ProcessRespond_Buffer(QByteArray datas)
{
    if (!datas.isEmpty())
    {
        m_frameQUery = datas;
        ///处理接收到的报文,组织需要返回的报文并发送组织好的报文
//        ShowMessage(1,datas.toHex());///显示接收的报文

        if (datas.count() < 12)
        {
            return;
        }
        datas.remove(0,6);

        QByteArray Respond;
        if (MakeRespond(datas,Respond))
        {
            SendFrame(Respond);
        }

    }else
    {
        return;
    }
}
void CProtocolDeliverModbusTcp::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nTimerID)
    {
//        ProcessRespond();
    }
}
int CProtocolDeliverModbusTcp::IsAFrame(BYTE *pInfo, int nInfoLength)
{
    if (0x00 != GetUInt(pInfo,nInfoLength,2,1))
    {
        return 0;
    }
    if (0x00 != GetUInt(pInfo,nInfoLength,3,1))
    {
        return 0;
    }
    int nLength = GetUInt(pInfo,nInfoLength,5,1);
    if (nLength+6 <= nInfoLength)
    {
        return nLength+6;///< 返回长度
    }else
    {
        return 0;
    }
}
unsigned int CProtocolDeliverModbusTcp::GetUInt(const BYTE *pBufferHead, const int nInfomationSize, const int nStartByte, const int nSize ) const
{
    Q_ASSERT(nStartByte >= 0);
    Q_ASSERT(nSize > 0);
    Q_ASSERT((nStartByte+nSize) <= nInfomationSize);

    unsigned int nReturnValue = 0;
    if (nStartByte>=0 && nStartByte+nSize<=nInfomationSize)
    {
        switch (nSize)
        {
        case 1:
            nReturnValue = *(unsigned char *)(pBufferHead + nStartByte);
            break;
        case 2:
            nReturnValue = *(unsigned short int *)(pBufferHead + nStartByte);
            break;
        case 4:
            nReturnValue = *(unsigned int *)(pBufferHead + nStartByte);
            break;
        case 3://不常见
            nReturnValue = *(unsigned int *)(pBufferHead + nStartByte) & 0xFFFFFF;
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
    return nReturnValue;
}
/*!
 * \brief  功能概述 是否成功连接的槽函数
 * \param  参数描述 bool类型的参数是代表是否成功连接的状态，如果为true则成功连接，如果为false则未成功连接
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CProtocolDeliverModbusTcp::slotConnect(bool)
{

}

#if 1
/*!
 * \brief  功能概述 初始化功能码1,2,3,4,5,6,16的Map
 * \param  参数描述 DeviceListElement_是通道配置文件中的设备列表节点<DeviceList>
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
/*!
 \brief CGI 使用

 \fn CProtocolDeliverModbusTcp::SetFunctionMap
 \param DeviceListElement_
*/
void CProtocolDeliverModbusTcp::SetFunctionMap(const QDomElement &DeviceListElement_)
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
        QDomNodeList TypeNodeList = DeviceElement.elementsByTagName("Type");
        for (int nType = 0; nType < TypeNodeList.count(); ++nType)
        {
            QDomElement TypeElem = TypeNodeList.at(nType).toElement();
            if (TypeElem.attribute("Name") == "YX"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_DI,DeviceAddress);
                }
            }else if (TypeElem.attribute("Name") == "YC"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_AI,DeviceAddress);
                }
            }else if (TypeElem.attribute("Name") == "YM"){
//                Q_ASSERT(false);
            }else if (TypeElem.attribute("Name") == "YK"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_DO,DeviceAddress);
                }
            }else if (TypeElem.attribute("Name") == "YS"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_AO,DeviceAddress);
                }
            }else{
                Q_ASSERT(false);
            }
        }///end for (int nType = 0; nType < TypeNodeList.count(); ++nType)
        break;///<zzy 2015/1/15 修改:目前转发通道只支持转发一个设备的点
    }
}

void CProtocolDeliverModbusTcp::CGI_SetFunctionMap(const QDomElement &DeviceListElement_)
{
    QDomNodeList DeviceList = DeviceListElement_.childNodes();
    for (int i = 0; i <DeviceList.count(); ++i)
    {
        QDomElement DeviceElement = DeviceList.at(i).toElement();
        if (DeviceElement.elementsByTagName("Device_Address").count() < 1)
        {
//            Q_ASSERT(false);
        }
        int DeviceAddress = DeviceElement.elementsByTagName("Device_Address").at(0).toElement().attribute("Device_Address").toInt();
        QDomNodeList TypeNodeList = DeviceElement.elementsByTagName("Type");
        for (int nType = 0; nType < TypeNodeList.count(); ++nType)
        {
            QDomElement TypeElem = TypeNodeList.at(nType).toElement();
            if (TypeElem.attribute("Name") == "YX"){/// 遥信点
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_DI,DeviceAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointBaseModbus *pPoint = new CPointBaseModbus;
                    int RegisterAddress = PointElem_.attribute("Device_YX_RegisterAddress_Strings").toInt();
                    int Function = PointElem_.attribute("Device_YX_Function_Strings").toInt();
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
                    pPoint->m_nDeviceAddr = DeviceAddress;
                    pPoint->m_nRegisterAddress = RegisterAddress;
                    pPoint->m_nFunction = Function;
                    pPoint->m_nRegisterCount = PointElem_.attribute("Device_YX_RegisterCount_Strings").toInt();
                    qDebug()<<"CProtocolDeliverModbusTcp YX"<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<pPoint->m_nFunction<<pPoint->m_nRegisterAddress;

                    int nDatatype;
                    QString strDataType = PointElem_.attribute("Device_YX_ValueType_Strings");
                    if (strDataType == "无符号型"){
                        nDatatype = DATA_TYPE_INT32U;
                    }else if (strDataType == "有符号型"){
                        nDatatype = DATA_TYPE_INT32S;
                    }else if (strDataType == "浮点型"){
                        nDatatype = DATA_TYPE_FP32;
                    }else if (strDataType == "布尔型"){
                        nDatatype = DATA_TYPE_BOOLEAN;
                    }else{
                        nDatatype = PointElem_.attribute("Device_YX_ValueType_Strings").toInt();
                    }

                    pPoint->m_nDataType = nDatatype;
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetNotify(pPoint);///设置转发点
                    }
                    if (!RXFunctinMap.contains(RegisterAddress))
                    {
                        RXFunctinMap.insert(RegisterAddress,pPoint);
                    }
                }
            }else if (TypeElem.attribute("Name") == "YC"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_AI,DeviceAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointBaseModbus *pPoint = new CPointBaseModbus;
                    int RegisterAddress = PointElem_.attribute("Device_YC_RegisterAddress_Strings").toInt();
                    int Function = PointElem_.attribute("Device_YC_Function_Strings").toInt();
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
                    pPoint->m_nDeviceAddr = DeviceAddress;
                    pPoint->m_nRegisterAddress = RegisterAddress;
                    pPoint->m_nFunction = Function;
                    pPoint->m_nRegisterCount = PointElem_.attribute("Device_YC_RegisterCount_Strings").toInt();
                    qDebug()<<"CProtocolDeliverModbusTcp YC"<<pPoint->m_strRTDBName<<pPoint->m_strPointName<<pPoint->m_nFunction<<pPoint->m_nRegisterAddress;

                    int nDatatype;
                    QString strDataType = PointElem_.attribute("Device_YC_ValueType_Strings");
                    if (strDataType == "无符号型"){
                        nDatatype = DATA_TYPE_INT32U;
                    }else if (strDataType == "有符号型"){
                        nDatatype = DATA_TYPE_INT32S;
                    }else if (strDataType == "浮点型"){
                        nDatatype = DATA_TYPE_FP32;
                    }else if (strDataType == "布尔型"){
                        nDatatype = DATA_TYPE_BOOLEAN;
                    }else{
                        nDatatype = PointElem_.attribute("Device_YC_ValueType_Strings").toInt();
                    }

                    pPoint->m_nDataType = nDatatype;
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetNotify(pPoint);///设置转发点
                    }
                    if (!RCFunctinMap.contains(RegisterAddress))
                    {
                        RCFunctinMap.insert(RegisterAddress,pPoint);
                    }
                }
            }else if (TypeElem.attribute("Name") == "YM"){
//                Q_ASSERT(false);
            }else if (TypeElem.attribute("Name") == "YK"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_DO,DeviceAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointBaseModbus *pPoint = new CPointBaseModbus;
                    int RegisterAddress = PointElem_.attribute("Device_YK_RegisterAddress_Strings").toInt();
                    int Function = PointElem_.attribute("Device_YK_Function_Strings").toInt();
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
                    pPoint->m_nDeviceAddr = DeviceAddress;
                    pPoint->m_nRegisterAddress = RegisterAddress;
                    pPoint->m_nFunction = Function;
                    pPoint->m_nRegisterCount = PointElem_.attribute("Device_YK_RegisterCount_Strings").toInt();

                    int nDatatype;
                    QString strDataType = PointElem_.attribute("Device_YK_ValueType_Strings");
                    if (strDataType == "无符号型"){
                        nDatatype = DATA_TYPE_INT32U;
                    }else if (strDataType == "有符号型"){
                        nDatatype = DATA_TYPE_INT32S;
                    }else if (strDataType == "浮点型"){
                        nDatatype = DATA_TYPE_FP32;
                    }else if (strDataType == "布尔型"){
                        nDatatype = DATA_TYPE_BOOLEAN;
                    }else{
                        nDatatype = PointElem_.attribute("Device_YK_ValueType_Strings").toInt();
                    }

                    pPoint->m_nDataType = nDatatype;
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetNotify(pPoint);///设置转发点
                    }
                    if (!RKFunctinMap.contains(RegisterAddress))
                    {
                        RKFunctinMap.insert(RegisterAddress,pPoint);
                    }
                }
            }else if (TypeElem.attribute("Name") == "YS"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
//                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_AO,DeviceAddress);
                    QDomElement PointElem_ = TypeElem.childNodes().at(nPoint).toElement();
                    CPointBaseModbus *pPoint = new CPointBaseModbus;
                    int RegisterAddress = PointElem_.attribute("Device_YS_RegisterAddress_Strings").toInt();
                    int Function = PointElem_.attribute("Device_YS_Function_Strings").toInt();
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
                    pPoint->m_nDeviceAddr = DeviceAddress;
                    pPoint->m_nRegisterAddress = RegisterAddress;
                    pPoint->m_nFunction = Function;
                    pPoint->m_nRegisterCount = PointElem_.attribute("Device_YS_RegisterCount_Strings").toInt();

                    int nDatatype;
                    QString strDataType = PointElem_.attribute("Device_YS_ValueType_Strings");
                    if (strDataType == "无符号型"){
                        nDatatype = DATA_TYPE_INT32U;
                    }else if (strDataType == "有符号型"){
                        nDatatype = DATA_TYPE_INT32S;
                    }else if (strDataType == "浮点型"){
                        nDatatype = DATA_TYPE_FP32;
                    }else if (strDataType == "布尔型"){
                        nDatatype = DATA_TYPE_BOOLEAN;
                    }else{
                        nDatatype = PointElem_.attribute("Device_YS_ValueType_Strings").toInt();
                    }

                    pPoint->m_nDataType = nDatatype;
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetNotify(pPoint);///设置转发点
                    }
                    if (!RSFunctinMap.contains(RegisterAddress))
                    {
                        RSFunctinMap.insert(RegisterAddress,pPoint);
                    }
                }
            }else{
                Q_ASSERT(false);
            }
        }///end for (int nType = 0; nType < TypeNodeList.count(); ++nType)
        break;///<zzy 2015/1/15 修改:目前转发通道只支持转发一个设备的点
    }
}
#else
/*!
 * \brief  功能概述 初始化功能码1,2,3,4,5,6,16的Map
 * \param  参数描述 DeviceListElement_是通道配置文件中的设备列表节点<DeviceList>
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CProtocolDeliverModbusTcp::SetFunctionMap(const QDomElement &DeviceListElement_)
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
        QDomNodeList TypeNodeList = DeviceElement.elementsByTagName("Type");
        for (int nType = 0; nType < TypeNodeList.count(); ++nType)
        {
            QDomElement TypeElem = TypeNodeList.at(nType).toElement();
            if (TypeElem.attribute("Name") == "YX"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_DI,DeviceAddress);
                }
            }else if (TypeElem.attribute("Name") == "YC"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_AI,DeviceAddress);
                }
            }else if (TypeElem.attribute("Name") == "YM"){
                Q_ASSERT(false);
            }else if (TypeElem.attribute("Name") == "YK"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_DO,DeviceAddress);
                }
            }else if (TypeElem.attribute("Name") == "YS"){
                int nPointCount = TypeElem.childNodes().count();
                for (int nPoint = 0; nPoint < nPointCount; ++nPoint)
                {
                    InsertPoint(TypeElem.childNodes().at(nPoint).toElement(),TAG_TYPE_AO,DeviceAddress);
                }
            }else{
                Q_ASSERT(false);
            }
        }///end for (int nType = 0; nType < TypeNodeList.count(); ++nType)
        break;///<zzy 2015/1/15 修改:目前转发通道只支持转发一个设备的点
    }
}
#endif

/*!
 * \brief  功能概述 向map中添加点
 * \param  参数描述 PointElem_是设备点的节点，nPointType是点的类型，nDeviceAddress是该点的设备地址
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CProtocolDeliverModbusTcp::InsertPoint(const QDomElement &PointElem_, const int &nPointType, const int &nDeviceAddress)
{
    CPointBaseModbus *pPoint = new CPointBaseModbus;
    int RegisterAddress = PointElem_.attribute("RegisterAddress").toInt();
    int Function = PointElem_.attribute("Function").toInt();
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
    pPoint->m_nDeviceAddr = nDeviceAddress;
    pPoint->m_nRegisterAddress = RegisterAddress;
    pPoint->m_nFunction = Function;
    pPoint->m_nRegisterCount = PointElem_.attribute("RegisterCount").toInt();

    int nDatatype;
    QString strDataType = PointElem_.attribute("ValueType");
    if (strDataType == "无符号整型"){
        nDatatype = DATA_TYPE_INT32U;
    }else if (strDataType == "有符号整型"){
        nDatatype = DATA_TYPE_INT32S;
    }else if (strDataType == "浮点型"){
        nDatatype = DATA_TYPE_FP32;
    }else if (strDataType == "布尔型"){
        nDatatype = DATA_TYPE_BOOLEAN;
    }else{
        nDatatype = PointElem_.attribute("ValueType").toInt();
    }

    pPoint->m_nDataType = nDatatype;
    pPoint->m_pProtocol = this;
    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
    if (pPoint->m_pTag)
    {
        pPoint->m_pTag->SetNotify(pPoint);///设置转发点
    }
    switch (nPointType) {
    case TAG_TYPE_DI:///遥信
        RXFunctinMap.insert(RegisterAddress,pPoint);
        break;
    case TAG_TYPE_AI:///遥测
//        qDebug()<<RegisterAddress<<pPoint->m_strRTDBName<<pPoint->m_strPointName;
        RCFunctinMap.insert(RegisterAddress,pPoint);
        break;
    case TAG_TYPE_MEM:///遥脉
        Q_ASSERT(false);
        break;
    case TAG_TYPE_DO:///遥控
        RKFunctinMap.insert(RegisterAddress,pPoint);
        break;
    case TAG_TYPE_AO:///遥设
        RSFunctinMap.insert(RegisterAddress,pPoint);
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}

/*!
 * \brief  功能概述 生成返回报文
 * \param  参数描述 Command是收到的报文，Respond是返回的报文
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CProtocolDeliverModbusTcp::MakeRespond(const QByteArray &Command, QByteArray &Respond)
{
    bool bReturn = false;
    int nFunctionCode = (BYTE)Command.at(1);
    switch(nFunctionCode)
    {
        case 1: // Read CoilStatus
        case 2: // Read InputStatus
            bReturn = MakeStatusResponse(Command, Respond);
            break;
        case 3: // Read Holding Register
        case 4: // Read Input Register
            bReturn = MakeRegisterResponse(Command, Respond);
            break;
        case 5: // Force Single Coil
            bReturn = ForceSinleCoil(Command, Respond);
            break;
        case 6: // Preset single Register
            bReturn = PresetSinleRegister(Command, Respond);
            break;
        case 16: // Preset single Register
            bReturn = PresetMultipleRegister(Command, Respond);
            break;
        default:
            bReturn = MakeErrorRespond(Command, Respond,EXCEPTION_CODE_ILLEGAL_FUNCTION);
            break;
    }
    return bReturn;
}

/*!
 * \brief  功能概述 生成返回报文
 * \param  参数描述 Command是收到的报文，Respond是返回的报文，pInfo是报文的头指针，nInfoSize是报文的长度，nFunction是功能码
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CProtocolDeliverModbusTcp::MakeResponse(const QByteArray &FrameQuery_, QByteArray &Respond, void *pInfo, int nInfoSize, int nFunction)
{
    //返回格式: {tcp头}=6 (地址) (功能码)	{内容} [CRC校验]
    Q_ASSERT(pInfo != NULL);
    Q_ASSERT(nInfoSize > 0);

    BYTE szBuffer[512] = {0};
    BYTE *pCur = szBuffer;
    *pCur++ = (BYTE)FrameQuery_.at(0);
    *pCur++ = (BYTE)FrameQuery_.at(1);
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 2+nInfoSize;

    *pCur++ = (BYTE)FrameQuery_.at(6);
    *pCur++ = nFunction;
    memcpy(pCur, pInfo, nInfoSize);
    pCur += nInfoSize;
//	SetData(szBuffer,pCur-szBuffer);
    Respond = QByteArray((char *)szBuffer,pCur-szBuffer);
    return pCur-szBuffer > 0;
}

/*!
 * \brief  功能概述 发送报文函数
 * \param  参数描述 SendFrame_是需要发送的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CProtocolDeliverModbusTcp::SendFrame(const QByteArray SendFrame_)
{
    if (m_pLink->write(SendFrame_))
        ShowMessage(0,SendFrame_.toHex());
}

/*!
 * \brief  功能概述 处理读取状态命令，功能码1,2
 * \param  参数描述 Command是接收到的报文，Respond是发送的报文
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CProtocolDeliverModbusTcp::MakeStatusResponse(const QByteArray &Command, QByteArray &Respond)
{
    //命令格式: (地址) (功能码)	[起始地址] [寄存器数目]	 [CRC校验]
    //返回格式: (地址) (功能码)	(字节数) {信息} [CRC校验]
    int nFunction = (BYTE)Command.at(1);
    WORD nStartAddress = GetUIntValue_Hl(Command,2,2);//   MAKEWORD(GetInt8u(3),GetInt8u(2));
    WORD nRegNumber = GetUIntValue_Hl(Command,4,2);// MAKEWORD(GetInt8u(5),GetInt8u(4));

    BYTE szBuffer[512] = {0};
    BYTE *pCur = szBuffer;
    BYTE nBytes = (nRegNumber+7)/8;
    *pCur++ = nBytes;

    for (int i=0; i<nRegNumber; i++)
    {
//        qDebug()<<"nStartAddress+i"<<nStartAddress+i;
        if (RXFunctinMap.value(nStartAddress+i) == NULL)//没有相应的变量
        {
            qDebug()<<"YX Channel Don't have this Point"<<(nStartAddress+i);
            return MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_ADDRESS);///非法数据地址
        }
        CTagI *tag = m_pRTDB->FindTag(RXFunctinMap.value(nStartAddress+i)->m_strRTDBName);
        if (tag == NULL)
        {
            qDebug()<<"YX Don't have this tag."<<RCFunctinMap.value(nStartAddress+i)->m_strRTDBName<<(nStartAddress+i);
            return MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_ADDRESS);///非法数据地址
        }
        unsigned int bValue =0;
        if (tag->GetPointCollection())
        {
            if (tag->GetPointCollection()->GetValue())
            {
                bValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
            }
        }
//        unsigned int bValue = tag->GetPointCollection()->GetValue()->GetVarValue().toUInt();
//        unsigned int bValue = m_TagMap->value(RXFunctinMap.value(nStartAddress+i)->m_strLink)->GetStrValue().toInt();
        if (bValue)
        {
            *(pCur+i/8) = SetBit(*(pCur+i/8),i%8);
        }
        else
        {//这句可以不要
            *(pCur+i/8) = ResetBit(*(pCur+i/8),i%8);
        }
    }
    pCur += nBytes;

    if (pCur-szBuffer < SEND_FRAME_MAX_LENGTH)
    {
        MakeResponse(m_frameQUery,Respond,szBuffer,pCur-szBuffer,nFunction);
    }
    else//超长
    {
        MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_VALUE);///非法的数据值
    }
    return true;
}

/*!
 * \brief  功能概述 处理读取寄存器命令，功能码 3,4
 * \param  参数描述 Command是接收到的报文，Respond是发送的报文
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CProtocolDeliverModbusTcp::MakeRegisterResponse(const QByteArray &Command, QByteArray &Respond)
{
    //命令格式: (地址) (功能码)	[起始地址] [寄存器数目]	 [CRC校验]
    //返回格式: (地址) (功能码)	(字节数) {信息} [CRC校验]
//    qDebug()<<"1111";
    int nFunction = (BYTE)Command.at(1);
//    qDebug()<<"2222";
    WORD nStartAddress = GetUIntValue_Hl(Command,2,2);
//    qDebug()<<"3333";
    WORD nRegNumber = GetUIntValue_Hl(Command,4,2);
//    qDebug()<<"4444";

    BYTE szBuffer[512] = {0};
    BYTE *pCur = szBuffer;
    BYTE nBytes=nRegNumber*2;
    *pCur++ = nBytes;
    for(int i=0; i<nRegNumber; )
    {
        if (RCFunctinMap.value(nStartAddress+i) == NULL)//没有相应的变量
        {
            qDebug()<<"YC Channel Don't have this Point"<<(nStartAddress+i);
            return MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_ADDRESS);///非法数据地址
        }
        {
            QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
            qDebug()<<debug;
        }

        CTagI *tag = m_pRTDB->FindTag(RCFunctinMap.value(nStartAddress+i)->m_strRTDBName);
        if (tag == NULL)
        {
            qDebug()<<"YC Don't have this tag."<<RCFunctinMap.value(nStartAddress+i)->m_strRTDBName<<(nStartAddress+i);
            return MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_ADDRESS);///非法数据地址
        }
        {
            QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
            qDebug()<<debug;
        }
        qDebug()<<"333";
        QVariant value;
        float fValue = 0;
        if (tag->GetPointCollection())
        {
            if (tag->GetPointCollection()->GetValue())
            {
                value = tag->GetPointCollection()->GetValue()->GetVarValue();
                fValue = tag->GetPointCollection()->GetValue()->GetVarValue().toFloat();
            }
        }
        qDebug()<<"value=="<<value<<tag->GetName();
        //modbus 从机地址,功能号,寄存器地址,数据类型,寄存器个数
        qDebug()<<"float fValue = tag->GetPointCollection()->GetValue()->GetVarValue().toFloat();"<<fValue;
        CPointBaseModbus *pPoint = RCFunctinMap.value(nStartAddress+i);
        int nDataType    = pPoint->m_nDataType;
        float fKValue    = pPoint->m_fKValue;
        float fBaseValue = pPoint->m_fBaseValue;
        qDebug()<<"fValue qian=="<<fValue<<fBaseValue<<fKValue;
        fValue = (fValue+fBaseValue)*fKValue;
        qDebug()<<"fValue  hou=="<<fValue<<fBaseValue<<fKValue;
//        qDebug()<<"555";
//        qDebug()<<QByteArray((char *)szBuffer,pCur+2-szBuffer).toHex()<<"aaa"<<fValue;
        switch (RCFunctinMap.value(nStartAddress+i)->m_nRegisterCount)//寄存器个数
        {
        case 1://单寄存器
            switch(nDataType)
            {
            case DATA_TYPE_INT32S://有符号数
            {
                short int nTemp = (short int) fValue;
                *pCur++ = nTemp/256;
                *pCur++ = nTemp%256;
            }
                *(short int *)pCur = (short int)fValue;
                break;
            case DATA_TYPE_INT32U://无符号数
                //*(WORD *)pCur = (WORD) fValue;
            {
                WORD nTemp = (WORD) fValue;
                *pCur++ = nTemp/256;
                *pCur++ = nTemp%256;
            }
                break;
            default:
                return MakeErrorRespond(Command,Respond,4);
            }
//            qDebug()<<QByteArray((char *)szBuffer,pCur-szBuffer).toHex()<<"bbb";
//            ReverseByte(pCur,2,2);
            i++;      //下一寄存器地址
//            qDebug()<<QByteArray((char *)szBuffer,pCur-szBuffer).toHex()<<"ccc";
            break;
        case 2://双寄存器
            switch(nDataType)
            {
            case DATA_TYPE_INT32S://有符号数
            {
//                *(int *)pCur = (int) fValue;

                int nTemp = (int) fValue;
#if defined(Q_OS_WIN)
                *pCur++ = (nTemp&0xFF000000) >> 24;
                *pCur++ = (nTemp&0xFF0000) >> 16;
                *pCur++ = (nTemp&0xFF00) >> 8;
                *pCur++ = (nTemp&0xFF);
#else
                *pCur++ = (nTemp&0xFF);
                *pCur++ = (nTemp&0xFF00) >> 8;
                *pCur++ = (nTemp&0xFF0000) >> 16;
                *pCur++ = (nTemp&0xFF000000) >> 24;
#endif
            }
                break;
            case DATA_TYPE_INT32U://无符号数
            {
//                *(DWORD *)pCur = (DWORD )fValue;

                DWORD nTemp = (DWORD) fValue;
#if defined(Q_OS_WIN)
                *pCur++ = (nTemp&0xFF000000) >> 24;
                *pCur++ = (nTemp&0xFF0000) >> 16;
                *pCur++ = (nTemp&0xFF00) >> 8;
                *pCur++ = (nTemp&0xFF);
#else
                *pCur++ = (nTemp&0xFF);
                *pCur++ = (nTemp&0xFF00) >> 8;
                *pCur++ = (nTemp&0xFF0000) >> 16;
                *pCur++ = (nTemp&0xFF000000) >> 24;
#endif
            }
                break;
            case DATA_TYPE_FP32://浮点数
            {
//                *(float *)pCur = fValue;

                int  nTemp = (int)fValue;
#if defined(Q_OS_WIN)
                *pCur++ = (nTemp & 0xFF0000000) >> 24;
                *pCur++ = (nTemp & 0xFF0000) >> 16;
                *pCur++ = (nTemp & 0xFF00) >> 8;
                *pCur++ = (nTemp & 0xFF);
#else
                *pCur++ = (nTemp & 0xFF);
                *pCur++ = (nTemp & 0xFF00) >> 8;
                *pCur++ = (nTemp & 0xFF0000) >> 16;
                *pCur++ = (nTemp & 0xFF0000000) >> 24;
#endif
//                *pCur++ = (nTemp & 0xFF0000000) >> 24;
//                *pCur++ = (nTemp & 0xFF0000) >> 16;
//                *pCur++ = (nTemp & 0xFF00) >> 8;
//                *pCur++ = (nTemp & 0xFF);
            }
                break;
            default:
                return MakeErrorRespond(Command,Respond,4);
            }
//            ReverseByte(pCur,4,4);
            i+=2;      //下一寄存器地址
//            pCur += 4;  //下一内存地址
            break;
        default://更多的寄存器
            return MakeErrorRespond(Command,Respond,4);
        }
    }

    if (pCur-szBuffer < SEND_FRAME_MAX_LENGTH)
    {
        MakeResponse(m_frameQUery,Respond,szBuffer,pCur-szBuffer,nFunction);
    }
    else//超长
    {
        return MakeErrorRespond(Command,Respond,3);
    }
    return true;
}
#if 0
{
    //命令格式: (地址) (功能码)	[起始地址] [寄存器数目]	 [CRC校验]
    //返回格式: (地址) (功能码)	(字节数) {信息} [CRC校验]
    int nFunction = (BYTE)Command.at(1);
    WORD nStartAddress = GetUIntValue_Hl(Command,2,2);
    WORD nRegNumber = GetUIntValue_Hl(Command,4,2);

    BYTE szBuffer[512] = {0};
    BYTE *pCur = szBuffer;
    BYTE nBytes=nRegNumber*2;
    *pCur++ = nBytes;
    for(int i=0; i<nRegNumber; )
    {
        if (RCFunctinMap.value(nStartAddress+i) == NULL)//没有相应的变量
        {
            qDebug()<<"YC channel don't have this point"<<(nStartAddress+i);
            return MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_ADDRESS);///非法数据地址
        }
//        qDebug()<<"111";

        CTagI *tag = m_pRTDB->FindTag(RCFunctinMap.value(nStartAddress+i)->m_strRTDBName);
        if (tag == NULL)
        {
            qDebug()<<"YC channel don't have this quan ju point"<<RCFunctinMap.value(nStartAddress+i)->m_strRTDBName;
            return MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_ADDRESS);///非法数据地址
        }

//        qDebug()<<"333";
        QVariant value = tag->GetPointCollection()->GetValue()->GetVarValue();
        qDebug()<<"value=="<<value<<tag->GetName();
        float fValue = tag->GetPointCollection()->GetValue()->GetVarValue().toFloat();
        //modbus 从机地址,功能号,寄存器地址,数据类型,寄存器个数
        qDebug()<<"float fValue = tag->GetPointCollection()->GetValue()->GetVarValue().toFloat();"<<fValue;
        CPointBaseModbus *pPoint = RCFunctinMap.value(nStartAddress+i);
        int nDataType    = pPoint->m_nDataType;
        float fKValue    = pPoint->m_fKValue;
        float fBaseValue = pPoint->m_fBaseValue;
        qDebug()<<"fValue qian=="<<fValue;
        fValue = (fValue+fBaseValue)*fKValue;
        qDebug()<<"fValue  hou=="<<fValue;
//        qDebug()<<"555";
//        qDebug()<<QByteArray((char *)szBuffer,pCur+2-szBuffer).toHex()<<"aaa"<<fValue;
        switch (RCFunctinMap.value(nStartAddress+i)->m_nRegisterCount)//寄存器个数
        {
        case 1://单寄存器
            switch(nDataType)
            {
            case DATA_TYPE_INT32S://有符号数
            {
                short int nTemp = (short int) fValue;
                *pCur++ = nTemp/256;
                *pCur++ = nTemp%256;
            }
                *(short int *)pCur = (short int)fValue;
                break;
            case DATA_TYPE_INT32U://无符号数
                //*(WORD *)pCur = (WORD) fValue;
            {
                WORD nTemp = (WORD) fValue;
                *pCur++ = nTemp/256;
                *pCur++ = nTemp%256;
            }
                break;
            default:
                return MakeErrorRespond(Command,Respond,4);
            }
//            qDebug()<<QByteArray((char *)szBuffer,pCur-szBuffer).toHex()<<"bbb";
//            ReverseByte(pCur,2,2);
            i++;      //下一寄存器地址
//            qDebug()<<QByteArray((char *)szBuffer,pCur-szBuffer).toHex()<<"ccc";
            break;
        case 2://双寄存器
            switch(nDataType)
            {
            case DATA_TYPE_INT32S://有符号数
            {
//                *(int *)pCur = (int) fValue;

                int nTemp = (int) fValue;
                *pCur++ = (nTemp&0xFF000000) >> 24;
                *pCur++ = (nTemp&0xFF0000) >> 16;
                *pCur++ = (nTemp&0xFF00) >> 8;
                *pCur++ = (nTemp&0xFF);
            }
                break;
            case DATA_TYPE_INT32U://无符号数
            {
//                *(DWORD *)pCur = (DWORD )fValue;

                DWORD nTemp = (DWORD) fValue;
                *pCur++ = (nTemp&0xFF000000) >> 24;
                *pCur++ = (nTemp&0xFF0000) >> 16;
                *pCur++ = (nTemp&0xFF00) >> 8;
                *pCur++ = (nTemp&0xFF);
            }
                break;
            case DATA_TYPE_FP32://浮点数
            {
//                *(float *)pCur = fValue;

                int  nTemp = (int)fValue;
                *pCur++ = (nTemp & 0xFF0000000) >> 24;
                *pCur++ = (nTemp & 0xFF0000) >> 16;
                *pCur++ = (nTemp & 0xFF00) >> 8;
                *pCur++ = (nTemp & 0xFF);
            }
                break;
            default:
                return MakeErrorRespond(Command,Respond,4);
            }
//            ReverseByte(pCur,4,4);
            i+=2;      //下一寄存器地址
//            pCur += 4;  //下一内存地址
            break;
        default://更多的寄存器
            return MakeErrorRespond(Command,Respond,4);
        }
    }

    if (pCur-szBuffer < 250)
    {
        MakeResponse(m_frameQUery,Respond,szBuffer,pCur-szBuffer,nFunction);
    }
    else//超长
    {
        return MakeErrorRespond(Command,Respond,3);
    }
    return true;
}
#endif

/*!
 * \brief  功能概述 处理设置状态命令，功能码5
 * \param  参数描述 Command是接收到的报文，Respond是发送的报文
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CProtocolDeliverModbusTcp::ForceSinleCoil(const QByteArray &Command, QByteArray &Respond)
{
    //1 bit  //Force Single Coil
    //格式: (地址) (功能码)	[起始地址]  [内容] [CRC校验]
    //返回: 与下发相同
    WORD nStartAddress = GetUIntValue_Hl(Command,2,2);
    WORD nValue = GetUIntValue_Hl(Command,4,2);
    QString strLink = "";
    CPointBaseModbus *pPointDeliver = RKFunctinMap.value(nStartAddress);
    if (pPointDeliver == NULL)//没有相应的变量
    {
        qDebug()<<"5 遥控通道中没有此变量";
        return MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_ADDRESS);///非法数据地址
    }
    CPointBaseModbus *pPoint = RKFunctinMap.value(nStartAddress);
    strLink = RKFunctinMap.value(nStartAddress)->m_strRTDBName;
    CTagI *tag = m_pRTDB->FindTag(strLink);
    if (tag == NULL)
    {
        qDebug()<<"5 遥控没有此全局变量";
        return MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_ADDRESS);///非法数据地址
    }

    if (nValue == 0xff00)//合
    {
        nValue = 1;
    }
    else if (nValue == 0)//分
    {
        nValue = 0;
    }
    else
    {
        return MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_VALUE);
    }
//    QString result;
//    CValueI *value = tag->GetPointCollection()->GetValue();
//    int n_ScanTime = pPointDeliver->m_nScanTime;//超时时间
    QVariant variant(nValue);

//    tag->GetPointCollection()->OnValueChange(variant);///< ZZY 2015/10/19 修改:
    tag->SetValue(pPoint,variant,variant);///< ZZY 2015/10/19 修改: 修改为SetValue
    int nConfig = CONTROL_SUCCEED;

//    bool nRet = value->WaitResult(result,10000);
//    int nConfig = value->GetnControlFlag();
//    CONTROL_UNDEFINE = -1,
//    CONTROL_FAILURE,//1
//    CONTROL_SUCCEED,//2
//    CONTROL_TIMEOUT,//3
    switch (nConfig) {
    case CONTROL_UNDEFINE:
        MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_VALUE);
        break;
    case CONTROL_FAILURE:
        MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_VALUE);
        break;
    case CONTROL_SUCCEED:
        MakeResponse(m_frameQUery,Respond,m_frameQUery.mid(8,4).data(),4,5);
        break;
    case CONTROL_TIMEOUT:
        MakeErrorRespond(Command,Respond,EXCEPTION_CODE_SLAVE_DEVICE_FAILURE);
        break;
    default:
        break;
    }
    return true;
}

/*!
 * \brief  功能概述 处理设置值命令，功能码6
 * \param  参数描述 Command是接收到的报文，Respond是发送的报文
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CProtocolDeliverModbusTcp::PresetSinleRegister(const QByteArray &Command, QByteArray &Respond)
{//1 bit  //Force Single Coil
    //格式: (地址) (功能码)	[起始地址]  [内容] [CRC校验]
    //返回: 与下发相同
    WORD nStartAddress = GetUIntValue_Hl(Command,2,2);
    WORD nValue = GetUIntValue_Hl(Command,4,2);
    QString strLink = "";
    CPointBaseModbus *pPointDeliver = RKFunctinMap.value(nStartAddress);
    if (pPointDeliver == NULL)//没有相应的变量
    {
        qDebug()<<"6 YK Channel Don't have this point.";
        return MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_ADDRESS);///非法数据地址
    }
    CPointBaseModbus* pPoint = RKFunctinMap.value(nStartAddress);
    strLink = RKFunctinMap.value(nStartAddress)->m_strRTDBName;
    CTagI *tag = m_pRTDB->FindTag(strLink);
    if (tag == NULL)
    {
        qDebug()<<"6 YK Channel Don't have this Tag.";
        return MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_ADDRESS);///非法数据地址
    }

    if (nValue == 0xff00)//合
    {
        nValue = 1;
    }
    else if (nValue == 0)//分
    {
        nValue = 0;
    }
    else
    {
        return MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_VALUE);
    }
//    QString result;
//    CValueI *value = tag->GetPointCollection()->GetValue();
    QVariant variant(nValue);

//    tag->GetPointCollection()->OnValueChange(variant);
    tag->SetValue(pPoint,variant,variant);///< ZZY 2015/10/19 修改: 修改为SetValue
    int nConfig = CONTROL_SUCCEED;
//    bool nRet = value->WaitResult(result,n_ScanTime);
//    bool nRet = value->WaitResult(result,10000);
//    int nConfig = value->GetnControlFlag();
//    CONTROL_UNDEFINE = -1,
//    CONTROL_FAILURE,//1
//    CONTROL_SUCCEED,//2
//    CONTROL_TIMEOUT,//3
    switch (nConfig) {
    case CONTROL_UNDEFINE:
        MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_VALUE);
        break;
    case CONTROL_FAILURE:
        MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_VALUE);
        break;
    case CONTROL_SUCCEED:
        MakeResponse(m_frameQUery,Respond,m_frameQUery.mid(8,4).data(),4,5);
        break;
    case CONTROL_TIMEOUT:
        MakeErrorRespond(Command,Respond,EXCEPTION_CODE_SLAVE_DEVICE_FAILURE);
        break;
    default:
        break;
    }
    return true;
}

/*!
 * \brief  功能概述 设置多个数据寄存器命令，功能码16，多个字节
 * \param  参数描述 Command是接收到的命令，Respond是合成的命令
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CProtocolDeliverModbusTcp::PresetMultipleRegister(const QByteArray &Command, QByteArray &Respond)
{
    //格式: (地址) (功能码)	[起始地址] [寄存器数目]	(字节数目) {内容} [CRC校验]
    //说明:只能更新功能码为3的量
    WORD nStartAddress = GetUIntValue_Hl(Command,2,2);
    WORD nRegNumber = GetUIntValue_Hl(Command,4,2);
    if (nRegNumber*2+7 > Command.count())
    {
        return false;
    }
    QString strLink = "";

    int nTagRegNumber = 0;
//    qDebug()<<Command.toHex();
    for(int i=0; i<nRegNumber; )
    {
        CPointBaseModbus *pPointDeliver = RSFunctinMap.value(nStartAddress+i);
        if (pPointDeliver == NULL)
        {
            qDebug()<<"16 YS Channel Don't have this point.";
            return MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_ADDRESS);///非法数据地址
        }
        strLink = pPointDeliver->m_strRTDBName;
        CTagI *tag = m_pRTDB->FindTag(strLink);
        if (tag == NULL)
        {
            qDebug()<<"16 YS Channel Don't have this tag.";
            return MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_ADDRESS);///非法数据地址
        }
        nTagRegNumber = pPointDeliver->m_nRegisterCount;
//        int nValue = GetUIntValue_Hl(Command,7+i*2,nTagRegNumber*2);//在这认为下发的数据是无符号数
//        float fValue = (float)GetUIntValue_Hl(Command,7+i*2,nTagRegNumber*2);//在这认为下发的数据是无符号数
        int nDataType = pPointDeliver->m_nDataType;
//        qDebug()<<"数据类型"<<nDataType;
        switch (pPointDeliver->m_nRegisterCount)//寄存器个数
        {
        case 1://单寄存器
            switch(nDataType)
            {
            case DATA_TYPE_INT32S://有符号数
            {
//                QString result;
//                CValueI *value = tag->GetPointCollection()->GetValue();
//                int n_ScanTime = pPointDeliver->m_nScanTime;//超时时间

                QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
                qDebug()<<debug<<Command.toHex()<<7+i*2<<nTagRegNumber*2;
                int nValue = GetUIntValue_Hl(Command,7+i*2,nTagRegNumber*2);//在这认为下发的数据是无符号数
                QVariant variant(nValue);

//                tag->GetPointCollection()->OnValueChange(variant);
                tag->SetValue(pPointDeliver,variant,variant);
                int nConfig = CONTROL_SUCCEED;
//                bool nRet = value->WaitResult(result,10000);
//                int nConfig = value->GetnControlFlag();
            //    CONTROL_UNDEFINE = -1,
            //    CONTROL_FAILURE,//1
            //    CONTROL_SUCCEED,//2
            //    CONTROL_TIMEOUT,//3
                switch (nConfig) {
                case CONTROL_UNDEFINE:
                    MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_VALUE);
                    break;
                case CONTROL_FAILURE:
                    MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_VALUE);
                    break;
                case CONTROL_SUCCEED:
                    MakeResponse(m_frameQUery,Respond,m_frameQUery.mid(8,4).data(),4,16);
//                    MakeResponse(m_frameQUery,Respond,m_frameQUery.mid(8,4).data(),4,5);
                    break;
                case CONTROL_TIMEOUT:
                    MakeErrorRespond(Command,Respond,EXCEPTION_CODE_SLAVE_DEVICE_FAILURE);
                    break;
                default:
                    break;
                }
            }
                break;
            case DATA_TYPE_INT32U://无符号数
            {
//                QString result;
//                CValueI *value = tag->GetPointCollection()->GetValue();
//                int n_ScanTime = pPointDeliver->m_nScanTime;//超时时间
                QString debug = QString("%1 %2 %3 :").arg(__FILE__).arg(__LINE__).arg(__func__);
                qDebug()<<debug<<Command.toHex()<<7+i*2<<nTagRegNumber*2;
                unsigned int uValue = GetUIntValue_Hl(Command,7+i*2,nTagRegNumber*2);//在这认为下发的数据是无符号数
                QVariant variant(uValue);

//                tag->GetPointCollection()->OnValueChange(variant);

                tag->SetValue(pPointDeliver,variant,variant);
                int nConfig = CONTROL_SUCCEED;
//                bool nRet = value->WaitResult(result,10000);
//                int nConfig = value->GetnControlFlag();
            //    CONTROL_UNDEFINE = -1,
            //    CONTROL_FAILURE,//1
            //    CONTROL_SUCCEED,//2
            //    CONTROL_TIMEOUT,//3
                switch (nConfig) {
                case CONTROL_UNDEFINE:
                    MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_VALUE);
                    break;
                case CONTROL_FAILURE:
                    MakeErrorRespond(Command,Respond,EXCEPTION_CODE_ILLEGAL_DATA_VALUE);
                    break;
                case CONTROL_SUCCEED:
                    MakeResponse(m_frameQUery,Respond,m_frameQUery.mid(8,4).data(),4,16);
//                    MakeResponse(m_frameQUery,Respond,m_frameQUery.mid(8,4).data(),4,5);
                    break;
                case CONTROL_TIMEOUT:
                    MakeErrorRespond(Command,Respond,EXCEPTION_CODE_SLAVE_DEVICE_FAILURE);
                    break;
                default:
                    break;
                }
            }
                break;
            default:
                return MakeErrorRespond(Command,Respond,4);
            }
            i++;      //下一寄存器地址
            break;
        case 2://双寄存器
            switch(nDataType)
            {
            case DATA_TYPE_INT32S://有符号数
//                count = m_TagMap->value(strLink)->m_cai_zhuan_deviceaddress.count();
//                for (int i = 0; i < count;++i)
//                {
//                    if (m_TagMap->value(strLink)->m_cai_zhuan_deviceaddress.at(i)->ZHUANFA_number == m_nChannelNumber)
//                    {
//                        caijiChannelNo = m_TagMap->value(strLink)->m_cai_zhuan_deviceaddress.at(i)->CAIJI_number;
//                        nDeviceAddr = m_TagMap->value(strLink)->m_cai_zhuan_deviceaddress.at(i)->DEVICEADDRESS;
//                        m_TagMap->value(strLink)->Control(caijiChannelNo,m_nChannelNumber,nDeviceAddr,YS,strLink,QString::number(fValue));
//                    }
//                }
                break;
            case DATA_TYPE_INT32U://无符号数
//                count = m_TagMap->value(strLink)->m_cai_zhuan_deviceaddress.count();
//                for (int i = 0; i < count;++i)
//                {
//                    if (m_TagMap->value(strLink)->m_cai_zhuan_deviceaddress.at(i)->ZHUANFA_number == m_nChannelNumber)
//                    {
//                        caijiChannelNo = m_TagMap->value(strLink)->m_cai_zhuan_deviceaddress.at(i)->CAIJI_number;
//                        nDeviceAddr = m_TagMap->value(strLink)->m_cai_zhuan_deviceaddress.at(i)->DEVICEADDRESS;
//                        m_TagMap->value(strLink)->Control(caijiChannelNo,m_nChannelNumber,nDeviceAddr,YS,strLink,QString::number(fValue));
//                    }
//                }
                break;
            case DATA_TYPE_FP32://浮点数
//                count = m_TagMap->value(strLink)->m_cai_zhuan_deviceaddress.count();
//                for (int i = 0; i < count;++i)
//                {
//                    if (m_TagMap->value(strLink)->m_cai_zhuan_deviceaddress.at(i)->ZHUANFA_number == m_nChannelNumber)
//                    {
//                        caijiChannelNo = m_TagMap->value(strLink)->m_cai_zhuan_deviceaddress.at(i)->CAIJI_number;
//                        nDeviceAddr = m_TagMap->value(strLink)->m_cai_zhuan_deviceaddress.at(i)->DEVICEADDRESS;
//                        m_TagMap->value(strLink)->Control(caijiChannelNo,m_nChannelNumber,nDeviceAddr,YS,strLink,QString::number(fValue));
//                    }
//                }
                break;
            default:
                return MakeErrorRespond(Command,Respond,4);
            }
            i+=2;      //下一寄存器地址
            break;
        default://更多的寄存器
            return MakeErrorRespond(Command,Respond,4);
        }
    }
//    MakeResponse(m_frameQUery,Respond,m_frameQUery.mid(8,4).data(),4,6);
//    frameRefRespond.SetData(m_frameQuery.GetBuffer(),m_frameQuery.GetInfoSize());//成功 //返回与下发相同
    return true;
}

/*!
 * \brief  功能概述 生成异常报文
 * \param  参数描述 Command是接收到的命令，Respond是合成的命令，nErrorCode是异常码
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CProtocolDeliverModbusTcp::MakeErrorRespond(const QByteArray &Command, QByteArray &Respond, int nErrorCode)
{
//    错误
    //生成错误信息
    //,BYTE nErrorCode  1功能码错误 ,2数据地址错误 ,5 超长  4类型错误  3写入数据非法
    //命令格式: (地址) (功能码)	[起始地址] {内容} [CRC校验]
    //返回格式: (地址) (功能码)	(错误号) [CRC校验]
//    qDebug()<<"MakeErrorRespond="<<Command.count();
    BYTE szBuffer[512] = {0};
    BYTE *pCur = szBuffer;
    *pCur++ = nErrorCode;
    BYTE nFunction = SetBit(Command.at(1),7);
    return MakeResponse(m_frameQUery, Respond,szBuffer, pCur-szBuffer,nFunction);
}

/*!
 * \brief  功能概述 获取报文中的值
 * \param  参数描述 data_是报文，nOffset_是开始的字节，nSize_是需要取的字节
 * \return 返回值描述 返回报文中的值
 * \author zzy
 * \date   2015/5/25
 */
unsigned int CProtocolDeliverModbusTcp::GetUIntValue_Hl(const QByteArray &data_, int nOffset_, int nSize_)
{
    Q_ASSERT(nOffset_>=0 && nOffset_+nSize_<=data_.count());
    unsigned int nReturnValue = 0;
    if (nOffset_>=0 && nOffset_+nSize_<=data_.count())
    {
        switch (nSize_)
        {
        case 1:
            nReturnValue = *(unsigned  char *)(data_.data() + nOffset_);
            break;
        case 2:
            nReturnValue = *(unsigned short int *)(data_.data() + nOffset_);
            ReverseByte(&nReturnValue, 2, 2);
            break;
        case 4:
            nReturnValue = *(unsigned int *)(data_.data() + nOffset_);
            ReverseByte(&nReturnValue, 4, 4);
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
    return nReturnValue;
}

/*!
 * \brief  功能概述 分组交换pInfo中内容，即(12)(34) ->(21)(43)  高低字节交换
 * \param  参数描述 pInfo是内容地址，nInfoSize是字节数，nGroupSize是分组大小
 * \return 返回值描述
 * \author zzy
 * \date   2015/5/25
 */
void CProtocolDeliverModbusTcp::ReverseByte(void *pInfo, int nInfoSize, int nGroupSize)
{
    Q_ASSERT(pInfo);
    Q_ASSERT(nInfoSize > 0);
    Q_ASSERT(nGroupSize > 0);
    Q_ASSERT(nInfoSize >= nGroupSize);
    Q_ASSERT(nInfoSize == (nInfoSize/nGroupSize)*nGroupSize);//信息长度是分组长度的整数倍

    BYTE nTemp;
    for (BYTE *pCur=(BYTE *)pInfo; pCur<(BYTE *)pInfo+nInfoSize; pCur+=nGroupSize)
    {
        for (int i=0; i<nGroupSize/2; i++)
        {
            nTemp = pCur[i];
            pCur[i] = pCur[nGroupSize-i-1];
            pCur[nGroupSize-i-1] = nTemp;
        }
    }
}

/*!
 * \brief  功能概述 设置变量的某一位为1
 * \param  参数描述 nData是要设置的源数据，nBit是要设置的位
 * \return 返回值描述 设置后的结果
 * \author zzy
 * \date   2015/5/25
 */
unsigned int CProtocolDeliverModbusTcp::SetBit(unsigned int nData, unsigned int nBit)
{
    Q_ASSERT(nBit < sizeof(nData)*CHAR_BIT);
    return ((nData) | (1<<(nBit)));
}

/*!
 * \brief  功能概述 设置变量的某一位为0
 * \param  参数描述 nData是要设置的源数据，nBit是要设置的位
 * \return 返回值描述 设置后的结果
 * \author zzy
 * \date   2015/5/25
 */
unsigned int CProtocolDeliverModbusTcp::ResetBit(unsigned int nData, unsigned int nBit)
{
    Q_ASSERT(nBit < sizeof(nData)*CHAR_BIT);
    return ((nData) & (~(1<<(nBit))));
}


/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
ModbusRtuQuery::ModbusRtuQuery()
{
    szSend = "";
    m_nResponseLength = -1;
    m_nStartAddress = -1;
}
