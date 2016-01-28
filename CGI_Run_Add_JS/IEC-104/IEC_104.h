/*!
 * \file    IEC_104.h
 * \brief   概述 IEC_104协议的宏定义和ASDU结构体等头文件
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

#ifndef IEC_104_H
#define IEC_104_H

#include <QString>
#include "../PortInclude/CGI_MacroFunction.h"


#define MAX_RXTXSEQNUM		((unsigned short)0x8000)
#define YS_UNKNOWN_STEP                          0
#define YK_SELECT_STEP                           1
#define YK_EXECUT_STEP                           2
#define YK_CANCEL_STEP                           3
//U控制帧    都与0x03相或   即 APP_UFORMAT_XXX | 0x03
#define APP_UFORMAT_STARTACT					0x04 ///< 启动命令
#define	APP_UFORMAT_STARTCON					0x08 ///< 启动确认
#define	APP_UFORMAT_STOPACT						0x10 ///< 停止命令
#define	APP_UFORMAT_STOPCON						0x20 ///< 停止确认
#define	APP_UFORMAT_TESTACT						0x40 ///< 测试命令
#define	APP_UFORMAT_TESTCON						0x80 ///< 测试确认

#define IEC104_IDLE								1	///< 空状态
#define IEC104_WAITALLDATACONF					2   ///< 等待全数据确认
#define IEC104_WAITTIMECONF						3   ///< 等待时间确认
#define IEC104_WAITALLKWHCONF					4   ///< 等待全电度确认
#define IEC104_WAITALLDATA						5   ///< 等待全数据
#define IEC104_WAITALLKWH						6   ///< 等待全电度
#define IEC104_WAITYKCONF						7   ///< 等待遥控确认
#define IEC104_WAITSTARTCONF					8   ///< 等待启动连接确认
#define IEC104_WAITSTOPCONF						9   ///< 等待停止连接确认
#define IEC104_WAITTESTCONF						10  ///< 等待测试链路确认
#define IEC104_WAITPROTCONF						11  ///< 等待保护确认
#define IEC104_WAITYTCONF						12  ///< 等待遥调确认
#define IEC104_WAITSETPOINTCONF					13  ///< 等待设定点确认
#define IEC104_WAITRSTPROCCONF					14  ///< 等待复位确认
#define IEC104_WAITTESTLINKCONF					15  ///< 等待测试链路确认

#define IEC104_RESET							30  ///< 重置状态
#define IEC104_STOPDT							31  ///< 停止数据传输
#define IEC104_STARTDT							32  ///< 启动数据传输
#define IEC104_TESTLINK							33  ///< 测试链路
#define IEC104_RESETPROCESS						34  ///< 复位进程
#define IEC104_TESTCON                          35

#define IEC104_WAITSETMEASPARA                  60
//////////////////////////////////////////////////////////////////////////

/*!
 * \enum   ASDU101_TYPE
 * \brief  概述 定义ASDU101类型标识
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
enum ASDU101_TYPE
{                   // 报文语义   其它说明
    UNKOWN_ASDU101_TYPE=0,
///监控方向的过程信息类型标识(上行信息)
    M_SP_NA_1=1,	///< 单位遥信    带品质描述、不带时标
    M_SP_TA_1,
    M_DP_NA_1=3,	///< 双位遥信    带品质描述、不带时标
    M_DP_TA_1,
    M_ST_NA_1,		///< 步位置信息(变压器分接头信息)
    M_ST_TA_1,
    M_BO_NA_1,		///< 子站远动终端状态
    M_BO_TA_1,		///< 有时标的块数据
    M_ME_NA_1=9,	///< 归一化遥测值    带品质描述、不带时标
    M_ME_TA_1,		///< 带品质描述带时标的遥测数据
    M_ME_NB_1=11,   ///< 标度化遥测值    带品质描述、不带时标
    M_ME_TB_1,		///< 测量值, 带时标的标度化值, Normalize Value, With Time Stamp
    M_ME_NC_1=13,	///< 短浮点遥测值    带品质描述、不带时标
    M_ME_TC_1,		///< 测量值, 带时标的短浮点数, Scaled Value, short floating With Time Stamp
    M_IT_NA_1=15,	///< 累计量         带品质描述、不带时标
    M_IT_TA_1,
    M_EP_TA_1,		///< 带时标的继电保护或重合闸设备单个事件
    M_EP_TB_1,		///< 带时标的继电保护装置成组启动事件
    M_EP_TC_1,		///< 带时标的继电保护装置成组输出电路信息事件
    M_PS_NA_1=20, 	///< 成组单位遥信    只带变位标志
    M_ME_ND_1=21,	///< 归一化遥测值    不带品质描述、不带时标
    VERDOR_EXTEND_22,
    VERDOR_EXTEND_23,
    VERDOR_EXTEND_24,
    VERDOR_EXTEND_25,
    VERDOR_EXTEND_26,
    VERDOR_EXTEND_27,
    VERDOR_EXTEND_28,
    VERDOR_EXTEND_29,
    M_SP_TB_1=30,   ///< 单位遥信(SOE)    带品质描述、带绝对时标      带CP56Time2a时标的单点信息
    M_DP_TB_1=31,	///< 双位遥信(SOE)    带品质描述、带绝对时标      带品质描述、不带时标带CP56Time2a时标的双点信息
    M_ST_TB_1,		///< 带CP56Time2a时标的步位置信息
    M_BO_TB_1,		///< 带CP56Time2a时标的32比特串
    M_ME_TD_1=34,	///< 归一化遥测值      带品质描述、带绝对时标      带CP56Time2a时标的测量值, 规一化值
    M_ME_TE_1=35,	///< 标度化遥测值      带品质描述、带绝对时标      带CP56Time2a时标的测量值, 标度化值
    M_ME_TF_1=36,	///< 短浮点遥测值      带品质描述、带绝对时标      带CP56Time2a时标的测量值, 短浮点数
    M_IT_TB_1=37,	///< 累计量           带品质描述、带绝对时标      带CP56Time2a时标的累计量
    M_EP_TD_1,		///< 带CP56Time2a时标的继电保护装置事件
    M_EP_TE_1,		///< 带CP56Time2a时标的继电保护装置成组启动事件
    M_EP_TF_1,		///< 带CP56Time2a时标的继电保护装置成组输出电路信息
    M_BD_NA_1=232,	///< BDC码(水位值)
///控制方向的过程信息类型标识(下行信息)
    C_SC_NA_1=45,   ///< 单位遥控命令   每个报文只能包含一个遥控信息体
    C_DC_NA_1=46,	///< 双位遥控命令   每个报文只能包含一个遥控信息体
    C_RC_NA_1=47,	///< 档位调节命令   每个报文只能包含一个档位信息体
    C_SE_NA_1=48,	///< 归一化设定值   每个报文只能包含一个设定值
    C_SE_NB_1=49,   ///< 标度化设定值   每个报文只能包含一个设定值
    C_SE_NC_1=50,   ///< 短浮点设定值   每个报文只能包含一个设定值
    C_BO_NA_1=51,   ///< 32位的位串
    C_SE_ND_1=136,  ///< 归一化设定值   每个报文可以包含多个设定值
/// 监视方向的系统信息(上行信息)
    M_EI_NA_1=70,	///< 初始化结束     报告站端初始化完成
/// 在控制方向的系统信息(下行信息)
    C_IC_NA_1=100,	///< 站召唤命令     带不同的限定词可以用于组召唤
    C_CI_NA_1=101,	///< 累计量召唤命令  带不同的限定词可以用于组召唤
    C_RD_NA_1=102,	///< 读命令         读单个信息对象值
    C_CS_NA_1=103,	///< 时钟同步命令   需要通过测量通道延时加以校正
    C_TS_NA_1,      ///< 测试命令
    C_RP_NA_1=105,	///< 复位进程命令   使用前需要与双方确认
    C_CD_NA_1,		///< 延时获得命令
    // 在控制方向上的参数
    P_ME_NA_1=110,		///< 装载参数命令
    P_AC_NA_1,		///< 激活参数
    // 文件参数
    F_FR_NA_1=120,		///< 文件已准备好
    F_SR_NA_1,			///< 节已准备好
    F_SC_NA_1,			///< 召唤目录、选择文件、召唤文件、召唤节
    F_LS_NA_1,			///< 最后的节、最后的段
    F_AF_NA_1,			///< 确认文件、确认节
    F_SG_NA_1,			///< 段
    F_DR_NA_1,			///< 目录
};

/*!
 * \enum   ASDU101_TR_REASON_TYPE
 * \brief  概述 定义 传送原因
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
enum ASDU101_TR_REASON_TYPE
{                       // 语义   应用方向
    UNKOWN_ASDU101_REASON_TYPE=0,
    ASDU101_TR_PER=1,     ///< 周期、循环    上行
    ASDU101_TR_BACK=2,    ///< 背景扫描     上行
    ASDU101_TR_SPONT=3,   ///< 突发         上行
    ASDU101_TR_INIT=4,    ///< 初始化       上行
    ASDU101_TR_REQ=5,     ///< 请求或被请求  上行、下行
    ASDU101_TR_ACT=6,     ///< 激活         下行(应用于重要功能过程的每个步骤中)
    ASDU101_TR_ACTCON=7,  ///< 激活确认      上行(应用于重要功能过程的每个步骤中)
    ASDU101_TR_DEACT=8,   ///< 停止激活      下行(应用于重要功能过程的每个步骤中)
    ASDU101_TR_DEACTCON=9,///< 停止激活确认   上行(应用于重要功能过程的每个步骤中)
    ASDU101_TR_ACTTERM=10,///< 激活终止       下行(应用于重要功能过程的每个步骤中)
    ASDU101_TR_RETREM=11, ///< 远程命令引起的返送信息    上行
    ASDU101_TR_RETLOC=12, ///< 当地命令引起的返送信息    上行
    ASDU101_TR_FILE,      ///< 文件
    ASDU101_TR_INTROGEN=20,	///< 响应站召唤    上行
    ASDU101_TR_INTRO1=21,	///< 响应第1组召唤    上行
    ASDU101_TR_INTRO2=22,	///< 响应第2组召唤    上行
    ASDU101_TR_INTRO3=23,	///< 响应第3组召唤    上行
    ASDU101_TR_INTRO4=24,	///< 响应第4组召唤    上行
    ASDU101_TR_INTRO5=25,	///< 响应第5组召唤    上行
    ASDU101_TR_INTRO6=26,	///< 响应第6组召唤    上行
    ASDU101_TR_INTRO7=27,	///< 响应第7组召唤    上行
    ASDU101_TR_INTRO8=28,	///< 响应第8组召唤    上行
    ASDU101_TR_INTRO9=29,	///< 响应第9组召唤    上行
    ASDU101_TR_INTRO10=30,  ///< 响应第10组召唤   上行
    ASDU101_TR_INTRO11=31,  ///< 响应第11组召唤   上行
    ASDU101_TR_INTRO12=32,  ///< 响应第12组召唤   上行
    ASDU101_TR_INTRO13=33,  ///< 响应第13组召唤   上行
    ASDU101_TR_INTRO14=34,  ///< 响应第14组召唤   上行
    ASDU101_TR_INTRO15=35,  ///< 响应第15组召唤   上行
    ASDU101_TR_INTRO16=36,  ///< 响应第16组召唤   上行
    ASDU101_TR_REQCOGCN=37,	///< 响应累计量站召唤     上行
    ASDU101_TR_REQCO1=38,   ///< 响应第1组计数量召唤  上行
    ASDU101_TR_REQCO2=39,   ///< 响应第2组计数量召唤  上行
    ASDU101_TR_REQCO3=40,   ///< 响应第3组计数量召唤  上行
    ASDU101_TR_REQCO4=41,   ///< 响应第4组计数量召唤  上行
    ASDU101_TR_UNKNOW_ASDU_TYPE=44,     ///< 未知的类型标识
    ASDU101_TR_UNKNOW_REASON_TYPE=45,   ///< 未知的传送原因
    ASDU101_TR_UNKNOW_PUBLIC_ADDR=46,   ///< 未知的应用服务数据单元公共地址
    ASDU101_TR_UNKNOW_MESSAGE_ADDR=47,  ///< 未知的信息对象地址
};

/*!
 * \struct ASDU101
 * \brief  概述 IEC-104协议的ASDU
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct ASDU101
{
//Attribs  可能 有变
// 原因长度=1
// 公共地址长度=1
    BYTE             m_nType;     ///< 类型标识
    union
    {
        struct
        {
            BYTE 	 NUMBER:7;	///< 个数
            BYTE     SQ:1;		///< 单个/顺序
        };
        BYTE m_var;///< 可变结构限定词
    };
    union
    {
        struct
        {
            BYTE 	 REASON:6;	///< 原因
            BYTE     PN:1;		///< 肯定/否定
            BYTE     T:1;		///< 未实验/试验
        };
        WORD  m_reason;///< 传输原因
    };

    WORD             m_nCommonAddress; ///< 应用服务数据单元公共地 址  (2BYTE);
//	WORD             m_nInfoAddress;   //信息体地址 点号
//	BYTE             m_info[100];      //信息
    BYTE             m_pInfo[4];       ///< 信息体
public:
    QString GetTypeName();
    QString GetReasonName();
    QString GetName();

    int GetCommonAddress();
    int GetCommonAddressSize();
    BYTE * GetInfoStart();

    int GetPoint(BYTE *pInfo);
    int  GetPointSize();
};

/*!
 * \struct _SYSTEMTIME
 * \brief  概述 系统本地时间 Clock  Time
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
typedef struct  _SYSTEMTIME
{
    WORD wYear;///< 年
    WORD wMonth;///< 月
    WORD wDayOfWeek;///< 周
    WORD wDay;///< 日
    WORD wHour;///< 时
    WORD wMinute;///< 分
    WORD wSecond;///< 秒
    WORD wMilliseconds;///< 毫秒
}SYSTEMTIME;

/*6.4.10  Clock / Time*/
//7.2.6.18七个八位位组二进制时间(CP56Time2a)
/*!
 * \struct ASDU101_CP56Time2a
 * \brief  概述 7.2.6.18七个八位位组二进制时间(CP56Time2a)
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct ASDU101_CP56Time2a
{
public:
    /* milliseconds*/
    WORD	MILLISECOND;///< milliseconds

    /* minutes */
    BYTE MINUTE:6;///< minutes
    /* reserve */
    BYTE RES1:1;///< reserve
    /*	<0> := valid
        <1> := invalid		*/
    BYTE IV:1;///< <0> := valid <1> := invalid

    /* Hours  0..23 */
    BYTE HOUR:5;///< Hours  0..23
    /* reserve */
    BYTE RES2:2;///< reserve
    /* su */
    BYTE SU:1;///< su

    /* Day of month  1..31*/
    BYTE DAY:5;///< Day of month  1..31
    /* Day of week 1..7 */
    BYTE WEEK:3;///< Day of week 1..7

    /* Months  1..12 */
    BYTE MONTH:4;///< Months  1..12
    /* reserve  */
    BYTE RES3:4;///< reserve

    /* years 0..99 */
    BYTE YEAR:7;///< years 0..99
    /* reserve  */
    BYTE RES4:1;///< reserve
// Implementation
public:
    void SetCurretTime();
    SYSTEMTIME GetTime();
    int GetSize(){return 7;}
};
void GetLocalTime(SYSTEMTIME *systemTime);
//7.2.6.20二个八位位组二进制时间(CP16Time2a)

/*!
 * \struct ASDU101_CP16Time2a
 * \brief  概述 7.2.6.20二个八位位组二进制时间(CP16Time2a)
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct ASDU101_CP16Time2a
{
public:
    WORD m_nValue;
public:
    int GetSize(){return 2;	}
};

/*6.4.11 Double command Object (Remote control command) */
/*!
 * \struct ASDU101_DCO
 * \brief  概述 6.4.11 Double command Object (Remote control command)
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct ASDU101_DCO
{
public:
    /* Double command state
        <0> := not permitted
        <1> := OFF
        <2> := ON
        <3> := not permitted */
    BYTE	DCS: 2;///< Double command state <0> := not permitted <1> := OFF <2> := ON <3> := not permitted

    BYTE	QU : 5; ///< <0> := no additional definition
                    ///< <1> := short pulse duration (circuit-breaker),duration determined by a system parameter in the outstation
                    ///< <2> := long duration pulse,mduration determined by a system parametert in the outstation
                    ///< <3> := persistent output
                    ///< <4..8>	:= reserved for standard definitions of this companion standard (compatible range)
                    ///< <9..15>	:= reserved for the selection of other predefined functions
                    ///< <16..31> := reserved for special use (private range)

    BYTE	S_E: 1; ///<	<0> := Excute
                    ///< <1> := Select
public:
    int GetSize(){return 1;}
};
/*6.4.11.. Regulating step command  */
//class  C_RCO :public CInfoUnit
//{
//public:
//    /* Double command state
//        <0> := not permitted
//        <1> := next step LOWER
//        <2> := next step HIGHER
//        <3> := not permitted */
//    BYTE	btRCS: 2;
//    /* <0> := no additional definition
//       <1> := short pulse duration (circuit-breaker),duration determined by a system parameter in the outstation
//       <2> := long duration pulse,mduration determined by a system parametert in the outstation
//       <3> := persistent output
//       <4..8>	:= reserved for standard definitions of this companion standard (compatible range)
//       <9..15>	:= reserved for the selection of other predefined functions
//       <16..31> := reserved for special use (private range)			*/
//    BYTE	btQU : 5;
//    /*	<0> := Excute
//        <1> := Select */
//    BYTE	btS_E: 1;
//// Implementation
//public:
//    size_t GetSize(){return 1;}
//};

/*6.4.12  Time*/
//7.2.6.19 三个八位位组二进制时间(CP24Time2a)
/*!
 * \struct ASDU101_CP24Time2a
 * \brief  概述 7.2.6.19 三个八位位组二进制时间(CP24Time2a)
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct ASDU101_CP24Time2a
{
public:
    /* Milliseconds  <0 ~ 59999>*/
    WORD	MILLISECOND;///< Milliseconds  <0 ~ 59999>
    /* minutes */
    BYTE	MINUTE : 6;///< minutes
    BYTE	Res : 1;
    BYTE	IV  : 1;///< invalid <0> := valid <0> := invalid
public:
    int GetSize(){return 3;}
};

/*6.4.13.1 single event of protection equipment */
//7.2.6.10继电保护设备单个事件(SEP)
/*!
 * \struct ASDU101_SEP
 * \brief  概述 7.2.6.10继电保护设备单个事件(SEP)
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct ASDU101_SEP
{
public:
    /* Event state */
    BYTE	ES	:2;///< Event state
    /* Reserve*/
    BYTE	RES	:1;///< Reserve
    /*	<0>:= Act time is valid
        <1>:= Act time is invalid
    */
    BYTE	EI	:1;
    BYTE	BL	:1;
    BYTE	SB	:1;
    BYTE	NT	:1;
    BYTE	IV	:1;

public:
    int GetSize(){return 1;}
};

//7.2.6.1 带品质描述词的单点信息(IEV 371-02-07)(SIQ)
/*!
 * \struct ASDU101_SIQ
 * \brief  概述 带品质描述词的单点信息(IEV 371-02-07)(SIQ)
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct ASDU101_SIQ
{
public:
    /*	单点信息状态
     * <0> := OFF 开
        <1> := ON 合	*/
    BYTE	SPI:1;
    /* Reserve	保留位	*/
    BYTE	RES:3;
    /*	封锁标志
     * <0> := not blocked 未被封锁
        <1> := blocked		该状态被当地封锁*/
    BYTE	BL :1;
    /*	取代标志
     * <0> := not substituted  未被取代
        <1> := substituted 该状态被人工设置或被其它装置取代*/
    BYTE	SB :1;
    /*	刷新标志
     * <0> := topical  当前值
        <1> := not topical 非当前状态，表示本次采样刷新没有成功*/
    BYTE	NT :1;
    /*	有效标志
     * <0> := valid  状态有效
        <1> := invalid 状态无效 */
    BYTE	IV :1;
// Implementation
public:
    size_t GetSize(){return 1;}
};

//7.2.6.2 带品质描述词的双点信息(IEV 371-02-08)(DIQ)
/*!
 * \struct ASDU101_DIQ
 * \brief  概述 7.2.6.2 带品质描述词的双点信息(IEV 371-02-08)(DIQ)
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct ASDU101_DIQ
{
public:
    ///	<0> := indeterminate or intermediate state
    /// <1> := determined state OFF
    /// <2> := determined state ON
    /// <3> := intermediate state
    BYTE	DPI:2;
    /* Reserve */
    BYTE	RES:2;///< Reserve
    /*	<0> := not blocked
        <1> := blocked		*/
    BYTE	BL :1;///< <0> := not blocked <1> := blocked
    /*	<0> := not substituted
        <1> := substituted	*/
    BYTE	SB :1;///< <0> := not substituted <1> := substituted
    /*	<0> := topical
        <1> := not topical	*/
    BYTE	NT :1;///< <0> := topical <1> := not topical
    /*	<0> := valid
        <1> := invalid		*/
    BYTE	IV :1;///< <0> := valid <1> := invalid

public:
    int GetSize(){return 1;}
};

//7.2.6.5带瞬变状态指示的值，用于变压器的步位置或者其他步位置信息(VTI)
/*!
 * \struct ASDU101_VTI
 * \brief  概述 7.2.6.5带瞬变状态指示的值，用于变压器的步位置或者其他步位置信息(VTI)
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct ASDU101_VTI
{
public:
//VTI
    /*	value := I7[1..7]<-64..+63> */
    signed char	VALUE:7;///< value := I7[1..7]<-64..+63>
    /*	<0> := equipment is not in transient state
        <1> := equipment is in transient state */
    BYTE	T:1;///< <0> := equipment is not in transient state <1> := equipment is in transient state
public:
    int GetSize(){return 1;}
};

//7.2.6.3 品质描述词(单个八位位组)(QDS)
/*!
 * \struct ASDU101_QDS
 * \brief  概述 7.2.6.3 品质描述词(单个八位位组)(QDS)
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct  ASDU101_QDS
{
public:
/*	<0> := no overflow 未溢出
    <1> := overflow	 溢出*/
    BYTE	OV:1;///< 溢出标志 <0> := no overflow 未溢出 <1> := overflow	 溢出
    /* Reserve		*/
    BYTE	RES:3;///< 保留位 Reserve
    /*	<0> := not blocked
    <1> := blocked		*/
    BYTE	BL :1;///< 封锁标志 <0> := not blocked <1> := blocked
    /*	<0> := not substituted
    <1> := substituted */
    BYTE	SB :1;///< 取代标志 <0> := not substituted <1> := substituted
    /*	<0> := topical
    <1> := not topical */
    BYTE	NT :1;///< 刷新标志 <0> := topical <1> := not topical
    /*	<0> := valid 有效
    <1> := invalid	无效	*/
    BYTE	IV :1;///< 有效标志 <0> := valid 有效 <1> := invalid	无效
public:
    int GetSize(){return 1;}
};

//7.2.6.13二进制状态信息(IEV371-02-03)(BSI)
/*!
 * \struct ASDU101_BSI
 * \brief  概述 7.2.6.13二进制状态信息(IEV371-02-03)(BSI)
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct ASDU101_BSI
{
public:
    DWORD BSI;
public:
    int GetSize(){return 4;}
};
//6.4.5  Analog //7.2.6.6规一化值(NVA)
// struct ASDU101_NVA
// {
// public:
// 	WORD	VALUE:15;
// 	WORD	S:1;
// public:
// 	int GetValue()
// 	{
// 		if(S)
// 			return -1*VALUE;
// 		else
// 			return VALUE;
// 	}
//
// 	int GetSize(){return 2;}
// };

/*!
 * \struct ASDU101_NVA
 * \brief  概述 7.2.6.6规一化值(NVA)
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct ASDU101_NVA
{
public:
    short int nValue;
public:
    int GetValue()
    {
        return nValue;
    }

    int GetSize(){return 2;}
};

//7.2.6.7标度化值(SVA)
/*!
 * \struct ASDU101_SVA
 * \brief  概述 7.2.6.7标度化值(SVA)
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct ASDU101_SVA
{
public:
    short int m_nValue;
public:
    int GetSize(){return 2;}

};

//7.2.6.8短浮点数(R32-IEEE STD 754)
/*!
 * \struct ASDU101_R32
 * \brief  概述 7.2.6.8短浮点数(R32-IEEE STD 754)
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct ASDU101_R32
{
public:
    float m_nValue;
public:
    int GetSize(){return 4;}
};

/*6.4.7  Binary counter reading.*/
//7.2.6.9二进计数量读数(BCR)
/*!
 * \struct ASDU101_BCR
 * \brief  概述 6.4.7  Binary counter reading. 7.2.6.9二进计数量读数(BCR)
 * \author zzy
 * \date   2015/5/26
 * 详细概述
 */
struct ASDU101_BCR
{
public:
    /*	counter reading */
    DWORD VALUE:31;///< counter reading
    DWORD S:1;
    /*	sequence number */
    BYTE	SQ:5;///< sequence number
    /*	carry
        <0> := no counter overflow occurred in the corresponding integration period
        <1> := counter overflow occurred in the corresponding integration period	*/
    BYTE	CY:1;///< carry <0> := no counter overflow occurred in the corresponding integration period <1> := counter overflow occurred in the corresponding integration period
    /*	counter was adjusted
        <0> := counter was not adjusted since last reading
        <1> := counter was  adjusted since last reading */
    BYTE	CV:1;///< counter was adjusted <0> := counter was not adjusted since last reading <1> := counter was  adjusted since last reading
    /*	Invalid
        <0> := counter reading is valid / No overflow
        <1> := counter reading is invalid / overflow	*/
    BYTE	IV:1;///< Invalid <0> := counter reading is valid / No overflow <1> := counter reading is invalid / overflow
// Implementation
public:
    int GetSize(){return 5;}
};
#endif // IEC_104_H

