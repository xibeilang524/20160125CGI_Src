/*!
 * \file    CModbusTcpProtocol.cpp
 * \brief   概述 CModbusTcpProtocol类的源文件
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

#include <QTimer>
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QDir>

#include "cmodbustcpprotocol.h"
#include "../PortInclude/CRTDBI.h"
#include "../PortInclude/CTagI.h"
#include "../PortInclude/cvaluebase.h"
#include "../PortInclude/CPRTI.h"
#include "../PortInclude/CPRTMapI.h"

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
    CModbusTcpProtocol *testProtocol = new CModbusTcpProtocol(parent);
    return  testProtocol;
}

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 parent是父对象的指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
CModbusTcpProtocol::CModbusTcpProtocol(QObject */*parent*/)
{

    m_pTimer = new QTimer;
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(OnTimeOut()));
    m_nLastTime = 0;
}

/*!
 * \brief  功能概述 析构函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
CModbusTcpProtocol::~CModbusTcpProtocol()
{
    //    ModbusDebugFileMessage(QTime::currentTime().toString("hh:mm:ss.zzz")+"----------------------关闭程序----------------------------");
}

void CModbusTcpProtocol::timerEvent(QTimerEvent *evnet)
{
    if (evnet->timerId() == m_nTimerID)
    {
        Polling();
    }
}

/*!
 * \brief  功能概述 创建此通道
 * \param  参数描述 strChannelFileName_是通道文件绝对路径名，pDatabaseReturn是实时库指针
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CModbusTcpProtocol::OnCreateChannel(const QString strChannelFileName_, CRTDBI *pDatabaseReturn)
{
    SetPROTOCOL_TYPE(Collect_Type);
    if (!CProtocolBase::OnCreateChannel(strChannelFileName_,pDatabaseReturn))
        return false;

    QFile file(strChannelFileName_);
    QDomDocument ChannelDoc;
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }
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
    if (docElem.childNodes().count() != 5)
        return false;
    QDomElement DeviceList = docElem.childNodes().at(4).toElement();


    SetFunctionMap(DeviceList);///初始化点表Map
    SetInitSend_RX_RC_FrameList();///初始化采集信息报文

    m_pTimer->start(300);
//    m_strTcpIp = MainPort.attribute("IP");
//    m_nPort = MainPort.attribute("Port").toInt();

//    OnOpenSocket();

    return true;
}

bool CModbusTcpProtocol::OnCreateChannel(const QDomElement &ChannelElement_, ChannelType nChannelType_, int nComOrCanNumber_, CRTDBI *pDatabaseReturn_)
{
    SetPROTOCOL_TYPE(Collect_Type);
    if (!CProtocolBase::OnCreateChannel(ChannelElement_,nChannelType_,nComOrCanNumber_,pDatabaseReturn_))
        return false;

    if (ChannelElement_.elementsByTagName("DeviceList").isEmpty())
        return false;
    QDomElement DeviceList = ChannelElement_.elementsByTagName("DeviceList").at(0).toElement();


    CGI_SetFunctionMap(DeviceList);///初始化点表Map
    SetInitSend_RX_RC_FrameList();///初始化采集信息报文

//    m_pTimer->start(100);
    m_nTimerID = startTimer(300);
//    m_strTcpIp = MainPort.attribute("IP");
//    m_nPort = MainPort.attribute("Port").toInt();

//    OnOpenSocket();

    return true;
}

/*!
 * \brief  功能概述 初始化Device_1_2_Map和Device_3_4_Map 功能码1、2、3、4的Map，初始化功能码1、2、3、4的Map
 * \param  参数描述 DeviceListElement_是通道配置文件中的设备列表节点<DeviceList>
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusTcpProtocol::SetFunctionMap(const QDomElement &DeviceListElement_)
{
    QDomNodeList DeviceList = DeviceListElement_.childNodes();
    for (int i = 0; i <DeviceList.count(); ++i)
    {
        QDomElement DeviceElement = DeviceList.at(i).toElement();
        int DeviceAddress;

        QMap<int,CPointBaseModbusRtu *> *Function_RX_Map = new QMap<int,CPointBaseModbusRtu *>;///遥信
        QMap<int,CPointBaseModbusRtu *> *Function_RC_Map = new QMap<int,CPointBaseModbusRtu *>;///遥测
        QMap<int,CPointBaseModbusRtu *> *Function_RK_Map = new QMap<int,CPointBaseModbusRtu *>;///遥控
        QMap<int,CPointBaseModbusRtu *> *Function_RS_Map = new QMap<int,CPointBaseModbusRtu *>;///遥设

        for (int nType = 0; nType < DeviceElement.childNodes().count(); ++nType)
        {
            QDomElement TypeElem = DeviceElement.childNodes().at(nType).toElement();
            if (TypeElem.tagName() == "Attribute")
            {
                DeviceAddress = TypeElem.attribute("Address").toInt();
            }else
            {
                for (int nPoint = 0; nPoint < TypeElem.childNodes().count(); ++nPoint)
                {
                    QDomElement tagElement = TypeElem.childNodes().at(nPoint).toElement();
                    CPointBaseModbusRtu *pPoint = new CPointBaseModbusRtu;
                    int RegisterAddress = tagElement.attribute("RegisterAddress").toInt();
                    int Function = tagElement.attribute("Function").toInt();
                    pPoint->m_strPointName = tagElement.attribute("Name");
                    pPoint->m_strPointComment = tagElement.attribute("Desc");
                    pPoint->m_nID = tagElement.attribute("SN").toInt();
                    pPoint->m_strRTDBName = tagElement.attribute("Link");
                    pPoint->m_nScanTime = tagElement.attribute("ScanTime").toInt();
                    pPoint->m_fKValue = tagElement.attribute("KValue").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = tagElement.attribute("BaseValue").toFloat();
//                    qDebug()<<tagElement.attribute("BaseValue")<<pPoint->m_fBaseValue<<"basevalue";
                    pPoint->m_nDeviceAddr = DeviceAddress;
                    pPoint->m_nRegisterAddress = RegisterAddress;
                    pPoint->m_nFunction = Function;
                    pPoint->m_nRegisterCount = tagElement.attribute("RegisterCount").toInt();

                    ///<zzy 2015/1/14 修改
                    int nDatatype;
                    QString strDataType = tagElement.attribute("ValueType");
                    if (strDataType == "无符号整型"){
                        nDatatype = DATA_TYPE_INT32U;
                    }else if (strDataType == "有符号整型"){
                        nDatatype = DATA_TYPE_INT32S;
                    }else if (strDataType == "浮点型"){
                        nDatatype = DATA_TYPE_FP32;
                    }else if (strDataType == "布尔型"){
                        nDatatype = DATA_TYPE_BOOLEAN;
                    }else{
                        nDatatype = tagElement.attribute("ValueType").toInt();
                    }
//                    pPoint->m_nDataType = tagElement.attribute("DataType").toInt();///<zzy 2015/1/14 修改
                    pPoint->m_nDataType = nDatatype;///<zzy 2015/1/14 修改
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }
                    if ((Function == 1) || (Function == 2))
                    {
                        pPoint->m_nTagType = TAG_TYPE_DI;
                        Function_RX_Map->insert(RegisterAddress,pPoint);
                    }else if ((Function == 3) || (Function == 4))
                    {
                        pPoint->m_nTagType = TAG_TYPE_AI;
                        Function_RC_Map->insert(RegisterAddress,pPoint);
                    }else if (Function == 5)
                    {
                        pPoint->m_nTagType = TAG_TYPE_DO;
                        Function_RK_Map->insert(RegisterAddress,pPoint);
                    }else if (Function == 16)
                    {
                        pPoint->m_nTagType = TAG_TYPE_AO;
                        Function_RS_Map->insert(RegisterAddress,pPoint);
                    }
                }
            }
        }
        if (!Function_RX_Map->isEmpty())
            Device_RX_Map.insert(DeviceAddress,Function_RX_Map);
        if (!Function_RC_Map->isEmpty())
            Device_RC_Map.insert(DeviceAddress,Function_RC_Map);
        if (!Function_RK_Map->isEmpty())
            Device_RK_Map.insert(DeviceAddress,Function_RK_Map);
        if (!Function_RS_Map->isEmpty())
            Device_RS_Map.insert(DeviceAddress,Function_RS_Map);

    }
}

void CModbusTcpProtocol::CGI_SetFunctionMap(const QDomElement &DeviceListElement_)
{
    QDomNodeList DeviceList = DeviceListElement_.childNodes();
    for (int i = 0; i <DeviceList.count(); ++i)
    {
        QDomElement DeviceElement = DeviceList.at(i).toElement();
        int DeviceAddress;

        QMap<int,CPointBaseModbusRtu *> *Function_RX_Map = new QMap<int,CPointBaseModbusRtu *>;///遥信
        QMap<int,CPointBaseModbusRtu *> *Function_RC_Map = new QMap<int,CPointBaseModbusRtu *>;///遥测
        QMap<int,CPointBaseModbusRtu *> *Function_RK_Map = new QMap<int,CPointBaseModbusRtu *>;///遥控
        QMap<int,CPointBaseModbusRtu *> *Function_RS_Map = new QMap<int,CPointBaseModbusRtu *>;///遥设

        for (int nType = 0; nType < DeviceElement.childNodes().count(); ++nType)
        {
            QDomElement TypeElem = DeviceElement.childNodes().at(nType).toElement();
            if (TypeElem.tagName() == "Attribute")
            {
                DeviceAddress = TypeElem.attribute("Device_Address").toInt();
            }else
            {
                if (TypeElem.attribute("Name") == "YX")
                {
                    for (int nPoint = 0; nPoint < TypeElem.childNodes().count(); ++nPoint)
                    {
                        qDebug()<<"asdfasdf";
                        QDomElement tagElement = TypeElem.childNodes().at(nPoint).toElement();
                        qDebug()<<"LLLLL"<<tagElement.attribute("Device_YX_Link_Strings");
                        CPointBaseModbusRtu *pPoint = new CPointBaseModbusRtu;
                        int RegisterAddress = tagElement.attribute("Device_YX_RegisterAddress_Strings").toInt();
                        int Function = tagElement.attribute("Device_YX_Function_Strings").toInt();
                        pPoint->m_strPointName = tagElement.attribute("Device_YX_Name_Strings");
                        pPoint->m_strPointComment = tagElement.attribute("Device_YX_Desc_Strings");
    //                    pPoint->m_strType = tagElement.attribute("Type");
                        pPoint->m_nID = tagElement.attribute("Device_YX_SN_Strings").toInt();
                        pPoint->m_strRTDBName = tagElement.attribute("Device_YX_Link_Strings");
                        qDebug()<<"LinkName :"<<pPoint->m_strRTDBName;
                        pPoint->m_nScanTime = tagElement.attribute("Device_YX_ScanTime_Strings").toInt();
                        pPoint->m_fKValue = tagElement.attribute("Device_YX_KValue_Strings").toFloat();
                        if (pPoint->m_fKValue == 0)
                        {
                            pPoint->m_fKValue = 1;
                        }
                        pPoint->m_fBaseValue = tagElement.attribute("Device_YX_BaseValue_Strings").toFloat();
                        pPoint->m_nDeviceAddr = DeviceAddress;
                        pPoint->m_nRegisterAddress = RegisterAddress;
                        pPoint->m_nFunction = Function;
                        pPoint->m_nRegisterCount = tagElement.attribute("Device_YX_RegisterCount_Strings").toInt();

                        ///<zzy 2015/1/14 修改
                        int nDatatype;
                        QString strDataType = tagElement.attribute("Device_YX_ValueType_Strings");
                        if (strDataType == "无符号型"){
                            nDatatype = DATA_TYPE_INT32U;
                        }else if (strDataType == "有符号型"){
                            nDatatype = DATA_TYPE_INT32S;
                        }else if (strDataType == "浮点型"){
                            nDatatype = DATA_TYPE_FP32;
                        }else if (strDataType == "布尔型"){
                            nDatatype = DATA_TYPE_BOOLEAN;
                        }else{
                            nDatatype = tagElement.attribute("Device_YX_ValueType_Strings").toInt();
                        }
                        pPoint->m_nDataType = nDatatype;///<zzy 2015/1/14 修改
                        pPoint->m_pProtocol = this;
                        pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                        if (pPoint->m_pTag)
                        {
                            pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                        }
                        if ((Function == 1) || (Function == 2))
                        {
                            pPoint->m_nTagType = TAG_TYPE_DI;
                            Function_RX_Map->insert(RegisterAddress,pPoint);
                        }else if ((Function == 3) || (Function == 4))
                        {
                            pPoint->m_nTagType = TAG_TYPE_AI;
                            Function_RC_Map->insert(RegisterAddress,pPoint);
                        }else if (Function == 5)
                        {
                            pPoint->m_nTagType = TAG_TYPE_DO;
                            Function_RK_Map->insert(RegisterAddress,pPoint);
                        }else if (Function == 16)
                        {
                            pPoint->m_nTagType = TAG_TYPE_AO;
                            Function_RS_Map->insert(RegisterAddress,pPoint);
                        }
                    }
                }else if (TypeElem.attribute("Name") == "YC")
                {
                    for (int nPoint = 0; nPoint < TypeElem.childNodes().count(); ++nPoint)
                    {
                        QDomElement tagElement = TypeElem.childNodes().at(nPoint).toElement();
                        CPointBaseModbusRtu *pPoint = new CPointBaseModbusRtu;
                        int RegisterAddress = tagElement.attribute("Device_YC_RegisterAddress_Strings").toInt();
                        int Function = tagElement.attribute("Device_YC_Function_Strings").toInt();
                        pPoint->m_strPointName = tagElement.attribute("Device_YC_Name_Strings");
                        pPoint->m_strPointComment = tagElement.attribute("Device_YC_Desc_Strings");
    //                    pPoint->m_strType = tagElement.attribute("Type");
                        pPoint->m_nID = tagElement.attribute("Device_YC_SN_Strings").toInt();
                        pPoint->m_strRTDBName = tagElement.attribute("Device_YC_Link_Strings");
                        qDebug()<<"LinkName :"<<pPoint->m_strRTDBName;
                        pPoint->m_nScanTime = tagElement.attribute("Device_YC_ScanTime_Strings").toInt();
                        pPoint->m_fKValue = tagElement.attribute("Device_YC_KValue_Strings").toFloat();
                        if (pPoint->m_fKValue == 0)
                        {
                            pPoint->m_fKValue = 1;
                        }
                        pPoint->m_fBaseValue = tagElement.attribute("Device_YC_BaseValue_Strings").toFloat();
                        pPoint->m_nDeviceAddr = DeviceAddress;
                        pPoint->m_nRegisterAddress = RegisterAddress;
                        pPoint->m_nFunction = Function;
                        pPoint->m_nRegisterCount = tagElement.attribute("Device_YC_RegisterCount_Strings").toInt();

                        ///<zzy 2015/1/14 修改
                        int nDatatype;
                        QString strDataType = tagElement.attribute("Device_YC_ValueType_Strings");
                        if (strDataType == "无符号型"){
                            nDatatype = DATA_TYPE_INT32U;
                        }else if (strDataType == "有符号型"){
                            nDatatype = DATA_TYPE_INT32S;
                        }else if (strDataType == "浮点型"){
                            nDatatype = DATA_TYPE_FP32;
                        }else if (strDataType == "布尔型"){
                            nDatatype = DATA_TYPE_BOOLEAN;
                        }else{
                            nDatatype = tagElement.attribute("Device_YC_ValueType_Strings").toInt();
                        }
                        pPoint->m_nDataType = nDatatype;///<zzy 2015/1/14 修改
                        pPoint->m_pProtocol = this;
                        pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                        if (pPoint->m_pTag)
                        {
                            pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                        }
                        if ((Function == 1) || (Function == 2))
                        {
                            pPoint->m_nTagType = TAG_TYPE_DI;
                            Function_RX_Map->insert(RegisterAddress,pPoint);
                        }else if ((Function == 3) || (Function == 4))
                        {
                            pPoint->m_nTagType = TAG_TYPE_AI;
                            Function_RC_Map->insert(RegisterAddress,pPoint);
                        }else if (Function == 5)
                        {
                            pPoint->m_nTagType = TAG_TYPE_DO;
                            Function_RK_Map->insert(RegisterAddress,pPoint);
                        }else if (Function == 16)
                        {
                            pPoint->m_nTagType = TAG_TYPE_AO;
                            Function_RS_Map->insert(RegisterAddress,pPoint);
                        }
                    }
                }else if (TypeElem.attribute("Name") == "YM")
                {

                }else if (TypeElem.attribute("Name") == "YK")
                {
                    for (int nPoint = 0; nPoint < TypeElem.childNodes().count(); ++nPoint)
                    {
                        QDomElement tagElement = TypeElem.childNodes().at(nPoint).toElement();
                        CPointBaseModbusRtu *pPoint = new CPointBaseModbusRtu;
                        int RegisterAddress = tagElement.attribute("Device_YK_RegisterAddress_Strings").toInt();
                        int Function = tagElement.attribute("Device_YK_Function_Strings").toInt();
                        pPoint->m_strPointName = tagElement.attribute("Device_YK_Name_Strings");
                        pPoint->m_strPointComment = tagElement.attribute("Device_YK_Desc_Strings");
    //                    pPoint->m_strType = tagElement.attribute("Type");
                        pPoint->m_nID = tagElement.attribute("Device_YK_SN_Strings").toInt();
                        pPoint->m_strRTDBName = tagElement.attribute("Device_YK_Link_Strings");
                        qDebug()<<"LinkName :"<<pPoint->m_strRTDBName;
                        pPoint->m_nScanTime = tagElement.attribute("Device_YK_ScanTime_Strings").toInt();
                        pPoint->m_fKValue = tagElement.attribute("Device_YK_KValue_Strings").toFloat();
                        if (pPoint->m_fKValue == 0)
                        {
                            pPoint->m_fKValue = 1;
                        }
                        pPoint->m_fBaseValue = tagElement.attribute("Device_YK_BaseValue_Strings").toFloat();
                        pPoint->m_nDeviceAddr = DeviceAddress;
                        pPoint->m_nRegisterAddress = RegisterAddress;
                        pPoint->m_nFunction = Function;
                        pPoint->m_nRegisterCount = tagElement.attribute("Device_YK_RegisterCount_Strings").toInt();

                        ///<zzy 2015/1/14 修改
                        int nDatatype;
                        QString strDataType = tagElement.attribute("Device_YK_ValueType_Strings");
                        if (strDataType == "无符号型"){
                            nDatatype = DATA_TYPE_INT32U;
                        }else if (strDataType == "有符号型"){
                            nDatatype = DATA_TYPE_INT32S;
                        }else if (strDataType == "浮点型"){
                            nDatatype = DATA_TYPE_FP32;
                        }else if (strDataType == "布尔型"){
                            nDatatype = DATA_TYPE_BOOLEAN;
                        }else{
                            nDatatype = tagElement.attribute("Device_YK_ValueType_Strings").toInt();
                        }
                        pPoint->m_nDataType = nDatatype;///<zzy 2015/1/14 修改
                        pPoint->m_pProtocol = this;
                        pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                        if (pPoint->m_pTag)
                        {
                            pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                        }
                        if ((Function == 1) || (Function == 2))
                        {
                            pPoint->m_nTagType = TAG_TYPE_DI;
                            Function_RX_Map->insert(RegisterAddress,pPoint);
                        }else if ((Function == 3) || (Function == 4))
                        {
                            pPoint->m_nTagType = TAG_TYPE_AI;
                            Function_RC_Map->insert(RegisterAddress,pPoint);
                        }else if (Function == 5)
                        {
                            pPoint->m_nTagType = TAG_TYPE_DO;
                            Function_RK_Map->insert(RegisterAddress,pPoint);
                        }else if (Function == 16)
                        {
                            pPoint->m_nTagType = TAG_TYPE_AO;
                            Function_RS_Map->insert(RegisterAddress,pPoint);
                        }
                    }
                }else if (TypeElem.attribute("Name") == "YS")
                {
                    for (int nPoint = 0; nPoint < TypeElem.childNodes().count(); ++nPoint)
                    {
                        QDomElement tagElement = TypeElem.childNodes().at(nPoint).toElement();
                        CPointBaseModbusRtu *pPoint = new CPointBaseModbusRtu;
                        int RegisterAddress = tagElement.attribute("Device_YS_RegisterAddress_Strings").toInt();
                        int Function = tagElement.attribute("Device_YS_Function_Strings").toInt();
                        pPoint->m_strPointName = tagElement.attribute("Device_YS_Name_Strings");
                        pPoint->m_strPointComment = tagElement.attribute("Device_YS_Desc_Strings");
    //                    pPoint->m_strType = tagElement.attribute("Type");
                        pPoint->m_nID = tagElement.attribute("Device_YS_SN_Strings").toInt();
                        pPoint->m_strRTDBName = tagElement.attribute("Device_YS_Link_Strings");
                        qDebug()<<"LinkName :"<<pPoint->m_strRTDBName;
                        pPoint->m_nScanTime = tagElement.attribute("Device_YS_ScanTime_Strings").toInt();
                        pPoint->m_fKValue = tagElement.attribute("Device_YS_KValue_Strings").toFloat();
                        if (pPoint->m_fKValue == 0)
                        {
                            pPoint->m_fKValue = 1;
                        }
                        pPoint->m_fBaseValue = tagElement.attribute("Device_YS_BaseValue_Strings").toFloat();
                        pPoint->m_nDeviceAddr = DeviceAddress;
                        pPoint->m_nRegisterAddress = RegisterAddress;
                        pPoint->m_nFunction = Function;
                        pPoint->m_nRegisterCount = tagElement.attribute("Device_YS_RegisterCount_Strings").toInt();

                        ///<zzy 2015/1/14 修改
                        int nDatatype;
                        QString strDataType = tagElement.attribute("Device_YS_ValueType_Strings");
                        if (strDataType == "无符号型"){
                            nDatatype = DATA_TYPE_INT32U;
                        }else if (strDataType == "有符号型"){
                            nDatatype = DATA_TYPE_INT32S;
                        }else if (strDataType == "浮点型"){
                            nDatatype = DATA_TYPE_FP32;
                        }else if (strDataType == "布尔型"){
                            nDatatype = DATA_TYPE_BOOLEAN;
                        }else{
                            nDatatype = tagElement.attribute("Device_YS_ValueType_Strings").toInt();
                        }
                        pPoint->m_nDataType = nDatatype;///<zzy 2015/1/14 修改
                        pPoint->m_pProtocol = this;
                        pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                        if (pPoint->m_pTag)
                        {
                            pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                        }
                        if ((Function == 1) || (Function == 2))
                        {
                            pPoint->m_nTagType = TAG_TYPE_DI;
                            Function_RX_Map->insert(RegisterAddress,pPoint);
                        }else if ((Function == 3) || (Function == 4))
                        {
                            pPoint->m_nTagType = TAG_TYPE_AI;
                            Function_RC_Map->insert(RegisterAddress,pPoint);
                        }else if (Function == 5)
                        {
                            pPoint->m_nTagType = TAG_TYPE_DO;
                            Function_RK_Map->insert(RegisterAddress,pPoint);
                        }else if (Function == 16)
                        {
                            pPoint->m_nTagType = TAG_TYPE_AO;
                            Function_RS_Map->insert(RegisterAddress,pPoint);
                        }
                    }
                }
            }
        }
        if (!Function_RX_Map->isEmpty())
            Device_RX_Map.insert(DeviceAddress,Function_RX_Map);
        if (!Function_RC_Map->isEmpty())
            Device_RC_Map.insert(DeviceAddress,Function_RC_Map);
        if (!Function_RK_Map->isEmpty())
            Device_RK_Map.insert(DeviceAddress,Function_RK_Map);
        if (!Function_RS_Map->isEmpty())
            Device_RS_Map.insert(DeviceAddress,Function_RS_Map);

    }
}

/*!
 * \brief  功能概述 初始化采集信息报文
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusTcpProtocol::SetInitSend_RX_RC_FrameList()
{
    QMap<int,CPointBaseModbusRtu *> *Function_RX_Map;
    QMap<int,CPointBaseModbusRtu *> *Function_RC_Map;
    QMap<int,FunctinMap *>::iterator RX_Iterator;
    QMap<int,FunctinMap *>::iterator RC_Iterator;
    RX_Iterator = Device_RX_Map.begin();
    RC_Iterator = Device_RC_Map.begin();
    while((RX_Iterator != Device_RX_Map.end()) || (RC_Iterator != Device_RC_Map.end()))
    {
        if (RX_Iterator == Device_RX_Map.end())
        {
            Function_RC_Map = RC_Iterator.value();
            //! **********************初始化Send_RX_RC_FrameList

            Make_RX_RC_Query(Function_RC_Map);

            /// end 初始化Send_RX_RC_FrameList*****************
            RC_Iterator++;
        }else if (RC_Iterator == Device_RC_Map.end())
        {
            Function_RX_Map = RX_Iterator.value();
            //! **********************初始化Send_RX_RC_FrameList

            Make_RX_RC_Query(Function_RX_Map);

            /// end 初始化Send_RX_RC_FrameList*****************
            RX_Iterator++;
        }else
        {
            if (RX_Iterator.key() == RC_Iterator.key())
            {
                Function_RX_Map = RX_Iterator.value();
                Function_RC_Map = RC_Iterator.value();
                //! **********************初始化Send_RX_RC_FrameList

                Make_RX_RC_Query(Function_RX_Map);
                Make_RX_RC_Query(Function_RC_Map);

                /// end 初始化Send_RX_RC_FrameList*****************
                RX_Iterator++;
                RC_Iterator++;
            }else if (RX_Iterator.key() < RC_Iterator.key())
            {
                Function_RX_Map = RX_Iterator.value();
                //! **********************初始化Send_RX_RC_FrameList

                Make_RX_RC_Query(Function_RX_Map);

                /// end 初始化Send_RX_RC_FrameList*****************
                RX_Iterator++;
            }else if (RX_Iterator.key() > RC_Iterator.key())
            {
                Function_RC_Map = RC_Iterator.value();
                //! **********************初始化Send_RX_RC_FrameList

                Make_RX_RC_Query(Function_RC_Map);

                /// end 初始化Send_RX_RC_FrameList*****************
                RC_Iterator++;
            }
        }
    }
    SendFrameListNumber = 0;///初始化为0
}

/*!
 * \brief  功能概述 生成查询命令列表，智能组包函数
 * \param  参数描述 pFunction_是点表的map
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusTcpProtocol::Make_RX_RC_Query(QMap<int, CPointBaseModbusRtu *> *pFunction_)
{
    QMap<int, CPointBaseModbusRtu *>::iterator PointIterator;
    BYTE szSend[100] = {0};
    BYTE *pCur = szSend;
    int nDeviceAddress = -1;
    int nFunctionNo = -1;
    int nStartAddress = -1;
    int nRegNumber = -1;
//    WORD nCRC = 0;
    for (PointIterator = pFunction_->begin(); PointIterator != pFunction_->end();)
    {
        pCur = szSend;
        nDeviceAddress = PointIterator.value()->m_nDeviceAddr;
        nFunctionNo = PointIterator.value()->m_nFunction;
        nStartAddress = PointIterator.value()->m_nRegisterAddress;
        nRegNumber = 0;
        nRegNumber += PointIterator.value()->m_nRegisterCount;
        switch (nFunctionNo) {
        case 1:
        case 2:
        {
            //格式: 地址 功能码	[起始地址] [寄存器数目]	 [CRC校验]
            *pCur++ = nDeviceAddress;
            *pCur++ = nFunctionNo;
            *pCur++ = HIBYTE(nStartAddress);
            *pCur++ = LOBYTE(nStartAddress);
            while (1)
            {
                if ( (++PointIterator!=pFunction_->end()) && (PointIterator.key()==(nStartAddress+nRegNumber)) )
                {
                    nRegNumber += PointIterator.value()->m_nRegisterCount;
                }else
                {
                    break;
                }
                if (nRegNumber >= 800)///寄存器数量过多时分包
                {
                    break;
                }
            }


            *pCur++ = HIBYTE(nRegNumber);
            *pCur++ = LOBYTE(nRegNumber);

            ModbusRtuQuery *pQuery = new ModbusRtuQuery;
            pQuery->m_nResponseLength = (nRegNumber+7)/8+9;///返回帧长度
            pQuery->m_nStartAddress = nStartAddress;///起始地址
            Make_RX_TCP_Query((char *)szSend,pCur-szSend,pQuery);
        }
            break;
        case 3:
        case 4:
        {
            //格式: 地址 功能码	[起始地址] [寄存器数目]	 [CRC校验]
            *pCur++ = nDeviceAddress;
            *pCur++ = nFunctionNo;

            *pCur++ = HIBYTE(nStartAddress);
            *pCur++ = LOBYTE(nStartAddress);
            while (1)
            {
                if ((++PointIterator!=pFunction_->end())&&(PointIterator).key() == (nStartAddress + nRegNumber))
                {
                    nRegNumber += PointIterator.value()->m_nRegisterCount;
                }else
                {
                    break;
                }
                if (nRegNumber >= 100)///寄存器数量过多时分包
                {
                    break;
                }
            }
            *pCur++ = HIBYTE(nRegNumber);
            *pCur++ = LOBYTE(nRegNumber);

            ModbusRtuQuery *pQuery = new ModbusRtuQuery;
            pQuery->m_nResponseLength = nRegNumber*2+9;///返回帧长度
            pQuery->m_nStartAddress = nStartAddress;///起始地址
            Make_RC_TCP_Query((char *)szSend,pCur-szSend,pQuery);
        }
            break;
        default:
            break;
        }
    }
}

/*!
 * \brief  功能概述 生成遥控报文
 * \param  参数描述 strLink_是变量的全局名称，nDeviceAdd_是设备地址，bOn_是控制开合状态
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusTcpProtocol::Make_RK_Query(QString strLink_, int nDeviceAdd_, bool bOn_)
{
    m_strLink = strLink_;
    QMap<int,CPointBaseModbusRtu *> *pFunction = Device_RK_Map.value(nDeviceAdd_);
    QMap<int,CPointBaseModbusRtu *>::iterator PointIterator;
    for (PointIterator = pFunction->begin(); PointIterator != pFunction->end(); ++PointIterator)
    {
        if (PointIterator.value()->m_strRTDBName == strLink_)
        {
            BYTE szSend[100] = {0};
            BYTE *pCur = szSend;
//            WORD nCRC;
            int nStartAddress = PointIterator.value()->m_nRegisterAddress;
            int nValue = 0;
            //格式: 设备地址  功能码	[起始地址] [    值    ]   [CRC校验]
            //      01        05     00 00     FF/00 00      XX XX
            *pCur++ = nDeviceAdd_;
            *pCur++ = 5;

            *pCur++ = HIBYTE(nStartAddress);
            *pCur++ = LOBYTE(nStartAddress);
            if (bOn_)
            {
                nValue = 0xFF00;
                *pCur++ = HIBYTE(nValue);
                *pCur++ = LOBYTE(nValue);
            }else
            {
                *pCur++ = HIBYTE(nValue);
                *pCur++ = LOBYTE(nValue);
            }
            ModbusRtuQuery *pQuery = new ModbusRtuQuery;
            pQuery->m_nResponseLength = 12;///返回帧长度
            pQuery->m_nStartAddress = nStartAddress;///起始地址
            Make_RK_TCP_Query((char *)szSend,pCur-szSend,pQuery);
            break;
        }
    }
}

/*!
 * \brief  功能概述 生成遥设报文
 * \param  参数描述 strLink_是变量的全局名称，nDeviceAdd_是设备地址，varValue_是设置的值
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusTcpProtocol::Make_RS_Query(QString strLink_, int nDeviceAdd_, QVariant varValue_)
{
    m_strLink = strLink_;
    QMap<int,CPointBaseModbusRtu *> *pFunction = Device_RS_Map.value(nDeviceAdd_);
    QMap<int,CPointBaseModbusRtu *>::iterator PointIterator;
    for (PointIterator = pFunction->begin(); PointIterator != pFunction->end(); ++PointIterator)
    {
        if (PointIterator.value()->m_strRTDBName == strLink_)
        {
            BYTE szSend[100] = {0};
            BYTE *pCur = szSend;
//            WORD nCRC;
            int nDataType = PointIterator.value()->m_nDataType;
            int nStartAddress = PointIterator.value()->m_nRegisterAddress;
            int nRegNumber = PointIterator.value()->m_nRegisterCount;
            //格式: 设备地址 功能码	[起始地址] [寄存器数目] [字节数] [设定值] [CRC校验]
            //       01     10       00 00    00 01        02     00 0A   xx xx
            *pCur++ = nDeviceAdd_;
            *pCur++ = 16;

            *pCur++ = HIBYTE(nStartAddress);
            *pCur++ = LOBYTE(nStartAddress);
            *pCur++ = HIBYTE(nRegNumber);
            *pCur++ = LOBYTE(nRegNumber);
            *pCur++ = nRegNumber*2;//字节数
            switch (nDataType) {
            case DATA_TYPE_BOOLEAN:

                break;
            case DATA_TYPE_INT32S:
            {
                int nValue = varValue_.toInt();
                *pCur++ = HIBYTE(nValue);
                *pCur++ = LOBYTE(nValue);
            }
                break;
            case DATA_TYPE_INT32U:
            {
                unsigned int uValue = varValue_.toInt();
                *pCur++ = HIBYTE(uValue);
                *pCur++ = LOBYTE(uValue);
            }
                break;
            default:
                break;
            }
            ModbusRtuQuery *pQuery = new ModbusRtuQuery;
            pQuery->m_nResponseLength = 12;///返回帧长度
            pQuery->m_nStartAddress = nStartAddress;///起始地址
            Make_RS_TCP_Query((char *)szSend,pCur-szSend,pQuery);
            break;
        }
    }
}

/*!
 * \brief  功能概述 生成TCP遥信报文,并向遥信遥测发送列表中添加上该报文对象的指针
 * \param  参数描述 szSend_是报文地址，length_是报文长度，pQuery_是报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusTcpProtocol::Make_RX_TCP_Query(const char *szSend_, int length_,ModbusRtuQuery *pQuery_)
{
    BYTE szSend[500] = {0};
    BYTE *pCur = szSend;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = (BYTE)length_;
    memcpy(pCur,szSend_,length_);
    QByteArray SendArray((char *)szSend,length_+6);
    pQuery_->szSend = SendArray;
    Send_RX_RC_FrameList.append(pQuery_);
}

/*!
 * \brief  功能概述 生成TCP遥测报文,并向遥信遥测发送列表中添加上该报文对象的指针
 * \param  参数描述 szSend_是报文地址，length_是报文长度，pQuery_是报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusTcpProtocol::Make_RC_TCP_Query(const char *szSend_, int length_, ModbusRtuQuery *pQuery_)
{
    BYTE szSend[500] = {0};
    BYTE *pCur = szSend;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = (BYTE)length_;
    memcpy(pCur,szSend_,length_);

    QByteArray SendArray((char *)szSend,length_+6);
    pQuery_->szSend = SendArray;
    Send_RX_RC_FrameList.append(pQuery_);
}

/*!
 * \brief  功能概述 生成TCP遥设报文,并向遥控遥设发送列表中添加上该报文对象的指针
 * \param  参数描述 szSend_是报文地址，length_是报文长度，pQuery_是报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusTcpProtocol::Make_RS_TCP_Query(const char *szSend_, int length_,ModbusRtuQuery *pQuery_)
{
    BYTE szSend[500] = {0};
    BYTE *pCur = szSend;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = (BYTE)length_;
    memcpy(pCur,szSend_,length_);

    QByteArray SendArray((char *)szSend,length_+6);
    pQuery_->szSend = SendArray;
    Send_RK_RS_FrameList.append(pQuery_);
}

/*!
 * \brief  功能概述 生成TCP遥控报文,并向遥控遥设发送列表中添加上该报文对象的指针
 * \param  参数描述 szSend_是报文地址，length_是报文长度，pQuery_是报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusTcpProtocol::Make_RK_TCP_Query(const char *szSend_, int length_, ModbusRtuQuery *pQuery_)
{
    BYTE szSend[500] = {0};
    BYTE *pCur = szSend;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = (BYTE)length_;
    memcpy(pCur,szSend_,length_);

    QByteArray SendArray((char *)szSend,length_+6);
    pQuery_->szSend = SendArray;
    Send_RK_RS_FrameList.append(pQuery_);
}

/*!
 * \brief  功能概述 生成CRC校验码，以下数据用于标准16位CRC码值计算
 * \param  参数描述 pInfo是数据地址，nLen是数据长度
 * \return 返回值描述 返回两字节的CRC校验码
 * \author zzy
 * \date   2015/5/25
 */
WORD CModbusTcpProtocol::CRC(const BYTE *pInfo, int nLen)
{
    //16位CRC校验表
    const  static unsigned char auchCRCHi[] = {
        0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
            0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40
    };
    const  static unsigned char auchCRCLo[] = {
        0x00,0xc0,0xc1,0x01,0xc3,0x03,0x02,0xc2,
            0xc6,0x06,0x07,0xc7,0x05,0xc5,0xc4,0x04,
            0xcc,0x0c,0x0d,0xcd,0x0f,0xcf,0xce,0x0e,
            0x0a,0xca,0xcb,0x0b,0xc9,0x09,0x08,0xc8,
            0xd8,0x18,0x19,0xd9,0x1b,0xdb,0xda,0x1a,
            0x1e,0xde,0xdf,0x1f,0xdd,0x1d,0x1c,0xdc,
            0x14,0xd4,0xd5,0x15,0xd7,0x17,0x16,0xd6,
            0xd2,0x12,0x13,0xd3,0x11,0xd1,0xd0,0x10,
            0xf0,0x30,0x31,0xf1,0x33,0xf3,0xf2,0x32,
            0x36,0xf6,0xf7,0x37,0xf5,0x35,0x34,0xf4,
            0x3c,0xfc,0xfd,0x3d,0xff,0x3f,0x3e,0xfe,
            0xfa,0x3a,0x3b,0xfb,0x39,0xf9,0xf8,0x38,
            0x28,0xe8,0xe9,0x29,0xeb,0x2b,0x2a,0xea,
            0xee,0x2e,0x2f,0xef,0x2d,0xed,0xec,0x2c,
            0xe4,0x24,0x25,0xe5,0x27,0xe7,0xe6,0x26,
            0x22,0xe2,0xe3,0x23,0xe1,0x21,0x20,0xe0,
            0xa0,0x60,0x61,0xa1,0x63,0xa3,0xa2,0x62,
            0x66,0xa6,0xa7,0x67,0xa5,0x65,0x64,0xa4,
            0x6c,0xac,0xad,0x6d,0xaf,0x6f,0x6e,0xae,
            0xaa,0x6a,0x6b,0xab,0x69,0xa9,0xa8,0x68,
            0x78,0xb8,0xb9,0x79,0xbb,0x7b,0x7a,0xba,
            0xbe,0x7e,0x7f,0xbf,0x7d,0xbd,0xbc,0x7c,
            0xb4,0x74,0x75,0xb5,0x77,0xb7,0xb6,0x76,
            0x72,0xb2,0xb3,0x73,0xb1,0x71,0x70,0xb0,
            0x50,0x90,0x91,0x51,0x93,0x53,0x52,0x92,
            0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,
            0x9c,0x5c,0x5d,0x9d,0x5f,0x9f,0x9e,0x5e,
            0x5a,0x9a,0x9b,0x5b,0x99,0x59,0x58,0x98,
            0x88,0x48,0x49,0x89,0x4b,0x8b,0x8a,0x4a,
            0x4e,0x8e,0x8f,0x4f,0x8d,0x4d,0x4c,0x8c,
            0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,
            0x82,0x42,0x43,0x83,0x41,0x81,0x80,0x40
    };
    // 16位CRC
    unsigned char uchCRCHi = 0xff;
    unsigned char uchCRCLo = 0xff;
    WORD uIndex;
    while (nLen > 0)
    {
        nLen--;
        uIndex = uchCRCHi^*(pInfo++);
        uchCRCHi = uchCRCLo^auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex];
    }
    return(uchCRCHi<<8 | uchCRCLo);
}

/*!
 * \brief  功能概述 下发命令帧
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusTcpProtocol::Polling()
{
    if ((GetSysAbsTime() - m_nLastTime) < 1)///如果最后发送时间小于1秒则不发送
    {
        return;
    }
//    qDebug()<<Send_RK_RS_FrameList.isEmpty();
    if (!Send_RK_RS_FrameList.isEmpty())
    {
        if (m_pLink == NULL)
            return;
//        QMutexLocker locker(&m_mutexProcessData);
        m_pModbusRtuQuery = Send_RK_RS_FrameList.at(0);
        if (m_pLink->write(Send_RK_RS_FrameList.at(0)->szSend))
        {
            qDebug()<<"发送了遥控报文--------"<<Send_RK_RS_FrameList.at(0)->szSend.toHex();
            ShowMessage(0,Send_RK_RS_FrameList.at(0)->szSend.toHex());
            Send_RK_RS_FrameList.removeAt(0);
            m_nLastTime = GetSysAbsTime();
        }
        return;
    }
    if (Send_RX_RC_FrameList.count() == 0)
    {
        return;
    }else if (SendFrameListNumber >= Send_RX_RC_FrameList.count())
    {
        SendFrameListNumber = 0;
    }
    if (m_pLink == NULL)
        return;

//    QMutexLocker lock(&m_mutexProcessData);
    m_pModbusRtuQuery = Send_RX_RC_FrameList.at(SendFrameListNumber);
    int aaa = m_pLink->write(Send_RX_RC_FrameList.at(SendFrameListNumber)->szSend);
    if (aaa)
    {
        qDebug()<<"发送了遥XC报文--------"<<QTime::currentTime().toString("hh:mm:ss.zzz")<<aaa<<Send_RX_RC_FrameList.at(SendFrameListNumber)->szSend.toHex();
        ShowMessage(0,Send_RX_RC_FrameList.at(SendFrameListNumber)->szSend.toHex());
        SendFrameListNumber++;
        m_nLastTime = GetSysAbsTime();
    }
    return;
}

/*!
 * \brief  功能概述 定时器槽函数，定时调用Polling函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusTcpProtocol::OnTimeOut()
{
//    qDebug()<<"timeout";
//    Polling();
}

/*!
 * \brief  功能概述 用于响应点的值发生变化的函数
 * \param  参数描述 pPointBase_是点的指针，VarSet_是点的值
 * \return 返回值描述 返回true
 * \author zzy
 * \date   2015/5/25
 */
bool CModbusTcpProtocol::OnTagChange(CPointBase *pPointBase_, QVariant VarSet_)
{
    /// 转发遥控、遥设
    /// 生成遥控、遥设报文
    /// 转发驱动中不做处理

    switch (pPointBase_->m_nTagType) {
    case TAG_TYPE_AI://遥测
//        m_pValue->SetVarValue(valueSet_);
        break;
    case TAG_TYPE_AO://遥设
        qDebug()<<"生成遥设报文 并下发下发遥设--------------";
        Make_RS_Query(pPointBase_->m_strRTDBName,pPointBase_->GetDeviceAddress(),VarSet_);
        break;
    case TAG_TYPE_DI://遥信
//        m_pValue->SetVarValue(valueSet_);
        break;
    case TAG_TYPE_DO://遥控
    {
        qDebug()<<"生成遥设报文 并下发下发遥设111--------------";
        if (VarSet_.toInt())
        {
            qDebug()<<"生成遥控报文 并下发下发遥控ffffff++++++++++++++++++++"<<VarSet_<<pPointBase_->m_strPointComment<<pPointBase_->GetDeviceAddress();
            Make_RK_Query(pPointBase_->m_strRTDBName,pPointBase_->GetDeviceAddress(),true);
        }else
        {
            qDebug()<<"生成遥控报文 并下发下发遥控000000++++++++++++++++++++"<<VarSet_;
            Make_RK_Query(pPointBase_->m_strRTDBName,pPointBase_->GetDeviceAddress(),false);
        }
    }
        break;
    case TAG_TYPE_MEM:

        break;
    default:
        break;
    }
    return true;
}
void CModbusTcpProtocol::ProcessRespond_SetValue(QByteArray &Respond_)
{
    QByteArray RespondArray = Respond_;
    if (RespondArray.count() < 9 )///<zzy 2015/1/17 修改:增加判断
        return;

    while (Respond_.count() >= 9)
    {
        RespondArray = Respond_;
        RespondArray.remove(0,6);///移除TCP头

        QByteArray SzSend= m_pModbusRtuQuery->szSend;
        SzSend.remove(0,6);

        int length = m_pModbusRtuQuery->m_nResponseLength;
        length -= 6;
        QString strChangeValue = "";
        QString strOldChannelValue = "";
        QVariant OriginalVariant;
        QVariant ProjectVariant;
        if ((BYTE)RespondArray.at(1)&0x80)///收到异常码 处理异常码事件
        {
            Respond_.remove(0,9);
            int nFunctionNumber = (BYTE)RespondArray.at(1)&0x7F;///获取功能码
            int nExceptionNumber = (BYTE)RespondArray.at(2);///获取异常码
            switch (nFunctionNumber)
            {
            case 1:

                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
            {
                CTagI *pTag = m_pRTDB->FindTag(m_strLink);
                if (pTag)
                {
                    strChangeValue = "执行->异常码:"+QString::number(nExceptionNumber);
//                    pTag->GetPointCollection()->GetValue()->SetWriteResult(0,strChangeValue);
                }
            }
                break;
            case 6:

                break;
            case 16:
            {
                CTagI *pTag = m_pRTDB->FindTag(m_strLink);
                if (pTag)
                {
                    strChangeValue = "执行->异常码:"+QString::number(nExceptionNumber);
//                    pTag->GetPointCollection()->GetValue()->SetWriteResult(0,strChangeValue);
                }
            }
                break;
            default:
                break;
            }
            continue;
        }

    //    ModbusBaoWen("报文应反lentgh: "+QString::number(m_pModbusRtuQuery->m_nResponseLength)+" 实反length: "+QString::number(RespondArray.count())+" 下行: "+m_pModbusRtuQuery->szSend.toHex()+"   上送: "+RespondArray.toHex());
        if (length <= RespondArray.count())///返回长度相等
        {
            qDebug()<<"返回长度相等"<<"Response.count="<<Respond_.count()<<"length="<<length;
            Respond_.remove(0,length+6);

            int nDeviceAddress = (BYTE)RespondArray.at(0);///设备地址
            int nFunctionNumber = (BYTE)RespondArray.at(1);///功能码

            CPointBaseModbusRtu *pTagAttribute;///点的信息指针
            QString strLink;
            int nTagRegCount;
            int nDataType;
            int nByteOffset;///偏移
            QMap<int,CPointBaseModbusRtu *> *pFunctionMap;
            switch (nFunctionNumber)
            {
            case 1:
            case 2:
            /** *设备地址  功能码    字节数  数据    CRC校验
             *    01     01/02      01    03   两字节校验
             *   处理遥测数据
            * **/
            {
                int nRXCount = SzSend.at(4)*256+SzSend.at(5);///遥信数量
                int nBitOffset = 0;///位偏移量
                pFunctionMap = Device_RX_Map.value(nDeviceAddress);
                if (pFunctionMap == NULL)
                {
                    continue;
                }
                for (int i = 0; i < nRXCount;)
                {
                    nByteOffset = (i+7)/8+3;///返回帧的字节偏移
                    BYTE valueByte = (BYTE)RespondArray.at(nByteOffset);///遥信量所在的字节
                    for (int j = 0; j < 8;++j)
                    {
                        nBitOffset = i%8;///位偏移量
                        pTagAttribute = pFunctionMap->value(m_pModbusRtuQuery->m_nStartAddress+i);
                        if (pTagAttribute == NULL)
                        {
                            continue;
                        }
                        nTagRegCount = pTagAttribute->m_nRegisterCount;
                        nDataType = pTagAttribute->m_nDataType;
                        strLink = pTagAttribute->m_strRTDBName;
                        unsigned int bValue = (valueByte>>nBitOffset)&0x1;
                        strChangeValue.setNum(bValue);
                        strOldChannelValue.setNum(bValue);
                        CTagI *pTag = pTagAttribute->GetTag();
                        if (pTag)
                        {
                            ProjectVariant.setValue(bValue);
                            OriginalVariant.setValue(bValue);
                            pTag->SetValue(pTagAttribute,ProjectVariant,OriginalVariant);
                        }
                        i++;
                        if (i >= nRXCount)
                        {
                            break;
                        }
                    }
                }
            }
                break;
            case 3:
            case 4:
            /** *设备地址  功能码  字节数  数据高字节  数据低字节  CRC校验
             *    01      03      02    08        02       两字节校验
             *   处理遥测数据
            * **/
            {
                int nRegCount = (BYTE)RespondArray.at(2)/2;///寄存器数
                float kValue;
                float BaseValue;
                pFunctionMap  = Device_RC_Map.value(nDeviceAddress);
                if (pFunctionMap == NULL)
                {
                    continue;
                }
                for (int i = 0; i < nRegCount;)
                {
                    nByteOffset = i*2+3;///返回帧的偏移
                    pTagAttribute = pFunctionMap->value(m_pModbusRtuQuery->m_nStartAddress+i);
                    if (pTagAttribute == NULL)
                    {
                        i++;
                        continue;
                    }
                    nTagRegCount = pTagAttribute->m_nRegisterCount;
                    nDataType = pTagAttribute->m_nDataType;
                    strLink = pTagAttribute->m_strRTDBName;
                    kValue = pTagAttribute->m_fKValue;
                    BaseValue = pTagAttribute->m_fBaseValue;
    //                qDebug()<<nRegCount<<nDataType<<pTagAttribute->m_strRTDBName;
                    switch (nTagRegCount)//寄存器个数
                    {
                    case 1://单寄存器
                        switch(nDataType)
                        {
                        case DATA_TYPE_INT32S://有符号数
                        {
                            int nValue = (((BYTE)RespondArray.at(nByteOffset)<<8)|(BYTE)RespondArray.at(nByteOffset+1));
                            ProjectVariant.setValue(((nValue+BaseValue)*kValue));
                            OriginalVariant.setValue((((BYTE)RespondArray.at(nByteOffset)<<8)|(BYTE)RespondArray.at(nByteOffset+1)));
                        }
                            break;
                        case DATA_TYPE_INT32U://无符号数
                        {
                            unsigned int uValue = (((BYTE)RespondArray.at(nByteOffset)<<8)|(BYTE)RespondArray.at(nByteOffset+1));
                            ProjectVariant.setValue(((uValue+BaseValue)*kValue));
                            OriginalVariant.setValue((((BYTE)RespondArray.at(nByteOffset)<<8)|(BYTE)RespondArray.at(nByteOffset+1)));
                        }
                            break;
                        default:
                            qDebug()<<"未知的类型"<<nDataType;
                            break;
                        }
                        i++;      //下一寄存器地址
                        break;
                    case 2://双寄存器
                        switch(nDataType)
                        {
                        case DATA_TYPE_INT32S://有符号数
                        {
                            int nValue = (((BYTE)RespondArray.at(nByteOffset)<<24)|((BYTE)RespondArray.at(nByteOffset+1)<<16)
                                          |((BYTE)RespondArray.at(nByteOffset+2)<<8)|(BYTE)RespondArray.at(nByteOffset+3));
                            ProjectVariant.setValue(((nValue+BaseValue)*kValue));
                            OriginalVariant.setValue(((BYTE)RespondArray.at(nByteOffset)<<24)|((BYTE)RespondArray.at(nByteOffset+1)<<16)
                                                       |((BYTE)RespondArray.at(nByteOffset+2)<<8)|(BYTE)RespondArray.at(nByteOffset+3));
                        }
                            break;
                        case DATA_TYPE_INT32U://无符号数
                        {
                            unsigned int uValue = (((BYTE)RespondArray.at(nByteOffset)<<24)|((BYTE)RespondArray.at(nByteOffset+1)<<16)
                                                   |((BYTE)RespondArray.at(nByteOffset+2)<<8)|(BYTE)RespondArray.at(nByteOffset+3));
                            ProjectVariant.setValue(((uValue+BaseValue)*kValue));
                            OriginalVariant.setValue(((BYTE)RespondArray.at(nByteOffset)<<24)|((BYTE)RespondArray.at(nByteOffset+1)<<16)
                                                       |((BYTE)RespondArray.at(nByteOffset+2)<<8)|(BYTE)RespondArray.at(nByteOffset+3));
                        }
                            break;
                        case DATA_TYPE_FP32://浮点数
                        {
                            BYTE temp[4];
                            temp[0] = (BYTE)RespondArray.at(nByteOffset);
                            temp[1] = (BYTE)RespondArray.at(nByteOffset+1);
                            temp[2] = (BYTE)RespondArray.at(nByteOffset+2);
                            temp[3] = (BYTE)RespondArray.at(nByteOffset+3);

                            int nInfoSize = 4;
                            int nGroupSize = 4;
                            float fValue = 0;
                            memcpy((void *)(&fValue),temp,4);
                            BYTE *pCur = (BYTE *)(&fValue);
                            BYTE *pEnd = (BYTE *)(&fValue)+nInfoSize;
                            for(; pCur<pEnd; pCur+=nGroupSize)
                            {
                                for(int i=0; i<nGroupSize/2; i++)
                                {
                                    BYTE nTemp = pCur[i];
                                    pCur[i] = pCur[nGroupSize-i-1];
                                    pCur[nGroupSize-i-1] = nTemp;
                                }
                            }
                            ProjectVariant.setValue(((fValue+BaseValue)*kValue));
                            OriginalVariant.setValue(fValue);
                        }
                            break;
                        default:
                            break;
                        }
                        i+=2;      //下一寄存器地址
                        break;
                    default://更多的寄存器
                        break;
                    }
                    CTagI *pTag = pTagAttribute->GetTag();
                    if (pTag)
                    {
    //                    ModbusDebugFileMessage("设置成功:"+strLink+"   "+ProjectVariant.toString()+"   "+OriginalVariant.toString());
                        pTag->SetValue(pTagAttribute,ProjectVariant,OriginalVariant);
                    }else
                    {
    //                    ModbusDebugFileMessage("实时库中没有此点:"+strLink+"   "+ProjectVariant.toString()+"   "+OriginalVariant.toString());
                    }
                }

            }
                break;
            case 5:
                /** *设备地址  功能码
                 *
                 * */
            {
                CTagI *tag = m_pRTDB->FindTag(m_strLink);
                if (SzSend == RespondArray)
                {
                    /// 改变全局map中的值 使其值为(6种情况):
                    /// 在遥控->选择 正在遥控->执行 正在遥控->撤销 选择->成功 执行->成功 撤销->成功  选择->失败 执行->失败 撤销->失败
                    if (tag)
                    {
                        strChangeValue = "执行->成功";
                        tag->GetPointCollection()->GetValue()->SetWriteResult(1,strChangeValue);
                    }
                }else
                {
                    if (tag)
                    {
                        strChangeValue = "执行->失败";
                        tag->GetPointCollection()->GetValue()->SetWriteResult(0,strChangeValue);
                    }
                }
            }
                break;
            case 16:
                /** *设备地址  功能码
                 *
                 * */
            {
                ///改变全局Map中的值  使其为: 修改成功
                CTagI *pTag = m_pRTDB->FindTag(m_strLink);
                if (pTag)
                {
                    strChangeValue = "设置->成功";
                    pTag->GetPointCollection()->GetValue()->SetWriteResult(1,strChangeValue);
                }
            }
                break;
            default:
                break;
            }
            ///处理完数据后改变m_nLastTime
            continue;
        }else
        {
            qDebug()<<"返回长度不相等";
            Respond_.remove(0,1);
        }
    }

    m_nLastTime = 0;
}

/*!
 * \brief  功能概述 响应readyRead(QByteArray)信号，解析收到的报文函数
 * \param  参数描述 data是收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusTcpProtocol::ProcessRespond(QByteArray Respond_)
{
    QByteArray RespondArray = Respond_;
    if (RespondArray.isEmpty())
        return;
    ShowMessage(1,RespondArray.toHex());
//    QMutexLocker lock(&m_mutexProcessData);
//    ProcessRespond_SetValue(Respond_);
    if (RespondArray.count() < 9 )///<zzy 2015/1/17 修改:增加判断
        return;

    while (Respond_.count() >= 9)
    {
        RespondArray = Respond_;
        if (RespondArray.at(2) != 0x00)
        {
            Respond_.remove(0,4);
            qDebug()<<"if (RespondArray.at(2) != 0x00)";
            continue;
        }
        if (RespondArray.at(3) != 0x00)
        {
            Respond_.remove(0,4);
            qDebug()<<"if (RespondArray.at(3) != 0x00)";
            continue;
        }
        RespondArray.remove(0,6);///移除TCP头

        QByteArray SzSend= m_pModbusRtuQuery->szSend;
        SzSend.remove(0,6);

        int length = m_pModbusRtuQuery->m_nResponseLength;
        length -= 6;
        QString strChangeValue = "";
        QString strOldChannelValue = "";
        QVariant OriginalVariant;
        QVariant ProjectVariant;
        if ((BYTE)RespondArray.at(1)&0x80)///收到异常码 处理异常码事件
        {
            Respond_.remove(0,9);
            int nFunctionNumber = (BYTE)RespondArray.at(1)&0x7F;///获取功能码
            int nExceptionNumber = (BYTE)RespondArray.at(2);///获取异常码
            switch (nFunctionNumber)
            {
            case 1:

                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
            {
                CTagI *pTag = m_pRTDB->FindTag(m_strLink);
                if (pTag)
                {
                    strChangeValue = "执行->异常码:"+QString::number(nExceptionNumber);
//                    pTag->GetPointCollection()->GetValue()->SetWriteResult(0,strChangeValue);
                }
            }
                break;
            case 6:

                break;
            case 16:
            {
                CTagI *pTag = m_pRTDB->FindTag(m_strLink);
                if (pTag)
                {
                    strChangeValue = "执行->异常码:"+QString::number(nExceptionNumber);
//                    pTag->GetPointCollection()->GetValue()->SetWriteResult(0,strChangeValue);
                }
            }
                break;
            default:
                break;
            }
            continue;
        }

    //    ModbusBaoWen("报文应反lentgh: "+QString::number(m_pModbusRtuQuery->m_nResponseLength)+" 实反length: "+QString::number(RespondArray.count())+" 下行: "+m_pModbusRtuQuery->szSend.toHex()+"   上送: "+RespondArray.toHex());
        if (length <= RespondArray.count())///返回长度相等
        {
            qDebug()<<"返回长度相等"<<"Response.count="<<Respond_.count()<<"length="<<length;
            Respond_.remove(0,length+6);

            int nDeviceAddress = (BYTE)RespondArray.at(0);///设备地址
            int nFunctionNumber = (BYTE)RespondArray.at(1);///功能码

            CPointBaseModbusRtu *pTagAttribute;///点的信息指针
            QString strLink;
            int nTagRegCount;
            int nDataType;
            int nByteOffset;///偏移
            QMap<int,CPointBaseModbusRtu *> *pFunctionMap;
            switch (nFunctionNumber)
            {
            case 1:
            case 2:
            /** *设备地址  功能码    字节数  数据    CRC校验
             *    01     01/02      01    03   两字节校验
             *   处理遥测数据
            * **/
            {
                int nRXCount = SzSend.at(4)*256+SzSend.at(5);///遥信数量
                int nBitOffset = 0;///位偏移量
                pFunctionMap = Device_RX_Map.value(nDeviceAddress);
                if (pFunctionMap == NULL)
                {
                    continue;
                }
                for (int i = 0; i < nRXCount;)
                {
                    nByteOffset = (i+7)/8+3;///返回帧的字节偏移
                    BYTE valueByte = (BYTE)RespondArray.at(nByteOffset);///遥信量所在的字节
                    for (int j = 0; j < 8;++j)
                    {
                        nBitOffset = i%8;///位偏移量
                        pTagAttribute = pFunctionMap->value(m_pModbusRtuQuery->m_nStartAddress+i);
                        if (pTagAttribute == NULL)
                        {
                            continue;
                        }
                        nTagRegCount = pTagAttribute->m_nRegisterCount;
                        nDataType = pTagAttribute->m_nDataType;
                        strLink = pTagAttribute->m_strRTDBName;
                        unsigned int bValue = (valueByte>>nBitOffset)&0x1;
                        strChangeValue.setNum(bValue);
                        strOldChannelValue.setNum(bValue);
                        CTagI *pTag = pTagAttribute->GetTag();
                        if (pTag)
                        {
                            ProjectVariant.setValue(bValue);
                            OriginalVariant.setValue(bValue);
                            pTag->SetValue(pTagAttribute,ProjectVariant,OriginalVariant);
                        }
                        i++;
                        if (i >= nRXCount)
                        {
                            break;
                        }
                    }
                }
            }
                break;
            case 3:
            case 4:
            /** *设备地址  功能码  字节数  数据高字节  数据低字节  CRC校验
             *    01      03      02    08        02       两字节校验
             *   处理遥测数据
            * **/
            {
                int nRegCount = (BYTE)RespondArray.at(2)/2;///寄存器数
                float kValue;
                float BaseValue;
                pFunctionMap  = Device_RC_Map.value(nDeviceAddress);
                if (pFunctionMap == NULL)
                {
                    continue;
                }
                for (int i = 0; i < nRegCount;)
                {
                    nByteOffset = i*2+3;///返回帧的偏移
                    pTagAttribute = pFunctionMap->value(m_pModbusRtuQuery->m_nStartAddress+i);
                    if (pTagAttribute == NULL)
                    {
                        i++;
                        continue;
                    }
                    nTagRegCount = pTagAttribute->m_nRegisterCount;
                    nDataType = pTagAttribute->m_nDataType;
                    strLink = pTagAttribute->m_strRTDBName;
                    kValue = pTagAttribute->m_fKValue;
                    BaseValue = pTagAttribute->m_fBaseValue;
    //                qDebug()<<nRegCount<<nDataType<<pTagAttribute->m_strRTDBName;
                    switch (nTagRegCount)//寄存器个数
                    {
                    case 1://单寄存器
                        switch(nDataType)
                        {
                        case DATA_TYPE_INT32S://有符号数
                        {
                            int nValue = (((BYTE)RespondArray.at(nByteOffset)<<8)|(BYTE)RespondArray.at(nByteOffset+1));
                            ProjectVariant.setValue(((nValue+BaseValue)*kValue));
                            OriginalVariant.setValue((((BYTE)RespondArray.at(nByteOffset)<<8)|(BYTE)RespondArray.at(nByteOffset+1)));
                        }
                            break;
                        case DATA_TYPE_INT32U://无符号数
                        {
                            unsigned int uValue = (((BYTE)RespondArray.at(nByteOffset)<<8)|(BYTE)RespondArray.at(nByteOffset+1));
                            ProjectVariant.setValue(((uValue+BaseValue)*kValue));
                            OriginalVariant.setValue((((BYTE)RespondArray.at(nByteOffset)<<8)|(BYTE)RespondArray.at(nByteOffset+1)));
                        }
                            break;
                        default:
                            qDebug()<<"未知的类型"<<nDataType;
                            break;
                        }
                        i++;      //下一寄存器地址
                        break;
                    case 2://双寄存器
                        switch(nDataType)
                        {
                        case DATA_TYPE_INT32S://有符号数
                        {
                            int nValue = (((BYTE)RespondArray.at(nByteOffset)<<24)|((BYTE)RespondArray.at(nByteOffset+1)<<16)
                                          |((BYTE)RespondArray.at(nByteOffset+2)<<8)|(BYTE)RespondArray.at(nByteOffset+3));
                            ProjectVariant.setValue(((nValue+BaseValue)*kValue));
                            OriginalVariant.setValue(((BYTE)RespondArray.at(nByteOffset)<<24)|((BYTE)RespondArray.at(nByteOffset+1)<<16)
                                                       |((BYTE)RespondArray.at(nByteOffset+2)<<8)|(BYTE)RespondArray.at(nByteOffset+3));
                        }
                            break;
                        case DATA_TYPE_INT32U://无符号数
                        {
                            unsigned int uValue = (((BYTE)RespondArray.at(nByteOffset)<<24)|((BYTE)RespondArray.at(nByteOffset+1)<<16)
                                                   |((BYTE)RespondArray.at(nByteOffset+2)<<8)|(BYTE)RespondArray.at(nByteOffset+3));
                            ProjectVariant.setValue(((uValue+BaseValue)*kValue));
                            OriginalVariant.setValue(((BYTE)RespondArray.at(nByteOffset)<<24)|((BYTE)RespondArray.at(nByteOffset+1)<<16)
                                                       |((BYTE)RespondArray.at(nByteOffset+2)<<8)|(BYTE)RespondArray.at(nByteOffset+3));
                        }
                            break;
                        case DATA_TYPE_FP32://浮点数
                        {
                            BYTE temp[4];
                            temp[0] = (BYTE)RespondArray.at(nByteOffset);
                            temp[1] = (BYTE)RespondArray.at(nByteOffset+1);
                            temp[2] = (BYTE)RespondArray.at(nByteOffset+2);
                            temp[3] = (BYTE)RespondArray.at(nByteOffset+3);

                            int nInfoSize = 4;
                            int nGroupSize = 4;
                            float fValue = 0;
                            memcpy((void *)(&fValue),temp,4);
                            BYTE *pCur = (BYTE *)(&fValue);
                            BYTE *pEnd = (BYTE *)(&fValue)+nInfoSize;
                            for(; pCur<pEnd; pCur+=nGroupSize)
                            {
                                for(int i=0; i<nGroupSize/2; i++)
                                {
                                    BYTE nTemp = pCur[i];
                                    pCur[i] = pCur[nGroupSize-i-1];
                                    pCur[nGroupSize-i-1] = nTemp;
                                }
                            }
                            ProjectVariant.setValue(((fValue+BaseValue)*kValue));
                            OriginalVariant.setValue(fValue);
                        }
                            break;
                        default:
                            break;
                        }
                        i+=2;      //下一寄存器地址
                        break;
                    default://更多的寄存器
                        break;
                    }
                    CTagI *pTag = pTagAttribute->GetTag();
                    if (pTag)
                    {
    //                    ModbusDebugFileMessage("设置成功:"+strLink+"   "+ProjectVariant.toString()+"   "+OriginalVariant.toString());
                        pTag->SetValue(pTagAttribute,ProjectVariant,OriginalVariant);
                    }else
                    {
    //                    ModbusDebugFileMessage("实时库中没有此点:"+strLink+"   "+ProjectVariant.toString()+"   "+OriginalVariant.toString());
                    }
                }

            }
                break;
            case 5:
                /** *设备地址  功能码
                 *
                 * */
            {
                CTagI *tag = m_pRTDB->FindTag(m_strLink);
                if (SzSend == RespondArray)
                {
                    /// 改变全局map中的值 使其值为(6种情况):
                    /// 在遥控->选择 正在遥控->执行 正在遥控->撤销 选择->成功 执行->成功 撤销->成功  选择->失败 执行->失败 撤销->失败
                    if (tag)
                    {
                        strChangeValue = "执行->成功";
                        tag->GetPointCollection()->GetValue()->SetWriteResult(1,strChangeValue);
                    }
                }else
                {
                    if (tag)
                    {
                        strChangeValue = "执行->失败";
                        tag->GetPointCollection()->GetValue()->SetWriteResult(0,strChangeValue);
                    }
                }
            }
                break;
            case 16:
                /** *设备地址  功能码
                 *
                 * */
            {
                ///改变全局Map中的值  使其为: 修改成功
                CTagI *pTag = m_pRTDB->FindTag(m_strLink);
                if (pTag)
                {
                    strChangeValue = "设置->成功";
                    pTag->GetPointCollection()->GetValue()->SetWriteResult(1,strChangeValue);
                }
            }
                break;
            default:
                break;
            }
            ///处理完数据后改变m_nLastTime
            continue;
        }else
        {
            qDebug()<<"返回长度不相等";
            Respond_.remove(0,1);
        }
    }

    m_nLastTime = 0;
    return;
}

/*!
 * \brief  功能概述 是否成功连接的槽函数
 * \param  参数描述 bool类型的参数是代表是否成功连接的状态，如果为true则成功连接，如果为false则未成功连接
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusTcpProtocol::slotConnect(bool)
{

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
