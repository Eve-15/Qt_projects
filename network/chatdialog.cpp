#include "chatdialog.h"
#include "ui_chatdialog.h"
#include "filesrvdlg.h"
ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog)
{
    ui->setupUi(this);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}



void ChatDialog::on_transPushButton_clicked()
{
    FileSrvDlg *FSD01=new FileSrvDlg;
       // 显示模特对话框
      FSD01->show();
}
