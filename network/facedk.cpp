#include "facedk.h"
#include "ui_facedk.h"

FaceDK::FaceDK(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FaceDK)
{
    ui->setupUi(this);
}

FaceDK::~FaceDK()
{
    delete ui;
}
