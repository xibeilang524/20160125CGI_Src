#-------------------------------------------------
#
# Project created by QtCreator 2015-10-12T16:16:34
#
#Linux运行主程序
#-------------------------------------------------

QT       += core xml sql testlib script

QT       -= gui
#CONFIG   += console
#CONFIG += console 屏蔽了能够使debug信息打印到QtCreator的下方输出区
CONFIG   -= app_bundle
TEMPLATE = app

DEFINES += YUNXING

SOURCES += main.cpp \
    ../PortInclude/XmlParser/CXmlParser.cpp \
    ../PortInclude/cvaluebase.cpp \
    CStart.cpp \
    Src/cmyfunction.cpp \
    Src/crtdbbase.cpp \
    Src/ctagbase.cpp \
    CChannelThread_CGI.cpp \
    Src/CPRTMapBase.cpp \
    Src/CPRTVBase.cpp \
    CRTDBSharedMemoryThread.cpp \
    CDatabaseThread.cpp \
    CJavaScript_Thread.cpp

HEADERS += \
    CStart.h \
    ../PortInclude/XmlParser/CXmlParser.h \
    ../PortInclude/CPRTI.h \
    ../PortInclude/CPRTMapI.h \
    ../PortInclude/CBinaryI.h \
    ../PortInclude/CPointI.h \
    ../PortInclude/CQualityI.h \
    ../PortInclude/CRTDBI.h \
    ../PortInclude/CTagI.h \
    ../PortInclude/cvaluebase.h \
    ../PortInclude/CValueI.h \
    Src/cmyfunction.h \
    Src/crtdbbase.h \
    Src/ctagbase.h \
    CChannelThread_CGI.h \
    Src/CPRTMapBase.h \
    Src/CPRTVBase.h \
    CRTDBSharedMemoryThread.h \
    CDatabaseThread.h \
    CJavaScript_Thread.h \
    ../PortInclude/defined.h

win32 {
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    # YUNXING
    TARGET  =       run_CGI_d
    DESTDIR =       $$PWD/../../cgi-bin
    MOC_DIR =       $$PWD/Debug/win32/obj/moc
    RCC_DIR =       $$PWD/Debug/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/win32/obj/temp
    DEFINES += TIAOSHI
    }else{
    # YUNXING
    TARGET  =       run_CGI
    DESTDIR =       $$PWD/../../cgi-bin
    MOC_DIR =       $$PWD/Release/win32/obj/moc
    RCC_DIR =       $$PWD/Release/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/win32/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
#DEFINES += QT_NO_DEBUG_OUTPUT
unix {
{
DEFINES += LONGTENGLANTIAN
SOURCES += CMAC.cpp
HEADERS += CMAC.h
}
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    # YUNXING
    TARGET  =       run_CGI_d
    DESTDIR =       $$PWD/../../unix/cgi-bin
    MOC_DIR =       $$PWD/Debug/unix/obj/moc
    RCC_DIR =       $$PWD/Debug/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/unix/obj/temp
    DEFINES += TIAOSHI
    }else{
    # YUNXING
    TARGET  =       run_CGI
    DESTDIR =       $$PWD/../../unix/cgi-bin
    MOC_DIR =       $$PWD/Release/unix/obj/moc
    RCC_DIR =       $$PWD/Release/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/unix/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}


