#include "CGI_SCADA_DLL_TreeItemAppUI.h"


/*!
 \brief 库文件导出接口函数

 \fn CreateDriver
 \param parent QObject对象指针
 \return CGI_SCADA_DLL_I 返回创建的驱动指针
*/
CGI_SCADA_DLL_I * CreateDriver(QObject *parent)
{
    QStringList strings;
    strings << "App UI";
    qDebug()<<__func__<<strings;
    CGI_SCADA_DLL_TreeItemAppUI *testProtocol = new CGI_SCADA_DLL_TreeItemAppUI(strings,CGI_SCADA_DLL_I::TreeItemType_App_UIM,parent);
    qDebug()<<testProtocol->text(0);
    return  testProtocol;
}
CGI_SCADA_DLL_TreeItemAppUI::CGI_SCADA_DLL_TreeItemAppUI(const QStringList &strings, int type, QObject *parent):
    CGI_SCADA_DLL_TreeItemBase(strings,type,parent)
{
//    QString str = QString("aaaaaaaaaaaa cha%1").arg(QString::number(type));
//    m_pLabel = new QLabel(str);
//    CGI_SCADA_DeviceConfig::InitManufactureMap();
}
