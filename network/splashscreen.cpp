#include "splashscreen.h"
#include <QDesktopWidget>

SplashScreen::SplashScreen(QWidget *parent) : QSplashScreen(parent)
{
    // 设置启动画面的图片，这里假设图片文件名为 "splash.png"，你需要将其替换为实际的图片路径
    setPixmap(QPixmap("C:/Users/pjw20/Documents/network/images/Applogo.png"));
    // 创建一个定时器，用于在一定时间后关闭启动画面
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SplashScreen::closeSplashScreen);
    // 3 秒后关闭启动画面，你可以根据需要调整时间
    timer->start(500);

}

SplashScreen::~SplashScreen()
{
    delete timer;
}
void SplashScreen::closeSplashScreen()
{
    // 关闭启动画面
    close();
    emit splashClosed();
}
