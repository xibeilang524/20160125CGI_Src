#ifndef UI_EQUIPMENT_TIME_H
#define UI_EQUIPMENT_TIME_H

#include <QWidget>

namespace Ui {
class UI_Equipment_time;
}

class UI_Equipment_time : public QWidget
{
    Q_OBJECT

public:
    explicit UI_Equipment_time(QWidget *parent = 0);
    ~UI_Equipment_time();

private:
    Ui::UI_Equipment_time *ui;
};

#endif // UI_EQUIPMENT_TIME_H
