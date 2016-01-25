#include "UI_Software_version.h"
#include "ui_UI_Software_version.h"

UI_Software_version::UI_Software_version(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UI_Software_version)
{
    ui->setupUi(this);
}

UI_Software_version::~UI_Software_version()
{
    delete ui;
}
