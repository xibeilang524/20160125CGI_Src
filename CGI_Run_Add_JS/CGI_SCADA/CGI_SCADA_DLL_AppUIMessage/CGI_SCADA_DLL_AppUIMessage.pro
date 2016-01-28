#-------------------------------------------------
#
# Project created by QtCreator 2016-01-04T15:16:47
#
#-------------------------------------------------
# APP UI节点


QT       += xml gui core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib

DEFINES += CGI_SCADA_DLL_LIBRARY

SOURCES += \
    ../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.cpp \
    CGI_SCADA_DLL_TreeItemAppUI.cpp \
    ../CGI_SCADA_include/CGI_SCADA_define.cpp

HEADERS += ../CGI_SCADA_include/CGI_SCADA_DLL_I.h \
    ../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h \
    CGI_SCADA_DLL_TreeItemAppUI.h \
    ../CGI_SCADA_include/CGI_SCADA_define.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG +=debug_and_release
CONFIG(debug, debug|release){
# YUNXING
TARGET  =       CGI_SCADA_DLL_AppUIMessage_d
DESTDIR =       $$PWD/../bin_SCADA
MOC_DIR =       $$PWD/Debug/obj/moc
RCC_DIR =       $$PWD/Debug/obj/rcc
OBJECTS_DIR =   $$PWD/Debug/obj/temp
DEFINES += TIAOSHI
}else{
# YUNXING
TARGET  =       CGI_SCADA_DLL_AppUIMessage
DESTDIR =       $$PWD/../bin_SCADA
MOC_DIR =       $$PWD/Release/obj/moc
RCC_DIR =       $$PWD/Release/obj/rcc
OBJECTS_DIR =   $$PWD/Release/obj/temp
}
