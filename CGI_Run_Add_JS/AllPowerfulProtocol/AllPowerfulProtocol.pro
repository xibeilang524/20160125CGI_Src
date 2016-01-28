#-------------------------------------------------
#
# Project created by QtCreator 2014-11-19T15:53:17
#
# DLT-645-07电表协议驱动
#-------------------------------------------------
QT += core xml network serialport
QT       -= gui
TEMPLATE = lib

DEFINES += MODBUSRTU_LIBRARY

SOURCES += \
    ../PortInclude/cvaluebase.cpp \
    ../PortInclude/cprotocolbase.cpp \
    ../PortInclude/cpointbase.cpp \
    ../PortInclude/CFrameSharedMemory.cpp \
    ../PortInclude/CBaseFrame.cpp \
    ../PortInclude/CBufferPool.cpp \
    ../PortInclude/CDecodeBuffer.cpp \
    ../PortInclude/CDecodeBuffer_Hl.cpp \
    ../PortInclude/CMyFunction.cpp \
    CAllPowerful_Query.cpp \
    CAllPowerful_Protocol.cpp \
    CAllPowerful_Response.cpp \
    CAllPowerful_Point.cpp

HEADERS += \
    ../PortInclude/CTagI.h \
    ../PortInclude/CBinaryI.h \
    ../PortInclude/CPointI.h \
    ../PortInclude/CQualityI.h \
    ../PortInclude/CRTDBI.h \
    ../PortInclude/CValueI.h \
    ../PortInclude/cvaluebase.h \
    ../PortInclude/cprotocoli.h \
    ../PortInclude/CPRTMapI.h \
    ../PortInclude/CPRTI.h \
    ../PortInclude/cprotocolbase.h \
    ../PortInclude/cpointbase.h \
    ../PortInclude/CFrameSharedMemory.h \
    ../PortInclude/CBaseFrame.h \
    ../PortInclude/CBufferPool.h \
    ../PortInclude/CDecodeBuffer.h \
    ../PortInclude/CDecodeBuffer_Hl.h \
    ../PortInclude/CMyFunction.h \
    ../PortInclude/typedef.h \
    ../PortInclude/CGI_CLINKI.h \
    CAllPowerful_Query.h \
    CAllPowerful_Protocol.h \
    CAllPowerful_Response.h \
    CAllPowerful_Point.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES +=

win32 {
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    TARGET  =       AllPowerfulProtocol_d
    DESTDIR =       $$PWD/../../cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/win32/obj/moc
    RCC_DIR =       $$PWD/Debug/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/win32/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       AllPowerfulProtocol
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
    TARGET  =       AllPowerfulProtocol_d
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/unix/obj/moc
    RCC_DIR =       $$PWD/Debug/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/unix/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       AllPowerfulProtocol
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Release/unix/obj/moc
    RCC_DIR =       $$PWD/Release/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/unix/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
