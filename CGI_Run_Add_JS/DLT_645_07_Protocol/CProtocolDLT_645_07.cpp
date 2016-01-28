/*!
 * \file    CProtocolDLT_645_07.cpp
 * \brief   概述 CProtocolDLT_645_07类的源文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/27
 *
 * \b       修改记录：
 * \li      2015/5/27
 *  添加注释
 */

#include "CProtocolDLT_645_07.h"
#include "../PortInclude/CPRTI.h"
#include "../PortInclude/CPRTMapI.h"
#include "../PortInclude/CRTDBI.h"
#include "../PortInclude/CTagI.h"
#include "CPointDLT_645_07.h"
#include "stdlib.h"

#include <QDomDocument>
#include <QDomElement>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QDir>

/*!
 * \brief 功能概述 库文件导出接口函数
 * \param 参数描述 parent是QObject对象指针
 * \return 返回值描述 返回创建的驱动指针
 * \author zzy
 * \date 2015/1/14
 */
CProtocolI * CreateDriver(QObject *parent)
{
    CProtocolDLT_645_07 *testProtocol = new CProtocolDLT_645_07(parent);
    return  testProtocol;
}

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 parent是父对象的指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
CProtocolDLT_645_07::CProtocolDLT_645_07(QObject */*parent*/)
{
    m_nLastTime = 0;
    m_nSendFrameListNumber = 0;

}

/*!
 * \brief  功能概述 创建此通道
 * \param  参数描述 strChannelFileName_是通道文件绝对路径名，pDatabaseReturn是实时库指针
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CProtocolDLT_645_07::OnCreateChannel(const QString strChannelFileName_, CRTDBI *pDatabaseReturn)
{
    SetPROTOCOL_TYPE(Collect_Type);
    if (!CProtocolBase::OnCreateChannel(strChannelFileName_,pDatabaseReturn))
        return false;

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
    if (docElem.childNodes().count() != 5)///<zzy 2015/1/15 修改:
        return false;
//    QDomElement Channel    = docElem.childNodes().at(0).toElement();
//    QDomElement Top        = docElem.childNodes().at(1).toElement();
//    QDomElement Protocol   = docElem.childNodes().at(2).toElement();
//    QDomElement MainPort   = docElem.childNodes().at(3).toElement();
    QDomElement DeviceList = docElem.childNodes().at(4).toElement();


    SetDataTypeMap(DeviceList);
    m_nTimerID = startTimer(2000);
    return true;
}

bool CProtocolDLT_645_07::OnCreateChannel(const QDomElement &ChannelElement_, ChannelType nChannelType_, int nComOrCanNumber_, CRTDBI *pDatabaseReturn_)
{
    SetPROTOCOL_TYPE(Collect_Type);
    if (!CProtocolBase::OnCreateChannel(ChannelElement_,nChannelType_,nComOrCanNumber_,pDatabaseReturn_))
        return false;

    if (ChannelElement_.elementsByTagName("DeviceList").isEmpty())
        return false;
    QDomElement DeviceList = ChannelElement_.elementsByTagName("DeviceList").at(0).toElement();


    CGI_SetDataTypeMap(DeviceList);
    m_nTimerID = startTimer(2000);
    return true;
}

/*!
 * \brief  功能概述 初始化Device_Read_Map的Map
 * \param  参数描述 DeviceListElement_是通道配置文件中的设备列表节点<DeviceList>
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CProtocolDLT_645_07::SetDataTypeMap(const QDomElement &DeviceListElement_)
{
    QDomNodeList DeviceList = DeviceListElement_.childNodes();
    for (int i = 0; i <DeviceList.count(); ++i)
    {
        QDomElement DeviceElement = DeviceList.at(i).toElement();
        QString strDeviceAddress;
        QByteArray arrayDeviceAddress;
        DLT_645_07PointMap_2 *pDLT_645_07PointMap = new DLT_645_07PointMap_2;

        for (int nType = 0; nType < DeviceElement.childNodes().count(); ++nType)
        {
            QDomElement TypeElem = DeviceElement.childNodes().at(nType).toElement();
            if (TypeElem.tagName() == "Attribute")///
            {
                /// 设备地址左侧补0，一共12个字符
                strDeviceAddress = TypeElem.attribute("Address");
                strDeviceAddress = strDeviceAddress.trimmed();///< 修整 将左侧和右侧的空格和换行符移除
                strDeviceAddress = "000000000000" + strDeviceAddress;
                strDeviceAddress = strDeviceAddress.right(12);
            }else
            {
                for (int nPoint = 0; nPoint < TypeElem.childNodes().count(); ++nPoint)
                {
                    QDomElement tagElement = TypeElem.childNodes().at(nPoint).toElement();
                    CPointDLT_645_07 *pPoint = new CPointDLT_645_07;
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
                    pPoint->m_nByteCount = tagElement.attribute("ByteCount").toInt();
                    pPoint->m_nStartByte = tagElement.attribute("StartByte").toInt();
                    pPoint->m_nDecimalPlaces = tagElement.attribute("DecimalPlaces").toInt();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }

                    char *p = NULL;
                    pPoint->m_bDI3 = (BYTE)strtoul(tagElement.attribute("DI3").toLatin1().mid(0,2),&p,16);
                    pPoint->m_bDI2 = (BYTE)strtoul(tagElement.attribute("DI2").toLatin1().mid(0,2),&p,16);
                    pPoint->m_bDI1 = (BYTE)strtoul(tagElement.attribute("DI1").toLatin1().mid(0,2),&p,16);
                    pPoint->m_bDI0 = (BYTE)strtoul(tagElement.attribute("DI0").toLatin1().mid(0,2),&p,16);
                    BYTE DI[4];/// = {pPoint->m_bDI0+0x33,pPoint->m_bDI1+0x33,pPoint->m_bDI2+0x33,pPoint->m_bDI3+0x33};
                    DI[0] = pPoint->m_bDI0+0x33;///,pPoint->m_bDI1+0x33,pPoint->m_bDI2+0x33,pPoint->m_bDI3+0x33};
                    DI[1] = pPoint->m_bDI1+0x33;
                    DI[2] = pPoint->m_bDI2+0x33;
                    DI[3] = pPoint->m_bDI3+0x33;

                    unsigned int value = 0;/// = (unsigned int)(*((unsigned int *)DI));
                    memcpy((void *)(&value),DI,sizeof(unsigned int));
                    pPoint->m_unDI = value;
                    qDebug()<<pPoint->m_unDI<<"===========";
                    BYTE *pCur = pPoint->m_bDeviceAddr+5;
                    for (int nStart = 0; nStart < 6; ++nStart)
                    {
                        *pCur-- = (BYTE)strtoul(strDeviceAddress.toLatin1().mid(nStart*2,2),&p,16);
                    }
                    QByteArray address((char *)pPoint->m_bDeviceAddr,6);
                    arrayDeviceAddress = address;
                    pPoint->m_strDeviceAddr = address.toHex();
                    pPoint->m_arrayDeviceAddr = address;
//                    strDeviceAddress = pPoint->m_strDeviceAddr;

                    BYTE bDI[4];
                    bDI[0] = pPoint->m_bDI0;
                    bDI[1] = pPoint->m_bDI1;
                    bDI[2] = pPoint->m_bDI2;
                    bDI[3] = pPoint->m_bDI3;
                    QByteArray arrayDI((char *)bDI,4);
                    pPoint->m_strDI = arrayDI.toHex();

                    qDebug()<<tagElement.attribute("DI3")<<pPoint->m_bDI1<<tagElement.attribute("DI2")
                              <<tagElement.attribute("DI1")<<tagElement.attribute("DI0")
                                <<strDeviceAddress<<address.toHex()<<pPoint->m_strDeviceAddr<<pPoint->m_strDI;
                    pDLT_645_07PointMap->insert(pPoint->m_unDI,pPoint);

                    m_PointNode.AddPoint(pPoint);

                    CDLT_645_07Query *pQuery = new CDLT_645_07Query;
                    pQuery->m_arrayAddress = address;
                    pQuery->MakeQuery(address.data(),0x11,arrayDI.data(),4);
                    qDebug()<<pQuery->m_ByteArrayFrame.toHex();
                    DLT_645_07QueryList.append(pQuery);
                }
            }
        }
        if (!pDLT_645_07PointMap->isEmpty())
        {
            Device_Read_Map.insert(arrayDeviceAddress,pDLT_645_07PointMap);
        }
    }
}

void CProtocolDLT_645_07::CGI_SetDataTypeMap(const QDomElement &DeviceListElement_)
{
    QDomNodeList DeviceList = DeviceListElement_.childNodes();
    for (int i = 0; i <DeviceList.count(); ++i)
    {
        QDomElement DeviceElement = DeviceList.at(i).toElement();
        QString strDeviceAddress;
        QByteArray arrayDeviceAddress;
        DLT_645_07PointMap_2 *pDLT_645_07PointMap = new DLT_645_07PointMap_2;

        for (int nType = 0; nType < DeviceElement.childNodes().count(); ++nType)
        {
            QDomElement TypeElem = DeviceElement.childNodes().at(nType).toElement();
            if (TypeElem.tagName() == "Attribute")///
            {
                /// 设备地址左侧补0，一共12个字符
                strDeviceAddress = TypeElem.attribute("Device_Address");
                strDeviceAddress = strDeviceAddress.trimmed();///< 修整 将左侧和右侧的空格和换行符移除
                strDeviceAddress = "000000000000" + strDeviceAddress;
                strDeviceAddress = strDeviceAddress.right(12);
            }else
            {
                for (int nPoint = 0; nPoint < TypeElem.childNodes().count(); ++nPoint)
                {
                    QDomElement tagElement = TypeElem.childNodes().at(nPoint).toElement();
                    CPointDLT_645_07 *pPoint = new CPointDLT_645_07;
                    pPoint->m_strPointName = tagElement.attribute("Device_YC_Name_Strings");
                    pPoint->m_strPointComment = tagElement.attribute("Device_YC_Desc_Strings");
                    pPoint->m_nID = tagElement.attribute("Device_YC_SN_Strings").toInt();
                    pPoint->m_strRTDBName = tagElement.attribute("Device_YC_Link_Strings");
                    pPoint->m_nScanTime = tagElement.attribute("ScanTime").toInt();
                    pPoint->m_fKValue = tagElement.attribute("Device_YC_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_fBaseValue = tagElement.attribute("Device_YC_BaseValue_Strings").toFloat();
                    pPoint->m_nByteCount = tagElement.attribute("Device_YC_ByteCount_Strings").toInt();
                    pPoint->m_nStartByte = tagElement.attribute("Device_YC_StartByte_Strings").toInt();
                    pPoint->m_nDecimalPlaces = tagElement.attribute("Device_YC_DecimalPlaces_Strings").toInt();
                    pPoint->m_pProtocol = this;
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                    if (pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                    }

                    char *p = NULL;
                    pPoint->m_bDI3 = (BYTE)strtoul(tagElement.attribute("Device_YC_DI3_Strings").toLatin1().mid(0,2),&p,16);
                    pPoint->m_bDI2 = (BYTE)strtoul(tagElement.attribute("Device_YC_DI2_Strings").toLatin1().mid(0,2),&p,16);
                    pPoint->m_bDI1 = (BYTE)strtoul(tagElement.attribute("Device_YC_DI1_Strings").toLatin1().mid(0,2),&p,16);
                    pPoint->m_bDI0 = (BYTE)strtoul(tagElement.attribute("Device_YC_DI0_Strings").toLatin1().mid(0,2),&p,16);
                    BYTE DI[4];/// = {pPoint->m_bDI0+0x33,pPoint->m_bDI1+0x33,pPoint->m_bDI2+0x33,pPoint->m_bDI3+0x33};
                    DI[0] = pPoint->m_bDI0+0x33;///,pPoint->m_bDI1+0x33,pPoint->m_bDI2+0x33,pPoint->m_bDI3+0x33};
                    DI[1] = pPoint->m_bDI1+0x33;///,pPoint->m_bDI2+0x33,pPoint->m_bDI3+0x33};
                    DI[2] = pPoint->m_bDI2+0x33;///,pPoint->m_bDI3+0x33};
                    DI[3] = pPoint->m_bDI3+0x33;

                    unsigned int value = 0;/// = (unsigned int)(*((unsigned int *)DI));
                    memcpy((void *)(&value),DI,sizeof(unsigned int));
//                    pPoint->m_unDI = (unsigned int)(*((unsigned int *)DI));
                    pPoint->m_unDI = value;
                    qDebug()<<pPoint->m_unDI<<"===========";
                    BYTE *pCur = pPoint->m_bDeviceAddr+5;
                    for (int nStart = 0; nStart < 6; ++nStart)
                    {
                        *pCur-- = (BYTE)strtoul(strDeviceAddress.toLatin1().mid(nStart*2,2),&p,16);
                    }
                    QByteArray address((char *)pPoint->m_bDeviceAddr,6);
                    arrayDeviceAddress = address;
                    pPoint->m_strDeviceAddr = address.toHex();
                    pPoint->m_arrayDeviceAddr = address;
                    qDebug()<<"----------AA"<<pPoint->m_strDeviceAddr<<pPoint->m_arrayDeviceAddr.toHex();
//                    strDeviceAddress = pPoint->m_strDeviceAddr;

                    BYTE bDI[4];
                    bDI[0] = pPoint->m_bDI0;
                    bDI[1] = pPoint->m_bDI1;
                    bDI[2] = pPoint->m_bDI2;
                    bDI[3] = pPoint->m_bDI3;
                    QByteArray arrayDI((char *)bDI,4);
                    pPoint->m_strDI = arrayDI.toHex();

                    qDebug()<<tagElement.attribute("Device_YC_DI3_Strings")<<pPoint->m_bDI1<<tagElement.attribute("Device_YC_DI2_Strings")
                              <<tagElement.attribute("Device_YC_DI1_Strings")<<tagElement.attribute("Device_YC_DI0_Strings")
                                <<strDeviceAddress<<address.toHex()<<pPoint->m_strDeviceAddr<<pPoint->m_strDI;
                    pDLT_645_07PointMap->insert(pPoint->m_unDI,pPoint);

                    m_PointNode.AddPoint(pPoint);

                    CDLT_645_07Query *pQuery = new CDLT_645_07Query;
                    pQuery->m_arrayAddress = address;
                    pQuery->MakeQuery(address.data(),0x11,arrayDI.data(),4);
                    qDebug()<<pQuery->m_ByteArrayFrame.toHex();
                    DLT_645_07QueryList.append(pQuery);
                }
            }
        }
        if (!pDLT_645_07PointMap->isEmpty())
        {
            Device_Read_Map.insert(arrayDeviceAddress,pDLT_645_07PointMap);
        }
    }
}

/*!
 * \brief  功能概述 时间事件函数
 * \param  参数描述 event是定时器事件指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CProtocolDLT_645_07::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nTimerID)
    {
        qDebug()<<1;
        Polling();
    }
}

/*!
 * \brief  功能概述 下发命令帧
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CProtocolDLT_645_07::Polling()
{
    if ((GetSysAbsTime() - m_nLastTime) < 1)///如果最后发送时间小于1秒则不发送
    {
        return;
    }
    if (DLT_645_07QueryList.count() == 0)
    {
        return;
    }else if (m_nSendFrameListNumber >= DLT_645_07QueryList.count())
    {
        m_nSendFrameListNumber = 0;
//        qDebug()<<"从零开始发送";
    }
//    qDebug()<<"发送第:"<<SendFrameListNumber<<"个";
    if (NULL == m_pLink)
        return;
    if (!m_pLink->GetLinkState())
        return;
    m_mutexProcessData.lock();
//    m_pModbusRtuQuery = DLT_645_07QueryList.at(m_nSendFrameListNumber);
    CDLT_645_07Query *pQuery = DLT_645_07QueryList.at(m_nSendFrameListNumber);
    if (NULL != pQuery)
    {
        if (m_pLink->write(pQuery->m_ByteArrayFrame.data(),pQuery->m_ByteArrayFrame.count()))
        {
            m_arrayCurrentAddress = pQuery->m_arrayAddress;
            m_arrayRespondBuffer.clear();
            m_mutexProcessData.unlock();
            ShowMessage(0,DLT_645_07QueryList.at(m_nSendFrameListNumber)->m_ByteArrayFrame.toHex());
            m_nSendFrameListNumber++;
        }
    }
    m_nLastTime = GetSysAbsTime();
}
#if 0
/*!
 * \brief  功能概述 响应readyRead(QByteArray)信号，解析收到的报文函数
 * \param  参数描述 Respond_是收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CProtocolDLT_645_07::ProcessRespond(QByteArray Respond_)
{

//    qDebug()<<Respond_.toHex()<<"打印收到的信息";
    QMutexLocker locker(&m_mutexProcessData);

    m_arrayRespondBuffer.append(Respond_);
    ShowMessage(1,m_arrayRespondBuffer.toHex());
    DecodeReceive(m_arrayRespondBuffer);
}
#else
/*!
 * \brief  功能概述 响应readyRead(QByteArray)信号，解析收到的报文函数
 * \param  参数描述 Respond_是收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CProtocolDLT_645_07::ProcessRespond(QByteArray Respond_)
{

//    qDebug()<<Respond_.toHex()<<"打印收到的信息";
    QMutexLocker locker(&m_mutexProcessData);

    m_bufferReceive.AppendData(Respond_.data(),Respond_.count());

    QByteArray array((char *)m_bufferReceive.GetBuffer(0),m_bufferReceive.GetInfoSize());
    qDebug()<<"array.toHex()"<<array.toHex();

    if (m_bufferReceive.GetInfoSize() < MinFrameLength)
        return ;
//    ShowMessage(1,m_arrayRespondBuffer.toHex());
//    DecodeReceive(m_arrayRespondBuffer);
    for(int nStart=0; nStart<=m_bufferReceive.GetInfoSize() - MinFrameLength; ++nStart)
    {
        int nFrameLength = IsAFrame(m_bufferReceive.GetBuffer(nStart),m_bufferReceive.GetInfoSize()-nStart);
        if(nFrameLength)
        {
            m_Response.SetData(m_bufferReceive.GetBuffer(nStart),nFrameLength);
            QByteArray frame;
            if (nStart != 0)
            {
                frame = QByteArray((char*)m_bufferReceive.GetBuffer(0),nStart);
                ShowMessage(SHOWMESSAGE_TYPE_DEBUG,frame.toHex().toUpper());
            }
            frame = QByteArray((char*)m_Response.GetBuffer(0),m_Response.GetInfoSize());
            ShowMessage(SHOWMESSAGE_TYPE_RECEIVE,frame.toHex().toUpper());
            m_bufferReceive.Fifo(nStart+nFrameLength);
            ProcessRespond(m_Response);
//            ProcessResponse(m_CDTResponse);
            nStart=-1;//重新再来
        }
    }
    if(m_bufferReceive.GetInfoSize()>MAX_FRAME)
    {
        m_bufferReceive.SetInfoSize(0);
        Q_ASSERT(false);
    }
}
#endif

/*!
 * \brief  功能概述 是否成功连接的槽函数
 * \param  参数描述 bool类型的参数是代表是否成功连接的状态，如果为true则成功连接，如果为false则未成功连接
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
void CProtocolDLT_645_07::slotConnect(bool)
{

}

/*!
 * \brief  功能概述 生成代码和
 * \param  参数描述 pInfo_是数据的头指针，nSize_是数据的个数
 * \return 返回值描述 返回生成的代码和
 * \author zzy
 * \date   2015/5/27
 */
DWORD CProtocolDLT_645_07::MakeSum(const void *pInfo_, size_t nSize_)
{
    //代码和
    Q_ASSERT(pInfo_ !=NULL);
    Q_ASSERT(nSize_ > 0);

    DWORD wResult = 0;
    for(size_t i=0; i<nSize_; i++)
    {
        wResult += ((BYTE *)pInfo_)[i];
    }
    return wResult;
}

void CProtocolDLT_645_07::ProcessRespond(CDLT_645_07Response &response)
{
    if (response.GetUIntValue(8,1) & 0x40)
    {
        return;
    }
    else
    {
        QByteArray address((char *)(response.GetBuffer(0)+1),6);/// 设备地址
        unsigned int unDeviceAddress = response.GetUIntValue(10,4);
        qDebug()<<"response.GetUIntValue(10,4);"<<unDeviceAddress;
        m_PointNode.FindPointMap(address,unDeviceAddress);
        DLT_645_07PointMap *pPointMap = m_PointNode.FindPointMap(address,unDeviceAddress);
        if (pPointMap)
        {
            qDebug()<<"if (pPointMap)"<<pPointMap->count()<<address.toHex()<<unDeviceAddress;
            DLT_645_07PointMap::iterator iterator;
            for (iterator = pPointMap->begin(); iterator != pPointMap->end(); ++iterator)
            {
                CPointDLT_645_07 *pPoint = iterator.value();
                if (pPoint)
                {
                    QString strLink = pPoint->m_strRTDBName;
                    int nByteCount = pPoint->m_nByteCount;
                    int nStartByte = pPoint->m_nStartByte;
                    qDebug()<<"if (pPoint)"<<pPoint->m_strRTDBName<<pPoint->m_nByteCount<<pPoint->m_nStartByte;

                    BYTE nLength = response.GetUIntValue(9,1);
                    if ((nByteCount+4) <= nLength)
                    {
                        qDebug()<<"if ((nByteCount+4) <= nLength)";
                        BYTE Value[nByteCount];
                        for (int i = 0; i < nByteCount; ++i)
                        {
                            Value[i] = response.GetUIntValue(i+14+nStartByte,1);
                            Value[i]-= 0x33;
                        }
                        BYTE Temp[nByteCount];
                        for (int i = 0; i < nByteCount; ++i)
                        {
                            Temp[i] = Value[nByteCount-1-i];
                        }
                        QByteArray baValue((char *)Temp,nByteCount);

                        baValue = baValue.toHex();
                        ShowMessage(2,baValue);

                        int nDecimalPlaces = pPoint->m_nDecimalPlaces;/// @note 小数点位数
                        QString Project_(baValue);
                        if (nDecimalPlaces)
                        {
                            if (Project_.count()<nDecimalPlaces)
                            {
                                Project_ = "00000000000000000000"+Project_;
                                Project_.insert(Project_.count()-nDecimalPlaces,'.');
                                Project_ = Project_.right(nDecimalPlaces+2);
                            }else
                            {
                                Project_.insert(Project_.count()-nDecimalPlaces,'.');
                            }
                        }
                        while(Project_.count())
                        {
                            if (Project_.at(0)!='0')
                            {
                                break;
                            }else
                            {
                                Project_.remove(0,1);
                            }
                        }
                        if (Project_.count())
                        {
                            if (Project_.at(0)=='.')
                                Project_.insert(0,'0');
                        }

                        QVariant VarProject_;
                        QVariant VarOriginal_;
                        if (Project_.contains('.'))
                        {
                            VarProject_  = QVariant(Project_.toFloat());
                            VarOriginal_ = QVariant(Project_.toFloat());
                        }else
                        {

                            VarProject_  = QVariant(Project_.toInt());
                            VarOriginal_ = QVariant(Project_.toInt());
                        }
                        qDebug()<<"Project_:="<< Project_<<Project_.toFloat()<<Project_.toInt()<<VarProject_;

                        ShowMessage(2,VarProject_.toString()+"---"+VarOriginal_.toString());

                        CTagI *pTag = pPoint->GetTag();;
                        if (pTag)
                        {
                            pTag->SetValue(pPoint,VarProject_,VarOriginal_);
                        }
                    }
                }
            }
        }else
        {
            qDebug()<<"if (pPointMap)  NULL";
        }
    }
#if 0
    else
    {
        QByteArray address((char *)(response.GetBuffer(0)+1),6);/// 设备地址
        unsigned int unDeviceAddress = response.GetUIntValue(10,4);
        qDebug()<<"response.GetUIntValue(10,4);"<<unDeviceAddress;
        DLT_645_07PointMap *pPointMap = Device_Read_Map.value(address);
        if (pPointMap)
        {
            qDebug()<<"if (pPointMap)";
            CPointDLT_645_07 *pPoint = pPointMap->value(unDeviceAddress);
            if (pPoint)
            {
                QString strLink = pPoint->m_strRTDBName;
                int nByteCount = pPoint->m_nByteCount;
                int nStartByte = pPoint->m_nStartByte;
                qDebug()<<"if (pPoint)"<<pPoint->m_strRTDBName<<pPoint->m_nByteCount<<pPoint->m_nStartByte;

                BYTE nLength = response.GetUIntValue(9,1);
                if ((nByteCount+4) <= nLength)
                {
                    qDebug()<<"if ((nByteCount+4) <= nLength)";
                    BYTE Value[nByteCount];
                    for (int i = 0; i < nByteCount; ++i)
                    {
                        Value[i] = response.GetUIntValue(i+14+nStartByte,1);
                        Value[i]-= 0x33;
                    }
                    BYTE Temp[nByteCount];
                    for (int i = 0; i < nByteCount; ++i)
                    {
                        Temp[i] = Value[nByteCount-1-i];
                    }
                    QByteArray baValue((char *)Temp,nByteCount);

                    baValue = baValue.toHex();
                    ShowMessage(2,baValue);

                    int nDecimalPlaces = pPoint->m_nDecimalPlaces;/// @note 小数点位数
                    QString Project_(baValue);
                    if (nDecimalPlaces)
                    {
                        if (Project_.count()<nDecimalPlaces)
                        {
                            Project_ = "00000000000000000000"+Project_;
                            Project_.insert(Project_.count()-nDecimalPlaces,'.');
                            Project_ = Project_.right(nDecimalPlaces+2);
                        }else
                        {
                            Project_.insert(Project_.count()-nDecimalPlaces,'.');
                        }
                    }
                    while(Project_.count())
                    {
                        if (Project_.at(0)!='0')
                        {
                            break;
                        }else
                        {
                            Project_.remove(0,1);
                        }
                    }
                    if (Project_.count())
                    {
                        if (Project_.at(0)=='.')
                            Project_.insert(0,'0');
                    }

                    QVariant VarProject_;
                    QVariant VarOriginal_;
                    if (Project_.contains('.'))
                    {
                        VarProject_  = QVariant(Project_.toFloat());
                        VarOriginal_ = QVariant(Project_.toFloat());
                    }else
                    {

                        VarProject_  = QVariant(Project_.toInt());
                        VarOriginal_ = QVariant(Project_.toInt());
                    }
                    qDebug()<<"Project_:="<< Project_<<Project_.toFloat()<<Project_.toInt()<<VarProject_;

                    ShowMessage(2,VarProject_.toString()+"---"+VarOriginal_.toString());
                    CTagI *pTag = m_pRTDB->FindTag(strLink);
                    if (pTag)
                    {
                        pTag->SetValue(VarProject_,VarOriginal_);
                    }
                }else
                {
                    qDebug()<<"if ((nByteCount+4) > nLength)"<<nLength<<nByteCount;
                }
            }
        }
    }
#endif

    m_nLastTime = 0;
}

/*!
 * \brief  功能概述 判断帧是否正确
 * \param  参数描述 pBufferHead_报文头指针,nInfomationSize_报文长度
 * \return 返回值描述 如果帧错误则返回false，如果正确则返回帧长度
 * \author zzy
 * \date   2015/5/27
 */
int CProtocolDLT_645_07::ReceiveIsTrue(const BYTE *pBufferHead_,const int nInfomationSize_)
{
    if (0x68 != GetUInt(pBufferHead_,nInfomationSize_,0,1))
    {
        return false;
    }
    if (0x68 != GetUInt(pBufferHead_,nInfomationSize_,7,1))
    {
        return false;
    }
//    qDebug()<<"22222222"<<nLength<<(BYTE)GetUInt(pBufferHead_,nInfomationSize_,10+nLength,1);
    BYTE nLength = GetUInt(pBufferHead_,nInfomationSize_,9,1);
    if (nInfomationSize_ != 12+nLength )
    {
        return false;
    }
    if (0x16 != GetUInt(pBufferHead_,nInfomationSize_,11+nLength,1))
    {
        return false;
    }
    BYTE nCS = MakeSum(pBufferHead_,10+nLength);
//    qDebug()<<"CS="<<nCS<<nLength<<(BYTE)GetUInt(pBufferHead_,nInfomationSize_,10+nLength,1);
    if ((BYTE)GetUInt(pBufferHead_,nInfomationSize_,10+nLength,1) != nCS)
    {
        return false;
    }
    return nLength+12;///< 返回长度
}

/*!
 * \brief  功能概述 解析报文函数
 * \param  参数描述 data是收到的报文
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CProtocolDLT_645_07::DecodeReceive(const QByteArray data)
{
    if (data.count() < 12)///< 只有够12个字节才开始判断,不够12个字节不做判断
        return;
    BYTE szReceive[65535] = {0};
    BYTE *pCur = szReceive;
    int nCur = 0;
    for (; nCur < data.count(); ++nCur)
    {
        szReceive[nCur] = (BYTE)data.at(nCur);
    }

    while (pCur < (szReceive+nCur))///< 开始解包
    {
        if ( (szReceive+nCur-pCur) < 12)
        {
            m_arrayCurrentAddress.clear();
            return ;
        }
        int nDecodeLength = ReceiveIsTrue(pCur,nCur);

        if (nDecodeLength)
        {
            if (pCur[8] & 0x40)
            {
//                continue;
            }else
            {
                qDebug()<<"if (nDecodeLength) is true";
                QByteArray address((char *)(pCur+1),6);/// 设备地址
                unsigned int unDeviceAddress = *((unsigned int *)(pCur+10));/// 设备地址
                qDebug()<<"-------*((unsigned int *)(pCur+10));"<<unDeviceAddress;
                DLT_645_07PointMap_2 *pPointMap = Device_Read_Map.value(address);
                if (pPointMap)
                {
                    CPointDLT_645_07 *pPoint = pPointMap->value(unDeviceAddress);
                    if (pPoint)
                    {
                        QString strLink = pPoint->m_strRTDBName;
    //                    qDebug()<<pPoint->m_strRTDBName;
                        int nByteCount = pPoint->m_nByteCount;
                        qDebug()<<"BYTE nLength = GetUInt(pCur,nCur,9,1);1";
                        BYTE nLength = GetUInt(pCur,nCur,9,1);
                        qDebug()<<"BYTE nLength = GetUInt(pCur,nCur,9,1);2";
                        if ((nByteCount+4) == nLength)
                        {
                            BYTE Value[nByteCount];
                            for (int i = 0; i < nByteCount; ++i)
                            {
                                Value[i] = *(pCur+i+14);
                                Value[i]-= 0x33;
                            }
                            BYTE Temp[nByteCount];
                            for (int i = 0; i < nByteCount; ++i)
                            {
                                Temp[i] = Value[nByteCount-1-i];
                            }
                            QByteArray baValue((char *)Temp,nByteCount);
    //                        baValue.swap(baValue);

                            baValue = baValue.toHex();
                            ShowMessage(2,baValue);

                            int nDecimalPlaces = pPoint->m_nDecimalPlaces;/// @note 小数点位数
                            QString Project_(baValue);
        //                    QString Project_(QString::number(unValue));
                            if (nDecimalPlaces)
                            {
                                if (Project_.count()<nDecimalPlaces)
                                {
                                    Project_ = "00000000000000000000"+Project_;
                                    Project_.insert(Project_.count()-nDecimalPlaces,'.');
                                    Project_ = Project_.right(nDecimalPlaces+2);
                                }else
                                {
                                    Project_.insert(Project_.count()-nDecimalPlaces,'.');
                                }
                            }
                            while(Project_.count())
                            {
                                if (Project_.at(0)!='0')
                                {
                                    break;
                                }else
                                {
                                    Project_.remove(0,1);
                                }
                            }
                            if (Project_.count())
                            {
                                if (Project_.at(0)=='.')
                                    Project_.insert(0,'0');
                            }


                            QVariant VarProject_(Project_);
                            QVariant VarOriginal_(Project_);
                            ShowMessage(2,VarProject_.toString()+"---"+VarOriginal_.toString());
                            CTagI *pTag = pPoint->GetTag();
                            if (pTag)
                            {
                                pTag->SetValue(pPoint,VarProject_,VarOriginal_);
                            }
                        }else
                        {

                        }
//                        unsigned int unValue = GetUInt(pCur,nCur,14,nByteCount);
//                        BYTE Value[4];
//                        for (int i = 0; i < 4; ++i)
//                        {
//                            Value[i] = *(pCur+i+14);
//                            Value[i]-= 0x33;
//                        }
//                        BYTE Temp[4];
//                        for (int i = 0; i < 4; ++i)
//                        {
//                            Temp[i] = Value[3-i];
//                        }
//                        QByteArray baValue((char *)Temp,4);
//                        unsigned int unValue = GetUInt(pCur,nCur,14,nByteCount);


                    }
                }
            }
            ShowMessage(1,"jiexi"+data.toHex());/// @note 报文长度正确才显示报文
            m_arrayRespondBuffer.remove(0,nDecodeLength);/// @note 解析完毕后清空接收到的报文
            m_nLastTime = 0;
            pCur += nDecodeLength;
        }else
        {
            qDebug()<<"if (nDecodeLength) is error.";
            pCur++;
        }
    }
}

/*!
 \brief 获取报文中的值

 \fn CProtocolDLT_645_07::GetUInt
 \param pBufferHead 头指针
 \param nInfomationSize 缓冲区长度
 \param nStartByte 开始字节
 \param nSize 字节个数
 \return unsigned int 返回报文中获取的值
 \author zzy
 \date   2015/6/10
*/
unsigned int CProtocolDLT_645_07::GetUInt(const BYTE *pBufferHead, const int nInfomationSize, const int nStartByte, const int nSize ) const
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
 * \brief  功能概述 用于响应点的值发生变化的函数
 * \param  参数描述 pPointBase_是点的指针，VarSet_是点的值
 * \return 返回值描述 返回true
 * \author zzy
 * \date   2015/5/26
 */
bool CProtocolDLT_645_07::OnTagChange(CPointBase */*pPointBase_*/, QVariant /*VarSet_*/)
{
    return true;

}

int CProtocolDLT_645_07::IsAFrame(BYTE *pInfo, int nInfoLength)
{
    if (0x68 != GetUInt(pInfo,nInfoLength,0,1))
    {
        return 0;
    }
    if (0x68 != GetUInt(pInfo,nInfoLength,7,1))
    {
        return 0;
    }
    qDebug()<<"22222222AA";
    BYTE nLength = GetUInt(pInfo,nInfoLength,9,1);
    qDebug()<<"22222222BB"<<nLength<<(BYTE)GetUInt(pInfo,nInfoLength,10+nLength,1);
    if (nInfoLength != 12+nLength )
    {
        return 0;
    }
    if (0x16 != GetUInt(pInfo,nInfoLength,11+nLength,1))
    {
        return 0;
    }
    BYTE nCS = MakeSum(pInfo,10+nLength);
    qDebug()<<"CS="<<nCS<<nLength<<(BYTE)GetUInt(pInfo,nInfoLength,10+nLength,1);
    if ((BYTE)GetUInt(pInfo,nInfoLength,10+nLength,1) != nCS)
    {
        return 0;
    }
    return nLength+12;///< 返回长度
}

