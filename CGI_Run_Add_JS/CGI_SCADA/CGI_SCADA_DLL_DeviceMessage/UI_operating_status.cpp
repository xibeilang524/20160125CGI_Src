#include "UI_operating_status.h"
#include "ui_UI_operating_status.h"
#include <QDebug>

UI_operating_status::UI_operating_status(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UI_operating_status)
{
    ui->setupUi(this);
}

UI_operating_status::~UI_operating_status()
{
    qDebug()<<__func__;
    delete ui;
}
