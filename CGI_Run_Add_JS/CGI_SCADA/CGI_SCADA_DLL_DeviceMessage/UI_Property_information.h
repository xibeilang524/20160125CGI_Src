#ifndef UI_PROPERTY_INFORMATION_H
#define UI_PROPERTY_INFORMATION_H

#include <QWidget>

namespace Ui {
class UI_Property_information;
}

class UI_Property_information : public QWidget
{
    Q_OBJECT

public:
    explicit UI_Property_information(QWidget *parent = 0);
    ~UI_Property_information();

private:
    Ui::UI_Property_information *ui;
};

#endif // UI_PROPERTY_INFORMATION_H
