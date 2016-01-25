#-------------------------------------------------
#
# Project created by QtCreator 2015-10-27T19:58:02
#
#-------------------------------------------------

TEMPLATE = subdirs

SUBDIRS += \
    CDT \
    CGI_Run \
    CGI_HDB \
    IEC-104 \
    CGI_RTDB \
    ServerRun \
    ModbusTcp \
    ModbusRtu \
    CGI_RTFrame \
    CGI_CLINKLib \
#    DeliverIEC-104 \
    CGI_DeliverIEC-104 \
    DeliverModbusTcp \
    DatabaseProtocol \
    DLT645_Protocol_97 \
    DLT_645_07_Protocol \
    RTDBSharedMemoryLib \
    AllPowerfulProtocol \
    MonitorDeliverIEC-104 \
    RTFrameSharedMemoryLib \
    CGI_JavaScript_DataChange \
    CGI_JavaScript_Timing \
    CGI_SCADA

win32 {
    SUBDIRS += CGI_Monitor \
    CGI_SCADA_WEB

}
unix {
    SUBDIRS += Demo \
    CGI_SetTime
}

