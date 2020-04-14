#include "Login.h"
#include "ui_login.h"
#include "Register.h"
#include "UserWidget.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_Login_clicked()
{


    QString username = ui->lineEdit_User->text();
    QString password = ui->lineEdit_Pass->text();


    //TODO in CLion
    //1. Check if username exists and if password is correct
}



void Login::on_pushButton_Reg_clicked()
{
    //open Register Form
    hide();
    Register regForm;
    regForm.setModal(true);
    regForm.exec();


}
