#include "SX_3D_Client.h"
#include "ui_SX_3D_Client.h"

SX_3D_Client::SX_3D_Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SX_3D_Client)
{
    ui->setupUi(this);
}

SX_3D_Client::~SX_3D_Client()
{
    delete ui;
}
