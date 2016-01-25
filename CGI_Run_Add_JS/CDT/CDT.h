#ifndef CDT_H
#define CDT_H
/*!
 \brief 处理遥测信息字，获取遥测量 RemoteMeasure

 \class FRAME_RM_TYPE CDT.h "CDT.h"
*/
struct FRAME_RM_TYPE
{
    int          VALUE:12;     //变量的值
    unsigned int RES:2;        //保留
    unsigned int OVERFALL:1;     //溢出
    unsigned int INVALID:1;      //无效
};
/*!
 \brief 获取遥脉量  RemotePulse

 \class FRAME_RP_TYPE CDT.h "CDT.h"
*/
struct FRAME_RP_TYPE
{
    unsigned int VALUE:24;       //变量的值
    unsigned int RES:4;          //作为扩展用
    unsigned int RES1:1;//保留
    unsigned int BCD:1;          //是否为BCD码
    unsigned int RES2:1;          //保留
    unsigned int INVALID:1;      //无效
};
/*!
 \brief 获取SOE信息

 \class FRAME_SOE_TYPE CDT.h "CDT.h"
*/
struct FRAME_SOE_TYPE
{
    unsigned int MILLSECOND:10;   //毫秒
    unsigned int RES1:6;
    unsigned int SECOND:6;       //秒
    unsigned int RES2:2;
    unsigned int MINUTE:6;       //分
    unsigned int RES3:2;
    unsigned int CRC1:8;         //crc
    unsigned int FUNCTION2:8;    //功能码2
    unsigned int HOUR:5;         //时
    unsigned int RES4:3;
    unsigned int DAY:5;          //日
    unsigned int RES5:3;
    unsigned int POINT:12;       //点号
    unsigned int RES6:3;
    unsigned int VALUE:1;        //状态值
};
#endif // CDT_H
