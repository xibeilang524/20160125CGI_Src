#include "../PortInclude/CPRTI.h"
#include "../PortInclude/CPRTMapI.h"
#include "../PortInclude/CRTDBI.h"
#include "../PortInclude/CTagI.h"
#include "stdlib.h"

#include <QDomDocument>
#include <QDomElement>
#include <QDebug>
#include <QFile>
#include <QDir>
#include "dlt645_protocol_97.h"
#include "dlt645_query.h"
/**
 **
 **  Brief：接口导出函数
 **  Parameter：
 **  Author：G.onion
 **  Creat Date：2015/09/24
 **  Last Modified：2015/09/24
 **
 **/
CProtocolI *CreateDriver(QObject *parent)
{
    DLT645_Protocol_97 *protocol = new DLT645_Protocol_97(parent);
    return protocol;
}

/**
 **
 **  Brief：构造函数
 **  Parameter： strChannelFile-通道绝对路径名   *pDataBaseReturn-实时库指针
 **  Author：G.onion
 **  Creat Date：2015/09/24
 **  Last Modified：2015/09/24
 **
 **/

DLT645_Protocol_97::DLT645_Protocol_97(QObject */*parent*/)
{
     pTimer = new QTimer;
     connect(pTimer,SIGNAL(timeout()),this,SLOT(OnTimeOut()));
     m_nLastTime = 0;
     SendFramListNumber = 0;
}

/**
 **
 **  Brief：通道创建函数
 **  Parameter：strChannelFile-通道绝对路径名    *pDataBaseReturn-实时库指针
 **  Author：G.onion
 **  Creat Date：2015/09/24
 **  Last Modified ：2015/10/08
 **
 **/

bool DLT645_Protocol_97::OnCreateChannel(const QString /*strChannelFile*/,CRTDBI */*pDataBaseReturn*/)
{
//    SetPROTOCOL_TYPE(Collect_Type);
//    if(!CProtocolBase::OnCreateChannel(strChannelFile,pDataBaseReturn))
//        return false;
//    QFile file(strChannelFile);
//    if(!file.open(QFile::ReadOnly|QFile::Text))
//        return false;
//    QDomDocument docchannel;
//    QString errorStr;
//    int errorLine;
//    int errorColumn;
//    if(!docchannel.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
//        return false;
//    file.close();
//    QDomElement docElement = docchannel.documentElement();
//    if(docElement.childNodes().count() != 5)
//        return false;
//    QDomElement DeviceList = docElement.childNodes().at(4).toElement();

//    SetFunctionMap(DeviceList);
//    m_nTimerID = startTimer(2000);
    return true;
//    SetInitSend_YC_Framlise();

}

bool DLT645_Protocol_97::OnCreateChannel(const QDomElement &ChannelElement_, ChannelType nChannelType_, int nComOrCanNumber_, CRTDBI *pDatabaseReturn_)
{
    SetPROTOCOL_TYPE(Collect_Type);
    if(!CProtocolBase::OnCreateChannel(ChannelElement_,nChannelType_,nComOrCanNumber_,pDatabaseReturn_))
        return false;

    if (ChannelElement_.elementsByTagName("DeviceList").isEmpty())
        return false;
    QDomElement DeviceList = ChannelElement_.elementsByTagName("DeviceList").at(0).toElement();

    CGI_SetFunctionMap(DeviceList);
    m_nTimerID = startTimer(2000);
    return true;

}

void DLT645_Protocol_97::CGI_SetFunctionMap(const QDomElement &DeviceListElement)
{
    QDomNodeList DeviceList =DeviceListElement.childNodes();
    for(int i=0 ;i<DeviceList.count(); ++i)
    {
        QDomElement DeviceElement = DeviceList.at(i).toElement();
        QString DeviceAddress;
 //       QMap<int, CPointBaseModbusRtu *> *Function_RX_Map;
  //      Function *Function_RC_Map = new Function;
 //       QMap<int, CPointBaseModbusRtu *> *Function_RK_Map;
 //       QMap<int, CPointBaseModbusRtu *> *Function_RS_Map;
        for(int nType=0; nType<DeviceElement.childNodes().count(); ++nType)
        {
            QDomElement TypeElement = DeviceElement.childNodes().at(nType).toElement();
            if(TypeElement.tagName() == "Attribute")
            {
                DeviceAddress = TypeElement.attribute("Device_Address");
                DeviceAddress = DeviceAddress.trimmed();
                DeviceAddress = "000000000000" + DeviceAddress;
                DeviceAddress = DeviceAddress.right(12);
            }
            else
            {
                for(int nPoint=0; nPoint<TypeElement.childNodes().count(); ++nPoint)
                {
                    QDomElement pointElement = TypeElement.childNodes().at(nPoint).toElement();
                    DLT645_Point *pPoint = new DLT645_Point;
                    pPoint->m_fBaseValue = pointElement.attribute("Device_YC_BaseValue_Strings").toFloat();
                    pPoint->m_fKValue = pointElement.attribute("Device_YC_KValue_Strings").toFloat();
                    if (pPoint->m_fKValue == 0)
                    {
                        pPoint->m_fKValue = 1;
                    }
                    pPoint->m_nID = pointElement.attribute("Device_YC_SN_Strings").toInt();
                    pPoint->m_nScanTime = pointElement.attribute("Device_YC_ScanTime_Strings").toInt();
                    pPoint->pb_Function = (BYTE)pointElement.attribute("Device_YC_DecimalPlaces_Strings").toInt();
                    pPoint->m_strRTDBName = pointElement.attribute("Device_YC_Link_Strings");
                    pPoint->m_strPointName = pointElement.attribute("Device_YC_Name_Strings");
                    pPoint->m_strPointComment = pointElement.attribute("Device_YC_Desc_Strings");
                    pPoint->pb_ByteLength = pointElement.attribute("Device_YC_ByteCount_Strings").toInt();
                    pPoint->pb_StartByte = pointElement.attribute("Device_YC_StartByte_Strings").toInt();

                    char *p = NULL;
                    pPoint->pb_Number_DI1 = (BYTE)strtoul(pointElement.attribute("Device_YC_DI1_Strings").toLatin1().mid(0,4),&p,16);
                    pPoint->pb_Number_DI0 = (BYTE)strtoul(pointElement.attribute("Device_YC_DI0_Strings").toLatin1().mid(0,4),&p,16);
                    BYTE DI[2];/// = {pPoint->pb_Number_DI0+0x33,pPoint->pb_Number_DI1+0x33};
                    DI[0] = pPoint->pb_Number_DI0+0x33;///,pPoint->pb_Number_DI1+0x33};
                    DI[1] = pPoint->pb_Number_DI1+0x33;
                    unsigned int value = 0;
                    memcpy((void *)(&value),DI,sizeof(2));

                    pPoint->pb_Number_DI =value;
                    qDebug()<<"pb_Number_DI的值:"<<pPoint->pb_Number_DI;

                    BYTE bDI[2];
                    bDI[0] = pPoint->pb_Number_DI0;
                    bDI[1] = pPoint->pb_Number_DI1;
                    QByteArray arrayDI((char *)bDI,2);
                    pPoint->Number_DI = arrayDI.toHex();


                    BYTE *pCur = pPoint->pb_mDeviceAddress+5;
                    for (int nStart = 0; nStart < 6; ++nStart)
                    {
                        *pCur-- = (BYTE)strtoul(DeviceAddress.toLatin1().mid(nStart*2,2),&p,16);
                    }
                    QByteArray address((char *)pPoint->pb_mDeviceAddress,6);
                    pPoint->pb_DeviceAddress = address.toHex();
                    pPoint->pb_ArrayDeviceAddress = address;

                    int nDataType = DATA_TYPE_UNDEFINE;
                    QString strDataType = pointElement.attribute("Device_YC_ValueType_Strings");
                    if(strDataType == "无符号型")
                    {
                        nDataType = DATA_TYPE_INT32U;
                    }else if(strDataType == "有符号型")
                    {
                        nDataType = DATA_TYPE_INT32S;
                    }else if(strDataType == "浮点型")
                    {
                        nDataType = DATA_TYPE_FP32;
                    }else if(strDataType == "布尔型")
                    {
                        nDataType = DATA_TYPE_BOOLEAN;
                    }
                    pPoint->Pb_ValueType =nDataType;
                    pPoint->m_pProtocol = this;  //变量所在的通道
                    pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName); //在实时数据库中查找变量的标签
                    if(pPoint->m_pTag)
                    {
                        pPoint->m_pTag->SetCollection(pPoint);  //设置采集点
                    }
                        Function_YC_Map.insert(pPoint->pb_Number_DI,pPoint);
                        DLT645_Query *pQuery = new DLT645_Query;
                        pQuery->pb_ArrayAddress = address;
                        pQuery->MakeQuery(address.data(),pPoint->pb_Number_DI,0x02);
                        Send_Framlist.append(pQuery);

                }
            }
        }
        if(!Function_YC_Map.isEmpty())
            qDebug()<<"设备地址:"<<DeviceAddress;
            Device_YC_Map.insert(DeviceAddress,Function_YC_Map);
    }
}

/**
 **
 ** Brief:下发命令报文
 ** Parameter：
 ** Author：G.onion
 ** Creat Date：2015/09/24
 ** Last Modified：2015/09/25
 **
 **/
void DLT645_Protocol_97::Polling()
{
    if ((GetSysAbsTime() - m_nLastTime) < 1)///如果最后发送时间小于1秒则不发送
    {
        return;
    }
    if (Send_Framlist.count() == 0)
    {
        return;
    }else if (SendFramListNumber >= Send_Framlist.count())
    {
        SendFramListNumber = 0;
    };
    if (NULL == m_pLink)
        return;
    if (!m_pLink->GetLinkState())
        return;
    LockData.lock();
    DLT645_Query *pQuery =Send_Framlist.at(SendFramListNumber);
    if (NULL != pQuery)
    {
        if (m_pLink->write(pQuery->m_ByteArrayFrame.data(),pQuery->m_ByteArrayFrame.count()))
        {
            m_arrayCurrentAddress = pQuery->pb_ArrayAddress;
            m_responseBuffer.clear();
            LockData.unlock();
            ShowMessage(0,Send_Framlist.at(SendFramListNumber)->m_ByteArrayFrame.toHex());
            SendFramListNumber++;
            qDebug()<<"发送成功";
        }
    }
    m_nLastTime = GetSysAbsTime();
}
/**
 **
 ** BriefL:是否连接槽函数
 ** Parameter：
 ** Author：G.onion
 ** Creat Date：2015/09/24
 ** Last Modified：2015/09/24
 **
 **/
void DLT645_Protocol_97::slotConnect(bool)
{

}
/**
 **
 ** Brief:时间事件函数
 ** Parameter：event：定时器事件指针
 ** Author：G.onion
 ** Creat Date：2015/09/25
 ** Last Modified：2015/09/25
 **
 **/
void DLT645_Protocol_97::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nTimerID)
    {
        qDebug()<<1;
        Polling();
    }
}
/**
 **
 ** Brief：检测报文是否正确，正确返回报文长度，错误返回false
 ** Parameter：pBufferHead_：报文头指针  nInfomationSize_：报文长度
 ** Author：G.onion
 ** Creat Date：2015/09/25
 ** Last Modified：2015/09/25
 **
 **/
int DLT645_Protocol_97::ReceiveIsTrue(const BYTE *pBufferHead_,const int nInfomationSize_)
{
    if (0x68 != GetUInt(pBufferHead_,nInfomationSize_,0,1))
    {
        return false;
    }
    if (0x68 != GetUInt(pBufferHead_,nInfomationSize_,7,1))
    {
        return false;
    }
    BYTE nLength = GetUInt(pBufferHead_,nInfomationSize_,9,1);
    if (0x16 != GetUInt(pBufferHead_,nInfomationSize_,11+nLength,1))
    {
        return false;
    }
    BYTE nCheckSum = MakeSum_(pBufferHead_,10+nLength);

    if ((BYTE)GetUInt(pBufferHead_,nInfomationSize_,10+nLength,1) != nCheckSum)
    {
        return false;
    }
    return nLength+12;///< 返回报文长度
}
/**
 **
 ** Brief:获取报文中的值，unsigned int：返回报文中获取的值
 ** Parameter：pBufferHead:报文头指针  nInfomationSize：报文长度   nStartByte：开始位置（字节）nSize：获取字节个数
 ** Author：G.onion
 ** Creat Date:2015/09/25
 ** Last Modified:2015/09/25
 **
 **/
unsigned int DLT645_Protocol_97::GetUInt(const BYTE *pBufferHead, const int nInfomationSize, const int nStartByte, const int nSize ) const
{
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
/**
 **
 ** Brief:报文解析函数
 ** Parameter：data：报文
 ** Author：G.onion
 ** Creat Date：2015/09/25
 ** Last Modified：2015/10/20
 **
 **/
void DLT645_Protocol_97::ProcessRespond(QByteArray data)
{
    qDebug()<<"开始解析***!!!!!!";
    QMutexLocker locker(&LockData);
    m_arrayRespondBuffer.AppendData(data.data(),data.count());
    QByteArray array((char *)m_arrayRespondBuffer.GetBuffer(0),m_arrayRespondBuffer.GetInfoSize());
    qDebug()<<"array.toHex()"<<array.toHex();
    if (m_arrayRespondBuffer.GetInfoSize() < MinFrameLength)  // 除去数据项报文共有12个字节，小于12字节不解析
    {
       return;
    }
    for(int nStart=0; nStart <= m_arrayRespondBuffer.GetInfoSize()-MinFrameLength; ++nStart)
    {
        int nFrameLength = ReceiveIsTrue(m_arrayRespondBuffer.GetBuffer(nStart),m_arrayRespondBuffer.GetInfoSize()-nStart);
        qDebug()<<"nFramlength:"<<nFrameLength;
        if(nFrameLength)
        {
            qDebug()<<"已经执行if";
            m_response.SetData(m_arrayRespondBuffer.GetBuffer(nStart),nFrameLength);
            QByteArray frame;
            if(nStart != 0)
            {
                frame = QByteArray((char *)m_arrayRespondBuffer.GetBuffer(0),nStart);
                ShowMessage(SHOWMESSAGE_TYPE_DEBUG,frame.toHex().toUpper());
            }
            frame = QByteArray((char *)m_response.GetBuffer(0),m_response.GetInfoSize());
            ShowMessage(SHOWMESSAGE_TYPE_RECEIVE,frame.toHex().toUpper());
            m_arrayRespondBuffer.Fifo(nStart+nFrameLength);
            ProcessRespond(m_response);
            nStart = -1;
        }
    }
    if(m_arrayRespondBuffer.GetInfoSize()>MAX_FRAME)
    {
        m_arrayRespondBuffer.SetInfoSize(0);
        Q_ASSERT(false);
    }
}
void DLT645_Protocol_97::ProcessRespond(DLT645_97_Response &response)
{
    if(response.GetUIntValue(8,1) & 0x40)
    {
        return;
    }
    else
    {
       QByteArray address((char *)(response.GetBuffer(0)+1),6);  ///获取设备地址
       QByteArray uDInumber((char *)(response.GetBuffer(10)),2);  ///获取点的DI标识
       QString mDInumber = uDInumber.toHex();
       qDebug()<<"mDInumber:"<<mDInumber;
       unsigned int DInumber = mDInumber.toUInt(0,16);
       QString nAddress = address.toHex();
       QMap<int,DLT645_Point *> pFunction = Device_YC_Map.value(nAddress);
       if(!pFunction.isEmpty())
        {
            qDebug()<<"Function_Map的数量："<<Function_YC_Map.count();
//            QMap<int,DLT645_Point *>::iterator iterator;
//         for(iterator = Function_YC_Map.begin(); iterator != Function_YC_Map.end();++iterator)
//         {
            DLT645_Point *pPoint = Function_YC_Map.value(DInumber);
            qDebug()<<"DI的值："<<DInumber;
            if(pPoint)
            {
               QString strLink = pPoint->m_strRTDBName;
               qDebug()<<"strLink:"<<pPoint->m_strRTDBName;
               int nByteCount = pPoint->pb_ByteLength;
               int nStartByte = pPoint->pb_StartByte;
//               qDebug()<<"nByteCount:"<<nByteCount<<"点的字节长度"<<pPoint->pb_ByteLength;
//               int nStart = pPoint->m_nID;
               BYTE nLength = response.GetUIntValue(9,1);
               if((nByteCount+2) <= nLength)
                {
                    BYTE Value[nByteCount];
                    for(int i=0;i<nByteCount;++i)
                    {
                        Value[i] = response.GetUIntValue(i+12+nStartByte,1);
                        Value[i] -= 0x33;
                    }
                    BYTE Temp[nByteCount];
                    for(int i = 0; i< nByteCount; ++i)
                    {
                        Temp[i] = Value[nByteCount-1-i];
                    }
                    QByteArray baValue((char *)Temp,nByteCount);

                    baValue = baValue.toHex();
                    ShowMessage(2,baValue);

                    int nDecimalPlaces = pPoint->pb_Function;
                    QString Project(baValue);
                    if(nDecimalPlaces)
                    {
                        if(Project.count()<nDecimalPlaces)
                        {
                            Project = "00000000000000000000"+Project;
                            Project.insert(Project.count()-nDecimalPlaces,'.');
                            Project=Project.right(nDecimalPlaces+2);
                        }
                        else
                        {
                            Project.indexOf(Project.count()-nDecimalPlaces,'.');
                        }
                    }
                    while(Project.count())
                    {
                        if(Project.at(0)!='0')
                        {
                            break;
                        }else
                        {
                            Project.remove(0,1);
                        }
                    }
                    if(Project.count())
                    {
                        if(Project.at(0)=='.')
                            Project.insert(0,'0');
                    }
                    QVariant VarProject;
                    QVariant VarOriginal;
                    if(Project.contains('.'))
                    {
                        VarProject = QVariant(Project.toFloat());
                        VarOriginal = QVariant(Project.toFloat());
                    }else
                    {
                        VarProject = QVariant(Project.toInt());
                        VarOriginal = QVariant(Project.toInt());
                    }
                    ShowMessage(2,VarProject.toString()+"---"+VarOriginal.toString());

                    CTagI *pTag = m_pRTDB->FindTag(strLink);
                    qDebug()<<"输出一下："<<m_pRTDB->FindTag(strLink);
                    if(pTag)
                    {
                        pTag->SetValue(pPoint,VarProject,VarOriginal);
                    }
                }
//            }
        }
    }else
    {
        qDebug()<<"Fail!!";
    }
   }
        m_nLastTime = 0;
}


void DLT645_Protocol_97::DecodeReceive(const QByteArray /*data*/)
{
}

/**
 **
 **  Brief：校验和生成函数
 **  Parameter： *pInfo_：报文起始地址指针  nSize_：报文长度
 **  Author：G.onion
 **  Creat Date：2015/09/24
 **  Last Modified：2015/10/08
 **
 **/
DWORD DLT645_Protocol_97::MakeSum_(const void *pInfo_, size_t nSize_)
{
    DWORD CheckSum = 0;
    for(size_t i=0; i<nSize_; i++)
    {
        CheckSum += ((BYTE *)pInfo_)[i];
    }
    return CheckSum;
}
/**
 **
 ** Brief:
 ** Parameter:
 ** Author:G.onion
 ** Creat Date:2015/09/24
 ** Last Modified:2015/10/08
 **
 **/
bool DLT645_Protocol_97::OnTagChange(CPointBase */*pPointBase_*/, QVariant /*varSet_*/)
{
    return true;
}
