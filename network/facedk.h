#ifndef FACEDK_H
#define FACEDK_H

#include <QDialog>

namespace Ui {
class FaceDK;
}

class FaceDK : public QDialog
{
    Q_OBJECT

public:
    explicit FaceDK(QWidget *parent = nullptr);
    ~FaceDK();

private:
    Ui::FaceDK *ui;
};

#endif // FACEDK_H
