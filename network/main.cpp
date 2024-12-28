#include "mainwindow.h"
#include <QApplication>
#include "splashscreen.h"
#include "logindialog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SplashScreen splash;
       splash.show();
       LoginDialog logindlg;
      logindlg.show();

      //  MainWindow mainWindow;
      // mainWindow.show();


    return a.exec();
}
