#include "CProtocolDemo.h"
#include <QTimerEvent>
#include <QDateTime>
#include <math.h>


#include <time.h>
#include <QDebug>
#if defined(Q_OS_LINUX)
#include <syscall.h>
#include <sys/sysinfo.h>
#endif

#include "../PortInclude/CRTDBI.h"
#include "../PortInclude/CTagI.h"
#include "CLinuxInfo.h"
#include <unistd.h>

/*!
 * \brief 功能概述 库文件导出接口函数
 * \param 参数描述 parent是QObject对象指针
 * \return 返回值描述 返回创建的驱动指针
 * \author zzy
 * \date 2015/1/14
 */
CProtocolI * CreateDriver(QObject *parent)
{
    CProtocolDemo *testProtocol = new CProtocolDemo(parent);
    return  testProtocol;
}


CProtocolDemo::CProtocolDemo(QObject */*parent*/)
{
    m_nTimerID = 0;
}

bool CProtocolDemo::OnCreateChannel(const QString strChannelFileName_, CRTDBI *pDatabaseReturn)
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


    return true;
}

bool CProtocolDemo::OnCreateChannel(const QDomElement &ChannelElement_, ChannelType nChannelType_, int nComOrCanNumber_, CRTDBI *pDatabaseReturn_)
{
    SetPROTOCOL_TYPE(Collect_Type);
    if (!CProtocolBase::OnCreateChannel(ChannelElement_,nChannelType_,nComOrCanNumber_,pDatabaseReturn_))
        return false;

    if (ChannelElement_.elementsByTagName("DeviceList").isEmpty())
        return false;
    QDomElement DeviceList = ChannelElement_.elementsByTagName("DeviceList").at(0).toElement();


    CGI_InitPointNode(DeviceList);
//    m_nTimerID = startTimer(2000);

    m_nTimerID = startTimer(100);
    return true;
}

bool CProtocolDemo::OnTagChange(CPointBase */*pPointBase_*/, QVariant /*VarSet_*/)
{
    bool result = false;


    return result;
}

void CProtocolDemo::ProcessRespond(QByteArray /*Respond_*/)
{

}

void CProtocolDemo::slotConnect(bool)
{

}

void CProtocolDemo::timerEvent(QTimerEvent *event_)
{
    if (event_->timerId() == m_nTimerID)
    {
        Polling();
    }
}

void CProtocolDemo::CGI_InitPointNode(const QDomElement &DeviceListElement_)
{
    QDomNodeList DeviceList = DeviceListElement_.childNodes();
    for (int i = 0; i <DeviceList.count(); ++i)
    {
        qDebug()<<"for (int i = 0; i <DeviceList.count(); ++i) "<<DeviceList.at(i).toElement().attribute("Name");
        QDomElement DeviceElement = DeviceList.at(i).toElement();
        int DeviceAddress = 0;///<zzy 2015/1/14 修改:
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
                        QDomElement tagElement = TypeElem.childNodes().at(nPoint).toElement();
                        CPointDemoBase *pPoint = new CPointDemoBase;
                        pPoint->m_strPointName = tagElement.attribute("Device_YX_Name_Strings");
                        pPoint->m_strPointComment = tagElement.attribute("Device_YX_Desc_Strings");
                        pPoint->m_nTagType = TAG_TYPE_DI;
                        pPoint->m_nID = tagElement.attribute("Device_YX_SN_Strings").toInt();
                        pPoint->m_strRTDBName = tagElement.attribute("Device_YX_Link_Strings");
                        qDebug()<<"LinkName :"<<pPoint->m_strRTDBName;
                        pPoint->m_nScanTime = tagElement.attribute("Device_YX_ScanTime_Strings").toUInt();
                        pPoint->m_fKValue = tagElement.attribute("Device_YX_KValue_Strings").toFloat();
                        if (pPoint->m_fKValue == 0)
                        {
                            pPoint->m_fKValue = 1;
                        }
                        pPoint->m_fBaseValue = tagElement.attribute("Device_YX_BaseValue_Strings").toFloat();
                        pPoint->m_nDeviceAddr = DeviceAddress;
                        pPoint->m_pProtocol = this;
                        pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                        if (pPoint->m_pTag)
                        {
                            pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                        }

                        int Function = tagElement.attribute("Device_YX_Function_Strings").toInt();
                        pPoint->m_nFunction = Function;                        
                        pPoint->m_nPara1 = tagElement.attribute("Device_YX_Para1_Strings").toInt();
                        pPoint->m_nPara2 = tagElement.attribute("Device_YX_Para2_Strings").toInt();
                        pPoint->m_nPara3 = tagElement.attribute("Device_YX_Para3_Strings").toInt();
//                        if (pPoint->m_pTag)
//                        {
//                            switch (Function) {
//                            case 70:
//                            {
//                                QVariant value(pPoint->m_nPara1);
//                                pPoint->m_pTag->SetValue(value,value);
//                            }
//                                break;
//                            default:
//                                break;
//                            }
//                        }
                        m_PointNode.AddPoint(pPoint);
                    }
                }else if (TypeElem.attribute("Name") == "YC")
                {
                    for (int nPoint = 0; nPoint < TypeElem.childNodes().count(); ++nPoint)
                    {
                        QDomElement tagElement = TypeElem.childNodes().at(nPoint).toElement();
                        CPointDemoBase *pPoint = new CPointDemoBase;
                        pPoint->m_strPointName = tagElement.attribute("Device_YC_Name_Strings");
                        pPoint->m_strPointComment = tagElement.attribute("Device_YC_Desc_Strings");
                        pPoint->m_nTagType = TAG_TYPE_AI;
                        pPoint->m_nID = tagElement.attribute("Device_YC_SN_Strings").toInt();
                        pPoint->m_strRTDBName = tagElement.attribute("Device_YC_Link_Strings");
                        qDebug()<<"LinkName :"<<pPoint->m_strRTDBName;
                        pPoint->m_nScanTime = tagElement.attribute("Device_YC_ScanTime_Strings").toUInt();
                        pPoint->m_fKValue = tagElement.attribute("Device_YC_KValue_Strings").toFloat();
                        if (pPoint->m_fKValue == 0)
                        {
                            pPoint->m_fKValue = 1;
                        }
                        pPoint->m_fBaseValue = tagElement.attribute("Device_YC_BaseValue_Strings").toFloat();
                        pPoint->m_nDeviceAddr = DeviceAddress;
                        pPoint->m_pProtocol = this;
                        qDebug()<<"LinkName 1:"<<pPoint->m_strRTDBName;
                        pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                        qDebug()<<"LinkName 2:"<<pPoint->m_strRTDBName;
                        if (pPoint->m_pTag)
                        {
                            pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                        }
                        qDebug()<<"LinkName 3:"<<pPoint->m_strRTDBName;

                        int Function = tagElement.attribute("Device_YC_Function_Strings").toInt();
                        pPoint->m_nFunction = Function;
                        pPoint->m_nPara1 = tagElement.attribute("Device_YC_Para1_Strings").toInt();
                        pPoint->m_nPara2 = tagElement.attribute("Device_YC_Para2_Strings").toInt();
                        pPoint->m_nPara3 = tagElement.attribute("Device_YC_Para3_Strings").toInt();
                        qDebug()<<"LinkName 4:"<<pPoint->m_strRTDBName;
//                        if (pPoint->m_pTag)
//                        {
//                            switch (Function) {
//                            case 70:
//                            {
//                                QVariant value(pPoint->m_nPara1);
//                                pPoint->m_pTag->SetValue(value,value);
//                            }
//                                break;
//                            default:
//                                break;
//                            }
//                        }
                        qDebug()<<"LinkName 5:"<<pPoint->m_strRTDBName;
                        m_PointNode.AddPoint(pPoint);
                    }
                }else if (TypeElem.attribute("Name") == "YM")
                {
                    for (int nPoint = 0; nPoint < TypeElem.childNodes().count(); ++nPoint)
                    {
                        QDomElement tagElement = TypeElem.childNodes().at(nPoint).toElement();
                        CPointDemoBase *pPoint = new CPointDemoBase;
                        pPoint->m_strPointName = tagElement.attribute("Device_YM_Name_Strings");
                        pPoint->m_strPointComment = tagElement.attribute("Device_YM_Desc_Strings");
                        pPoint->m_nTagType = TAG_TYPE_MEM;
                        pPoint->m_nID = tagElement.attribute("Device_YM_SN_Strings").toInt();
                        pPoint->m_strRTDBName = tagElement.attribute("Device_YM_Link_Strings");
                        qDebug()<<"LinkName :"<<pPoint->m_strRTDBName;
                        pPoint->m_nScanTime = tagElement.attribute("Device_YM_ScanTime_Strings").toUInt();
                        pPoint->m_fKValue = tagElement.attribute("Device_YM_KValue_Strings").toFloat();
                        if (pPoint->m_fKValue == 0)
                        {
                            pPoint->m_fKValue = 1;
                        }
                        pPoint->m_fBaseValue = tagElement.attribute("Device_YM_BaseValue_Strings").toFloat();
                        pPoint->m_nDeviceAddr = DeviceAddress;
                        pPoint->m_pProtocol = this;
                        pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                        if (pPoint->m_pTag)
                        {
                            pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                        }

                        int Function = tagElement.attribute("Device_YM_Function_Strings").toInt();
                        pPoint->m_nFunction = Function;
                        pPoint->m_nPara1 = tagElement.attribute("Device_YM_Para1_Strings").toInt();
                        pPoint->m_nPara2 = tagElement.attribute("Device_YM_Para2_Strings").toInt();
                        pPoint->m_nPara3 = tagElement.attribute("Device_YM_Para3_Strings").toInt();
//                        if (pPoint->m_pTag)
//                        {
//                            switch (Function) {
//                            case 70:
//                            {
//                                QVariant value(pPoint->m_nPara1);
//                                pPoint->m_pTag->SetValue(value,value);
//                            }
//                                break;
//                            default:
//                                break;
//                            }
//                        }
                        m_PointNode.AddPoint(pPoint);
                    }
                }else if (TypeElem.attribute("Name") == "YK")
                {
                    for (int nPoint = 0; nPoint < TypeElem.childNodes().count(); ++nPoint)
                    {
                        QDomElement tagElement = TypeElem.childNodes().at(nPoint).toElement();
                        CPointDemoBase *pPoint = new CPointDemoBase;
                        pPoint->m_strPointName = tagElement.attribute("Device_YK_Name_Strings");
                        pPoint->m_strPointComment = tagElement.attribute("Device_YK_Desc_Strings");
                        pPoint->m_nTagType = TAG_TYPE_DO;
                        pPoint->m_nID = tagElement.attribute("Device_YK_SN_Strings").toInt();
                        pPoint->m_strRTDBName = tagElement.attribute("Device_YK_Link_Strings");
                        qDebug()<<"LinkName :"<<pPoint->m_strRTDBName;
                        pPoint->m_nScanTime = tagElement.attribute("Device_YK_ScanTime_Strings").toUInt();
                        pPoint->m_fKValue = tagElement.attribute("Device_YK_KValue_Strings").toFloat();
                        if (pPoint->m_fKValue == 0)
                        {
                            pPoint->m_fKValue = 1;
                        }
                        pPoint->m_fBaseValue = tagElement.attribute("Device_YK_BaseValue_Strings").toFloat();
                        pPoint->m_nDeviceAddr = DeviceAddress;
                        pPoint->m_pProtocol = this;
                        pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                        if (pPoint->m_pTag)
                        {
                            pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                        }

                        int Function = tagElement.attribute("Device_YK_Function_Strings").toInt();
                        pPoint->m_nFunction = Function;
                        pPoint->m_nPara1 = tagElement.attribute("Device_YK_Para1_Strings").toInt();
                        pPoint->m_nPara2 = tagElement.attribute("Device_YK_Para2_Strings").toInt();
                        pPoint->m_nPara3 = tagElement.attribute("Device_YK_Para3_Strings").toInt();
//                        if (pPoint->m_pTag)
//                        {
//                            switch (Function) {
//                            case 70:
//                            {
//                                QVariant value(pPoint->m_nPara1);
//                                pPoint->m_pTag->SetValue(value,value);
//                            }
//                                break;
//                            default:
//                                break;
//                            }
//                        }
                        m_PointNode.AddPoint(pPoint);
                    }
                }else if (TypeElem.attribute("Name") == "YS")
                {
                    for (int nPoint = 0; nPoint < TypeElem.childNodes().count(); ++nPoint)
                    {
                        QDomElement tagElement = TypeElem.childNodes().at(nPoint).toElement();
                        CPointDemoBase *pPoint = new CPointDemoBase;
                        pPoint->m_strPointName = tagElement.attribute("Device_YS_Name_Strings");
                        pPoint->m_strPointComment = tagElement.attribute("Device_YS_Desc_Strings");
                        pPoint->m_nTagType = TAG_TYPE_AO;
                        pPoint->m_nID = tagElement.attribute("Device_YS_SN_Strings").toInt();
                        pPoint->m_strRTDBName = tagElement.attribute("Device_YS_Link_Strings");
                        qDebug()<<"LinkName :"<<pPoint->m_strRTDBName;
                        pPoint->m_nScanTime = tagElement.attribute("Device_YS_ScanTime_Strings").toUInt();
                        pPoint->m_fKValue = tagElement.attribute("Device_YS_KValue_Strings").toFloat();
                        if (pPoint->m_fKValue == 0)
                        {
                            pPoint->m_fKValue = 1;
                        }
                        pPoint->m_fBaseValue = tagElement.attribute("Device_YS_BaseValue_Strings").toFloat();
                        pPoint->m_nDeviceAddr = DeviceAddress;
                        pPoint->m_pProtocol = this;
                        pPoint->m_pTag = m_pRTDB->FindTag(pPoint->m_strRTDBName);
                        if (pPoint->m_pTag)
                        {
                            pPoint->m_pTag->SetCollection(pPoint);///设置采集点
                        }

                        int Function = tagElement.attribute("Device_YS_Function_Strings").toInt();
                        pPoint->m_nFunction = Function;
                        pPoint->m_nPara1 = tagElement.attribute("Device_YS_Para1_Strings").toInt();
                        pPoint->m_nPara2 = tagElement.attribute("Device_YS_Para2_Strings").toInt();
                        pPoint->m_nPara3 = tagElement.attribute("Device_YS_Para3_Strings").toInt();
//                        if (pPoint->m_pTag)
//                        {
//                            switch (Function) {
//                            case 70:
//                            {
//                                QVariant value(pPoint->m_nPara1);
//                                pPoint->m_pTag->SetValue(value,value);
//                            }
//                                break;
//                            default:
//                                break;
//                            }
//                        }
                        m_PointNode.AddPoint(pPoint);
                    }
                }
            }
        }
    }
}

void CProtocolDemo::Polling()
{
//    qDebug()<<__func__<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QMap<unsigned int ,CDemoPointNode *>::iterator iterator;
    for (iterator = m_PointNode.m_PointNodeMap.begin(); iterator != m_PointNode.m_PointNodeMap.end(); ++iterator)
    {
        if (ABS(GetSysAbsTime()-iterator.value()->m_nStorageMoment) >= iterator.value()->m_nPeriodicStorage)
        {
//            qDebug()<<__func__<<"AAAAAAAA"<<iterator.value()->m_nStorageMoment<<iterator.value()->m_nPeriodicStorage;
            iterator.value()->m_nStorageMoment = GetSysAbsTime();
            for (int i = 0; i < iterator.value()->m_PointList.count(); ++i)
            {
//                qDebug()<<__func__<<"bbbbbbbbbb"<<iterator.value()->m_PointList.at(i)->m_strRTDBName;
                ChangeValue(iterator.value()->m_PointList.at(i));
            }
        }
    }
}
bool CProtocolDemo::ChangeValue(CPointDemoBase *pPoint)
{
    switch(pPoint->m_nFunction)
    {
    //cpu
    case 10://CPU 占用率
        GetCPUInfo(pPoint);
        break;
    //内存 主存 虚存
    case 20://物理内存总数
        GetMemoryInfo(pPoint);
        break;
    case 22://物理内存空闲
        GetMemoryInfo(pPoint);
        break;
    case 24://物理内存使用
        GetMemoryInfo(pPoint);
        break;
    case 26://虚内存总数
        GetMemoryInfo(pPoint);
        break;
    case 28://虚内存空闲
        GetMemoryInfo(pPoint);
        break;
    case 30://虚内存占用
        GetMemoryInfo(pPoint);
        break;
    case 32://文件页总数
        GetMemoryInfo(pPoint);
        break;
    case 34://文件页空闲
        GetMemoryInfo(pPoint);
        break;
    case 36://文件页占用
        GetMemoryInfo(pPoint);
        break;
    case 38://内存占用率
        GetMemoryInfo(pPoint);
//        （2）   内存使用率
//          这里需要从/proc/meminfo文件中提取两个数据，当前内存的使用量(cmem)以及内存总量(amem)。
//          内存使用百分比   =   100   *   (cmem   /   umem)
        break;
    case 40://当前程序占用空间
        GetMemoryInfo(pPoint);
        break;
    case 42://内存使用峰值
        GetMemoryInfo(pPoint);
        break;
    case 44://占用的虚拟内存大小
        GetMemoryInfo(pPoint);
        break;
    case 46://占用的虚拟内存峰值
        GetMemoryInfo(pPoint);
        break;

    //时间 当前时间,系统启动时间,程序运行时间
    case 50://windows系统已经运行时间
//        fValue=GetTimeInfo(pPoint->m_nFunction);
        GetTimeInfo(pPoint);
        break;
    case 52://程序启动时间 秒
//        fValue=GetTimeInfo(pPoint->m_nFunction);
        GetTimeInfo(pPoint);
        break;
    case 54://程序运行时间 秒
//        fValue=GetTimeInfo(pPoint->m_nFunction);
        GetTimeInfo(pPoint);
        break;
    case 56://内核时间
//        fValue=GetTimeInfo(pPoint->m_nFunction);
//        GetTimeInfo(pPoint->m_nFunction);
        GetTimeInfo(pPoint);
        break;
    case 58://用户时间
//        fValue=GetTimeInfo(pPoint->m_nFunction);
//        GetTimeInfo(pPoint->m_nFunction);
        GetTimeInfo(pPoint);
        break;

        //磁盘空间 a b  c d e f g   占用/总/空闲
    case 60:
//        fValue=GetDiskInfo(pPoint->m_nPara1,pPoint->m_nPara2);
        break;

    //各种随机数据
    case 70://常量   参数为:70　常量值
        GetRand(pPoint);
        break;
    case 72://一定范围内的随机量 参数为:72　最大值　最小值
        GetRand(pPoint);
        break;
    case 74://单调增加 参数为:74 初值  终值 步长(<0 为单调递减)
        GetRand(pPoint);
        break;
    case 76://正弦变量 参数为:76 基值 幅值 步长(角度)
        GetRand(pPoint);
        break;
    case 78://方波  参数为:78 高值,低值(扫描周期就是方波长度）
        GetRand(pPoint);
        break;
    case 80://遥信量 参数为:80 (扫描周期就是变位周期）
        GetRand(pPoint);
        break;
    case 82://遥信量  参数为:82每次变位的可能性(0-100)
        GetRand(pPoint);
        break;
    default:
        Q_ASSERT(false);//"未定义的取值方式\n"
        break;
    }
    return true;
}
void CProtocolDemo::GetTimeInfo(CPointDemoBase *pPoint_)
{
//    CPU_OCCUPY cpu_stat1;
//    CPU_OCCUPY cpu_stat2;
//    MEM_OCCUPY mem_stat;
//    int cpu;
    //获取内存
//    CLinuxInfo::get_memoccupy ((MEM_OCCUPY *)&mem_stat);

    //第一次获取cpu使用情况
//    CLinuxInfo::get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);
//    sleep(10);

    //第二次获取cpu使用情况
//    CLinuxInfo::get_cpuoccupy((CPU_OCCUPY *)&cpu_stat2);

    //计算cpu使用率
//    cpu = CLinuxInfo::cal_cpuoccupy ((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2);
    CTagI *pTempTagI = pPoint_->m_pTag;
    if (pTempTagI)
    {
        double fValue=0.0;
        switch(pPoint_->m_nFunction)
        {
        case 50://windows系统已经运行时间
    //        fValue= GetTickCount()/1000.0f;
        {
            struct sysinfo info;
            time_t cur_time = 0;
//            time_t boot_time = 0;
//            struct tm *ptm = NULL;
            if (sysinfo(&info))
            {
        //        PrintRunInfo("get systime from boottime to now");
                return ;
            }
            qDebug()<<info.uptime;
            time(&cur_time);
//            if (cur_time > info.uptime) {
//                boot_time = cur_time - info.uptime;
//            }
//            else {
//                boot_time = info.uptime - cur_time;
//            }
            fValue = info.uptime;
        }
            break;
        case 52://程序启动时间 秒
//            FileTimeToSystemTime(&timeCreate,&systemTime);
//            fValue= CTime(systemTime).GetTime();
        {
            fValue = pPoint_->m_nStartRunTime;
        }
            break;
        case 54://程序运行时间 秒
//            fValue= clock()/(float)CLOCKS_PER_SEC;
            fValue = GetSysAbsTime() - pPoint_->m_nStartRunTime;
            break;
        case 56://内核运行时间
//            FileTimeToSystemTime(&timeKernel,&systemTime);
//            fValue= systemTime.wHour*3600+systemTime.wMinute*60+systemTime.wSecond+systemTime.wMilliseconds/1000.0;
        {

        }
            break;
        case 58://用户运行时间
//            FileTimeToSystemTime(&timeUser,&systemTime);
//            fValue= systemTime.wHour*3600+systemTime.wMinute*60+systemTime.wSecond+systemTime.wMilliseconds/1000.0;
            fValue = clock();
            break;
        default:
            Q_ASSERT(false);//"IO变量定义无效:未定义的时间变量取值方式 funcno=4 param1=%d\n",nType);
            break;
        }
        QVariant value(fValue);
        pTempTagI->SetValue(pPoint_,value,value);
    }

    //    return fValue;
}

void CProtocolDemo::GetCPUInfo(CPointDemoBase *pPoint_)
{
//        （1）   处理器使用率
//            这里要从/proc/stat中提取四个数据：用户模式（user）、低优先级的用户模式（nice）、内核模式（system）以及空闲的处理器时间（idle）。它们均位于/proc/stat文件的第一行。CPU的利用率使用如下公式来计算。
//            CPU利用率   =   100   *（user   +   nice   +   system）/（user   +   nice   +   system   +   idle）
    CTagI *pTempTagI = pPoint_->m_pTag;
    if (pTempTagI)
    {
        double fValue=0.0;
        FILE *fd;
//        int n;
        char buff[256];
        CPU_OCCUPY cpu_occupy;
//        cpu_occupy=cpust;

        fd = fopen (PROC_STAT, "r");
        fgets (buff, sizeof(buff), fd);

        sscanf (buff, "%s %u %u %u %u", cpu_occupy.name, &cpu_occupy.user, &cpu_occupy.nice,&cpu_occupy.system, &cpu_occupy.idle);
        fclose(fd);
        qDebug()<<"======================"<<cpu_occupy.user<<cpu_occupy.nice<<cpu_occupy.system<<cpu_occupy.idle;
        fValue = 100 * (cpu_occupy.user+cpu_occupy.nice+cpu_occupy.system)/(cpu_occupy.user+cpu_occupy.nice+cpu_occupy.system+cpu_occupy.idle);

        QVariant Value(fValue);
        pTempTagI->SetValue(pPoint_,Value,Value);
    }
}

void CProtocolDemo::GetMemoryInfo(CPointDemoBase *pPoint_)
{
    CTagI *pTempTagI = pPoint_->m_pTag;
    if (pTempTagI)
    {
        double fValue=0.0;
        switch(pPoint_->m_nFunction)
        {
        case 20://物理内存总数
        {
            QString strTitle_("MemTotal");
            QFile file(PROC_MEMINFO);
            MEM_OCCUPY m;
            if (file.open(QIODevice::ReadOnly| QIODevice::Text))
            {
                QTextStream in(&file);
                QString data = in.readAll();
                QStringList lineList = data.split('\n');
                for (int i = 0; i < lineList.count(); ++i)
                {
                    if (lineList.at(i).contains(strTitle_))
                    {
                        sscanf (lineList.at(i).toStdString().data(), "%s %lu %s", m.name, &m.total, m.name2);
                        fValue = m.total;
                        break;
                    }
                }
                file.close();
            }
        }
            break;
        case 22://物理内存空闲
        {
            QString strTitle_("MemFree");
            QFile file(PROC_MEMINFO);
            MEM_OCCUPY m;
            if (file.open(QIODevice::ReadOnly| QIODevice::Text))
            {
                QTextStream in(&file);
                QString data = in.readAll();
                QStringList lineList = data.split('\n');
                for (int i = 0; i < lineList.count(); ++i)
                {
                    if (lineList.at(i).contains(strTitle_))
                    {
                        sscanf (lineList.at(i).toStdString().data(), "%s %lu %s", m.name, &m.total, m.name2);
                        fValue = m.total;
                        break;
                    }
                }
                file.close();
            }
        }
            break;
        case 24://物理内存使用
        {
            QString strTitle_1("MemTotal");
            QString strTitle_2("MemFree");
            double value1 = 0.0;
            double value2 = 0.0;
            QFile file(PROC_MEMINFO);
            MEM_OCCUPY m;
            if (file.open(QIODevice::ReadOnly| QIODevice::Text))
            {
                QTextStream in(&file);
                QString data = in.readAll();
                QStringList lineList = data.split('\n');
                for (int i = 0; i < lineList.count(); ++i)
                {
                    if (lineList.at(i).contains(strTitle_1))
                    {
                        sscanf (lineList.at(i).toStdString().data(), "%s %lu %s", m.name, &m.total, m.name2);
                        value1 = m.total;
                        break;
                    }else if (lineList.at(i).contains(strTitle_2))
                    {
                        sscanf (lineList.at(i).toStdString().data(), "%s %lu %s", m.name, &m.total, m.name2);
                        value2 = m.total;
                        break;
                    }
                }
                file.close();
                fValue = value1 - value2;
            }
        }
            break;
        case 26://虚内存总数
        {
            QString strTitle_("VmallocTotal");
            QFile file(PROC_MEMINFO);
            MEM_OCCUPY m;
            if (file.open(QIODevice::ReadOnly| QIODevice::Text))
            {
                QTextStream in(&file);
                QString data = in.readAll();
                QStringList lineList = data.split('\n');
                for (int i = 0; i < lineList.count(); ++i)
                {
                    if (lineList.at(i).contains(strTitle_))
                    {
                        sscanf (lineList.at(i).toStdString().data(), "%s %lu %s", m.name, &m.total, m.name2);
                        fValue = m.total;
                        break;
                    }
                }
                file.close();
            }
        }
            break;
        case 28://虚内存空闲
        {
            QString strTitle_1("VmallocTotal");
            QString strTitle_2("VmallocUsed");
            double value1 = 0.0;
            double value2 = 0.0;
            QFile file(PROC_MEMINFO);
            MEM_OCCUPY m;
            if (file.open(QIODevice::ReadOnly| QIODevice::Text))
            {
                QTextStream in(&file);
                QString data = in.readAll();
                QStringList lineList = data.split('\n');
                for (int i = 0; i < lineList.count(); ++i)
                {
                    if (lineList.at(i).contains(strTitle_1))
                    {
                        sscanf (lineList.at(i).toStdString().data(), "%s %lu %s", m.name, &m.total, m.name2);
                        value1 = m.total;
                        break;
                    }else if (lineList.at(i).contains(strTitle_2))
                    {
                        sscanf (lineList.at(i).toStdString().data(), "%s %lu %s", m.name, &m.total, m.name2);
                        value2 = m.total;
                        break;
                    }
                }
                file.close();
                fValue = value1 - value2;
            }
        }
            break;
        case 30://虚内存占用
        {
            QString strTitle_("VmallocUsed");
            QFile file(PROC_MEMINFO);
            MEM_OCCUPY m;
            if (file.open(QIODevice::ReadOnly| QIODevice::Text))
            {
                QTextStream in(&file);
                QString data = in.readAll();
                QStringList lineList = data.split('\n');
                for (int i = 0; i < lineList.count(); ++i)
                {
                    if (lineList.at(i).contains(strTitle_))
                    {
                        sscanf (lineList.at(i).toStdString().data(), "%s %lu %s", m.name, &m.total, m.name2);
                        fValue = m.total;
                        break;
                    }
                }
                file.close();
            }
        }

            break;
        case 32://文件页总数
            break;
        case 34://文件页空闲
            break;
        case 36://文件页占用
            break;
        case 38://内存占用率
            break;
        case 40://当前程序占用空间
        {
            uid_t current_pid = getpid();   //获取进程ID
            QString strTitle_("VmallocUsed");
            QString filename = QString("/proc/%1/stat").arg(QString::number(current_pid));
            qDebug()<<"current pid="<<current_pid;
            QFile file(filename);
            MEM_OCCUPY m;
            if (file.open(QIODevice::ReadOnly| QIODevice::Text))
            {
                QTextStream in(&file);
                QString data = in.readAll();
                QStringList lineList = data.split('\n');
                for (int i = 0; i < lineList.count(); ++i)
                {
                    if (lineList.at(i).contains(strTitle_))
                    {
                        sscanf (lineList.at(i).toStdString().data(), "%s %lu %s", m.name, &m.total, m.name2);
                        fValue = m.total;
                        break;
                    }
                }
                file.close();
            }


        }
            break;
        case 42://内存使用峰值
        {
            QString strValue;
            if (pTempTagI->GetPointCollection())
            {
                strValue = pTempTagI->GetProjectValue()->GetVarValue().toString();
            }
            int m_nLastValue = strValue.toInt();
            QString strTitle_1("MemTotal");
            QString strTitle_2("MemFree");
            double value1 = 0.0;
            double value2 = 0.0;
            QFile file(PROC_MEMINFO);
            MEM_OCCUPY m;
            if (file.open(QIODevice::ReadOnly| QIODevice::Text))
            {
                QTextStream in(&file);
                QString data = in.readAll();
                QStringList lineList = data.split('\n');
                for (int i = 0; i < lineList.count(); ++i)
                {
                    if (lineList.at(i).contains(strTitle_1))
                    {
                        sscanf (lineList.at(i).toStdString().data(), "%s %lu %s", m.name, &m.total, m.name2);
                        value1 = m.total;
                        break;
                    }else if (lineList.at(i).contains(strTitle_2))
                    {
                        sscanf (lineList.at(i).toStdString().data(), "%s %lu %s", m.name, &m.total, m.name2);
                        value2 = m.total;
                        break;
                    }
                }
                file.close();
                if (m_nLastValue < value1-value2)
                {
                    fValue = value1 - value2;
                }
            }
        }
            break;
        case 44://占用的虚拟内存大小
        {
            QString strTitle_("VmallocUsed");
            QFile file(PROC_MEMINFO);
            MEM_OCCUPY m;
            if (file.open(QIODevice::ReadOnly| QIODevice::Text))
            {
                QTextStream in(&file);
                QString data = in.readAll();
                QStringList lineList = data.split('\n');
                for (int i = 0; i < lineList.count(); ++i)
                {
                    if (lineList.at(i).contains(strTitle_))
                    {
                        sscanf (lineList.at(i).toStdString().data(), "%s %lu %s", m.name, &m.total, m.name2);
                        fValue = m.total;
                        break;
                    }
                }
                file.close();
            }
        }
            break;
        case 46://占用的虚拟内存峰值
        {
            QString strValue;
            if (pTempTagI->GetPointCollection())
            {
                strValue = pTempTagI->GetProjectValue()->GetVarValue().toString();
            }
            int m_nLastValue = strValue.toInt();
            QString strTitle_("VmallocUsed");
            QFile file(PROC_MEMINFO);
            MEM_OCCUPY m;
            if (file.open(QIODevice::ReadOnly| QIODevice::Text))
            {
                QTextStream in(&file);
                QString data = in.readAll();
                QStringList lineList = data.split('\n');
                for (int i = 0; i < lineList.count(); ++i)
                {
                    if (lineList.at(i).contains(strTitle_))
                    {
                        sscanf (lineList.at(i).toStdString().data(), "%s %lu %s", m.name, &m.total, m.name2);
                        if (m_nLastValue < (int)m.total)
                        {
                            fValue = m.total;
                        }
                        break;
                    }
                }
                file.close();
            }
        }
            break;
        default:
            Q_ASSERT(false);//("IO变量定义无效:未定义的随机变量取值方式 funcno=100 param1=%d\n",nType);
            break;
        }
        QVariant Value(fValue);
//        qDebug()<<"Value_.setValue(fValue);"<<Value;
        pTempTagI->SetValue(pPoint_,Value,Value);
    }
}

void CProtocolDemo::GetRand(CPointDemoBase *pPoint_)
{
//    qDebug()<<"xxxxxxxxxxxxxxxxxxxxxxx"<<pPoint_->m_nFunction<<pPoint_->m_strRTDBName;
    QVariant Value;
    int nType = pPoint_->m_nFunction;
    int nMax = pPoint_->m_nPara1;
    int nMin = pPoint_->m_nPara2;
    int nStep = pPoint_->m_nPara3;
    CTagI *pTempTagI = pPoint_->m_pTag;
//    qDebug()<<__func__<<"pTempTagI ="<<pTempTagI;
//    qDebug()<<"pPoint_->m_pTag"<<pPoint_->m_pTag->GetName();
    if (pTempTagI)
    {
        QString strValue;
        if (pTempTagI->GetPointCollection())
        {
            strValue = pTempTagI->GetProjectValue()->GetVarValue().toString();
        }
        int m_nLastValue = strValue.toInt();
        //功能:生成各种随机数据
        //参数:nType生成数据的方式
        //返回值:
        double fValue=0.0;
        //	srand(time(NULL));
        switch(nType)
        {
        case 70://常量   参数为:70　常量值
            fValue = nMax;
            break;
        case 72://一定范围内的随机量 参数为:72　最大值　最小值
            fValue=  rand()%(nMax-nMin)+nMin;
            break;
        case 74://单调增加 参数为:74 初值  终值 步长(<0 为单调递减)
            {
                int nStartValue = nMax;
                int nEndValue = nMin;
                Q_ASSERT(nStartValue <= nEndValue);

                m_nLastValue += nStep;
                if(m_nLastValue>nEndValue) //不在有效值范围内)
                {
                    m_nLastValue = nStartValue;
                }
                else if(m_nLastValue<nStartValue)
                {
                    m_nLastValue = nEndValue;
                }
            }
            fValue=  m_nLastValue;
            break;
        case 76://正弦变量 参数为:76 基值 幅值 步长(角度)
            {
                int nBaseValue = nMax;
                int nK = nMin;
                fValue = nBaseValue + nK * sin(m_nLastValue*3.14/180.0);
                m_nLastValue += nStep;
                if(m_nLastValue>360)
                {
                    m_nLastValue -= 360;
                }
            }
            break;
        case 78://方波  参数为:78 高值,低值(扫描周期就是方波长度）
            if(m_nLastValue>0)
            {
                fValue = nMax;
            }
            else
            {
                fValue = nMin;
            }
            m_nLastValue = 1 - m_nLastValue;
            break;
        case 80://遥信量 参数为:80 (扫描周期就是变位周期）
//            qDebug()<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAA"<<pPoint_->m_nFunction<<pPoint_->m_strRTDBName;
            m_nLastValue = 1 - m_nLastValue;
            fValue=m_nLastValue;
            break;
        case 82://遥信量  参数为:82 每次变位的可能性(0-100)
            {
                int nRand= rand()%101;//0--100
//                qDebug()<<"ZZZZZZZZz"<<m_nLastValue<<nRand;
                if(nRand<nMax)
                {
                    m_nLastValue = 1 - m_nLastValue;
                }
                fValue=m_nLastValue;
            }
            break;
        default:
            Q_ASSERT(false);//("IO变量定义无效:未定义的随机变量取值方式 funcno=100 param1=%d\n",nType);
            break;
        }
        Value.setValue(fValue);
//        qDebug()<<"Value_.setValue(fValue);"<<Value;
        pTempTagI->SetValue(pPoint_,Value,Value);
    }

//    return true;
}
