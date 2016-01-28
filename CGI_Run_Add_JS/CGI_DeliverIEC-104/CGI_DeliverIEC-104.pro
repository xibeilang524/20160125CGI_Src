#-------------------------------------------------
#
# Project created by QtCreator 2014-11-19T15:53:17
#
#-------------------------------------------------
QT += core serialport xml network
QT       -= gui
TEMPLATE = lib


DEFINES += MODBUSRTU_LIBRARY

SOURCES += \
    ../PortInclude/cvaluebase.cpp \
    cpointiec_104.cpp \
    IEC_104.cpp \
    cprotocoldeliveriec_104.cpp \
    ciec104deliverquery.cpp \
    ../PortInclude/cprotocolbase.cpp \
    ../PortInclude/cpointbase.cpp \
    ../PortInclude/CFrameSharedMemory.cpp \
    CIEC104DeliverQuery_new.cpp \
    CIEC104BaseFrame.cpp \
    CGI_D_IEC_104_PointNode.cpp \
    CGI_D_IEC_104_PointBase.cpp \
    CGI_D_IEC_104_PointYX.cpp \
    CGI_D_IEC_104_PointYC.cpp \
    CGI_D_IEC_104_PointYM.cpp \
    CGI_D_IEC_104_PointYS.cpp \
    CGI_D_IEC_104_PointYK.cpp \
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
    ../PortInclude/typedef.h \
    ../PortInclude/cvaluebase.h \
    ../PortInclude/cprotocolbase.h \
    ../PortInclude/cprotocoli.h \
    ../PortInclude/cpointbase.h \
    cprotocoldeliveriec_104.h \
    ciec104deliverquery.h \
    cpointiec_104.h \
    IEC_104.h \
    ../PortInclude/CFrameSharedMemory.h \
    ../PortInclude/CGI_CLINKI.h \
    CIEC104DeliverQuery_new.h \
    CIEC104BaseFrame.h \
    CGI_D_IEC_104_PointNode.h \
    CGI_D_IEC_104_PointBase.h \
    CGI_D_IEC_104_PointYX.h \
    CGI_D_IEC_104_PointYC.h \
    CGI_D_IEC_104_PointYM.h \
    CGI_D_IEC_104_PointYS.h \
    CGI_D_IEC_104_PointYK.h \
    ../PortInclude/CBaseFrame.h \
    ../PortInclude/CBufferPool.h \
    ../PortInclude/CDecodeBuffer.h \
    ../PortInclude/CDecodeBuffer_Hl.h \
    ../PortInclude/CMyFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    TARGET  =       DeliverIEC_104_d
    DESTDIR =       $$PWD/../../cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/win32/obj/moc
    RCC_DIR =       $$PWD/Debug/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/win32/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       DeliverIEC_104
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
    TARGET  =       DeliverIEC_104_d
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/unix/obj/moc
    RCC_DIR =       $$PWD/Debug/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/unix/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       DeliverIEC_104
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Release/unix/obj/moc
    RCC_DIR =       $$PWD/Release/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/unix/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
