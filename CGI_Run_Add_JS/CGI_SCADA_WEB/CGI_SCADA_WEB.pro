#-------------------------------------------------
#
# Project created by QtCreator 2016-01-04T13:10:20
#
#-------------------------------------------------

QT       += core gui xml webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CGI_SCADA
TEMPLATE = app


SOURCES += main.cpp\
        CMainWindow.cpp \
    CDownLoadMainWindow.cpp

HEADERS  += CMainWindow.h \
    CDownLoadMainWindow.h

FORMS    += CMainWindow.ui \
    CDownLoadMainWindow.ui
