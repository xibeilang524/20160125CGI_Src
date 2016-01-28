#ifndef UI_OPERATING_STATUS_H
#define UI_OPERATING_STATUS_H

#include <QWidget>

namespace Ui {
class UI_operating_status;
}

class UI_operating_status : public QWidget
{
    Q_OBJECT

public:
    explicit UI_operating_status(QWidget *parent = 0);
    ~UI_operating_status();

private:
    Ui::UI_operating_status *ui;
};

#endif // UI_OPERATING_STATUS_H
