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
    CGI_IEC_104 \
#    IEC-104 \
    CGI_RTDB \
    CGI_RTDB_web3 \
    ServerRun \
    ModbusTcp \
    ModbusRtu \
    CGI_RTFrame \
    CGI_RTFrame_web3 \
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
    RTDBSharedMemoryLib_AddYKYS \
    CGI_RTDB_web3_SetValue

win32 {
    SUBDIRS += CGI_Monitor \
    CGI_SCADA_WEB \
    CGI_SCADA

}
unix {
    SUBDIRS += Demo \
    CGI_SetTime
}

