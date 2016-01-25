#include "UI_Hardware_model.h"
#include "ui_UI_Hardware_model.h"

UI_Hardware_model::UI_Hardware_model(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UI_Hardware_model)
{
    ui->setupUi(this);
}

UI_Hardware_model::~UI_Hardware_model()
{
    delete ui;
}
