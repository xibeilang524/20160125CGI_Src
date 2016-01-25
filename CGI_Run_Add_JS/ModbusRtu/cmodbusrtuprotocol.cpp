/*!
 * \file  CModbusRtuProtocol
 * \brief 概述 CModbusRtuProtocol类的源文件
 *
 *详细概述
 *
 * \author zzy
 * \version 版本号
 * \date 2015/1/14
 *
 * \b 修改记录：
 * \li 2015/1/14
 *  添加注释
 */

#include <QTimer>
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QDateTime>

#include "cmodbusrtuprotocol.h"
#include "../PortInclude/CRTDBI.h"
#include "../PortInclude/CTagI.h"
#include "../PortInclude/cvaluebase.h"
#include <assert.h>
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
    CModbusRtuProtocol *testProtocol = new CModbusRtuProtocol(parent);
    return  testProtocol;
}

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 parent是父对象的指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
CModbusRtuProtocol::CModbusRtuProtocol(QObject */*parent*/)
{
//    m_pSerialPort = new CSerialPort;
//    connect(m_pSerialPort,SIGNAL(readyRead()),this,SLOT(ProcessRespond()));
    m_pTimer = new QTimer;
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(OnTimeOut()));
    m_nLastTime = 0;
    m_pModbusRtuQuery = NULL;
//    m_pTimer->start(1000);
}

/*!
 * \brief  功能概述 创建此通道
 * \param  参数描述 strChannelFileName_是通道文件绝对路径名，pDatabaseReturn是实时库指针
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CModbusRtuProtocol::OnCreateChannel(const QString strChannelFileName_, CRTDBI *pDatabaseReturn)
{
    SetPROTOCOL_TYPE(Collect_Type);
    if (!CProtocolBase::OnCreateChannel(strChannelFileName_,pDatabaseReturn))
        return false;
//    qDebug()<<"bool CModbusRtuProtocol::OnCreateChannel"<<QThread::currentThreadId();
    QFile file(strChannelFileName_);
    QDomDocument ChannelDoc;
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return false;
    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!ChannelDoc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
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

    SetFunctionMap(DeviceList);///< 初始化点表Map
    SetInitSend_RX_RC_FrameList();///< 初始化采集信息报文

//    m_pTimer->start(500);///循环开始

    return true;
}

bool CModbusRtuProtocol::OnCreateChannel(const QDomElement &ChannelElement_, ChannelType nChannelType_, int nComOrCanNumber_, CRTDBI *pDatabaseReturn_)
{
    SetPROTOCOL_TYPE(Collect_Type);
    if (!CProtocolBase::OnCreateChannel(ChannelElement_,nChannelType_,nComOrCanNumber_,pDatabaseReturn_))
        return false;

    if (ChannelElement_.elementsByTagName("DeviceList").isEmpty())
        return false;
    QDomElement DeviceList = ChannelElement_.elementsByTagName("DeviceList").at(0).toElement();

    SetFunctionMap(DeviceList);///< 初始化点表Map
    SetInitSend_RX_RC_FrameList();///< 初始化采集信息报文

//    m_pTimer->start(500);///循环开始

    return true;
}

#if 0
/*!
 * \brief  功能概述 初始化Device_1_2_Map和Device_3_4_Map 功能码1、2、3、4的Map，初始化功能码1、2、3、4的Map
 * \param  参数描述 DeviceListElement_是通道配置文件中的设备列表节点<DeviceList>
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusRtuProtocol::SetFunctionMap(const QDomElement &DeviceListElement_)
{
    QDomNodeList DeviceList = DeviceListElement_.childNodes();
    for (int i = 0; i <DeviceList.count(); ++i)
    {
        QDomElement DeviceElement = DeviceList.at(i).toElement();
//        int DeviceAddress = DeviceElement.attribute("Address").toInt();///<zzy 2015/1/14 修改:
        int DeviceAddress/* = DeviceElement.attribute("Address").toInt()*/;///<zzy 2015/1/14 修改:
//        QDomNodeList PointElement = DeviceElement.childNodes();

        QMap<int,CPointBaseModbusRtu *> *Function_RX_Map = new QMap<int,CPointBaseModbusRtu *>;///遥信
        QMap<int,CPointBaseModbusRtu *> *Function_RC_Map = new QMap<int,CPointBaseModbusRtu *>;///遥测
        QMap<int,CPointBaseModbusRtu *> *Function_RK_Map = new QMap<int,CPointBaseModbusRtu *>;///遥控
        QMap<int,CPointBaseModbusRtu *> *Function_RS_Map = new QMap<int,CPointBaseModbusRtu *>;///遥设
//        DeviceAddress = DeviceElement.childNodes().at(0).toElement().attribute("Attribute").toInt();///<zzy 2015/1/14 修改:
        for (int nType = 0; nType < DeviceElement.childNodes().count(); ++nType)
        {
            QDomElement TypeElem = DeviceElement.childNodes().at(nType).toElement();
            if (TypeElem.tagName() == "Attribute")///
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
//                    pPoint->m_strType = tagElement.attribute("Type");
                    pPoint->m_nID = tagElement.attribute("SN").toInt();
                    pPoint->m_strRTDBName = tagElement.attribute("Link");
                    pPoint->m_nScanTime = tagElement.attribute("ScanTime").toInt();
                    pPoint->m_fKValue = tagElement.attribute("KValue").toFloat();
                    pPoint->m_fBaseValue = tagElement.attribute("BaseValue").toFloat();
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
                        nDatatype = tagElement.attribute("DataType").toInt();
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
#else
/*!
 * \brief  功能概述 初始化Device_1_2_Map和Device_3_4_Map 功能码1、2、3、4的Map，初始化功能码1、2、3、4的Map
 * \param  参数描述 DeviceListElement_是通道配置文件中的设备列表节点<DeviceList>
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
/*!
 \brief CGI 使用

 \fn CModbusRtuProtocol::SetFunctionMap
 \param DeviceListElement_
*/
void CModbusRtuProtocol::SetFunctionMap(const QDomElement &DeviceListElement_)
{
    QDomNodeList DeviceList = DeviceListElement_.childNodes();
    for (int i = 0; i <DeviceList.count(); ++i)
    {
        qDebug()<<"for (int i = 0; i <DeviceList.count(); ++i) "<<DeviceList.at(i).toElement().attribute("Name");
        QDomElement DeviceElement = DeviceList.at(i).toElement();
//        int DeviceAddress = DeviceElement.attribute("Address").toInt();///<zzy 2015/1/14 修改:
        int DeviceAddress/* = DeviceElement.attribute("Address").toInt()*/;///<zzy 2015/1/14 修改:
//        QDomNodeList PointElement = DeviceElement.childNodes();

        QMap<int,CPointBaseModbusRtu *> *Function_RX_Map = new QMap<int,CPointBaseModbusRtu *>;///遥信
        QMap<int,CPointBaseModbusRtu *> *Function_RC_Map = new QMap<int,CPointBaseModbusRtu *>;///遥测
        QMap<int,CPointBaseModbusRtu *> *Function_RK_Map = new QMap<int,CPointBaseModbusRtu *>;///遥控
        QMap<int,CPointBaseModbusRtu *> *Function_RS_Map = new QMap<int,CPointBaseModbusRtu *>;///遥设
//        DeviceAddress = DeviceElement.childNodes().at(0).toElement().attribute("Attribute").toInt();///<zzy 2015/1/14 修改:
        for (int nType = 0; nType < DeviceElement.childNodes().count(); ++nType)
        {
            QDomElement TypeElem = DeviceElement.childNodes().at(nType).toElement();
            if (TypeElem.tagName() == "Attribute")///
            {
                DeviceAddress = TypeElem.attribute("Device_Address").toInt();
                qDebug()<<"DeviceAddress="<<DeviceAddress;
            }else
            {
                qDebug()<<"DeviceElement.childNodes().at(nType).toElement();"<<TypeElem.attribute("Name")<<TypeElem.childNodes().count();
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
#endif

/*!
 * \brief  功能概述 初始化采集信息报文
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusRtuProtocol::SetInitSend_RX_RC_FrameList()
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
void CModbusRtuProtocol::Make_RX_RC_Query(QMap<int, CPointBaseModbusRtu *> *pFunction_)
{
    QMap<int, CPointBaseModbusRtu *>::iterator PointIterator;
    BYTE szSend[100] = {0};
    BYTE *pCur = szSend;
    int nDeviceAddress = -1;
    int nFunctionNo = -1;
    int nStartAddress = -1;
    int nRegNumber = -1;
    WORD nCRC = 0;
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
        {
            //格式: 地址 功能码	[起始地址] [寄存器数目]	 [CRC校验]
            *pCur++ = nDeviceAddress;
            *pCur++ = nFunctionNo;
            *pCur++ = HIBYTE(nStartAddress);
            *pCur++ = LOBYTE(nStartAddress);
            while (1)
            {
                if ( (++PointIterator!=pFunction_->end()) && (PointIterator.key()==(nStartAddress+nRegNumber)))
                {
                    nFunctionNo = PointIterator.value()->m_nFunction;
                    if (nFunctionNo == 1)
                    {
                        nRegNumber += PointIterator.value()->m_nRegisterCount;
                    }else
                    {
                        break;
                    }
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

            nCRC = CRC(szSend,pCur-szSend);
            *pCur++ = HIBYTE(nCRC);
            *pCur++ = LOBYTE(nCRC);
            QByteArray SendArray((char *)szSend,pCur-szSend);
            ModbusRtuQuery *pQuery = new ModbusRtuQuery;
            pQuery->szSend = SendArray;
            pQuery->m_nResponseLength = (nRegNumber+7)/8+5;///返回帧长度
            pQuery->m_nResponseZiJieChangDu = (nRegNumber+7)/8;
            pQuery->m_nStartAddress = nStartAddress;///起始地址
            pQuery->m_nFunction = 1;
            Send_RX_RC_FrameList.append(pQuery);
        }
            break;
        case 2:
        {
            //格式: 地址 功能码	[起始地址] [寄存器数目]	 [CRC校验]
            *pCur++ = nDeviceAddress;
            *pCur++ = nFunctionNo;
            *pCur++ = HIBYTE(nStartAddress);
            *pCur++ = LOBYTE(nStartAddress);
            while (1)
            {
                if ( (++PointIterator!=pFunction_->end()) && (PointIterator.key()==(nStartAddress+nRegNumber)) && (nFunctionNo == 2) )
                {
                    nFunctionNo = PointIterator.value()->m_nFunction;
                    if (nFunctionNo == 2)
                    {
                        nRegNumber += PointIterator.value()->m_nRegisterCount;
                    }else
                    {
                        break;
                    }
//                    nRegNumber += PointIterator.value()->m_nRegisterCount;
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

            nCRC = CRC(szSend,pCur-szSend);
            *pCur++ = HIBYTE(nCRC);
            *pCur++ = LOBYTE(nCRC);
            QByteArray SendArray((char *)szSend,pCur-szSend);
            ModbusRtuQuery *pQuery = new ModbusRtuQuery;
            pQuery->szSend = SendArray;
            pQuery->m_nResponseLength = (nRegNumber+7)/8+5;///返回帧长度
            pQuery->m_nResponseZiJieChangDu = (nRegNumber+7)/8;
            pQuery->m_nStartAddress = nStartAddress;///起始地址
            pQuery->m_nFunction = 2;
            Send_RX_RC_FrameList.append(pQuery);
        }
            break;
        case 3:
        {
            //格式: 地址 功能码	[起始地址] [寄存器数目]	 [CRC校验]
            *pCur++ = nDeviceAddress;
            *pCur++ = nFunctionNo;

            *pCur++ = HIBYTE(nStartAddress);
            *pCur++ = LOBYTE(nStartAddress);
            while (1)
            {
                if ((++PointIterator!=pFunction_->end())&&(PointIterator).key() == (nStartAddress + nRegNumber) && (nFunctionNo == 3) )
                {
                    nFunctionNo = PointIterator.value()->m_nFunction;
                    if (nFunctionNo == 3)
                    {
                        nRegNumber += PointIterator.value()->m_nRegisterCount;
                    }else
                    {
                        break;
                    }
//                    nRegNumber += PointIterator.value()->m_nRegisterCount;
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

            nCRC = CRC(szSend,pCur-szSend);
            *pCur++ = HIBYTE(nCRC);
            *pCur++ = LOBYTE(nCRC);
            QByteArray SendArray((char *)szSend,pCur-szSend);
            ModbusRtuQuery *pQuery = new ModbusRtuQuery;
            pQuery->szSend = SendArray;
            pQuery->m_nResponseLength = nRegNumber*2+5;///返回帧长度
            pQuery->m_nResponseZiJieChangDu = nRegNumber*2;
            pQuery->m_nStartAddress = nStartAddress;///起始地址
            pQuery->m_nFunction = 3;
            Send_RX_RC_FrameList.append(pQuery);
        }
            break;
        case 4:
        {
            //格式: 地址 功能码	[起始地址] [寄存器数目]	 [CRC校验]
            *pCur++ = nDeviceAddress;
            *pCur++ = nFunctionNo;

            *pCur++ = HIBYTE(nStartAddress);
            *pCur++ = LOBYTE(nStartAddress);
            while (1)
            {
                if ((++PointIterator!=pFunction_->end())&&(PointIterator).key() == (nStartAddress + nRegNumber) && (nFunctionNo == 4) )
                {
                    nFunctionNo = PointIterator.value()->m_nFunction;
                    if (nFunctionNo == 4)
                    {
                        nRegNumber += PointIterator.value()->m_nRegisterCount;
                    }else
                    {
                        break;
                    }
//                    nRegNumber += PointIterator.value()->m_nRegisterCount;
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

            nCRC = CRC(szSend,pCur-szSend);
            *pCur++ = HIBYTE(nCRC);
            *pCur++ = LOBYTE(nCRC);
            QByteArray SendArray((char *)szSend,pCur-szSend);
            ModbusRtuQuery *pQuery = new ModbusRtuQuery;
            pQuery->szSend = SendArray;
            pQuery->m_nResponseLength = nRegNumber*2+5;///返回帧长度
            pQuery->m_nResponseZiJieChangDu = nRegNumber*2;
            pQuery->m_nStartAddress = nStartAddress;///起始地址
            pQuery->m_nFunction = 4;
            Send_RX_RC_FrameList.append(pQuery);
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
void CModbusRtuProtocol::Make_RK_Query(QString strLink_, int nDeviceAdd_, bool bOn_)
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
            WORD nCRC;
            int nStartAddress = PointIterator.value()->m_nRegisterAddress;
            int nFunctionNumber = PointIterator.value()->m_nFunction;
            int nValue = 0;
            //格式: 设备地址  功能码	[起始地址] [    值    ]   [CRC校验]
            //      01        05     00 00     FF/00 00      XX XX
            *pCur++ = nDeviceAdd_;
            *pCur++ = nFunctionNumber;

            *pCur++ = HIBYTE(nStartAddress);
            *pCur++ = LOBYTE(nStartAddress);
            switch(nFunctionNumber)
            {
            case 5:
            {
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
            }
                break;
            case 6:
            {
                if (bOn_)
                {
                    nValue = 0x0100;
                    *pCur++ = HIBYTE(nValue);
                    *pCur++ = LOBYTE(nValue);
                }else
                {
                    *pCur++ = HIBYTE(nValue);
                    *pCur++ = LOBYTE(nValue);
                }
            }
                break;
            default:
                return;
            }



            nCRC = CRC(szSend,pCur-szSend);
            *pCur++ = HIBYTE(nCRC);
            *pCur++ = LOBYTE(nCRC);
            QByteArray SendArray((char *)szSend,pCur-szSend);
            ModbusRtuQuery *pQuery = new ModbusRtuQuery;
            pQuery->szSend = SendArray;
            pQuery->m_nResponseLength = 8;///返回帧长度
//            pQuery->m_nResponseZiJieChangDu = nRegNumber*2;
            pQuery->m_nStartAddress = nStartAddress;///起始地址
            pQuery->m_nFunction = nFunctionNumber;
            Send_RK_RS_FrameList.append(pQuery);
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
void CModbusRtuProtocol::Make_RS_Query(QString strLink_, int nDeviceAdd_, QVariant varValue_)
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
            WORD nCRC;
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
            nCRC = CRC(szSend,pCur-szSend);
            *pCur++ = HIBYTE(nCRC);
            *pCur++ = LOBYTE(nCRC);
            QByteArray SendArray((char *)szSend,pCur-szSend);
            ModbusRtuQuery *pQuery = new ModbusRtuQuery;
            pQuery->szSend = SendArray;
            pQuery->m_nResponseLength = 8;///返回帧长度
            pQuery->m_nStartAddress = nStartAddress;///起始地址
            pQuery->m_nFunction = 0x10;
            Send_RK_RS_FrameList.append(pQuery);
            break;
        }
    }
}


/*!
 * \brief  功能概述 生成CRC校验码，以下数据用于标准16位CRC码值计算
 * \param  参数描述 pInfo是数据地址，nLen是数据长度
 * \return 返回值描述 返回两字节的CRC校验码
 * \author zzy
 * \date   2015/5/25
 */
WORD CModbusRtuProtocol::CRC(const BYTE *pInfo, int nLen)
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
void CModbusRtuProtocol::Polling()
{

    if ((GetSysAbsTime() - m_nLastTime) < 1)///如果最后发送时间小于1秒则不发送
    {
        return;
    }
//    qDebug()<<Send_RK_RS_FrameList.count()<<"=====";
    if (!Send_RK_RS_FrameList.isEmpty())
    {
        m_mutexProcessData.lock();
        if (m_pLink->write(Send_RK_RS_FrameList.at(0)->szSend))
        {
            m_baReceiveFrame.clear();
            m_pModbusRtuQuery = Send_RK_RS_FrameList.at(0);
//            qDebug()<<"发送了遥控报文--------"<<Send_RK_RS_FrameList.at(0)->szSend.toHex();
            ShowMessage(0,Send_RK_RS_FrameList.at(0)->szSend.toHex());
            Send_RK_RS_FrameList.removeAt(0);
            m_nLastTime = GetSysAbsTime();
        }
//        m_pSerialPort->write(Send_RK_RS_FrameList.at(0)->szSend);
        m_mutexProcessData.unlock();
        return;
    }

//    qDebug()<<Send_RX_RC_FrameList.count()<<"--------------1";
    if (Send_RX_RC_FrameList.count() == 0)
    {
        return;
    }else if (SendFrameListNumber >= Send_RX_RC_FrameList.count())
    {
        SendFrameListNumber = 0;
//        qDebug()<<"从零开始发送";
    }
//    qDebug()<<"发送第:"<<SendFrameListNumber<<"个";
    m_mutexProcessData.lock();
    if (m_pLink->write(Send_RX_RC_FrameList.at(SendFrameListNumber)->szSend))
    {
        m_baReceiveFrame.clear();
        m_pModbusRtuQuery = Send_RX_RC_FrameList.at(SendFrameListNumber);
        ShowMessage(0,Send_RX_RC_FrameList.at(SendFrameListNumber)->szSend.toHex());
        SendFrameListNumber++;
        m_nLastTime = GetSysAbsTime();
    }
//    m_pSerialPort->write(Send_RX_RC_FrameList.at(SendFrameListNumber)->szSend);
    m_mutexProcessData.unlock();
}


/*!
 * \brief  功能概述 定时器槽函数，定时调用Polling函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusRtuProtocol::OnTimeOut()
{
//    qDebug()<<"void CModbusRtuProtocol::OnTimeOut()"<<QThread::currentThreadId()<<QTime::currentTime().toString("hh:mm:ss.zzz");
    Polling();
}

/*!
 * \brief  功能概述 用于响应点的值发生变化的函数
 * \param  参数描述 pPointBase_是点的指针，VarSet_是点的值
 * \return 返回值描述 返回true
 * \author zzy
 * \date   2015/5/25
 */
bool CModbusRtuProtocol::OnTagChange(CPointBase *pPointBase_, QVariant VarSet_)
{
    /// 转发遥控、遥设
    /// 生成遥控、遥设报文
    /// 转发驱动中不做处理
//    qDebug()<<"bool CModbusRtuProtocol::OnTagChange("<<QThread::currentThreadId();

    switch (pPointBase_->m_nTagType) {
    case TAG_TYPE_AI://遥测
//        m_pValue->SetVarValue(valueSet_);
        break;
    case TAG_TYPE_AO://遥设
//        qDebug()<<"生成遥设报文 并下发下发遥设--------------";
        Make_RS_Query(pPointBase_->m_strRTDBName,pPointBase_->GetDeviceAddress(),VarSet_);
        break;
    case TAG_TYPE_DI://遥信
//        m_pValue->SetVarValue(valueSet_);
        break;
    case TAG_TYPE_DO://遥控
    {
        if (VarSet_.toInt())
        {
//            qDebug()<<"生成遥控报文 并下发下发遥控ffffff++++++++++++++++++++"<<VarSet_<<pPointBase_->m_strPointComment<<pPointBase_->GetDeviceAddress();
            Make_RK_Query(pPointBase_->m_strRTDBName,pPointBase_->GetDeviceAddress(),true);
        }else
        {
//            qDebug()<<"生成遥控报文 并下发下发遥控000000++++++++++++++++++++"<<VarSet_;
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
void CModbusRtuProtocol::ProcessRespond(QByteArray data)
{
    if (m_pModbusRtuQuery == NULL)
    {
        return;
    }

    m_baReceiveFrame.append(data);
    if (m_baReceiveFrame.isEmpty())
        return;
    ShowMessage(1,m_baReceiveFrame.toHex());
    if (m_baReceiveFrame.count() < 3)
        return;
    QMutexLocker locker(&m_mutexProcessData);
//    m_mutexProcessData.lock();///处理数据锁
    QString strChangeValue = "";
    QString strOldChannelValue = "";
    QVariant OriginalVariant;
    QVariant ProjectVariant;

    m_bufferRecieve.SetData(m_baReceiveFrame.data(),m_baReceiveFrame.count());
#if 0
    WORD FrameCRC = m_bufferRecieve.GetUIntValue_Hl(m_bufferRecieve.GetBufferSize()-2,2);
    WORD MakeCRC = CRC(m_bufferRecieve.GetBuffer(0),m_bufferRecieve.GetBufferSize()-2);
    if (FrameCRC != MakeCRC)/// 首先判断CRC校验
    {
        qDebug()<<"cuowu"<<QByteArray((char *)m_bufferRecieve.GetBuffer(0),m_bufferRecieve.GetBufferSize()).toHex();
        qDebug()<<QByteArray((char *)&FrameCRC,2).toHex()<<QByteArray((char*)&MakeCRC,2).toHex();
//        m_mutexProcessData.unlock();
        return;
    }
        qDebug()<<"zhengque"<<QByteArray((char *)m_bufferRecieve.GetBuffer(0),m_bufferRecieve.GetBufferSize()).toHex();
        qDebug()<<QByteArray((char *)&FrameCRC,2).toHex()<<QByteArray((char*)&MakeCRC,2).toHex();
#endif
    if (m_bufferRecieve.GetUIntValue(1,1) & 0x80)
    {
        int nFunctionNumber = m_bufferRecieve.GetUIntValue(1,1) & 0x7F;
        int nExceptionNumber = m_bufferRecieve.GetUIntValue(2,1);
        switch (nFunctionNumber) {
        case 0x01:

            break;
        case 0x02:

            break;
        case 0x03:

            break;
        case 0x04:

            break;
        case 0x05:
        {
            CTagI *pTag = m_pRTDB->FindTag(m_strLink);
            if (pTag)
            {
                strChangeValue = "执行->异常码:"+QString::number(nExceptionNumber);
                pTag->GetPointCollection()->GetValue()->SetWriteResult(0,strChangeValue);
            }
        }
            break;
        case 0x06:

            break;
        case 0x10:
        {
            CTagI *pTag = m_pRTDB->FindTag(m_strLink);
            if (m_pRTDB->FindTag(m_strLink))
            {
                strChangeValue = "执行->异常码:"+QString::number(nExceptionNumber);
                pTag->GetPointCollection()->GetValue()->SetWriteResult(0,strChangeValue);
            }
        }
            break;
        default:
            break;
        }

        /// 处理完数据后改变m_nLastTime
        m_nLastTime = 0;
//        m_mutexProcessData.unlock();///解锁
        return;
    }

    int nCount = m_bufferRecieve.GetBufferSize();
    if (m_pModbusRtuQuery->m_nResponseLength <= nCount)
    {
        int nDeviceAddress = m_bufferRecieve.GetUIntValue(0,1);///设备地址
        int nFunctionNumber = m_bufferRecieve.GetUIntValue(1,1);///功能码
//        qDebug()<<"if (m_pModbusRtuQuery->m_nResponseLength <= nCount)"<<m_pModbusRtuQuery->m_nResponseLength<<m_bufferRecieve.GetBufferSize();
        if (nFunctionNumber != m_pModbusRtuQuery->m_nFunction)
        {
            qDebug()<<"if (nFunctionNumber != m_pModbusRtuQuery->m_nFunction)"<<"nFunctionNumber ="<<nFunctionNumber<<"m_pModbusRtuQuery->m_nFunction ="<<m_pModbusRtuQuery->m_nFunction;
//            m_mutexProcessData.unlock();///解锁
            return;
        }
        CPointBaseModbusRtu *pTagAttribute;///点的信息指针
        QString strLink;
        int nTagRegCount;
        int nDataType;
        int nByteOffset;///偏移
        QMap<int,CPointBaseModbusRtu *> *pFunctionMap;


        switch (nFunctionNumber) {
        case 0x01:
        case 0x02:
            /** *设备地址  功能码    字节数  数据    CRC校验
             *    01     01/02      01    03   两字节校验
             *   处理遥测数据
            * **/
            {
//                if (m_bufferRecieve.GetUIntValue(2,1) != m_pModbusRtuQuery->m_nResponseZiJieChangDu)
//                {
//                    m_mutexProcessData.unlock();
//                    return;
//                }
            if (!Device_RX_Map.contains(nDeviceAddress))/// 判断是否有此设备地址
            {
//                m_mutexProcessData.unlock();
                return;
            }
            pFunctionMap = Device_RX_Map.value(nDeviceAddress);
            int nRegCount = m_bufferRecieve.GetUIntValue(2,1)/2;///寄存器数

            int nRXCount = (BYTE)m_pModbusRtuQuery->szSend.at(4)*256+(BYTE)m_pModbusRtuQuery->szSend.at(5);///遥信数量
            int nSizeCount = m_bufferRecieve.GetUIntValue(2,1);
            if (nSizeCount+3 > m_bufferRecieve.GetBufferSize())
            {
//                m_mutexProcessData.unlock();
                return;
            }
            if (nSizeCount*8<nRegCount)
            {
                nRegCount = nSizeCount*8;
            }
            int nBitOffset = 0;///位偏移量

            for (int i = 0; i < nRXCount;)
            {
                nByteOffset = (i+7)/8+3;///返回帧的字节偏移
                BYTE valueByte = m_bufferRecieve.GetUIntValue(nByteOffset,1);///遥信量所在的字节
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
//                    CTagI *pTag = m_pRTDB->FindTag(strLink);
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
        /// 处理完数据后改变m_nLastTime
        m_nLastTime = 0;
//        m_mutexProcessData.unlock();///解锁
        return;
//            break;
        case 0x03:
        case 0x04:
        /** *设备地址  功能码  字节数  数据高字节  数据低字节  CRC校验
        *    01      03      02    08        02       两字节校验
        *   处理遥测数据
        * **/
        {
//                if (m_bufferRecieve.GetUIntValue(2,1) != m_pModbusRtuQuery->m_nResponseZiJieChangDu)
//                {
//                    m_mutexProcessData.unlock();
//                    return;
//                }

            if (!Device_RC_Map.contains(nDeviceAddress))/// 判断是否有此设备地址
            {
                qDebug()<<"if (!Device_RC_Map.contains(nDeviceAddress))/// 判断是否有此设备地址 无此设备地址"<<"nDeviceAddress ="<<nDeviceAddress;
//                m_mutexProcessData.unlock();
                return;
            }
            pFunctionMap = Device_RC_Map.value(nDeviceAddress);
            int nSizeCount = m_bufferRecieve.GetUIntValue(2,1);
            if (nSizeCount+3 > m_bufferRecieve.GetBufferSize())
            {
//                m_mutexProcessData.unlock();
                return;
            }
//            qDebug()<<nSizeCount<<QByteArray((char *)m_bufferRecieve.GetBuffer(0),m_bufferRecieve.GetBufferSize()).toHex();
            int nRegCount = m_bufferRecieve.GetUIntValue(2,1)/2;///寄存器数
            int nSendFrameRegCount = (BYTE)m_pModbusRtuQuery->szSend.at(4)*256+(BYTE)m_pModbusRtuQuery->szSend.at(5);
//            qDebug()<<"<nRegCount<<nSendFrameRegCount"<<nRegCount<<nSendFrameRegCount;
            if (nSendFrameRegCount<nRegCount)
            {
                nRegCount = nSendFrameRegCount;
            }
//            qDebug()<<nRegCount<<nSendFrameRegCount;

            float kValue;
            float BaseValue;
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
                switch (nTagRegCount)//寄存器个数
                {
                case 1://单寄存器
                    switch(nDataType)
                    {
                    case DATA_TYPE_INT32S://有符号数
                    {
                        int nValue = m_bufferRecieve.GetIntValue_Hl(nByteOffset,2);//((BYTE)m_baReceiveFrame.at(nByteOffset)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+1));
                        ProjectVariant.setValue(((nValue+BaseValue)*kValue));
                        OriginalVariant.setValue(m_bufferRecieve.GetIntValue_Hl(nByteOffset,2));
                    }
                        break;
                    case DATA_TYPE_INT32U://无符号数
                    {
                        unsigned int uValue = m_bufferRecieve.GetUIntValue_Hl(nByteOffset,2);//(((BYTE)m_baReceiveFrame.at(nByteOffset)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+1));
                        ProjectVariant.setValue(((uValue+BaseValue)*kValue));
                        OriginalVariant.setValue(m_bufferRecieve.GetUIntValue_Hl(nByteOffset,2));//(((BYTE)m_baReceiveFrame.at(nByteOffset)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+1)));
                    }
                        break;
                    default:
                        break;
                    }
                    i++;      //下一寄存器地址
                    break;
                case 2://双寄存器
                    switch(nDataType)
                    {
                    case DATA_TYPE_INT32S://有符号数
                    {
//                            int nValue = (((BYTE)m_baReceiveFrame.at(nByteOffset)<<24)|((BYTE)m_baReceiveFrame.at(nByteOffset+1)<<16)
//                                          |((BYTE)m_baReceiveFrame.at(nByteOffset+2)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+3));
                        int nValue = m_bufferRecieve.GetIntValue_Hl(nByteOffset,4);
                        ProjectVariant.setValue(((nValue+BaseValue)*kValue));
                        OriginalVariant.setValue(m_bufferRecieve.GetIntValue_Hl(nByteOffset,4));//((BYTE)m_baReceiveFrame.at(nByteOffset)<<24)|((BYTE)m_baReceiveFrame.at(nByteOffset+1)<<16)
//                                                       |((BYTE)m_baReceiveFrame.at(nByteOffset+2)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+3));
                    }
                        break;
                    case DATA_TYPE_INT32U://无符号数
                    {
//                            unsigned int uValue = (((BYTE)m_baReceiveFrame.at(nByteOffset)<<24)|((BYTE)m_baReceiveFrame.at(nByteOffset+1)<<16)
//                                                   |((BYTE)m_baReceiveFrame.at(nByteOffset+2)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+3));
                        unsigned int uValue = m_bufferRecieve.GetUIntValue_Hl(nByteOffset,4);
                        ProjectVariant.setValue(((uValue+BaseValue)*kValue));
                        OriginalVariant.setValue(m_bufferRecieve.GetUIntValue_Hl(nByteOffset,4));//((BYTE)m_baReceiveFrame.at(nByteOffset)<<24)|((BYTE)m_baReceiveFrame.at(nByteOffset+1)<<16)
//                                                       |((BYTE)m_baReceiveFrame.at(nByteOffset+2)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+3));
                    }
                        break;
                    case DATA_TYPE_FP32://浮点数
                    {
                        BYTE temp[4];
//                            temp[0] = (BYTE)m_baReceiveFrame.at(nByteOffset);
//                            temp[1] = (BYTE)m_baReceiveFrame.at(nByteOffset+1);
//                            temp[2] = (BYTE)m_baReceiveFrame.at(nByteOffset+2);
//                            temp[3] = (BYTE)m_baReceiveFrame.at(nByteOffset+3);
                        temp[0] = m_bufferRecieve.GetUIntValue(nByteOffset,1);
                        temp[1] = m_bufferRecieve.GetUIntValue(nByteOffset+1,1);
                        temp[2] = m_bufferRecieve.GetUIntValue(nByteOffset+2,1);
                        temp[3] = m_bufferRecieve.GetUIntValue(nByteOffset+3,1);

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
//                CTagI *pTag = m_pRTDB->FindTag(strLink);
                if (pTag)
                {
                    pTag->SetValue(pTagAttribute,ProjectVariant,OriginalVariant);
                }
            }

        }
            /// 处理完数据后改变m_nLastTime
            m_nLastTime = 0;
//            m_mutexProcessData.unlock();///解锁
            return;
            break;
        case 0x05:
            /** *设备地址  功能码
             *
             * */
        {
            CTagI *tag = m_pRTDB->FindTag(m_strLink);
            if (m_pModbusRtuQuery->szSend == m_baReceiveFrame)
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
            /// 处理完数据后改变m_nLastTime
            m_nLastTime = 0;
//            m_mutexProcessData.unlock();///解锁
            return;
//            break;
//        case 0x06:
//            break;
        case 0x10:
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
            /// 处理完数据后改变m_nLastTime
            m_nLastTime = 0;
//            m_mutexProcessData.unlock();///解锁
            return;
//            break;
        default:
//            m_mutexProcessData.unlock();///解锁
            return;
//            break;
        }

//        m_mutexProcessData.unlock();///解锁
        return;
    }

//    m_mutexProcessData.unlock();///解锁
    return;
////   function end
}

#if 0
/*!
 * \brief  功能概述 响应readyRead(QByteArray)信号，解析收到的报文函数
 * \param  data 是收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusRtuProtocol::ProcessRespond(QByteArray data)
{
    /// 此槽函数由串口的readyRead()信号触发 处理返回的帧 即解包函数
//    QByteArray m_baReceiveFrame = data;

    if (m_pModbusRtuQuery == NULL)
    {
        return;
    }
    m_baReceiveFrame.append(data);
    if (m_baReceiveFrame.isEmpty())
        return;
    ShowMessage(1,m_baReceiveFrame.toHex());
    if (m_baReceiveFrame.count() < 3)
        return;
    m_mutexProcessData.lock();///处理数据锁
    QString strChangeValue = "";
    QString strOldChannelValue = "";
    QVariant OriginalVariant;
    QVariant ProjectVariant;
    if ((BYTE)m_baReceiveFrame.at(1)&0x80)///收到异常码 处理异常码事件
    {
//        qDebug()<<"异常码";
        int nFunctionNumber = (BYTE)m_baReceiveFrame.at(1)&0x7F;///获取功能码
        int nExceptionNumber = (BYTE)m_baReceiveFrame.at(2);///获取异常码
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
                pTag->GetPointCollection()->GetValue()->SetWriteResult(0,strChangeValue);
            }
        }
            break;
        case 6:

            break;
        case 16:
        {
            CTagI *pTag = m_pRTDB->FindTag(m_strLink);
            if (m_pRTDB->FindTag(m_strLink))
            {
                strChangeValue = "执行->异常码:"+QString::number(nExceptionNumber);
                pTag->GetPointCollection()->GetValue()->SetWriteResult(0,strChangeValue);
            }
        }
            break;
        default:
            break;
        }

        /// 处理完数据后改变m_nLastTime
        m_nLastTime = 0;
        m_mutexProcessData.unlock();///解锁
        return;
    }
    int nCount = m_baReceiveFrame.count();
    if (m_pModbusRtuQuery->m_nResponseLength <= nCount)///返回长度相等
    {
        int nDeviceAddress = (BYTE)m_baReceiveFrame.at(0);///设备地址
        int nFunctionNumber = (BYTE)m_baReceiveFrame.at(1);///功能码

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
            int nRXCount = (BYTE)m_pModbusRtuQuery->szSend.at(4)*256+(BYTE)m_pModbusRtuQuery->szSend.at(5);///遥信数量
            int nBitOffset = 0;///位偏移量
            pFunctionMap = Device_RX_Map.value(nDeviceAddress);
            if (pFunctionMap == NULL)
                break;
            for (int i = 0; i < nRXCount;)
            {
                nByteOffset = (i+7)/8+3;///返回帧的字节偏移
                if (m_baReceiveFrame.count()<=nByteOffset)
                {
                    i++;
                    continue;
                }
                BYTE valueByte = (BYTE)m_baReceiveFrame.at(nByteOffset);///遥信量所在的字节
                for (int j = 0; j < 8;++j)
                {

                    nBitOffset = i%8;///位偏移量
                    pTagAttribute = pFunctionMap->value(m_pModbusRtuQuery->m_nStartAddress+i);
                    nTagRegCount = pTagAttribute->m_nRegisterCount;
                    nDataType = pTagAttribute->m_nDataType;
                    strLink = pTagAttribute->m_strRTDBName;
                    unsigned int bValue = (valueByte>>nBitOffset)&0x1;
                    strChangeValue.setNum(bValue);
                    strOldChannelValue.setNum(bValue);
                    CTagI *pTag = m_pRTDB->FindTag(strLink);
                    if (pTag)
                    {
                        ProjectVariant.setValue(bValue);
                        OriginalVariant.setValue(bValue);
                        pTag->SetValue(ProjectVariant,OriginalVariant);
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
            int nRegCount = (BYTE)m_baReceiveFrame.at(2)/2;///寄存器数
            float kValue;
            float BaseValue;
            pFunctionMap  = Device_RC_Map.value(nDeviceAddress);
            if (pFunctionMap == NULL)
                break;
            for (int i = 0; i < nRegCount;)
            {
                nByteOffset = i*2+3;///返回帧的偏移
                pTagAttribute = pFunctionMap->value(m_pModbusRtuQuery->m_nStartAddress+i);
                nTagRegCount = pTagAttribute->m_nRegisterCount;
                nDataType = pTagAttribute->m_nDataType;
                strLink = pTagAttribute->m_strRTDBName;
                kValue = pTagAttribute->m_fKValue;
                BaseValue = pTagAttribute->m_fBaseValue;
                switch (nTagRegCount)//寄存器个数
                {
                case 1://单寄存器
                    switch(nDataType)
                    {
                    case DATA_TYPE_INT32S://有符号数
                    {
                        if (m_baReceiveFrame.count()<= nByteOffset+1)
                        {
                            i++;
                            continue;
                        }
                        int nValue = (((BYTE)m_baReceiveFrame.at(nByteOffset)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+1));
                        ProjectVariant.setValue(((nValue+BaseValue)*kValue));
                        OriginalVariant.setValue((((BYTE)m_baReceiveFrame.at(nByteOffset)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+1)));
                    }
                        break;
                    case DATA_TYPE_INT32U://无符号数
                    {
                        if (m_baReceiveFrame.count()<= nByteOffset+1)
                        {
                            i++;
                            continue;
                        }
                        unsigned int uValue = (((BYTE)m_baReceiveFrame.at(nByteOffset)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+1));
                        ProjectVariant.setValue(((uValue+BaseValue)*kValue));
                        OriginalVariant.setValue((((BYTE)m_baReceiveFrame.at(nByteOffset)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+1)));
                    }
                        break;
                    default:
                        break;
                    }
                    i++;      //下一寄存器地址
                    break;
                case 2://双寄存器
                    switch(nDataType)
                    {
                    case DATA_TYPE_INT32S://有符号数
                    {
                        if (m_baReceiveFrame.count()<= nByteOffset+3)
                        {
                            i+=2;
                            continue;
                        }
                        int nValue = (((BYTE)m_baReceiveFrame.at(nByteOffset)<<24)|((BYTE)m_baReceiveFrame.at(nByteOffset+1)<<16)
                                      |((BYTE)m_baReceiveFrame.at(nByteOffset+2)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+3));
                        ProjectVariant.setValue(((nValue+BaseValue)*kValue));
                        OriginalVariant.setValue(((BYTE)m_baReceiveFrame.at(nByteOffset)<<24)|((BYTE)m_baReceiveFrame.at(nByteOffset+1)<<16)
                                                   |((BYTE)m_baReceiveFrame.at(nByteOffset+2)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+3));
                    }
                        break;
                    case DATA_TYPE_INT32U://无符号数
                    {
                        if (m_baReceiveFrame.count()<= nByteOffset+3)
                        {
                            i+=2;
                            continue;
                        }
                        unsigned int uValue = (((BYTE)m_baReceiveFrame.at(nByteOffset)<<24)|((BYTE)m_baReceiveFrame.at(nByteOffset+1)<<16)
                                               |((BYTE)m_baReceiveFrame.at(nByteOffset+2)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+3));
                        ProjectVariant.setValue(((uValue+BaseValue)*kValue));
                        OriginalVariant.setValue(((BYTE)m_baReceiveFrame.at(nByteOffset)<<24)|((BYTE)m_baReceiveFrame.at(nByteOffset+1)<<16)
                                                   |((BYTE)m_baReceiveFrame.at(nByteOffset+2)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+3));
                    }
                        break;
                    case DATA_TYPE_FP32://浮点数
                    {
                        if (m_baReceiveFrame.count()<= nByteOffset+3)
                        {
                            i+=2;
                            continue;
                        }
                        BYTE temp[4];
                        temp[0] = (BYTE)m_baReceiveFrame.at(nByteOffset);
                        temp[1] = (BYTE)m_baReceiveFrame.at(nByteOffset+1);
                        temp[2] = (BYTE)m_baReceiveFrame.at(nByteOffset+2);
                        temp[3] = (BYTE)m_baReceiveFrame.at(nByteOffset+3);

                        int nInfoSize = 4;
                        int nGroupSize = 4;
                        float fValue = *(float *)temp;
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
                CTagI *pTag = m_pRTDB->FindTag(strLink);
                if (pTag)
                {
                    pTag->SetValue(ProjectVariant,OriginalVariant);
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
            if (m_pModbusRtuQuery->szSend == m_baReceiveFrame)
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
        m_nLastTime = 0;
        m_mutexProcessData.unlock();///解锁
        return;
    }
    m_mutexProcessData.unlock();///解锁
    return;
}
#endif
#if 0
/*!
 * \brief  功能概述 响应readyRead(QByteArray)信号，解析收到的报文函数
 * \param  data 是收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusRtuProtocol::ProcessRespond(QByteArray data)
{
    /// 此槽函数由串口的readyRead()信号触发 处理返回的帧 即解包函数
//    QByteArray m_baReceiveFrame = data;

    if (m_pModbusRtuQuery == NULL)
    {
        return;
    }
    m_baReceiveFrame.append(data);
    if (m_baReceiveFrame.isEmpty())
        return;
    ShowMessage(1,m_baReceiveFrame.toHex());
    if (m_baReceiveFrame.count() <= 3)
        return;
    qDebug()<<m_baReceiveFrame.toHex()<<"----";
    m_mutexProcessData.lock();///处理数据锁
    QString strChangeValue = "";
    QString strOldChannelValue = "";
    QVariant OriginalVariant;
    QVariant ProjectVariant;
    qDebug()<<m_baReceiveFrame.toHex()<<"--2";
    if ((BYTE)m_baReceiveFrame.at(1)&0x80)///收到异常码 处理异常码事件
    {
//        qDebug()<<"异常码";
        int nFunctionNumber = (BYTE)m_baReceiveFrame.at(1)&0x7F;///获取功能码
        int nExceptionNumber = (BYTE)m_baReceiveFrame.at(2);///获取异常码
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
                pTag->GetPointCollection()->GetValue()->SetWriteResult(0,strChangeValue);
            }
        }
            break;
        case 6:

            break;
        case 16:
        {
            CTagI *pTag = m_pRTDB->FindTag(m_strLink);
            if (m_pRTDB->FindTag(m_strLink))
            {
                strChangeValue = "执行->异常码:"+QString::number(nExceptionNumber);
                pTag->GetPointCollection()->GetValue()->SetWriteResult(0,strChangeValue);
            }
        }
            break;
        default:
            break;
        }

        /// 处理完数据后改变m_nLastTime
        m_nLastTime = 0;
        m_mutexProcessData.unlock();///解锁
        return;
    }
    int nCount = m_baReceiveFrame.count();
    qDebug()<<m_baReceiveFrame.toHex()<<"--3";
    if (m_pModbusRtuQuery->m_nResponseLength <= nCount)///返回长度相等
    {
        int nDeviceAddress = (BYTE)m_baReceiveFrame.at(0);///设备地址
        int nFunctionNumber = (BYTE)m_baReceiveFrame.at(1);///功能码

        CPointBaseModbusRtu *pTagAttribute;///点的信息指针
        QString strLink;
        int nTagRegCount;
        int nDataType;
        int nByteOffset;///偏移
        QMap<int,CPointBaseModbusRtu *> *pFunctionMap;
        qDebug()<<m_baReceiveFrame.toHex()<<"--4";
        switch (nFunctionNumber)
        {
        case 1:
        case 2:
        /** *设备地址  功能码    字节数  数据    CRC校验
         *    01     01/02      01    03   两字节校验
         *   处理遥测数据
        * **/
        {
            qDebug()<<m_baReceiveFrame.toHex()<<"--5";
            int nRXCount = (BYTE)m_pModbusRtuQuery->szSend.at(4)*256+(BYTE)m_pModbusRtuQuery->szSend.at(5);///遥信数量
            int nBitOffset = 0;///位偏移量
            pFunctionMap = Device_RX_Map.value(nDeviceAddress);
            if (pFunctionMap == NULL)
                break;
            for (int i = 0; i < nRXCount;)
            {
                nByteOffset = (i+7)/8+3;///返回帧的字节偏移
                if (nByteOffset >= (BYTE)m_baReceiveFrame.count())
                    break;
                BYTE valueByte = (BYTE)m_baReceiveFrame.at(nByteOffset);///遥信量所在的字节
                for (int j = 0; j < 8;++j)
                {

                    nBitOffset = i%8;///位偏移量
                    pTagAttribute = pFunctionMap->value(m_pModbusRtuQuery->m_nStartAddress+i);
                    nTagRegCount = pTagAttribute->m_nRegisterCount;
                    nDataType = pTagAttribute->m_nDataType;
                    strLink = pTagAttribute->m_strRTDBName;
                    unsigned int bValue = (valueByte>>nBitOffset)&0x1;
                    strChangeValue.setNum(bValue);
                    strOldChannelValue.setNum(bValue);
                    CTagI *pTag = m_pRTDB->FindTag(strLink);
                    if (pTag)
                    {
                        ProjectVariant.setValue(bValue);
                        OriginalVariant.setValue(bValue);
                        pTag->SetValue(ProjectVariant,OriginalVariant);
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
            qDebug()<<m_baReceiveFrame.toHex()<<"--6";
            int nRegCount = (BYTE)m_baReceiveFrame.at(2)/2;///寄存器数
            float kValue;
            float BaseValue;
            pFunctionMap  = Device_RC_Map.value(nDeviceAddress);
            if (pFunctionMap == NULL)
                break;
            for (int i = 0; i < nRegCount;)
            {
                nByteOffset = i*2+3;///返回帧的偏移
                pTagAttribute = pFunctionMap->value(m_pModbusRtuQuery->m_nStartAddress+i);
                nTagRegCount = pTagAttribute->m_nRegisterCount;
                nDataType = pTagAttribute->m_nDataType;
                strLink = pTagAttribute->m_strRTDBName;
                kValue = pTagAttribute->m_fKValue;
                BaseValue = pTagAttribute->m_fBaseValue;

                if (nByteOffset >= (BYTE)m_baReceiveFrame.count())
                    break;
                switch (nTagRegCount)//寄存器个数
                {
                case 1://单寄存器
                    switch(nDataType)
                    {
                    case DATA_TYPE_INT32S://有符号数
                    {
                        int nValue = (((BYTE)m_baReceiveFrame.at(nByteOffset)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+1));
                        ProjectVariant.setValue(((nValue+BaseValue)*kValue));
                        OriginalVariant.setValue((((BYTE)m_baReceiveFrame.at(nByteOffset)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+1)));
                    }
                        break;
                    case DATA_TYPE_INT32U://无符号数
                    {
                        unsigned int uValue = (((BYTE)m_baReceiveFrame.at(nByteOffset)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+1));
                        ProjectVariant.setValue(((uValue+BaseValue)*kValue));
                        OriginalVariant.setValue((((BYTE)m_baReceiveFrame.at(nByteOffset)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+1)));
                    }
                        break;
                    default:
                        break;
                    }
                    i++;      //下一寄存器地址
                    break;
                case 2://双寄存器
                    switch(nDataType)
                    {
                    case DATA_TYPE_INT32S://有符号数
                    {
                        int nValue = (((BYTE)m_baReceiveFrame.at(nByteOffset)<<24)|((BYTE)m_baReceiveFrame.at(nByteOffset+1)<<16)
                                      |((BYTE)m_baReceiveFrame.at(nByteOffset+2)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+3));
                        ProjectVariant.setValue(((nValue+BaseValue)*kValue));
                        OriginalVariant.setValue(((BYTE)m_baReceiveFrame.at(nByteOffset)<<24)|((BYTE)m_baReceiveFrame.at(nByteOffset+1)<<16)
                                                   |((BYTE)m_baReceiveFrame.at(nByteOffset+2)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+3));
                    }
                        break;
                    case DATA_TYPE_INT32U://无符号数
                    {
                        unsigned int uValue = (((BYTE)m_baReceiveFrame.at(nByteOffset)<<24)|((BYTE)m_baReceiveFrame.at(nByteOffset+1)<<16)
                                               |((BYTE)m_baReceiveFrame.at(nByteOffset+2)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+3));
                        ProjectVariant.setValue(((uValue+BaseValue)*kValue));
                        OriginalVariant.setValue(((BYTE)m_baReceiveFrame.at(nByteOffset)<<24)|((BYTE)m_baReceiveFrame.at(nByteOffset+1)<<16)
                                                   |((BYTE)m_baReceiveFrame.at(nByteOffset+2)<<8)|(BYTE)m_baReceiveFrame.at(nByteOffset+3));
                    }
                        break;
                    case DATA_TYPE_FP32://浮点数
                    {
                        BYTE temp[4];
                        temp[0] = (BYTE)m_baReceiveFrame.at(nByteOffset);
                        temp[1] = (BYTE)m_baReceiveFrame.at(nByteOffset+1);
                        temp[2] = (BYTE)m_baReceiveFrame.at(nByteOffset+2);
                        temp[3] = (BYTE)m_baReceiveFrame.at(nByteOffset+3);

                        int nInfoSize = 4;
                        int nGroupSize = 4;
                        float fValue = *(float *)temp;
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
                CTagI *pTag = m_pRTDB->FindTag(strLink);
                if (pTag)
                {
                    pTag->SetValue(ProjectVariant,OriginalVariant);
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
            if (m_pModbusRtuQuery->szSend == m_baReceiveFrame)
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
        m_nLastTime = 0;
        m_mutexProcessData.unlock();///解锁
        return;
    }
    m_mutexProcessData.unlock();///解锁
    return;
}
#endif
/*!
 * \brief  功能概述 是否成功连接的槽函数
 * \param  参数描述 bool类型的参数是代表是否成功连接的状态，如果为true则成功连接，如果为false则未成功连接
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CModbusRtuProtocol::slotConnect(bool result_)
{
    if (result_)
    {
        m_pTimer->start(100);///循环开始
    }else
    {
        ;
    }
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
