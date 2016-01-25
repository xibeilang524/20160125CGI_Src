#ifndef DLT645_POINT_H
#define DLT645_POINT_H
#include <QString>
#include "../../PortInclude/cpointbase.h"

class DLT645_Point:public CPointBase
{
public:
    DLT645_Point();
    int GetDeviceAddress();

public:
    BYTE pb_mDeviceAddress[6]; //设备地址
    QByteArray pb_ArrayDeviceAddress; //设备地址
    BYTE pb_Number_DI1;
    BYTE pb_Number_DI0;
    unsigned int pb_Number_DI; /// **数据标示强转无符号整型**
    int pb_Function; //小数点
    int pb_ByteLength; //字节长度
    int Pb_ValueType; //值类型
    QString Number_DI;/// **点在报文中的数据标识**
    QString pb_DeviceAddress; //设备地址
    int pb_StartByte; //起始字节




};

#endif // DLT645_POINT_H
