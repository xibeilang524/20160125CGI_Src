#-------------------------------------------------
#
# Project created by QtCreator 2015-11-24T17:58:39
#
#-------------------------------------------------

QT       += network xml serialport

QT       -= gui

TEMPLATE = lib

DEFINES += CGI_CLINKLIB_LIBRARY

SOURCES += \
    CLink.cpp \
    cserialport.cpp \
    CMyNetWorkBase.cpp \
    CTcpClient.cpp \
    CTcpServer.cpp

HEADERS += \
    ../PortInclude/CGI_CLINKI.h \
    ../PortInclude/typedef.h \
    CLink.h \
    CMyNetWorkI.h \
    cserialport.h \
    CMyNetWorkBase.h \
    CTcpClient.h \
    CTcpServer.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    CANInfo.txt

win32 {
    #HEADERS += CGI_CLINKI.h
    #SOURCES += hellowin.cpp
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    TARGET  =       CGI_CLINKLib_d
    DESTDIR =       $$PWD/../../cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/win32/obj/moc
    RCC_DIR =       $$PWD/Debug/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/win32/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       CGI_CLINKLib
    DESTDIR =       $$PWD/../../cgi-bin/Protocol
    MOC_DIR =       $$PWD/Release/win32/obj/moc
    RCC_DIR =       $$PWD/Release/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/win32/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
#DEFINES += QT_NO_DEBUG_OUTPUT
unix {
    HEADERS += CCANThread.h
    SOURCES += CCANThread.cpp
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    TARGET  =       CGI_CLINKLib_d
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/unix/obj/moc
    RCC_DIR =       $$PWD/Debug/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/unix/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       CGI_CLINKLib
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Release/unix/obj/moc
    RCC_DIR =       $$PWD/Release/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/unix/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
