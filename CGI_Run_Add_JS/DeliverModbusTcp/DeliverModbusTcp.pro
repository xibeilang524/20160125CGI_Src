#-------------------------------------------------
#
# Project created by QtCreator 2014-12-02T09:22:08
#
# Modbus TCP转发驱动
#-------------------------------------------------

QT       += network xml serialport
QT       -= gui
TEMPLATE = lib



DEFINES += MODBUSRTU_LIBRARY

SOURCES +=../PortInclude/cvaluebase.cpp \
    cdelivermodbustcp.cpp \
    cpointbasemodbus.cpp \
    ../PortInclude/cprotocolbase.cpp \
    ../PortInclude/cpointbase.cpp \
    ../PortInclude/CFrameSharedMemory.cpp \
    ../PortInclude/CBaseFrame.cpp \
    ../PortInclude/CBufferPool.cpp \
    ../PortInclude/CDecodeBuffer.cpp \
    ../PortInclude/CDecodeBuffer_Hl.cpp \
    ../PortInclude/CMyFunction.cpp

HEADERS += \
    ../PortInclude/CBinaryI.h \
    ../PortInclude/CPointI.h \
    ../PortInclude/CQualityI.h \
    ../PortInclude/CRTDBI.h \
    ../PortInclude/CTagI.h \
    ../PortInclude/cvaluebase.h \
    ../PortInclude/CValueI.h \
    ../PortInclude/typedef.h \
    cdelivermodbustcp.h \
    cpointbasemodbus.h \
    ../PortInclude/cprotocoli.h \
    ../PortInclude/cprotocolbase.h \
    ../PortInclude/cpointbase.h \
    ../PortInclude/CFrameSharedMemory.h \
    ../PortInclude/CBaseFrame.h \
    ../PortInclude/CBufferPool.h \
    ../PortInclude/CDecodeBuffer.h \
    ../PortInclude/CDecodeBuffer_Hl.h \
    ../PortInclude/CMyFunction.h \
    ../PortInclude/CGI_CLINKI.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    TARGET  =       DeliverModbusTcp_d
    DESTDIR =       $$PWD/../../cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/win32/obj/moc
    RCC_DIR =       $$PWD/Debug/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/win32/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       DeliverModbusTcp
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
    TARGET  =       DeliverModbusTcp_d
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/unix/obj/moc
    RCC_DIR =       $$PWD/Debug/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/unix/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       DeliverModbusTcp
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Release/unix/obj/moc
    RCC_DIR =       $$PWD/Release/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/unix/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
