#include "UI_Equipment_time.h"
#include "ui_UI_Equipment_time.h"

UI_Equipment_time::UI_Equipment_time(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UI_Equipment_time)
{
    ui->setupUi(this);
}

UI_Equipment_time::~UI_Equipment_time()
{
    delete ui;
}
