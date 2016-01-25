#include "UI_AboutMessage.h"
#include "ui_UI_AboutMessage.h"

UI_AboutMessage::UI_AboutMessage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UI_AboutMessage)
{
    ui->setupUi(this);
}

UI_AboutMessage::~UI_AboutMessage()
{
    delete ui;
}
