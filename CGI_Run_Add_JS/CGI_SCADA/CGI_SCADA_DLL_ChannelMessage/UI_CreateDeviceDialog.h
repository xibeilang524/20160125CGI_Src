#ifndef UI_CREATEDEVICEDIALOG_H
#define UI_CREATEDEVICEDIALOG_H

#include <QDialog>
#include "CGI_SCADA_DLL_define.h"

namespace Ui {
class UI_CreateDeviceDialog;
}

class UI_CreateDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UI_CreateDeviceDialog(QWidget *parent = 0);
    ~UI_CreateDeviceDialog();
    Device GetNewCreateDevice();
    QString GetDeviceName();



private slots:
    void on_comboBox_Factory_currentIndexChanged(const QString &arg1);

    void on_comboBox_Device_currentIndexChanged(const QString &arg1);

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::UI_CreateDeviceDialog *ui;

    QMap<QString,QList<Device> >ManufacturerMap;///< 厂家、设备map
    Device m_CurrentDevice;
};

#endif // UI_CREATEDEVICEDIALOG_H
