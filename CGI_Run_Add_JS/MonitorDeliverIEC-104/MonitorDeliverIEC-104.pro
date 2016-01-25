#-------------------------------------------------
#
# Project created by QtCreator 2014-11-19T15:53:17
#
# MonitorDeliverIEC-104 监视工具的104转发驱动
#-------------------------------------------------
QT += core serialport xml network
QT       -= gui
TEMPLATE = lib

DEFINES += MODBUSRTU_LIBRARY

SOURCES += \
    cpointbase.cpp \
    ../PortInclude/cvaluebase.cpp \
    cprotocolbase.cpp \
    cpointiec_104.cpp \
    IEC_104.cpp \
    ciec104deliverquery.cpp \
    CProtocolDeliverPRT_IEC_104.cpp \
    CWriteFrame.cpp

HEADERS += \
    ../PortInclude/CTagI.h \
    ../PortInclude/CBinaryI.h \
    ../PortInclude/CPointI.h \
    ../PortInclude/CQualityI.h \
    ../PortInclude/CRTDBI.h \
    ../PortInclude/CValueI.h \
    cpointbase.h \
    ../PortInclude/cvaluebase.h \
    cprotocolbase.h \
    cpointiec_104.h \
    IEC_104.h \
    ciec104deliverquery.h \
    CProtocolDeliverPRT_IEC_104.h \
    ../PortInclude/cprotocoli.h \
    CWriteFrame.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
win32 {
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    TARGET  =       MonitorDeliverIEC-104_d
    DESTDIR =       $$PWD/../../cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/win32/obj/moc
    RCC_DIR =       $$PWD/Debug/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/win32/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       MonitorDeliverIEC-104
    DESTDIR =       $$PWD/../../cgi-bin/Protocol
    MOC_DIR =       $$PWD/Release/win32/obj/moc
    RCC_DIR =       $$PWD/Release/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/win32/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
#DEFINES += QT_NO_DEBUG_OUTPUT
unix {
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    TARGET  =       MonitorDeliverIEC-104_d
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/unix/obj/moc
    RCC_DIR =       $$PWD/Debug/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/unix/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       MonitorDeliverIEC-104
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Release/unix/obj/moc
    RCC_DIR =       $$PWD/Release/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/unix/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
