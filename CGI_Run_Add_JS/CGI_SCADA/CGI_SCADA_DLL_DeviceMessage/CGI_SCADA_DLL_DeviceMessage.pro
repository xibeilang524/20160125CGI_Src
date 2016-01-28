#-------------------------------------------------
#
# Project created by QtCreator 2016-01-04T15:16:47
#
#-------------------------------------------------
# 设备信息节点


QT       += xml gui core webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib

DEFINES += CGI_SCADA_DLL_LIBRARY

SOURCES += \
    CGI_SCADA_DLL_DeviceMessageItem.cpp \
    UI_operating_status.cpp \
    ../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.cpp \
    CGI_SCADA_DLL_TreeItemOperating_status.cpp \
    CGI_SCADA_DLL_TreeItemHardware_model.cpp \
    UI_Hardware_model.cpp \
    CGI_SCADA_DLL_TreeItemSoftware_version.cpp \
    UI_Software_version.cpp \
    CGI_SCADA_DLL_TreeItemCurrent_users.cpp \
    UI_Current_users.cpp \
    CGI_SCADA_DLL_TreeItemEquipment_time.cpp \
    UI_Equipment_time.cpp \
    CGI_SCADA_DLL_TreeItemCurrent_project.cpp \
    UI_Current_project.cpp \
    UI_Property_information.cpp \
    CGI_SCADA_DLL_TreeItemProperty_information.cpp \
    ../CGI_SCADA_include/CGI_SCADA_define.cpp \
    CGI_SCADA_DLL_TreeItemChildItem.cpp

HEADERS += ../CGI_SCADA_include/CGI_SCADA_DLL_I.h \
    CGI_SCADA_DLL_DeviceMessageItem.h \
    UI_operating_status.h \
    ../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h \
    CGI_SCADA_DLL_TreeItemOperating_status.h \
    CGI_SCADA_DLL_TreeItemHardware_model.h \
    UI_Hardware_model.h \
    CGI_SCADA_DLL_TreeItemSoftware_version.h \
    UI_Software_version.h \
    CGI_SCADA_DLL_TreeItemCurrent_users.h \
    UI_Current_users.h \
    CGI_SCADA_DLL_TreeItemEquipment_time.h \
    UI_Equipment_time.h \
    CGI_SCADA_DLL_TreeItemCurrent_project.h \
    UI_Current_project.h \
    UI_Property_information.h \
    CGI_SCADA_DLL_TreeItemProperty_information.h \
    ../CGI_SCADA_include/CGI_SCADA_define.h \
    CGI_SCADA_DLL_DeviceMessageDefine.h \
    CGI_SCADA_DLL_TreeItemChildItem.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG +=debug_and_release
CONFIG(debug, debug|release){
# YUNXING
TARGET  =       CGI_SCADA_DLL_DeviceMessage_d
DESTDIR =       $$PWD/../bin_SCADA
MOC_DIR =       $$PWD/Debug/obj/moc
RCC_DIR =       $$PWD/Debug/obj/rcc
OBJECTS_DIR =   $$PWD/Debug/obj/temp
DEFINES += TIAOSHI
}else{
# YUNXING
TARGET  =       CGI_SCADA_DLL_DeviceMessage
DESTDIR =       $$PWD/../bin_SCADA
MOC_DIR =       $$PWD/Release/obj/moc
RCC_DIR =       $$PWD/Release/obj/rcc
OBJECTS_DIR =   $$PWD/Release/obj/temp
}

FORMS += \
    UI_operating_status.ui \
    UI_Hardware_model.ui \
    UI_Software_version.ui \
    UI_Current_users.ui \
    UI_Equipment_time.ui \
    UI_Current_project.ui \
    UI_Property_information.ui

RESOURCES +=
