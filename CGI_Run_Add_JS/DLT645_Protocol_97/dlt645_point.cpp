#include "dlt645_point.h"
/**
 **
 ** Brief:构造函数
 ** Parameter:
 ** Author:G.onion
 ** Craet Date:2015/09/24
 ** Last Modified:2015/09/24
 **
 **/
DLT645_Point::DLT645_Point()
{
    pb_Number_DI1 = 0x00;
    pb_Number_DI0 = 0x00;
    pb_ByteLength = 0;
    Number_DI = "";
    pb_DeviceAddress = "";
    pb_StartByte = 0;
}
/**
 **
 ** Brief:获取地址
 ** Parameter:
 ** Author：G.onion
 ** Creat Date:2015/09/24
 ** Last Modified:2015/09/24
 **
 **/
 int DLT645_Point::GetDeviceAddress()
 {
     return 0;
 }
