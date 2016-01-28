#ifndef UI_CURRENT_PROJECT_H
#define UI_CURRENT_PROJECT_H

#include <QWidget>

namespace Ui {
class UI_Current_project;
}

class UI_Current_project : public QWidget
{
    Q_OBJECT

public:
    explicit UI_Current_project(QWidget *parent = 0);
    ~UI_Current_project();

private:
    Ui::UI_Current_project *ui;
};

#endif // UI_CURRENT_PROJECT_H
