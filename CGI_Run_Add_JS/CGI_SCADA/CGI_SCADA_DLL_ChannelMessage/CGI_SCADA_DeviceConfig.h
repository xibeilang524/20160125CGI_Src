#ifndef CGI_SCADA_DEVICECONFIG_H
#define CGI_SCADA_DEVICECONFIG_H

#include "CGI_SCADA_DLL_define.h"

typedef QMap<QString , Device> Factory_Device;
class CGI_SCADA_DeviceConfig : public QObject
{
    Q_OBJECT
public:
    CGI_SCADA_DeviceConfig();
    static void InitManufactureMap();
    static Device GetDevice(QString strFactoryName_, QString strDeviceName_);

    static QMap<QString,Factory_Device > s_ManufacturerMap;///< 厂家、设备map

};

#endif // CGI_SCADA_DEVICECONFIG_H
