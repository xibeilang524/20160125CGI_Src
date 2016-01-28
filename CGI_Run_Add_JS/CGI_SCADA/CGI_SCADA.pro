#-------------------------------------------------
#
# Project created by QtCreator 2015-10-27T19:58:02
#
#-------------------------------------------------

TEMPLATE = subdirs

SUBDIRS += CGI_SCADA_Main \             # 组态软件主程序
    CGI_SCADA_DLL_AboutMessage \        # 关于节点
    CGI_SCADA_DLL_AlarmMessage \        # 报警服务节点
    CGI_SCADA_DLL_AppUIMessage \        # APP UI节点
    CGI_SCADA_DLL_ChannelMessage \      # 通道信息节点
    CGI_SCADA_DLL_ControlMessage \      # 控制逻辑节点
    CGI_SCADA_DLL_DeviceMessage \       # 设备信息节点
    CGI_SCADA_DLL_HistoryMessage \      # 历史数据节点
    CGI_SCADA_DLL_UserManageMessage \   # 用户管理节点
    CGI_SCADA_DLL_WebUIMessage          # WEB UI节点




