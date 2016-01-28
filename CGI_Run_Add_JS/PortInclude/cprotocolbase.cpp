/*!
 * \file    cprotocolbase.cpp
 * \brief   概述 CProtocolBase类的源文件
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

#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QDir>
#include <QDateTime>

#include "cprotocolbase.h"
#include "CGI_CLINKI.h"
#include "CRTDBI.h"
#include "CTagI.h"
#include "cvaluebase.h"

CProtocolBase::CProtocolBase()
{
    InitParameter();
}

/*!
 * \brief  功能概述 初始化显示函数的函数指针
 * \param  参数描述 ShowMessage_是显示函数的函数指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CProtocolBase::SetShowMessage(void (*ShowMessage_)(int, QString, int))
{
    m_ShowMessage = ShowMessage_;
}

/*!
 * \brief  功能概述 设置此通道的通道号
 * \param  参数描述 nChannelNumber_是系统分配的通道号
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CProtocolBase::SetChannelNumber(int nChannelNumber_)
{
    m_nChannelNumber = nChannelNumber_;
}

/*!
 * \brief  功能概述 初始化通道参数
 * \param  参数描述 strChannelFileName_是通道文件绝对路径名，pDatabaseReturn是实时库指针
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/27
 */
bool CProtocolBase::OnCreateChannel(const QString strChannelFileName_, CRTDBI *pDatabaseReturn)
{
    m_strChannelFileName = strChannelFileName_;
    CPRTI *pPRTI = m_pPRTMapI->FindChannelNumber(m_nChannelNumber);
    if (NULL != pPRTI)
    {
        pPRTI->SetDriverType(m_nPROTOCOL_TYPE);/// @note 设置驱动的类型 该驱动为转发驱动
    }
//    m_pPRTMapI->FindChannelNumber(m_nChannelNumber)->SetDriverType(m_nPROTOCOL_TYPE);/// @note 设置驱动的类型 该驱动为转发驱动
    m_pRTDB = pDatabaseReturn;/// @note 设置实时库指针

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
    if (docElem.childNodes().count() != 5)///<zzy 2015/1/15 修改:
    {
        return false;
    }
    QDomElement Channel    = docElem.childNodes().at(0).toElement();
    QDomElement Top        = docElem.childNodes().at(1).toElement();
//    QDomElement Protocol   = docElem.childNodes().at(2).toElement();
    QDomElement MainPort   = docElem.childNodes().at(3).toElement();
//    QDomElement DeviceList = docElem.childNodes().at(4).toElement();
    QString strZTChannelNumber = Top.attribute("ZTChannelNumber");
    if (strZTChannelNumber.isEmpty())
    {
        m_nZTChannelNumber = -1;
    }else
    {
        m_nZTChannelNumber = strZTChannelNumber.toInt();/// @note 组态时设置的通道号
    }

    QStringList strListZTDeliversChannelNumberList = Top.attribute("NotifyNumber").split(",", QString::SkipEmptyParts);
    for (int i = 0; i < strListZTDeliversChannelNumberList.count(); ++i)
    {
        qDebug()<<m_nChannelNumber<<strListZTDeliversChannelNumberList.at(i).toInt()<<"===================";
        m_ZTDeliversChannelNumberList.push_back(strListZTDeliversChannelNumberList.at(i).toInt());
    }
    QString strChannelFileName2 = strChannelFileName_;

    /// @note 是否保存报文部分的处理
    if (Top.attribute("SaveFrame") == "True")
    {
        m_IsSaveFrame = true;
        QDir dir(strChannelFileName2);
        QString strFileName = dir.dirName();
        strFileName = strFileName.remove(".xml").append(".log");

        m_FileLog.setFileName(strFileName);
        if (!m_FileLog.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
        {
            qDebug()<<"未生成文件"<<strFileName;
        }else
        {
            qDebug()<<"生成文件了"<<strFileName;

        }
    }else
    {
        m_IsSaveFrame = false;
    }///是否保存报文部分的处理 end

//    InitPoint(DeviceList);
    QString strChannelName = tr("__")+Channel.attribute("Name").append("__");


    if (SetInitLink(Top,MainPort))
    {
        qDebug()<<"CLink Open succeed."<<m_pLink->GetLinkType()<<strChannelName;
    }else
    {
        qDebug()<<"CLink Open failed."<<m_pLink->GetLinkType()<<strChannelName;
    }
//#if defined(Q_PROCESSOR_ARM)
    m_pFrameSharedMemory = new CFrameSharedMemory(strChannelName,1024*30,this);
    m_bCreateStatusFrameSharedMemory = m_pFrameSharedMemory->CreateSharedMemory();
//#elif defined(Q_PROCESSOR_X86)

//#endif
    return true;
}

bool CProtocolBase::OnCreateChannel(const QDomElement &ChannelElement_, ChannelType nChannelType_, int nComOrCanNumber_, CRTDBI *pDatabaseReturn_)
{
    CPRTI *pPRTI = m_pPRTMapI->FindChannelNumber(m_nChannelNumber);
    if (NULL != pPRTI)
    {
        pPRTI->SetDriverType(m_nPROTOCOL_TYPE);/// @note 设置驱动的类型 该驱动为转发驱动
    }
//    m_pPRTMapI->FindChannelNumber(m_nChannelNumber)->SetDriverType(m_nPROTOCOL_TYPE);/// @note 设置驱动的类型 该驱动为转发驱动
    m_pRTDB = pDatabaseReturn_;/// @note 设置实时库指针

    ChannelElement_.elementsByTagName("Top");
    if (ChannelElement_.elementsByTagName("Top").isEmpty())
    {
        return false;
    }

#if 0
    QDomElement docElem = ChannelDoc.documentElement();
    if (docElem.childNodes().count() != 5)///<zzy 2015/1/15 修改:
    {
        return false;
    }
    QDomElement Channel    = docElem.childNodes().at(0).toElement();
    QDomElement Top        = docElem.childNodes().at(1).toElement();
    QString strZTChannelNumber = Top.attribute("ZTChannelNumber");
    if (strZTChannelNumber.isEmpty())
    {
        m_nZTChannelNumber = -1;
    }else
    {
        m_nZTChannelNumber = strZTChannelNumber.toInt();/// @note 组态时设置的通道号
    }

    QStringList strListZTDeliversChannelNumberList = Top.attribute("NotifyNumber").split(",", QString::SkipEmptyParts);
    for (int i = 0; i < strListZTDeliversChannelNumberList.count(); ++i)
    {
        qDebug()<<m_nChannelNumber<<strListZTDeliversChannelNumberList.at(i).toInt()<<"===================";
        m_ZTDeliversChannelNumberList.push_back(strListZTDeliversChannelNumberList.at(i).toInt());
    }
#endif
#if 0
    QString strChannelFileName2 = strChannelFileName_;
    /// @note 是否保存报文部分的处理
    if (Top.attribute("SaveFrame") == "True")
    {
        m_IsSaveFrame = true;
        QDir dir(strChannelFileName2);
        QString strFileName = dir.dirName();
        strFileName = strFileName.remove(".xml").append(".log");

        m_FileLog.setFileName(strFileName);
        if (!m_FileLog.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
        {
            qDebug()<<"未生成文件"<<strFileName;
        }else
        {
            qDebug()<<"生成文件了"<<strFileName;

        }
    }else
    {
        m_IsSaveFrame = false;
    }///是否保存报文部分的处理 end
#endif
    ChannelElement_.elementsByTagName("MainPort");
    if (ChannelElement_.elementsByTagName("MainPort").isEmpty())
    {
        return false;
    }
    QDomElement MainPort   = ChannelElement_.elementsByTagName("MainPort").at(0).toElement();

    QString strChannelName;/// = tr("__")+Channel.attribute("Name").append("__");
    switch (nChannelType_)
    {
    case ChannelType_COM:
        MainPort.setAttribute("Name",QString("COM%1").arg(nComOrCanNumber_));
        strChannelName = tr("__")+MainPort.attribute("Name").append("__");
        break;
    case ChannelType_NET:
        if (ChannelElement_.tagName() != "Link")
        {
            return false;
        }
        MainPort = ChannelElement_;
        strChannelName = tr("__")+MainPort.attribute("Name").append("__");
        break;
    case ChannelType_CAN:
        MainPort.setAttribute("Name",QString("can%1").arg(nComOrCanNumber_-1));
        strChannelName = tr("__")+MainPort.attribute("Name").append("__");
        break;
    default:
        break;
    }
    if (SetInitLink(nChannelType_,MainPort))
    {
        qDebug()<<"CLink Open succeed."<<m_pLink->GetLinkType();
    }else
    {
        qDebug()<<"CLink Open failed."<<m_pLink->GetLinkType();
    }
//#if 0
    qDebug()<<__func__<<"SharedMemoryKey ="<<strChannelName;
    m_pFrameSharedMemory = new CFrameSharedMemory(strChannelName,1024*30,this);
    m_bCreateStatusFrameSharedMemory = m_pFrameSharedMemory->CreateSharedMemory();
//#endif
    return true;
}

/*!
 * \brief  功能概述 无 启动通道
 * \param  参数描述 无
 * \return 返回值描述 返回true
 * \author zzy
 * \date   2015/5/27
 */
bool CProtocolBase::OnStart()
{
    return true;
}

/*!
 * \brief  功能概述 无 扫描通道
 * \param  参数描述 无
 * \return 返回值描述 返回true
 * \author zzy
 * \date   2015/5/27
 */
bool CProtocolBase::OnScan()
{
    return true;
}

/*!
 * \brief  功能概述 无 停止通道
 * \param  参数描述 无
 * \return 返回值描述 返回true
 * \author zzy
 * \date   2015/5/27
 */
bool CProtocolBase::OnStop()
{
    return true;
}

/*!
 * \brief  功能概述 初始化通道管理类对象的指针
 * \param  参数描述 pPRTMapI_是运行程序中的通道管理类对象指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CProtocolBase::SetPRTMap(CPRTMapI *pPRTMapI_)
{
    m_pPRTMapI = pPRTMapI_;
}

/*!
 * \brief  功能概述 供其它驱动调用的函数，用于报文的透明转发
 * \param  参数描述 byteArray是需要透明转发的报文
 * \return 返回值描述 返回发送的数据个数
 * \author zzy
 * \date   2015/5/27
 */
qint64 CProtocolBase::Write(const QByteArray /*byteArray*/)
{
    return 0;
}

/*!
 * \brief  功能概述 供其它驱动调用的函数，用于报文的透明转发
 * \param  参数描述 data是需要透明转发的数据的头指针
 * \return 返回值描述 返回发送的数据个数
 * \author zzy
 * \date   2015/5/27
 */
qint64 CProtocolBase::Write(const char */*data*/)
{
    return 0;
}

/*!
 * \brief  功能概述 供其它驱动调用的函数，用于报文的透明转发
 * \param  参数描述 data是需要透明转发的数据的头指针，maxSize是要转发的数据个数
 * \return 返回值描述 返回发送的数据个数
 * \author zzy
 * \date   2015/5/27
 */
qint64 CProtocolBase::Write(const char */*data*/, qint64 /*maxSize*/)
{
    return 0;
}

/*!
 * \brief  功能概述 获取该通道组态时组态的通道号
 * \param  参数描述 无
 * \return 返回值描述 返回该通道组态时组态的通道号
 * \author zzy
 * \date   2015/5/27
 */
int CProtocolBase::GetZTChannelNumber()
{
    return m_nZTChannelNumber;
}

/*!
 * \brief  功能概述 获取组态时组态的转发到（）通道列表
 * \param  参数描述 无
 * \return 返回值描述 返回组态时组态的转发到（）通道列表
 * \author zzy
 * \date   2015/5/27
 */
QList<int> CProtocolBase::GetZTDeliversChannelNumberList()
{
    return m_ZTDeliversChannelNumberList;
}

/*!
 * \brief  功能概述 此函数用于监视工具的驱动，作用是向驱动中写发送和接收的报文
 * \param  参数描述 type_是类型（接收、发送、Debug），message_是报文，nChannelNumber_是系统分配给每个通道的通道号
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CProtocolBase::WriteMessage(int /*type_*/, QString /*message_*/, int /*nChannelNumber_*/)
{

}

bool CProtocolBase::GetLinkStatus()
{
    bool result = false;

    if (NULL != m_pLink)
    {
        result = m_pLink;
    }
    return result;
}

/*!
 * \brief  功能概述 用于显示报文的函数
 * \param  参数描述 type_是类型（接收、发送、Debug），strMessage_是报文
 * \return 返回值描述
 * \author zzy
 * \date   2015/5/27
 */
void CProtocolBase::ShowMessage(int type_, QString strMessage_)
{
    ///
    /// type=0发送
    /// type=1接收
    /// type=2向窗口打印调试信息
    m_ShowMessage(type_,strMessage_,m_nChannelNumber);
    QString str;
    switch (type_) {
    case SHOWMESSAGE_TYPE_SEND:
        str = QString("Time:"+QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")+"\nTX-->:"+strMessage_.toUpper()+'\n'+'\0');
        break;
    case SHOWMESSAGE_TYPE_RECEIVE:
        str = QString("Time:"+QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")+"\nRX<--:"+strMessage_.toUpper()+'\n'+'\0');
        break;
    case SHOWMESSAGE_TYPE_DEBUG:
        str = QString("Time:"+QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")+"\nDebug-->:"+strMessage_.toUpper()+'\n'+'\0');
        break;
    default:
        break;
    }
    if (m_IsSaveFrame)
    {
        m_TextStreamOut.setDevice(&m_FileLog);
        m_TextStreamOut <<str;
        m_FileLog.flush();
    }
//#if defined(Q_PROCESSOR_ARM)
    if (m_bCreateStatusFrameSharedMemory)
    {
        m_pFrameSharedMemory->ShowMessage(type_,str);
    }
//#elif defined(Q_PROCESSOR_X86)

//#endif
}

/*!
 * \brief  功能概述 获取绝对秒
 * \param  参数描述 无
 * \return 返回值描述 返回绝对秒
 * \author zzy
 * \date   2015/5/27
 */
int CProtocolBase::GetSysAbsTime()
{
    ///获取绝对秒
    int nTime = QDateTime::currentDateTime().secsTo(QDateTime(QDate(1970,1,1),QTime(0,0,0,0)));
    if(nTime < 0)
    {
        nTime = -nTime;
    }
    return nTime;
}

int CProtocolBase::GetSysAbsTimeMSECS()
{
    ///获取绝对秒
    int nTime = QDateTime::currentDateTime().msecsTo(QDateTime(QDate(1970,1,1),QTime(0,0,0,0)));
    if(nTime < 0)
    {
        nTime = -nTime;
    }
    return nTime;
}

/*!
 \brief 初始化链路

 \fn CProtocolBase::SetInitLink
 \param Top_ 通道文件中的Top节点
 \param MainPort_ 通道文件中的MainPort节点
 \return bool 成功初始化链路则返回true，失败则返回false
 \author zzy
 \date   2015/6/9
*/
bool CProtocolBase::SetInitLink(const QDomElement &/*Top_*/, const QDomElement &/*MainPort_*/)
{
//    m_pLink = new CLink;
//    connect(m_pLink,SIGNAL(ReadyRead(QByteArray)),this,SLOT(ProcessRespond(QByteArray)));
//    connect(m_pLink,SIGNAL(signalConnect(bool)),this,SLOT(slotConnect(bool)));
//    return m_pLink->SetInitLink(Top_,MainPort_);
    return false;
}

bool CProtocolBase::SetInitLink(ChannelType nChannelType_, const QDomElement &MainPort_)
{
#if defined(TIAOSHI)
        m_CGI_CLinkLib.setFileName("Protocol/CGI_CLINKLib_d");//文件名
#else
        m_CGI_CLinkLib.setFileName("Protocol/CGI_CLINKLib");//文件名
#endif
    if (m_CGI_CLinkLib.load())
    {
        qDebug()<<"DLL load is OK!"<<m_CGI_CLinkLib.fileName();
        typedef CGI_CLINKI * (DLLAPI_CreateDriver)(QObject *parent );//初始化协议
        DLLAPI_CreateDriver *pCreateDriver = (DLLAPI_CreateDriver *)m_CGI_CLinkLib.resolve("CreateCGI_CLINKI");//获取 用于获取类对象的全局函数
        if (pCreateDriver)
        {
            qDebug()<<"Link to Function is OK!"<<m_CGI_CLinkLib.fileName();
            m_pLink = pCreateDriver(this);///< 库中导出类的初始化
            if (m_pLink != NULL)
            {
                connect(m_pLink,SIGNAL(ReadyRead(QByteArray)),this,SLOT(ProcessRespond(QByteArray)));
                connect(m_pLink,SIGNAL(signalConnect(bool)),this,SLOT(slotConnect(bool)));
                return m_pLink->CGI_SetInitLink(nChannelType_,MainPort_);
            }else
            {
                    qDebug()<<"if (MainPort_.tagName() != \"Link\")"<<MainPort_.tagName();
                    return false;
            }

        }else
        {
            qDebug() << m_CGI_CLinkLib.errorString();
            qDebug()<<"Linke to Function is not OK!!!!"<<m_CGI_CLinkLib.fileName();
            return false;
        }
    }else
    {
        qDebug()<<"DLL is not loaded!"<<m_CGI_CLinkLib.fileName();
        return false;
    }
    return false;
//    m_pLink = new CLink;
//    connect(m_pLink,SIGNAL(ReadyRead(QByteArray)),this,SLOT(ProcessRespond(QByteArray)));
//    connect(m_pLink,SIGNAL(signalConnect(bool)),this,SLOT(slotConnect(bool)));
//    return m_pLink->CGI_SetInitLink(nChannelType_,MainPort_);
}

/*!
 \brief 初始化类中的变量参数

 \fn CProtocolBase::InitParameter
 \author zzy
 \date   2015/6/8
*/
void CProtocolBase::InitParameter()
{
    m_pLink = NULL;
    m_nLastTime = 0;
    m_pRTDB = NULL;
    m_pPRTMapI = NULL;
    m_nChannelNumber = 0;
    m_nZTChannelNumber = 0;
    m_IsSaveFrame = false;
    m_pFrameSharedMemory = NULL;
//    m_bWriteFrameSharedMemoryFlag = false;
    m_bCreateStatusFrameSharedMemory = false;
    m_nPROTOCOL_TYPE = Undefinition_Type;
}

/*!
 \brief 设置驱动的类型 采集类型 转发类型

 \fn CProtocolBase::SetPROTOCOL_TYPE
 \param nPROTOCOL_TYPE_
 \author zzy
 \date   2015/7/16
*/
void CProtocolBase::SetPROTOCOL_TYPE(PROTOCOL_TYPE nPROTOCOL_TYPE_)
{
    Q_ASSERT( (Collect_Type  == nPROTOCOL_TYPE_)
           || (Transform_Type== nPROTOCOL_TYPE_) );
    m_nPROTOCOL_TYPE = nPROTOCOL_TYPE_;
}
