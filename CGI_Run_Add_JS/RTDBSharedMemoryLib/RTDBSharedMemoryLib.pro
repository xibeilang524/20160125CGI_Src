#-------------------------------------------------
#
# Project created by QtCreator 2015-07-03T14:26:43
#
# RTDBSharedMemoryLib 是实时值共享驱动
#-------------------------------------------------

QT       -= gui
QT += xml
TEMPLATE = lib

DEFINES += RTDBSHAREDMEMORYLIB_LIBRARY

SOURCES += \
    CRTDBSharedMemoryLibBase.cpp \
    ../PortInclude/XmlParser/CXmlParser.cpp

HEADERS += \
    CRTDBSharedMemoryLibBase.h \
    ../PortInclude/CRTDBSharedMemoryLibI.h \
    ../PortInclude/CSharedMemoryPoint.h \
    ../PortInclude/XmlParser/CXmlParser.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    TARGET  =       RTDBSharedMemoryLib_d
    DESTDIR =       $$PWD/../../cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/win32/obj/moc
    RCC_DIR =       $$PWD/Debug/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/win32/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       RTDBSharedMemoryLib
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
    TARGET  =       RTDBSharedMemoryLib_d
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Debug/unix/obj/moc
    RCC_DIR =       $$PWD/Debug/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/unix/obj/temp
    DEFINES += TIAOSHI
    }else{
    TARGET  =       RTDBSharedMemoryLib
    DESTDIR =       $$PWD/../../unix/cgi-bin/Protocol
    MOC_DIR =       $$PWD/Release/unix/obj/moc
    RCC_DIR =       $$PWD/Release/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/unix/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
