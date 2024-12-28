#ifndef LOGIN_REG_H
#define LOGIN_REG_H

#include <QWidget>
#include <QPixmap>

namespace Ui {
class login_reg;
}

class login_reg : public QWidget
{
    Q_OBJECT

public:
    explicit login_reg(QWidget *parent = nullptr);
    ~login_reg();

private:
    Ui::login_reg *ui;
    QPixmap backgroundPixmap;
};

#endif // LOGIN_REG_H
