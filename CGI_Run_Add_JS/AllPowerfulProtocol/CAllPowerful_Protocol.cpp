#include "CAllPowerful_Protocol.h"
#include "../PortInclude/CRTDBI.h"
#include "../PortInclude/CTagI.h"

/*!
 * \brief 功能概述 库文件导出接口函数
 * \param 参数描述 parent是QObject对象指针
 * \return 返回值描述 返回创建的驱动指针
 * \author zzy
 * \date 2015/1/14
 */
CProtocolI * CreateDriver(QObject *parent)
{
    CAllPowerful_Protocol *testProtocol = new CAllPowerful_Protocol(parent);
    return  testProtocol;
}
CAllPowerful_Protocol::CAllPowerful_Protocol(QObject */*parent*/)
{
    m_pCurrentQuery = NULL;
//    m_nTimerID = startTimer(10);
    m_nTimerID = startTimer(100);
    m_nLastSendFrameTime = 0;

}

bool CAllPowerful_Protocol::OnCreateChannel(const QString /*strChannelFileName_*/, CRTDBI */*pDatabaseReturn*/)
{
    return false;
}

bool CAllPowerful_Protocol::OnCreateChannel(const QDomElement &ChannelElement_, ChannelType nChannelType_, int nComOrCanNumber_, CRTDBI *pDatabaseReturn_)
{
    SetPROTOCOL_TYPE(Collect_Type);
    if (!CProtocolBase::OnCreateChannel(ChannelElement_,nChannelType_,nComOrCanNumber_,pDatabaseReturn_))
        return false;

    if (ChannelElement_.elementsByTagName("DeviceList").isEmpty())
        return false;
    QDomElement DeviceList = ChannelElement_.elementsByTagName("DeviceList").at(0).toElement();


//    CGI_SetDataTypeMap(DeviceList);
//    m_nTimerID = startTimer(2000);
    InitQuery(DeviceList);
    return true;
}

void CAllPowerful_Protocol::InitQuery(const QDomElement &DeviceListElement_)
{
    QDomNodeList DeviceList = DeviceListElement_.childNodes();
    for (int i = 0; i <DeviceList.count(); ++i)
    {
        QDomElement DeviceElement = DeviceList.at(i).toElement();
//        int strDeviceAddress;
        CAllPowerful_Query *pQuery = NULL;
        if (DeviceElement.elementsByTagName("Attribute").isEmpty())
        {
            continue ;
        }else
        {
//            strDeviceAddress = DeviceElement.elementsByTagName("Attribute").at(0).toElement().attribute("Device_Address").toInt();
        }

        for (int nType = 0; nType < DeviceElement.childNodes().count(); ++nType)
        {
            QDomElement TypeElem = DeviceElement.childNodes().at(nType).toElement();

            if (TypeElem.tagName() == "Attribute")
            {
//                strDeviceAddress = TypeElem.attribute("Device_Address").toInt();
            }else
            {
                if (TypeElem.attribute("Name") == "YX")
                {

                }else if (TypeElem.attribute("Name") == "YC")
                {
                    for (int nPoint = 0; nPoint < TypeElem.childNodes().count(); ++nPoint)
                    {
                        QDomElement tagElement = TypeElem.childNodes().at(nPoint).toElement();
                        CAllPowerful_Point *pPoint = new CAllPowerful_Point;
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
                        pPoint->m_pProtocol = this;
                        pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                        if (pPoint->m_pTag)
                        {
                            pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                        }
                        ///<zzy 2015/12/1 修改
                        int nValueType;
                        QString strDataType = tagElement.attribute("Device_YC_ValueType_Strings");
                        if (strDataType == "无符号型"){
                            nValueType = DATA_TYPE_INT32U;
                        }else if (strDataType == "有符号型"){
                            nValueType = DATA_TYPE_INT32S;
                        }else if (strDataType == "浮点型"){
                            nValueType = DATA_TYPE_FP32;
                        }else if (strDataType == "布尔型"){
                            nValueType = DATA_TYPE_BOOLEAN;
                        }else{
                            nValueType = tagElement.attribute("Device_YC_ValueType_Strings").toInt();
                        }
                        pPoint->m_nValueType = nValueType;///<zzy 2015/12/1 修改
                        QByteArray baSendFrame = tagElement.attribute("Device_YC_SendFrame_Strings").toUtf8().toUpper();
                        qDebug()<<"sssss:"<<baSendFrame;
                        if (m_AllPowerful_QueryMAP.contains(baSendFrame))
                        {
                            pQuery = m_AllPowerful_QueryMAP.value(baSendFrame);
                            pQuery->AddPoint(pPoint);
                        }else
                        {
                            pQuery = new CAllPowerful_Query;
                            pQuery->SetBaSendFrame(baSendFrame);
                            pQuery->AddPoint(pPoint);
                            m_AllPowerful_QueryMAP.insert(baSendFrame,pQuery);
                        }
                    }
                }else if (TypeElem.attribute("Name") == "YM")
                {

                }else if (TypeElem.attribute("Name") == "YK")
                {

                }else if (TypeElem.attribute("Name") == "YS")
                {

                }
            }
        }
    }
}

void CAllPowerful_Protocol::Polling()
{
    qDebug()<<"AAA"<<m_nLastSendFrameTime<<GetSysAbsTimeMSECS()<<ABS((GetSysAbsTimeMSECS() - m_nLastSendFrameTime));
    if (ABS((GetSysAbsTimeMSECS() - m_nLastSendFrameTime)) < 1000)///如果最后发送时间小于1秒则不发送
    {
        return;
    }
    qDebug()<<"BBB"<<m_nLastSendFrameTime<<GetSysAbsTimeMSECS()<<m_pCurrentQuery;
    if (m_pCurrentQuery == NULL)
    {
        qDebug()<<"CCC"<<m_nLastSendFrameTime<<GetSysAbsTimeMSECS();
        if (!m_AllPowerful_QueryMAP.isEmpty())
        {
//            m_AllPowerful_QueryMAP.begin().key();
            qDebug()<<"DDD"<<m_nLastSendFrameTime<<GetSysAbsTimeMSECS();
            m_pCurrentQuery = m_AllPowerful_QueryMAP.begin().value();
            qDebug()<<"DDD"<<m_nLastSendFrameTime<<GetSysAbsTimeMSECS()<<m_pCurrentQuery;
            if (SendFrame(m_pCurrentQuery))
            {
                if (m_bufferReceive.GetInfoSize() != 0)
                {
                    QByteArray frame = QByteArray((char *)m_bufferReceive.GetBuffer(),m_bufferReceive.GetInfoSize()).toHex().toUpper();
                    ShowMessage(SHOWMESSAGE_TYPE_DEBUG,frame);
                    m_bufferReceive.ClearBuffer();
                }
                ShowMessage(SHOWMESSAGE_TYPE_SEND,m_pCurrentQuery->GetbaSendFrame());
                m_nLastSendFrameTime = GetSysAbsTimeMSECS();
            }else
            {
                m_pCurrentQuery = NULL;
            }
        }
    }else
    {
        qDebug()<<"EEE"<<m_nLastSendFrameTime<<GetSysAbsTimeMSECS();
        if (!m_AllPowerful_QueryMAP.isEmpty())
        {
            qDebug()<<"FFF"<<m_nLastSendFrameTime<<GetSysAbsTimeMSECS();
            QMap<QByteArray, CAllPowerful_Query*>::iterator iterator;
            iterator = ++m_AllPowerful_QueryMAP.find(m_pCurrentQuery->GetbaSendFrame());
            if (iterator == m_AllPowerful_QueryMAP.end())
            {///< 最后一个
                m_pCurrentQuery = m_AllPowerful_QueryMAP.begin().value();
                if (SendFrame(m_pCurrentQuery))
                {
                    if (m_bufferReceive.GetInfoSize() != 0)
                    {
                        QByteArray frame = QByteArray((char *)m_bufferReceive.GetBuffer(),m_bufferReceive.GetInfoSize()).toHex().toUpper();
                        ShowMessage(SHOWMESSAGE_TYPE_DEBUG,frame);
                        m_bufferReceive.ClearBuffer();
                    }
                    ShowMessage(SHOWMESSAGE_TYPE_SEND,m_pCurrentQuery->GetbaSendFrame());
                    m_nLastSendFrameTime = GetSysAbsTimeMSECS();
                }else
                {
                    m_pCurrentQuery = NULL;
                }
            }else
            {///<
                m_pCurrentQuery = iterator.value();
                if (SendFrame(m_pCurrentQuery))
                {
                    if (m_bufferReceive.GetInfoSize() != 0)
                    {
                        QByteArray frame = QByteArray((char *)m_bufferReceive.GetBuffer(),m_bufferReceive.GetInfoSize()).toHex().toUpper();
                        ShowMessage(SHOWMESSAGE_TYPE_DEBUG,frame);
                        m_bufferReceive.ClearBuffer();
                    }
                    ShowMessage(SHOWMESSAGE_TYPE_SEND,m_pCurrentQuery->GetbaSendFrame());
                    m_nLastSendFrameTime = GetSysAbsTimeMSECS();
                }
            }
        }
    }
}

quint64 CAllPowerful_Protocol::SendFrame(CAllPowerful_Query *pQuery_)
{
    QByteArray sendFrame = QByteArray::fromHex(pQuery_->GetbaSendFrame());
    qDebug()<<"SEND"<<pQuery_->GetbaSendFrame();
    return m_pLink->write(sendFrame);
}

bool CAllPowerful_Protocol::OnTagChange(CPointBase */*pPointBase_*/, QVariant /*VarSet_*/)
{
    return true;
}

/*!
 \brief 解析报文函数

 \fn CAllPowerful_Protocol::ProcessRespond
 \param Respond_ 接收到的数据
*/
void CAllPowerful_Protocol::ProcessRespond(QByteArray Respond_)
{
    QMutexLocker lock(&m_mutexProcessData);
    m_bufferReceive.AppendData(Respond_.data(),Respond_.count());
    /// 查找一帧正确的报文并解析
    m_Response.SetData(m_bufferReceive.GetBuffer(0),m_bufferReceive.GetInfoSize());
    ProcessRespond(m_Response);

}

void CAllPowerful_Protocol::ProcessRespond(CAllPowerful_Response &response)
{
    if (m_pCurrentQuery == NULL)
    {/// 如果还未下发报文则查看是否为自动上传的协议类型

    }else
    {
        if (response.GetInfoSize() >= m_pCurrentQuery->GetMinResponseFrameLength())
        {/// 开始解析
            QByteArray frame = QByteArray((char *)response.GetBuffer(),response.GetInfoSize()).toHex().toUpper();
            ShowMessage(SHOWMESSAGE_TYPE_RECEIVE,frame);
            m_bufferReceive.ClearBuffer();
            QMap<int,CAllPowerful_Query::ByteCount_PointMAP*> *pMap = m_pCurrentQuery->Get_pAllPowerful_PointMAP();
            QMap<int,CAllPowerful_Query::ByteCount_PointMAP*>::iterator iterator;

            for (iterator = pMap->begin(); iterator != pMap->end(); ++iterator)
            {
                CAllPowerful_Query::ByteCount_PointMAP::iterator PointIterator;
                CAllPowerful_Query::ByteCount_PointMAP *pPointMap = iterator.value();
                for (PointIterator = pPointMap->begin(); PointIterator != pPointMap->end(); ++PointIterator)
                {
                    CAllPowerful_Point *pPoint = PointIterator.value();
                    CTagI *pTag = pPoint->GetTag();
                    if (pTag != NULL)
                    {
                        QVariant VarProject_;///< 工程值
                        QVariant VarOriginal_;///< 原始值
                        if (pPoint->m_nByteCount >= 5000)
                        {
                            int nStartBit = (pPoint->m_nByteCount%5000)/100;
                            int nBitCount = (pPoint->m_nByteCount%100);
                            switch (pPoint->m_nValueType) {
                            case DATA_TYPE_INT32U:/// "无符号型"
                            case DATA_TYPE_BOOLEAN:/// "布尔型"
                            {
                                unsigned int value = response.GetMidBitValue_u(pPoint->m_nStartByte,nStartBit,nBitCount);
                                VarOriginal_.setValue(value);
                                VarProject_.setValue((value+pPoint->m_fBaseValue)*pPoint->m_fKValue);
                            }
                                break;
                            case DATA_TYPE_INT32S:/// "有符号型"
                            case DATA_TYPE_FP32:/// "浮点型"
                            {
                                int value = response.GetMidBitValue_s(pPoint->m_nStartByte,nStartBit,nBitCount);
                                VarOriginal_.setValue(value);
                                VarProject_.setValue((value+pPoint->m_fBaseValue)*pPoint->m_fKValue);
                            }
                                break;
                            default:
                            {
                                unsigned int value = response.GetMidBitValue_u(pPoint->m_nStartByte,nStartBit,nBitCount);
                                VarOriginal_.setValue(value);
                                VarProject_.setValue((value+pPoint->m_fBaseValue)*pPoint->m_fKValue);
                            }
                                break;
                            }

                        }else if ( (pPoint->m_nByteCount>=10) && (pPoint->m_nByteCount<=17) )
                        {
                            int nBitOffset = pPoint->m_nByteCount%10;
                            unsigned int value = response.GetBit(pPoint->m_nStartByte,nBitOffset);
                            VarOriginal_.setValue(value);
                            VarProject_.setValue((value+pPoint->m_fBaseValue)*pPoint->m_fKValue);

                        }else if ( (pPoint->m_nByteCount >=1) && (pPoint->m_nByteCount<=4) )
                        {
                            qDebug()<<"pPoint->m_nValueType"<<pPoint->m_nValueType;
                            switch (pPoint->m_nValueType) {
                            case DATA_TYPE_INT32U:/// "无符号型"
                            {
                                unsigned int value = response.GetUIntValue_Hl(pPoint->m_nStartByte,pPoint->m_nByteCount);
                                VarOriginal_.setValue(value);
                                VarProject_.setValue((value+pPoint->m_fBaseValue)*pPoint->m_fKValue);
                            }
                                break;
                            case DATA_TYPE_BOOLEAN:/// "布尔型"
                            {
                                bool value = response.GetUIntValue_Hl(pPoint->m_nStartByte,pPoint->m_nByteCount);
                                VarOriginal_.setValue(value);
                                VarProject_.setValue(value);
                            }
                                break;
                            case DATA_TYPE_INT32S:/// "有符号型"
                            {
                                int value = response.GetIntValue_Hl(pPoint->m_nStartByte,pPoint->m_nByteCount);
                                VarOriginal_.setValue(value);
                                VarProject_.setValue((value+pPoint->m_fBaseValue)*pPoint->m_fKValue);
                            }
                                break;
                            case DATA_TYPE_FP32:/// "浮点型"
                            {
                                float value = response.GetFloatValue_Hl(pPoint->m_nStartByte,pPoint->m_nByteCount);
                                VarOriginal_.setValue(value);
                                VarProject_.setValue((value+pPoint->m_fBaseValue)*pPoint->m_fKValue);
                            }
                                break;
                            default:
                            {
                                unsigned int value = response.GetUIntValue_Hl(pPoint->m_nStartByte,pPoint->m_nByteCount);
                                VarOriginal_.setValue(value);
                                VarProject_.setValue((value+pPoint->m_fBaseValue)*pPoint->m_fKValue);
                            }
                                break;
                            }
                        }else
                        {

                        }
                        qDebug()<<VarProject_<<VarOriginal_<<pPoint->m_strRTDBName;
                        pTag->SetValue(pPoint,VarProject_,VarOriginal_);
                    }
                }///< for (PointIterator = pPointMap->begin(); PointIterator != pPointMap->end(); ++PointIterator)
            }///< for (iterator = pMap.begin(); iterator != pMap.end(); ++iterator) end
            qDebug()<<"Reset time.";
            m_nLastSendFrameTime = 0;///< 重置最后接收时间
        }
    }
}

void CAllPowerful_Protocol::slotConnect(bool)
{

}

void CAllPowerful_Protocol::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nTimerID)
    {
        Polling();
    }
}

bool CAllPowerful_Protocol::Add(const QByteArray &baSendFrmae_, CAllPowerful_Point *pPoint_)
{
    bool result = false;
    CAllPowerful_Query *pQuery = m_AllPowerful_QueryMAP.value(baSendFrmae_);
    if (pQuery == NULL)
    {
        pQuery = new CAllPowerful_Query;
        m_AllPowerful_QueryMAP.insert(baSendFrmae_,pQuery);
        result = pQuery->AddPoint(pPoint_);
    }else
    {
        result = pQuery->AddPoint(pPoint_);
    }
    return result;
}
