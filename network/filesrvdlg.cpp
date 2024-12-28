#include "filesrvdlg.h"
#include "ui_filesrvdlg.h"
FileSrvDlg::FileSrvDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSrvDlg)
{
    ui->setupUi(this);
    myTcpSrv = new QTcpServer(this);
    mySrvPort = 5555;
    connect(myTcpSrv, SIGNAL(newConnection()), this, SLOT(sndChatMsg()));
    myTcpSrv->close();
    myTotalBytes = 0;
    mySendBytes = 0;
    myBytesTobeSend = 0;
    myPayloadSize = 64 * 1024;
    ui->sendProgressBar->reset();
    ui->openFilePushButton->setEnabled(true);
    ui->sendFilePushButton->setEnabled(false);
}

void FileSrvDlg::sndChatMsg()
{
    ui->sendFilePushButton->setEnabled(false);
    mySrvSocket = myTcpSrv->nextPendingConnection();
    connect(mySrvSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(refreshProgress (qint64)));
    myLocPathFile = new QFile(myPathFile);
    myLocPathFile->open((QFile::ReadOnly));				//(a)
    myTotalBytes = myLocPathFile->size();				//(b)
    QDataStream sendOut(&myOutputBlock, QIODevice::WriteOnly);		//(c)
    sendOut.setVersion(QDataStream::Qt_5_11);
    mytime.start();							//(d)
    QString curFile = myPathFile.right(myPathFile.size() - myPathFile.lastIndexOf ('/') - 1);											//(e)
    sendOut << qint64(0) << qint64(0) << curFile;				//(f)
    myTotalBytes += myOutputBlock.size();
    sendOut.device()->seek(0);						//(g)
    sendOut << myTotalBytes << qint64((myOutputBlock.size() - sizeof(qint64) * 2)); //(h)
    myBytesTobeSend = myTotalBytes - mySrvSocket->write(myOutputBlock);												//(i)
    myOutputBlock.resize(0);						//(j)
}

void FileSrvDlg::refreshProgress(qint64 bynum)
{
    qApp->processEvents();						//(a)
    mySendBytes += (int)bynum;
    if(myBytesTobeSend > 0)
    {
        myOutputBlock=myLocPathFile->read(qMin(myBytesTobeSend, myPayloadSize));
        myBytesTobeSend -= (int)mySrvSocket->write(myOutputBlock);
        myOutputBlock.resize(0);
    } else {
        myLocPathFile->close();
    }
    ui->sendProgressBar->setMaximum(myTotalBytes);
    ui->sendProgressBar->setValue(mySendBytes);
    ui->sfileSizeLineEdit->setText(tr("%1").arg(myTotalBytes/(1024 * 1024)) + " MB");													//填写文件总大小栏
    ui->sendSizeLineEdit->setText(tr("%1").arg(mySendBytes/(1024*1024))+"MB");													//填写已发送栏
    if(mySendBytes == myTotalBytes)
    {
        myLocPathFile->close();
        myTcpSrv->close();
        QMessageBox::information(0, QObject::tr("完毕"), "文件传输完成！");
    }
}

void FileSrvDlg::on_openFilePushButton_clicked()
{
    myPathFile = QFileDialog::getOpenFileName(this);
    if(!myPathFile.isEmpty())
    {
        myFileName = myPathFile.right(myPathFile.size() - myPathFile.lastIndexOf ('/') - 1);
        ui->sfileNameLineEdit->setText(tr("%1").arg(myFileName));
        ui->sendFilePushButton->setEnabled(true);
        ui->openFilePushButton->setEnabled(false);
    }
}

void FileSrvDlg::on_sendFilePushButton_clicked()
{
    if(!myTcpSrv->listen(QHostAddress::Any, mySrvPort))		//开始监听
    {
        QMessageBox::warning(0, QObject::tr("异常"), "打开TCP端口出错,请检查网络连接！");
        close();
        return;
    }
    emit sendFileName(myFileName);
}

void FileSrvDlg::on_srvClosePushButton_clicked()
{
    if(myTcpSrv->isListening())
    {
        myTcpSrv->close();
        myLocPathFile->close();
        mySrvSocket->abort();
    }
    close();
}

void FileSrvDlg::closeEvent(QCloseEvent *)
{
    on_srvClosePushButton_clicked();
}

void FileSrvDlg::cntRefused()
{
    myTcpSrv->close();
    QMessageBox::warning(0, QObject::tr("提示"), "对方拒绝接收！");
}
FileSrvDlg::~FileSrvDlg()
{

}
