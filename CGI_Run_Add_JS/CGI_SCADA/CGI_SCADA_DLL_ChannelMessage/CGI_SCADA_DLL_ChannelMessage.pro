#-------------------------------------------------
#
# Project created by QtCreator 2016-01-04T15:16:47
#
#-------------------------------------------------
# 通道信息节点


QT       += xml gui core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -Wno-unused-parameter
TEMPLATE = lib

DEFINES += CGI_SCADA_DLL_LIBRARY

SOURCES += \
    CGI_SCADA_DLL_ChannelMessageItem.cpp \
    ../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.cpp \
    CGI_SCADA_DLL_TreeItemCOM.cpp \
    ../CGI_SCADA_include/XmlParser/CXmlParser.cpp \
    UI_CreateDeviceDialog.cpp \
    CGI_SCADA_DLL_TreeItemDevice.cpp \
    UI_DeviceAttribute.cpp \
    CGI_SCADA_DLL_DeviceTableView.cpp \
    CGI_SCADA_DLL_TreeItemCAN.cpp \
    UI_COMCAN_Attribute.cpp \
    CGI_SCADA_DLL_TreeItemNET.cpp \
    UI_NetAttribute.cpp \
    CGI_SCADA_DLL_TableView.cpp \
    CGI_SCADA_DLL_TreeItemNetLink.cpp \
    CGI_SCADA_DeviceConfig.cpp \
    UI_LinkNameDialog.cpp \
    ../CGI_SCADA_include/CBatchChangeDialogDialog.cpp \
    ../CGI_SCADA_include/CReplaceDialog.cpp \
    ../CGI_SCADA_include/ItemDelegate/CItemDelegateAll.cpp \
    ../CGI_SCADA_include/ItemDelegate/CItemDelegateCombobox.cpp \
    ../CGI_SCADA_include/ItemDelegate/CItemDelegateEdit.cpp \
    ../CGI_SCADA_include/ItemDelegate/CItemDelegateLabel.cpp \
    ../CGI_SCADA_include/ItemDelegate/CItemDelegateReadOnly.cpp \
    ../CGI_SCADA_include/CGI_SCADA_define.cpp \
    CGI_SCADA_DLL_CommandThread.cpp

HEADERS += ../CGI_SCADA_include/CGI_SCADA_DLL_I.h \
    CGI_SCADA_DLL_ChannelMessageItem.h \
    ../CGI_SCADA_include/CGI_SCADA_DLL_TreeItemBase.h \
    CGI_SCADA_DLL_TreeItemCOM.h \
    CGI_SCADA_DLL_define.h \
    ../CGI_SCADA_include/XmlParser/CXmlParser.h \
    UI_CreateDeviceDialog.h \
    CGI_SCADA_DLL_TreeItemDevice.h \
    UI_DeviceAttribute.h \
    CGI_SCADA_DLL_DeviceTableView.h \
    CGI_SCADA_DLL_TreeItemCAN.h \
    UI_COMCAN_Attribute.h \
    CGI_SCADA_DLL_TreeItemNET.h \
    UI_NetAttribute.h \
    CGI_SCADA_DLL_TableView.h \
    CGI_SCADA_DLL_TreeItemNetLink.h \
    CGI_SCADA_DeviceConfig.h \
    UI_LinkNameDialog.h \
    ../CGI_SCADA_include/CBatchChangeDialogDialog.h \
    ../CGI_SCADA_include/CReplaceDialog.h \
    ../CGI_SCADA_include/ItemDelegate/CItemDelegateAll.h \
    ../CGI_SCADA_include/ItemDelegate/CItemDelegateCombobox.h \
    ../CGI_SCADA_include/ItemDelegate/CItemDelegateEdit.h \
    ../CGI_SCADA_include/ItemDelegate/CItemDelegateLabel.h \
    ../CGI_SCADA_include/ItemDelegate/CItemDelegateReadOnly.h \
    ../CGI_SCADA_include/CGI_SCADA_define.h \
    CGI_SCADA_DLL_CommandThread.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG +=debug_and_release
CONFIG(debug, debug|release){
# YUNXING
TARGET  =       CGI_SCADA_DLL_ChannelMessage_d
DESTDIR =       $$PWD/../bin_SCADA
MOC_DIR =       $$PWD/Debug/obj/moc
RCC_DIR =       $$PWD/Debug/obj/rcc
OBJECTS_DIR =   $$PWD/Debug/obj/temp
DEFINES += TIAOSHI
}else{
# YUNXING
TARGET  =       CGI_SCADA_DLL_ChannelMessage
DESTDIR =       $$PWD/../bin_SCADA
MOC_DIR =       $$PWD/Release/obj/moc
RCC_DIR =       $$PWD/Release/obj/rcc
OBJECTS_DIR =   $$PWD/Release/obj/temp
}

FORMS += \
    UI_COM_Attribute.ui \
    UI_CreateDeviceDialog.ui \
    UI_DeviceAttribute.ui \
    UI_NetAttribute.ui \
    UI_LinkNameDialog.ui \
    ../CGI_SCADA_include/CBatchChangeDialogDialog.ui \
    ../CGI_SCADA_include/CReplaceDialog.ui

INCLUDEPATH +=  $$PWD/../CGI_SCADA_include \
                $$PWD/../CGI_SCADA_include/ItemDelegate \
                $$PWD/../CGI_SCADA_include/qtpropertybrowser

DEPENDPATH += $$PWD/../CGI_SCADA_include/qtpropertybrowser


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin_SCADA/ -lQtSolutions_PropertyBrowser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin_SCADA/ -lQtSolutions_PropertyBrowserd
else:unix: LIBS += -L$$PWD/../bin_SCADA/ -lQtSolutions_PropertyBrowser
