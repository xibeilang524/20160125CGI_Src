#-------------------------------------------------
#
# Project created by QtCreator 2014-11-19T15:53:17
# CDT采集驱动，实现了采集遥信、遥测、总加遥测、遥脉
#-------------------------------------------------
QT += core serialport xml network
QT       -= gui
TEMPLATE = lib
DEFINES += MODBUSRTU_LIBRARY

SOURCES += \
    ../PortInclude/cvaluebase.cpp \
    ../PortInclude/cprotocolbase.cpp \
    ../PortInclude/cpointbase.cpp \
    ../PortInclude/CFrameSharedMemory.cpp \
    ../PortInclude/CBufferPool.cpp \
    ../PortInclude/CMyFunction.cpp \
    ../PortInclude/CDecodeBuffer.cpp \
    ../PortInclude/CDecodeBuffer_Hl.cpp \
    ../PortInclude/CBaseFrame.cpp \
    CProtocolCDT.cpp \
    ../PortInclude/XmlParser/CXmlParser.cpp \
    CBaseFrameCDT.cpp \
    CCDTQuery.cpp \
    CCDTResponse.cpp \
    CPointCDT.cpp \
    CPointNodeCDT.cpp

HEADERS += \
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
    ../PortInclude/CBufferPool.h \
    ../PortInclude/CMyFunction.h \
    ../PortInclude/CDecodeBuffer.h \
    ../PortInclude/CDecodeBuffer_Hl.h \
    ../PortInclude/CBaseFrame.h \
    CProtocolCDT.h \
    ../PortInclude/XmlParser/CXmlParser.h \
    CBaseFrameCDT.h \
    CCDTQuery.h \
    CCDTResponse.h \
    CDT.h \
    CPointCDT.h \
    CPointNodeCDT.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES +=

win32 {
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    TARGET  =       CDT_d
    DESTDIR =       $$PWD/../../cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/win32/obj/moc
    RCC_DIR =       $$PWD/Debug/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/win32/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       CDT
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
    TARGET  =       CDT_d
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/unix/obj/moc
    RCC_DIR =       $$PWD/Debug/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/unix/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       CDT
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Release/unix/obj/moc
    RCC_DIR =       $$PWD/Release/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/unix/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
