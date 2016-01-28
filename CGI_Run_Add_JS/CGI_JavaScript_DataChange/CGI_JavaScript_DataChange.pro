#-------------------------------------------------
#
# Project created by QtCreator 2015-12-04T13:43:27
#
#-------------------------------------------------

QT       += core script xml

QT       -= gui

#TARGET = CGI_JavaScript
TEMPLATE = lib

DEFINES += CGI_JAVASCRIPT_LIBRARY

SOURCES += \
    CJavaScript_Protocol.cpp \
    CJavaScript_PointBase.cpp \
    ../PortInclude/cvaluebase.cpp \
    CJavaScript_DataChange_Point.cpp

HEADERS += CJavaScript_Protocol.h \
    ../PortInclude/CJavaScript_ProtocolI.h \
    ../PortInclude/CPointI.h \
    CJavaScript_PointBase.h \
    ../PortInclude/CRTDBI.h \
    ../PortInclude/CTagI.h \
    ../PortInclude/cvaluebase.h \
    ../PortInclude/CValueI.h \
    CJavaScript_DataChange_Point.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
win32 {
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    TARGET  =       JavaScriptProtocol_DataChange_d
    DESTDIR =       $$PWD/../../cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/win32/obj/moc
    RCC_DIR =       $$PWD/Debug/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/win32/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       JavaScriptProtocol_DataChange
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
    TARGET  =       JavaScriptProtocol_DataChange_d
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/unix/obj/moc
    RCC_DIR =       $$PWD/Debug/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/unix/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       JavaScriptProtocol_DataChange
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Release/unix/obj/moc
    RCC_DIR =       $$PWD/Release/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/unix/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
