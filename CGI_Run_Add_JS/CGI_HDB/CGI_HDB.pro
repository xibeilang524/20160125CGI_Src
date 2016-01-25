#-------------------------------------------------
#
# Project created by QtCreator 2015-10-31T10:31:02
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

TARGET = history.cgi
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

SOURCES += main.cpp \
    CHDBRead.cpp

HEADERS += \
    CHDBRead.h

win32 {
    ####################Debug版本和Release版本 start ##################
    CONFIG +=debug_and_release
    CONFIG(debug, debug|release){
    # YUNXING
    TARGET  =       history.cgi_d
    DESTDIR =       $$PWD/../../cgi-bin
    MOC_DIR =       $$PWD/Debug/win32/obj/moc
    RCC_DIR =       $$PWD/Debug/win32/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/win32/obj/temp
    DEFINES += TIAOSHI
    }else{
    # YUNXING
    TARGET  =       history.cgi
    DESTDIR =       $$PWD/../../cgi-bin
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
    # YUNXING
    TARGET  =       history.cgi_d
    DESTDIR =       $$PWD/../../unix/cgi-bin
    MOC_DIR =       $$PWD/Debug/unix/obj/moc
    RCC_DIR =       $$PWD/Debug/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Debug/unix/obj/temp
    DEFINES += TIAOSHI
    }else{
    # YUNXING
    TARGET  =       history.cgi
    DESTDIR =       $$PWD/../../unix/cgi-bin
    MOC_DIR =       $$PWD/Release/unix/obj/moc
    RCC_DIR =       $$PWD/Release/unix/obj/rcc
    OBJECTS_DIR =   $$PWD/Release/unix/obj/temp
    }
    ####################Debug版本和Release版本 end ##################
}
