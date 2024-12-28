#include "logindialog.h"
#include "ui_logindialog.h"
#include "login_reg.h"
#include "menu.h"
LoginDialog::LoginDialog(QWidget *parent) :QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->pwdLineEdit->setFocus();//输入焦点初始置于密码框
    backgroundPixmap = QPixmap("C:/Users/pjw20/Documents/network/images/start.png");
    // 设置背景图片
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(backgroundPixmap));
    this->setPalette(palette);
    ui->loginPushButton->setStyleSheet("QPushButton#loginPushButton{background-color: lightgreen; color: white; border: 2px solid gray; border-radius: 5px;font-size: 30px; }");
    ui->loginPushButton->setStyleSheet(ui->loginPushButton->styleSheet() + "QPushButton#loginPushButton:hover {background-color: green;}");
    ui->loginPushButton->setStyleSheet(ui->loginPushButton->styleSheet() + "QPushButton#loginPushButton:pressed {background-color: lightgreen;}");
    ui->pushButton_2->setStyleSheet("QPushButton#pushButton_2{background-color: lightgreen; color: white; border: 2px solid gray; border-radius: 5px;font-size: 30px; }");
    ui->pushButton_2->setStyleSheet(ui->pushButton_2->styleSheet() + "QPushButton#pushButton_2:hover {background-color: green;}");
    ui->pushButton_2->setStyleSheet(ui->pushButton_2->styleSheet() + "QPushButton#pushButton_2:pressed {background-color: lightgreen;}");
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginPushButton_clicked()
{
    showWeiChatWindow();							//调用验证显示聊天窗口的方法
}

/**----------实现登录验证功能----------*/
void LoginDialog::showWeiChatWindow()
{
    QFile file("D:\\Qt_projects\\Qt_projects\\network\\userlog.xml");						//创建XML文件对象
    mydoc.setContent(&file);		//将XML对象赋给QdomDocument类型的Qt文档句柄
    QDomElement root = mydoc.documentElement();		//获取XML文档的DOM根元素
    if(root.hasChildNodes())
    {
        QDomNodeList userList = root.childNodes();	//获取根元素的全部子节点
        bool exist = false;							//指示用户是否存在
        for(int i = 0; i < userList.count(); i++)
        {
            QDomNode user = userList.at(i);			//根据当前索引i获取用户节点元素
            QDomNodeList record = user.childNodes();	//该用户的全部属性元素
           //解析出用户名及密码
            QString uname = record.at(0).toElement().text();
            QString pword = record.at(1).toElement().text();
            if(uname == ui->usrLineEdit->text())
            {
                exist = true;							//用户存在
                if(!(pword == ui->pwdLineEdit->text()))
                {
                    QMessageBox::warning(nullptr, QObject::tr("提示"), "口令错！请重新输入。");
                    ui->pwdLineEdit->clear();
                    ui->pwdLineEdit->setFocus();
                    return;
                }
            }
        }
        if(!exist)
        {
            QMessageBox::warning(nullptr, QObject::tr("提示"), "此用户不存在！请重新输入。");
            ui->usrLineEdit->clear();
            ui->pwdLineEdit->clear();
            ui->usrLineEdit->setFocus();
            return;
        }
       //用户存在且密码验证通过
        weiChatWindow = new MainWindow(nullptr);
        weiChatWindow->setWindowTitle(ui->usrLineEdit->text());
        weiChatWindow->show();						//显示聊天窗口
    }
}


void LoginDialog::on_pushButton_2_clicked()
{
    login_reg *LOG01 =new login_reg;
    LOG01->show();
}
