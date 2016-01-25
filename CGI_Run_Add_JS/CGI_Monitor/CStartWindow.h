#ifndef CSTARTWINDOW_H
#define CSTARTWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class CStartWindow;
}

class CStartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CStartWindow(QWidget *parent = 0);
    ~CStartWindow();

private slots:
    void on_action_triggered();

private:
    Ui::CStartWindow *ui;

    MainWindow *m_pMainWindow;
};

#endif // CSTARTWINDOW_H
