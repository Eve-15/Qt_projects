#include "menu.h"
#include "ui_menu.h"
#include "mainwindow.h"
#include "facedk.h"
Menu::Menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_pushButton_clicked()
{
    MainWindow *dia01=new MainWindow;
       // 显示模特对话框
      dia01->show();

}

void Menu::on_pushButton_3_clicked()
{
    FaceDK *FA01=new FaceDK;
       // 显示模特对话框
      FA01->show();
}
