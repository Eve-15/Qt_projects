#include "mainwindow.h"
#include "filecntdlg.h"
#include "filesrvdlg.h"
#include "ui_mainwindow.h"
#include "ui_filecntdlg.h"
#include "ui_filesrvdlg.h"
#include <QtDebug>
#include <QWidget>
 MainWindow::MainWindow(QWidget *parent) : //构造函数
    QMainWindow(parent), //调用父类构造函数
    ui(new Ui::MainWindow) //初始化ui
{
    ui->setupUi(this); //初始化ui
    initMainWindow(); //初始化主窗口
}



void MainWindow::initMainWindow() //初始化主窗口
{
    myUdpSocket = new QUdpSocket(this); //创建UDP套接字
    myUdpPort = 8080; //设置UDP端口
    myUdpSocket->bind(myUdpPort, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint); //绑定UDP端口
    connect(myUdpSocket, SIGNAL(readyRead()), this, SLOT(recvAndProcessChatMsg ())); //连接UDP套接字的readyRead()信号和recvAndProcessChatMsg()槽函数
    myfsrv = new FileSrvDlg(this); //  初始化文件接收对话框
    connect(myfsrv, SIGNAL(sendFileName(QString)), this, SLOT(getSfileName (QString))); //连接文件接收对话框的sendFileName()信号和getSfileName()槽函数

}

void MainWindow::on_searchPushButton_clicked()
{
    myname = this->windowTitle(); // 获取窗口标题
    qDebug() << "myname is :" << myname;
    ui->userLabel->setText(myname); // 设置用户标签

    // 如果尚未发送上线消息，则发送
    if (!isOnline)
    {
        sendChatMsg(OnLine); // 发送上线消息
        isOnline = true;      // 标记已发送上线消息
    }
}

void MainWindow::sendChatMsg(ChatMsgType msgType, QString rmtName) //发送聊天消息
{

    QByteArray qba; //创建字节数组
    QDataStream write(&qba, QIODevice::WriteOnly); //创建数据写入流
    QString locHostIp = getLocHostIp(); //获取本地主机IP地址
    QString locChatMsg = getLocChatMsg(); //获取本地聊天消息
    write << msgType << myname;								//(1)
    switch (msgType) //根据消息类型进行处理
    {
    case ChatMsg:											 	//(b)
        write << locHostIp << locChatMsg;
        break;
    case OnLine:												//(c)
        write << locHostIp;
        break;
    case OffLine:											//(d)
        break;
    case SfileName:											//(e)
        write << locHostIp << rmtName << myFileName;
        break;
    case RefFile:
        write << locHostIp << rmtName; //发送拒绝接收文件消息
        break;
    }
    myUdpSocket->writeDatagram(qba, qba.length(), QHostAddress::Broadcast,myUdpPort); //(f) //发送UDP数据报
}

void MainWindow::recvAndProcessChatMsg() //接收并处理聊天消息
{
    while (myUdpSocket->hasPendingDatagrams())				//(a) //有待处理的UDP数据报
    {
        QByteArray qba;
        qba.resize(myUdpSocket->pendingDatagramSize());		//(b) //调整字节数组大小
        myUdpSocket->readDatagram(qba.data(), qba.size()); //(c) //读取数据报
        QDataStream read(&qba, QIODevice::ReadOnly);        //(d) //创建数据读取流
        int msgType;
        read >> msgType;										//(c)//读取消息类型
        QString name, hostip, chatmsg, rname, fname; //定义字符串变量
        QString curtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh: mm:ss"); //获取当前时间
        switch (msgType) //根据消息类型进行处理
        {
        case ChatMsg: {
            read >> name >> hostip >> chatmsg;				//(d) //读取消息内容
            ui->chatTextBrowser->setTextColor(Qt::darkGreen); //设置字体颜色
            ui->chatTextBrowser->setCurrentFont(QFont("Times New Roman", 14)); //设置字体
            ui->chatTextBrowser->append("【" + name + "】" + curtime); //(e) //显示聊天消息
            ui->chatTextBrowser->append(chatmsg); //(e) //显示聊天消息
            break;
        }
        case OnLine:											//(e)//在线消息
            read >> name >> hostip; //读取在线消息
            onLine(name, curtime); //在线消息
            break;
        case OffLine:										//(f)//离线消息
            read >> name;
            offLine(name, curtime); //离线消息
            break;
        case SfileName:
            read >> name >> hostip >> rname >> fname; //读取文件名
            recvFileName(name, hostip, rname, fname); //接收文件名
            break;
        case RefFile:
            read >> name >> hostip >> rname; //读取拒绝接收文件消息
            if(myname == rname) myfsrv->cntRefused(); //拒绝接收文件
            break;
        }
    }
}

void MainWindow::onLine(QString name, QString time)
{
    // 检查用户是否已存在
    qDebug() << "Find items result: " << ui->userListTableWidget->findItems(name, Qt::MatchExactly).size();
    bool notExist = ui->userListTableWidget->findItems(name, Qt::MatchExactly).isEmpty(); 
    if (notExist)
    {
        // 新用户添加到表格中
        qDebug() << "User name: " << name;
        QTableWidgetItem *newuser = new QTableWidgetItem(name);
        qDebug() << "Inserting new row at index 0";
        ui->userListTableWidget->insertRow(0);
        qDebug() << "Inserting item at (0, 0)";
        ui->userListTableWidget->setItem(0, 0, newuser);


        // 显示上线消息
        ui->chatTextBrowser->setTextColor(Qt::gray);
        ui->chatTextBrowser->setCurrentFont(QFont("Times New Roman", 12));
        ui->chatTextBrowser->append(tr("%1 %2 上线！").arg(time).arg(name));

        // 如果尚未发送上线消息，则发送
        if (!isOnline)
        {
            sendChatMsg(OnLine); // 发送上线消息
            isOnline = true;      // 标记已发送上线消息
        }
    }
}

void MainWindow::offLine(QString name, QString time) //离线消息
{
    int row = ui->userListTableWidget->findItems(name, Qt::MatchExactly).first ()->row(); //获取用户行号
    ui->userListTableWidget->removeRow(row); //删除用户
    ui->chatTextBrowser->setTextColor(Qt::gray); //设置字体颜色
    ui->chatTextBrowser->setCurrentFont(QFont("Times New Roman", 12)); //设置字体
    ui->chatTextBrowser->append(tr("%1 %2 离线！").arg(time).arg(name)); //显示离线消息
}

void MainWindow::closeEvent(QCloseEvent *event) //关闭事件
{
    //qDebug()<<"offline is used";
    sendChatMsg(OffLine); //发送离线消息
}

QString MainWindow::getLocHostIp() //获取本地主机IP地址
{
    QList<QHostAddress> addrlist = QNetworkInterface::allAddresses(); //获取本地主机IP地址
    foreach (QHostAddress addr, addrlist) //遍历IP地址列表
    {
        if(addr.protocol() == QAbstractSocket::IPv4Protocol) return addr. toString(); //返回IPv4地址
    }
    return nullptr; //返回空指针
}

QString MainWindow::getLocChatMsg() //获取本地聊天消息
{
    QString chatmsg = ui->chatTextEdit->toHtml(); //获取聊天消息
    ui->chatTextEdit->clear(); //清空聊天消息
    ui->chatTextEdit->setFocus(); //设置焦点
    return chatmsg;
}

void MainWindow::on_sendPushButton_clicked()
{
    sendChatMsg(ChatMsg); //发送聊天消息
}


void MainWindow::getSfileName(QString fname)
{
    myFileName = fname; //获取文件名
    int row = ui->userListTableWidget->currentRow(); //获取选中行号
    QString rmtName = ui->userListTableWidget->item(row, 0)->text(); //获取选中用户名
    sendChatMsg(SfileName, rmtName); //发送文件名
}

void MainWindow::on_transPushButton_clicked()
{
    if(ui->userListTableWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(nullptr, tr("选择好友"), tr("请先选择文件接收方！"), QMessageBox::Ok); //弹出警告框
        return;
    }
    myfsrv->show();
}

void MainWindow::recvFileName(QString name, QString hostip, QString rmtname, QString filename)
{
    if(myname == rmtname)
    {
        int result = QMessageBox::information(this, tr("收到文件"), tr("好友 %1 给您发文件：\r\n%2，是否接收？").arg(name).arg(filename), QMessageBox::Yes, QMessageBox::No); //弹出提示框
        if(result == QMessageBox::Yes)
        {
            QString fname = QFileDialog::getSaveFileName(nullptr, tr("保 存"), filename); //弹出文件对话框
            if(!fname.isEmpty())
            {
                FileCntDlg *fcnt = new FileCntDlg(this); //创建文件接收对话框
                fcnt->getLocPath(fname); //获取本地路径
                fcnt->getSrvAddr(QHostAddress(hostip)); //获取服务器地址
                fcnt->show(); //显示文件接收对话框
            }
        } else {
            sendChatMsg(RefFile, name); //发送拒绝接收文件消息
        }
    }
}
MainWindow::~MainWindow() {

}
