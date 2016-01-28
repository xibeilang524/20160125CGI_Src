/*!
 * \file    IEC_104.cpp
 * \brief   概述 ASDU结构体等头文件
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

#include "IEC_104.h"
#include <QDateTime>
#include <QDebug>


/*!
 * \brief  功能概述 获取类型名称
 * \param  参数描述 无
 * \return 返回值描述 返回类型名称
 * \author zzy
 * \date   2015/5/26
 */
QString ASDU101::GetTypeName()
{
    //取ASDU101类型名称
        QString strAsduTypeName;
        switch(m_nType)
        {
        case M_SP_NA_1:		// 单点信息
            strAsduTypeName = QString("%1:不带时标单点信息(M_SP_NA_1)").arg(m_nType);
            break;
        case M_SP_TA_1:
            strAsduTypeName = QString("%1:带时标单点信息(M_SP_TA_1)").arg(m_nType);
            break;
        case M_DP_NA_1:		// 双点信息
            strAsduTypeName = QString("%1:不带时标双点信息(M_DP_NA_1)").arg(m_nType);
            break;
        case M_DP_TA_1:
            strAsduTypeName = QString("%1:带时标双点信息(M_DP_TA_1)").arg(m_nType);
            break;
        case M_ST_NA_1:		// 步位置信息(变压器分接头信息)
            strAsduTypeName = QString("%1:不带时标步位置信息(M_ST_NA_1)").arg(m_nType);
            break;
        case M_ST_TA_1:
            strAsduTypeName = QString("%1:带时标步位置信息(M_ST_TA_1)").arg(m_nType);
            break;
        case M_BO_NA_1:		// 子站远动终端状态
            strAsduTypeName = QString("%1:子站远动终端状态(M_BO_NA_1)").arg(m_nType);
            break;
        case M_BO_TA_1:		// 有时标的块数据
            strAsduTypeName = QString("%1:有时标的块数据(M_BO_TA_1)").arg(m_nType);
            break;
        case M_ME_NA_1:		// 带品质描述不带时标的遥测数据
            strAsduTypeName = QString("%1:带品质描述的遥测数据(M_ME_NA_1)").arg(m_nType);
            break;
        case M_ME_TA_1:		// 带品质描述带时标的遥测数据
            strAsduTypeName = QString("%1:带品质描述带时标的遥测数据(M_ME_TA_1)").arg(m_nType);
            break;
        case M_ME_NB_1:		// 测量值, 标度化值, Normalize Value, Without Time Stamp
            strAsduTypeName = QString("%1:带品质描述的标度化值(M_ME_NB_1)").arg(m_nType);
            break;
        case M_ME_TB_1:		// 测量值, 带时标的标度化值, Normalize Value, With Time Stamp
            strAsduTypeName = QString("%1:带时标的标度化值(M_ME_TB_1)").arg(m_nType);
            break;
        case M_ME_NC_1:		// 测量值, 短浮点数, Scaled Value, short floating Without Time Stamp
            strAsduTypeName = QString("%1:短浮点数(M_ME_NC_1)").arg(m_nType);
            break;
        case M_ME_TC_1:		// 测量值, 带时标的短浮点数, Scaled Value, short floating With Time Stamp
            strAsduTypeName = QString("%1:带时标的短浮点数(M_ME_TC_1)").arg(m_nType);
            break;
        case M_IT_NA_1:	// 电能脉冲计数
            strAsduTypeName = QString("%1:电能脉冲计数(M_IT_NA_1)").arg(m_nType);
            break;
        case M_IT_TA_1:
            strAsduTypeName = QString("%1:带时标电能脉冲计数(M_IT_TA_1)").arg(m_nType);
            break;
        case M_EP_TA_1:		// 带时标的继电保护或重合闸设备单个事件
            strAsduTypeName = QString("%1:继电保护装置成组启动事件(M_EP_TA_1)").arg(m_nType);
            break;
        case M_EP_TB_1:		// 带时标的继电保护装置成组启动事件
            strAsduTypeName = QString("%1:继电保护装置成组启动事件(M_EP_TB_1)").arg(m_nType);
            break;
        case M_EP_TC_1:		// 带时标的继电保护装置成组输出电路信息事件
            strAsduTypeName = QString("%1:继电保护装置成组输出电路信息事件(M_EP_TC_1)").arg(m_nType);
            break;
        case M_PS_NA_1:		// 具有状态变位检出的成组单点信息
            strAsduTypeName = QString("%1:具有状态变位检出的成组单点信息(M_PS_TA_1)").arg(m_nType);
            break;
        case M_ME_ND_1:		// 不带品质描述测量值
            strAsduTypeName = QString("%1:不带品质描述测量值(M_ME_ND_1)").arg(m_nType);
            break;
        case VERDOR_EXTEND_22:
        case VERDOR_EXTEND_23:
        case VERDOR_EXTEND_24:
        case VERDOR_EXTEND_25:
        case VERDOR_EXTEND_26:
        case VERDOR_EXTEND_27:
        case VERDOR_EXTEND_28:
        case VERDOR_EXTEND_29:
            strAsduTypeName = QString("%1:用户扩展").arg(m_nType);
            break;
        case M_SP_TB_1: // 带CP56Time2a时标的单点信息
            strAsduTypeName = QString("%1:带CP56Time2a时标的单点信息(M_SP_TB_1)").arg(m_nType);
            break;
        case M_DP_TB_1:		// 带CP56Time2a时标的双点信息
            strAsduTypeName = QString("%1:带CP56Time2a时标的双点信息(M_DP_TB_1)").arg(m_nType);
            break;
        case M_ST_TB_1:		// 带CP56Time2a时标的步位置信息
            strAsduTypeName = QString("%1:带CP56Time2a时标的步位置信息(M_ST_TB_1)").arg(m_nType);
            break;
        case M_BO_TB_1:		// 带CP56Time2a时标的32比特串
            strAsduTypeName = QString("%1:带CP56Time2a时标的32比特串(M_BO_TB_1)").arg(m_nType);
            break;
        case M_ME_TD_1:		// 带CP56Time2a时标的测量值, 规一化值
            strAsduTypeName = QString("%1:带CP56Time2a时标的规一化值(M_ME_TD_1)").arg(m_nType);
            break;
        case M_ME_TE_1:		// 带CP56Time2a时标的测量值, 标度化值
            strAsduTypeName = QString("%1:带CP56Time2a时标的标度化值(M_ME_TE_1)").arg(m_nType);
            break;
        case M_ME_TF_1:		// 带CP56Time2a时标的测量值, 短浮点数
            strAsduTypeName = QString("%1:带CP56Time2a时标的短浮点数(M_ME_TF_1)").arg(m_nType);
            break;
        case M_IT_TB_1:		// 带CP56Time2a时标的累计量
            strAsduTypeName = QString("%1:带CP56Time2a累计量(M_ME_TF_1)").arg(m_nType);
            break;
        case M_EP_TD_1:		// 带CP56Time2a时标的继电保护装置事件
            strAsduTypeName = QString("%1:带CP56Time2a装置事件(M_EP_TD_1)").arg(m_nType);
            break;
        case M_EP_TE_1:		// 带CP56Time2a时标的继电保护装置成组启动事件
            strAsduTypeName = QString("%1:带CP56Time2a成组启动事件(M_EP_TE_1)").arg(m_nType);
            break;
        case M_EP_TF_1:		// 带CP56Time2a时标的继电保护装置成组输出电路信息
            strAsduTypeName = QString("%1:带CP56Time2a成组输出电路信息(M_EP_TF_1)").arg(m_nType);
            break;
        case M_BD_NA_1:	// BDC码(水位值)
            strAsduTypeName = QString("%1:BDC码(水位值)(M_BD_NA_1)").arg(m_nType);
            break;
            // 主站->子站 在控制方向的过程信息
        case C_DC_NA_1:	// 双点遥控命令
            strAsduTypeName = QString("%1:双点遥控命令(C_DC_NA_1)").arg(m_nType);
            break;
        case C_RC_NA_1:		// 升降命令
            strAsduTypeName = QString("%1:升降命令(C_RC_NA_1)").arg(m_nType);
            break;
        case C_SE_NA_1:		// 设定命令
            strAsduTypeName = QString("%1:设定命令(C_RC_NA_1)").arg(m_nType);
            break;
        case C_BO_NA_1:  //  32位的位串
            strAsduTypeName = QString("%1:32位的位串(C_BO_NA_1)").arg(m_nType);
            break;
            // 子站->主站  在监视方向的系统信息
        case M_EI_NA_1:	// 初始化结束
            strAsduTypeName = QString("%1:初始化结束(M_EI_NA_1)").arg(m_nType);
            break;
            // 主站到子站，在控制方向的系统信息
        case C_IC_NA_1:		// 召唤命令
            strAsduTypeName = QString("%1:召唤命令(C_IC_NA_1)").arg(m_nType);
            break;
        case C_CI_NA_1:		// 电能脉冲召唤命令
            strAsduTypeName = QString("%1:电能脉冲召唤命令(C_CI_NA_1)").arg(m_nType);
            break;
        case C_RD_NA_1:		// 读数据命令
            strAsduTypeName = QString("%1:读数据命令(C_RD_NA_1)").arg(m_nType);
            break;
        case C_CS_NA_1:		// 时钟同步命令
            strAsduTypeName = QString("%1:时钟同步命令(C_CS_NA_1)").arg(m_nType);
            break;
        case C_TS_NA_1:		// 测试命令
            strAsduTypeName = QString("%1:测试命令(C_TS_NA_1)").arg(m_nType);
            break;
        case C_RP_NA_1:		// 复位进程命令
            strAsduTypeName = QString("%1:复位进程命令(C_RP_NA_1)").arg(m_nType);
            break;
        case C_CD_NA_1:		// 延时获得命令
            strAsduTypeName = QString("%1:延时获得命令(C_CD_NA_1)").arg(m_nType);
            break;
            // 在控制方向上的参数
        case P_ME_NA_1:		// 装载参数命令
            strAsduTypeName = QString("%1:装载参数命令(P_ME_NA_1)").arg(m_nType);
            break;
        case P_AC_NA_1:		// 激活参数
            strAsduTypeName = QString("%1:激活参数(P_AC_NA_1)").arg(m_nType);
            break;
            // 文件参数
        case F_FR_NA_1:		// 文件已准备好
            strAsduTypeName = QString("%1:文件已准备好(F_FR_NA_1)").arg(m_nType);
            break;
        case F_SR_NA_1:			// 节已准备好
            strAsduTypeName = QString("%1:节已准备好(F_SR_NA_1)").arg(m_nType);
            break;
        case F_SC_NA_1:			// 召唤目录、选择文件、召唤文件、召唤节
            strAsduTypeName = QString("%1:召唤目录(节、文件)(F_SC_NA_1)").arg(m_nType);
            break;
        case F_LS_NA_1:			// 最后的节、最后的段
            strAsduTypeName = QString("%1:最后的节、最后的段(F_LS_NA_1)").arg(m_nType);
            break;
        case F_AF_NA_1:			// 确认文件、确认节
            strAsduTypeName = QString("%1:确认文件、确认节(F_AF_NA_1)").arg(m_nType);
            break;
        case F_SG_NA_1:			// 段
            strAsduTypeName = QString("%1:段(F_SG_NA_1)").arg(m_nType);
            break;
        case F_DR_NA_1:			// 目录
            strAsduTypeName = QString("%1:目录(F_DR_NA_1)").arg(m_nType);
            break;
        default:
            strAsduTypeName = QString("%1:(未知)").arg(m_nType);
            break;
        }
        return strAsduTypeName;
}

/*!
 * \brief  功能概述 获取传送原因
 * \param  参数描述 无
 * \return 返回值描述 返回传送原因
 * \author zzy
 * \date   2015/5/26
 */
QString ASDU101::GetReasonName()
{
    QString strReason;
        switch(REASON)//  REASON)
        {
        case 	ASDU101_TR_PER:	//1. 周期.循环
            strReason = QString("%1:周期.循环").arg(REASON);
            break;
        case 	ASDU101_TR_BACK:	//2 背景扫描
            strReason = QString("%1:背景扫描").arg(REASON);
            break;
        case 	ASDU101_TR_SPONT:  //3 突发
            strReason = QString("%1:突发").arg(REASON);
            break;
        case 	ASDU101_TR_INIT:   //4 初始化
            strReason = QString("%1:初始化").arg(REASON);
            break;
        case 	ASDU101_TR_REQ:	//5 请求或被请求
            strReason = QString("%1:请求或被请求").arg(REASON);
            break;
        case 	ASDU101_TR_ACT:	//6 激活
            strReason = QString("%1:激活").arg(REASON);
            break;
        case 	ASDU101_TR_ACTCON:	//7 激活确认
            strReason = QString("%1:激活确认").arg(REASON);
            break;
        case 	ASDU101_TR_DEACT:	//8 停止激活
            strReason = QString("%1:停止激活").arg(REASON);
            break;
        case 	ASDU101_TR_DEACTCON:	//9 停止激活确认
            strReason = QString("%1:停止激活确认").arg(REASON);
            break;
        case 	ASDU101_TR_ACTTERM:	//10 激活结束
            strReason = QString("%1:激活结束").arg(REASON);
            break;
        case 	ASDU101_TR_RETREM:		//11 远程命令引起的返送信息
            strReason = QString("%1:远程命令引起的返送信息").arg(REASON);
            break;
        case 	ASDU101_TR_RETLOC:		//12 当地命令引起的返送信息
            strReason = QString("%1:当地命令引起的返送信息").arg(REASON);
            break;
        case 	ASDU101_TR_FILE:		//13 文件传送
            strReason = QString("%1:文件传送").arg(REASON);
            break;
        case 	ASDU101_TR_INTROGEN:	//20 响应总召唤
            strReason = QString("%1:响应总召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO1:			//21 响应第1组召唤
            strReason = QString("%1:响应第1组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO2:			//22 响应第2组召唤
            strReason = QString("%1:响应第2组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO3:			//23 响应第3组召唤
            strReason = QString("%1:响应第3组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO4:			//24 响应第4组召唤
            strReason = QString("%1:响应第4组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO5:			//25 响应第5组召唤
            strReason = QString("%1:响应第5组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO6:			//26 响应第6组召唤
            strReason = QString("%1:响应第6组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO7:			//27 响应第7组召唤
            strReason = QString("%1:响应第7组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO8:			//28 响应第8组召唤
            strReason = QString("%1:响应第8组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO9:			//29 响应第9组召唤
            strReason = QString("%1:响应第9组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO10:			//30 响应第10组召唤
            strReason = QString("%1:响应第10组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO11:			//31 响应第11组召唤
            strReason = QString("%1:响应第11组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO12:			//32 响应第12组召唤
            strReason = QString("%1:响应第12组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO13:			//33 响应第13组召唤
            strReason = QString("%1:响应第13组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO14:			//34 响应第14组召唤
            strReason = QString("%1:响应第14组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO15:			//35 响应第15组召唤
            strReason = QString("%1:响应第15组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_INTRO16:			//36 响应第16组召唤
            strReason = QString("%1:响应第16组召唤").arg(REASON);
            break;
        case 	ASDU101_TR_REQCOGCN:			//37 响应计数量总召唤
            strReason = QString("%1:响应计数量总召唤").arg(REASON);
            break;
        case 	ASDU101_TR_REQCO1:				//38 响应第1组计数量召唤
            strReason = QString("%1:响应第1组计数量召唤").arg(REASON);
            break;
        case 	ASDU101_TR_REQCO2:				//39 响应第2组计数量召唤
            strReason = QString("%1:响应第2组计数量召唤").arg(REASON);
            break;
        case 	ASDU101_TR_REQCO3:				//40 响应第3组计数量召唤
            strReason = QString("%1:响应第3组计数量召唤").arg(REASON);
            break;
        case 	ASDU101_TR_REQCO4:				//41 响应第4组计数量召唤
            strReason = QString("%1:响应第4组计数量召唤").arg(REASON);
            break;
        default:
            strReason = QString("%1:未知").arg(REASON);
            break;
        }
        return strReason;
}

/*!
 * \brief  功能概述 获取类型名和传送原因
 * \param  参数描述 无
 * \return 返回值描述 返回类型名和传送原因
 * \author zzy
 * \date   2015/5/26
 */
QString ASDU101::GetName()
{
    Q_ASSERT( sizeof(ASDU101)==8 );
    return GetTypeName()+":"+GetReasonName();
}

/*!
 * \brief  功能概述 获取公共地址
 * \param  参数描述 无
 * \return 返回值描述 返回公共地址
 * \author zzy
 * \date   2015/5/26
 */
int ASDU101::GetCommonAddress()
{
    return (int)m_nCommonAddress;
//    return *(WORD *)m_nCommonAddress;
}

/*!
 * \brief  功能概述 获取公共地址所占长度
 * \param  参数描述 无
 * \return 返回值描述 返回公共地址所占长度
 * \author zzy
 * \date   2015/5/26
 */
int ASDU101::GetCommonAddressSize()
{
    return 2;
}

/*!
 * \brief  功能概述 获取信息体地址
 * \param  参数描述 无
 * \return 返回值描述 返回信息体地址
 * \author zzy
 * \date   2015/5/26
 */
BYTE *ASDU101::GetInfoStart()
{
    return m_pInfo;
}

/*!
 * \brief  功能概述 获取指定信息体的地址
 * \param  参数描述 pInfo是信息体地址
 * \return 返回值描述 返回指定信息体的地址
 * \author zzy
 * \date   2015/5/26
 */
int ASDU101::GetPoint(BYTE *pInfo)
{
    return *(DWORD *)(pInfo) & 0xFFFFFF;
}

/*!
 * \brief  功能概述 获取点长度
 * \param  参数描述 无
 * \return 返回值描述 返回点长度
 * \author zzy
 * \date   2015/5/26
 */
int ASDU101::GetPointSize()
{
    return 3;
}


/*!
 * \brief  功能概述 设置当前时间
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void ASDU101_CP56Time2a::SetCurretTime()
{
    SYSTEMTIME systemTime;
    GetLocalTime(&systemTime);///获取本地时间
    MILLISECOND	=  systemTime.wSecond*1000+systemTime.wMilliseconds;
    MINUTE	=  systemTime.wMinute;
    IV		= 0;// 0 = valid 1= invalid
    HOUR	=  systemTime.wHour;
    SU		= 0;// 0 = standard 1= down
    DAY		=  systemTime.wDay;
    MONTH	=  systemTime.wMonth;
    WEEK		=  systemTime.wDayOfWeek;
    RES1		= 0;
    RES2		= 0;
    RES3		= 0;
    RES4		= 0;
    YEAR		= systemTime.wYear%100;
}

#if 0
MILLISECOND = dateTime.time().second()*1000+dateTime.time().msec();
MINUTE	=  dateTime.time().minute();
IV		= 0;// 0 = valid 1= invalid
HOUR	=  dateTime.time().hour();
SU		= 0;// 0 = standard 1= down
DAY		=  dateTime.date().day();

MONTH	=  dateTime.date().month();
WEEK    =  dateTime.date().dayOfWeek();
RES1    = 0;
RES2    = 0;
RES3    = 0;
RES4    = 0;
YEAR    = dateTime.date().year()%2000;
#endif

/*!
 * \brief  功能概述 获取本地系统当前时间
 * \param  参数描述 systemTime是SYSTEMTIME类型的指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/26
 */
void GetLocalTime(SYSTEMTIME *systemTime)
{
    QDateTime dateTime(QDateTime::currentDateTime());
    systemTime->wYear = dateTime.date().year()%2000;
    systemTime->wMonth = dateTime.date().month();
    systemTime->wDayOfWeek = dateTime.date().dayOfWeek();
    systemTime->wDay = dateTime.date().day();
    systemTime->wHour = dateTime.time().hour();
    systemTime->wMinute = dateTime.time().minute();
    systemTime->wSecond = dateTime.time().second();
    systemTime->wMilliseconds = dateTime.time().msec();
}

/*!
 * \brief  功能概述 获取系统时间
 * \param  参数描述 无
 * \return 返回值描述 返回系统时间
 * \author zzy
 * \date   2015/5/26
 */
SYSTEMTIME ASDU101_CP56Time2a::GetTime()
{
    SYSTEMTIME systemTime;
    systemTime.wMilliseconds=MILLISECOND%1000;
    systemTime.wSecond=MILLISECOND/1000;
    systemTime.wMinute= MINUTE;
    systemTime.wHour= HOUR;
    systemTime.wDay= DAY;
    systemTime.wMonth= MONTH;
    systemTime.wYear= YEAR+2000;
    return systemTime;
}
