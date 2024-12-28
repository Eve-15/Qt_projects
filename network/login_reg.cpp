#include "login_reg.h"
#include "ui_login_reg.h"
#include <QtDebug>
login_reg::login_reg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login_reg)
{
    ui->setupUi(this);
    // 加载背景图片，假设图片已经添加到资源文件，路径为:/background.jpg
       backgroundPixmap = QPixmap("C:/Users/pjw20/Documents/network/images/start.png");
       // 设置背景图片
       QPalette palette;
       palette.setBrush(QPalette::Background, QBrush(backgroundPixmap));
       this->setPalette(palette);
       ui->pushButton->setStyleSheet("QPushButton#pushButton{background-color: lightgreen; color: white; border: 2px solid gray; border-radius: 5px;font-size: 30px; }");
       ui->pushButton->setStyleSheet(ui->pushButton->styleSheet() + "QPushButton#pushButton_1:hover {background-color: green;}");
       ui->pushButton->setStyleSheet(ui->pushButton->styleSheet() + "QPushButton#pushButton_1:pressed {background-color: lightgreen;}");
}

login_reg::~login_reg()
{
    delete ui;
}
