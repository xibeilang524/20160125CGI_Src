#include "UI_Current_users.h"
#include "ui_UI_Current_users.h"

UI_Current_users::UI_Current_users(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UI_Current_users)
{
    ui->setupUi(this);
}

UI_Current_users::~UI_Current_users()
{
    delete ui;
}
