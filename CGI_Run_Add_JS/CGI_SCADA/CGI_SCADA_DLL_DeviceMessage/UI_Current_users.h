#ifndef UI_CURRENT_USERS_H
#define UI_CURRENT_USERS_H

#include <QWidget>

namespace Ui {
class UI_Current_users;
}

class UI_Current_users : public QWidget
{
    Q_OBJECT

public:
    explicit UI_Current_users(QWidget *parent = 0);
    ~UI_Current_users();

private:
    Ui::UI_Current_users *ui;
};

#endif // UI_CURRENT_USERS_H
