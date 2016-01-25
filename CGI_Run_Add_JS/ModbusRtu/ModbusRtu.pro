#-------------------------------------------------
#
# Project created by QtCreator 2014-11-19T15:53:17
#
# ModbusRtu采集驱动
#-------------------------------------------------
QT += core serialport xml network
QT       -= gui
TEMPLATE = lib


DEFINES += MODBUSRTU_LIBRARY

SOURCES += \
    cmodbusrtuprotocol.cpp \
    cpointbasemodbusrtu.cpp \
    ../PortInclude/cvaluebase.cpp \
    ../PortInclude/cprotocolbase.cpp \
    ../PortInclude/cpointbase.cpp \
    ../PortInclude/CFrameSharedMemory.cpp \
    ../PortInclude/CBaseFrame.cpp \
    ../PortInclude/CBufferPool.cpp \
    ../PortInclude/CDecodeBuffer.cpp \
    ../PortInclude/CDecodeBuffer_Hl.cpp \
    ../PortInclude/CMyFunction.cpp

HEADERS += \
    cmodbusrtuprotocol.h \
    cpointbasemodbusrtu.h \
    ../PortInclude/CTagI.h \
    ../PortInclude/CBinaryI.h \
    ../PortInclude/CPointI.h \
    ../PortInclude/CQualityI.h \
    ../PortInclude/CRTDBI.h \
    ../PortInclude/CValueI.h \
    ../PortInclude/cvaluebase.h \
    ../PortInclude/cprotocoli.h \
    ../PortInclude/cprotocolbase.h \
    ../PortInclude/cpointbase.h \
    ../PortInclude/typedef.h \
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

OTHER_FILES += \
    ../PortInclude/CANInfo.txt
win32 {
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    TARGET  =       ModbusRtu_d
    DESTDIR =       $$PWD/../../cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/win32/obj/moc
    RCC_DIR =       $$PWD/Debug/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/win32/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       ModbusRtu
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
    TARGET  =       ModbusRtu_d
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/unix/obj/moc
    RCC_DIR =       $$PWD/Debug/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/unix/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       ModbusRtu
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Release/unix/obj/moc
    RCC_DIR =       $$PWD/Release/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/unix/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
