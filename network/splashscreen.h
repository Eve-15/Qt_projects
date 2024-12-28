#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QSplashScreen>
#include <QTimer>
#include <QWidget>
#include <QMouseEvent>

class SplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    SplashScreen(QWidget *parent = nullptr);
     ~SplashScreen();
signals:
    void splashClosed();
private:
    QTimer *timer;

    void closeSplashScreen();
};

#endif // SPLASHSCREEN_H
