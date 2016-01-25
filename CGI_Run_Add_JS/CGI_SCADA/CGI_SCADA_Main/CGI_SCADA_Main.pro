#-------------------------------------------------
#
# Project created by QtCreator 2016-01-04T14:55:21
#
#-------------------------------------------------
# 组态软件主程序


QT       += core gui testlib xml webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app



SOURCES += main.cpp\
        CMainWindow.cpp \
    CTreeItemThread.cpp \
    ../CGI_SCADA_include/XmlParser/CXmlParser.cpp \
    UI_UpLoadDialog.cpp \
    UI_DownloadDialog.cpp \
    UI_Login_Dialog.cpp \
    CNetworkCookie.cpp \
    CDockWidgetFoot.cpp \
    CTextBrowserDebug.cpp \
    ../CGI_SCADA_include/CGI_SCADA_define.cpp

HEADERS  += CMainWindow.h \
    CTreeItemThread.h \
    ../CGI_SCADA_include/XmlParser/CXmlParser.h \
    UI_UpLoadDialog.h \
    UI_DownloadDialog.h \
    UI_Login_Dialog.h \
    CNetworkCookie.h \
    CDockWidgetFoot.h \
    CTextBrowserDebug.h \
    ../CGI_SCADA_include/CGI_SCADA_define.h

FORMS    += CMainWindow.ui \
    UI_UpLoadDialog.ui \
    UI_DownloadDialog.ui \
    UI_Login_Dialog.ui


CONFIG +=debug_and_release
CONFIG(debug, debug|release){
# YUNXING
TARGET  =       CGI_SCADA_d
DESTDIR =       $$PWD/../bin_SCADA
MOC_DIR =       $$PWD/Debug/obj/moc
RCC_DIR =       $$PWD/Debug/obj/rcc
OBJECTS_DIR =   $$PWD/Debug/obj/temp
DEFINES += TIAOSHI
}else{
# YUNXING
TARGET  =       CGI_SCADA
DESTDIR =       $$PWD/../bin_SCADA
MOC_DIR =       $$PWD/Release/obj/moc
RCC_DIR =       $$PWD/Release/obj/rcc
OBJECTS_DIR =   $$PWD/Release/obj/temp
}
INCLUDEPATH +=  $$PWD/../CGI_SCADA_include
QMAKE_CXXFLAGS += -Wno-unused-parameter
