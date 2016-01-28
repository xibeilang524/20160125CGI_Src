#ifndef UI_HARDWARE_MODEL_H
#define UI_HARDWARE_MODEL_H

#include <QWidget>

namespace Ui {
class UI_Hardware_model;
}

class UI_Hardware_model : public QWidget
{
    Q_OBJECT

public:
    explicit UI_Hardware_model(QWidget *parent = 0);
    ~UI_Hardware_model();

private:
    Ui::UI_Hardware_model *ui;
};

#endif // UI_HARDWARE_MODEL_H
