#include "CMainWindow.h"
#include <QApplication>
#include "UI_Login_Dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UI_Login_Dialog login;
    login.exec();
    CMainWindow w;
    if (login.GetLoginStatus())
    {
        w.Init();
        w.SetTreeItemDllNameList(login.GetTreeItemDllName());
        w.show();
    }else
    {
        return 0;
    }

    return a.exec();
}
