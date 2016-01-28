#include "UI_Property_information.h"
#include "ui_UI_Property_information.h"

UI_Property_information::UI_Property_information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UI_Property_information)
{
    ui->setupUi(this);
}

UI_Property_information::~UI_Property_information()
{
    delete ui;
}
