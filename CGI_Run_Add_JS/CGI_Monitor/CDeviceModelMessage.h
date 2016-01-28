#ifndef CDEVICEMODELMESSAGE_H
#define CDEVICEMODELMESSAGE_H

#include <QObject>
#include "EnumClass.h"
#include <QMap>

class CDeviceModelMessage : public QObject
{
    Q_OBJECT
public:
    explicit CDeviceModelMessage(QObject *parent = 0);
    static void InitDeviceModelMessage(QString strFileName_);
    static QMap<QString, Device_Type_Property* > m_DeviceTypeMessageMap;///< QString为设备名称

signals:

public slots:

};

#endif // CDEVICEMODELMESSAGE_H
