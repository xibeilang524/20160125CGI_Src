#-------------------------------------------------
#
# Project created by QtCreator 2015-10-21T13:50:40
#
#-------------------------------------------------

QT       += core gui xml sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#TARGET = CGI_Monitor
TEMPLATE = app

####################Debug版本和Release版本 start ##################
CONFIG +=debug_and_release
CONFIG(debug, debug|release){
# YUNXING
#TARGET = Run_CGI_d_YUNXING

TARGET = CGI_Monitor_d

DESTDIR = $$PWD/../../cgi-bin

MOC_DIR = $$PWD/Debug/obj/moc
RCC_DIR = $$PWD/Debug/obj/rcc
OBJECTS_DIR = $$PWD/Debug/obj/temp

DEFINES += TIAOSHI
}else{

# YUNXING
#TARGET = Run_CGI_YUNXING

TARGET = CGI_Monitor

DESTDIR = $$PWD/../../cgi-bin

MOC_DIR = $$PWD/Release/obj/moc
RCC_DIR = $$PWD/Release/obj/rcc
OBJECTS_DIR = $$PWD/Release/obj/temp

}
####################Debug版本和Release版本 end ##################


SOURCES += main.cpp\
        mainwindow.cpp \
    CStart.cpp \
    CInitRTDB.cpp \
    ../PortInclude/XmlParser/CXmlParser.cpp \
    Src/cmyfunction.cpp \
    Src/crtdbbase.cpp \
    Src/ctagbase.cpp \
    ../PortInclude/cvaluebase.cpp \
    CStartWindow.cpp \
    CChannelMessage.cpp \
    CTreeWidgetItem.cpp \
    CDeviceModelMessage.cpp \
    CProtocolFileMessage.cpp \
    CChannelThread_CGI.cpp \
    Src/CPRTMapBase.cpp \
    Src/CPRTVBase.cpp \
    cmytextbrowser.cpp \
    CMyTextBrowserSocket.cpp \
    CSignFrameDialog.cpp

HEADERS  += mainwindow.h \
    CStart.h \
    CInitRTDB.h \
    ../PortInclude/XmlParser/CXmlParser.h \
    ../PortInclude/CRTDBI.h \
    ../PortInclude/CTagI.h \
    Src/cmyfunction.h \
    Src/crtdbbase.h \
    Src/ctagbase.h \
    ../PortInclude/cvaluebase.h \
    ../PortInclude/CValueI.h \
    CStartWindow.h \
    CChannelMessage.h \
    CTreeWidgetItem.h \
    CDeviceModelMessage.h \
    CProtocolFileMessage.h \
    EnumClass.h \
    CChannelThread_CGI.h \
    Src/CPRTMapBase.h \
    Src/CPRTMapI.h \
    Src/CPRTVBase.h \
    cmytextbrowser.h \
    CMyTextBrowserSocket.h \
    CSignFrameDialog.h

FORMS    += mainwindow.ui \
    CStartWindow.ui \
    CSignFrameDialog.ui

RESOURCES += \
    image.qrc

OTHER_FILES +=
