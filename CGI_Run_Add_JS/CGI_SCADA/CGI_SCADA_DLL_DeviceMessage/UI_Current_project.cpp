#include "UI_Current_project.h"
#include "ui_UI_Current_project.h"

UI_Current_project::UI_Current_project(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UI_Current_project)
{
    ui->setupUi(this);
}

UI_Current_project::~UI_Current_project()
{
    delete ui;
}
