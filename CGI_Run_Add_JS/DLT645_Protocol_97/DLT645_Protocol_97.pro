#-------------------------------------------------
#
# Project created by QtCreator 2015-09-23T13:53:06
#
# DLT-645-97电表协议驱动
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
    dlt645_protocol_97.cpp \
    dlt645_point.cpp \
    dtl645_query.cpp \
    dlt645_97_response.cpp \
    cbaseframecdt.cpp \
    ../PortInclude/CBaseFrame.cpp \
    ../PortInclude/CBufferPool.cpp \
    ../PortInclude/CDecodeBuffer.cpp \
    ../PortInclude/CDecodeBuffer_Hl.cpp \
    ../PortInclude/CMyFunction.cpp

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
    dlt645_protocol_97.h \
    dlt645_point.h \
    dlt645_query.h \
    dlt645_97_response.h \
    cbaseframecdt.h \
    ../PortInclude/CBaseFrame.h \
    ../PortInclude/CBufferPool.h \
    ../PortInclude/CDecodeBuffer.h \
    ../PortInclude/CDecodeBuffer_Hl.h \
    ../PortInclude/CMyFunction.h \
    ../PortInclude/typedef.h \
    ../PortInclude/CGI_CLINKI.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    TARGET  =       DLT_645_97_d
    DESTDIR =       $$PWD/../../cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/win32/obj/moc
    RCC_DIR =       $$PWD/Debug/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/win32/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       DLT_645_97
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
    TARGET  =       DLT_645_97_d
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/unix/obj/moc
    RCC_DIR =       $$PWD/Debug/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/unix/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       DLT_645_97
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Release/unix/obj/moc
    RCC_DIR =       $$PWD/Release/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/unix/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
