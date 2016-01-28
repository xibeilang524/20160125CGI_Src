#ifndef UI_ABOUTMESSAGE_H
#define UI_ABOUTMESSAGE_H

#include <QWidget>

namespace Ui {
class UI_AboutMessage;
}

class UI_AboutMessage : public QWidget
{
    Q_OBJECT

public:
    explicit UI_AboutMessage(QWidget *parent = 0);
    ~UI_AboutMessage();

private:
    Ui::UI_AboutMessage *ui;
};

#endif // UI_ABOUTMESSAGE_H
