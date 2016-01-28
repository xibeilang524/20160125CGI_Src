#ifndef UI_SOFTWARE_VERSION_H
#define UI_SOFTWARE_VERSION_H

#include <QWidget>

namespace Ui {
class UI_Software_version;
}

class UI_Software_version : public QWidget
{
    Q_OBJECT

public:
    explicit UI_Software_version(QWidget *parent = 0);
    ~UI_Software_version();

private:
    Ui::UI_Software_version *ui;
};

#endif // UI_SOFTWARE_VERSION_H
