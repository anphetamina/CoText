#include "Join.h"
#include "ui_Join.h"

Join::Join(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Join)
{
    ui->setupUi(this);
    ui->errorLabel->setText("");
}

Join::~Join()
{
    delete ui;
}

void Join::on_pushButton_join_clicked()
{
    emit(sendJoin(user.getId(), 0, ui->invitationCode->text()));
    //qDebug() << "[JOIN] sendJoin userId = "<< user.getId() << " invCode = "<< ui->invitationCode->text();
}

void Join::closeJoin(){
    this->close();
}

void Join::showError(){
    ui->errorLabel->setText("Wrong invitation code !");
}