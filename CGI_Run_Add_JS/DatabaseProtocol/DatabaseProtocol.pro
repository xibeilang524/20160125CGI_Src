#-------------------------------------------------
#
# Project created by QtCreator 2015-03-16T09:33:14
#
# 数据库驱动，可以设置变化存储和周期存储
#-------------------------------------------------

QT       -= gui
QT       += sql xml
TEMPLATE = lib


DEFINES += DATABASEPROTOCOL_LIBRARY

SOURCES +=\
    DatabaseProtocol/CDatabaseProtocolDatabase.cpp \
    ../PortInclude/XmlParser/CXmlParser.cpp \
    ../PortInclude/cvaluebase.cpp \
    CPointBase.cpp \



HEADERS +=\
    DatabaseProtocol/CDatabaseProtocolDatabase.h \
    ../PortInclude/CBinaryI.h \
    ../PortInclude/CPointI.h \
    ../PortInclude/CQualityI.h \
    ../PortInclude/CRTDBI.h \
    ../PortInclude/CTagI.h \
    ../PortInclude/cvaluebase.h \
    ../PortInclude/CValueI.h \
    ../PortInclude/typedef.h \
    CPointBase.h \
    ../PortInclude/CDatabaseProtocolI.h \
    ../PortInclude/XmlParser/CXmlParser.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    TARGET  =       DatabaseProtocol_d
    DESTDIR =       $$PWD/../../cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/win32/obj/moc
    RCC_DIR =       $$PWD/Debug/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/win32/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       DatabaseProtocol
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
    TARGET  =       DatabaseProtocol_d
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/unix/obj/moc
    RCC_DIR =       $$PWD/Debug/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/unix/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       DatabaseProtocol
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Release/unix/obj/moc
    RCC_DIR =       $$PWD/Release/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/unix/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
