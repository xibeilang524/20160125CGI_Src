#ifndef UI_UPLOADDIALOG_H
#define UI_UPLOADDIALOG_H

#include <QDialog>
#include <QWebView>

namespace Ui {
class UI_UpLoadDialog;
}

class UI_UpLoadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UI_UpLoadDialog(QWidget *parent = 0);
    ~UI_UpLoadDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::UI_UpLoadDialog *ui;
    QWebView *m_pWebView;
};

#endif // UI_UPLOADDIALOG_H
