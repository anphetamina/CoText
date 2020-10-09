#include "ServerDisconnected.h"
#include "ui_ServerDisconnected.h"

ServerDisconnected::ServerDisconnected(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerDisconnected)
{
    ui->setupUi(this);
}

ServerDisconnected::~ServerDisconnected()
{
    delete ui;
}

void ServerDisconnected::on_pushButton_clicked()
{
    emit quitClicked();
}
