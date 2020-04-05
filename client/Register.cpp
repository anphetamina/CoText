#include "Register.h"
#include "ui_register.h"

#include <QMessageBox>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
}

Register::~Register()
{
    delete ui;
}

void clearInput() {


}

void Register::on_pushButton_Register_clicked()
{
    name = ui->lineEdit_Name->text();
    surname = ui->lineEdit_Surname->text();
    nickname = ui->lineEdit_Nick->text();
    pass1 = ui->lineEdit_Pass1->text();
    pass2 = ui->lineEdit_Pass2->text();
    email = ui->lineEdit_email->text();
    bool null = false;
    bool empty = false;


    if(name.isNull() || surname.isNull() || nickname.isNull() || pass1.isNull() || pass2.isNull() || email.isNull())
        null = true;
    if(name.isEmpty() || surname.isEmpty() || nickname.isEmpty() || pass1.isEmpty() || pass2.isEmpty() || email.isEmpty())
        empty = true;

    if(null || empty)
        QMessageBox::warning(this, "Register failure", "You have missed some fields, please compile all fields in the form");
    else {
        //if nickname exists -> warning
        //if mail exists -> warning
        if(pass1 != pass2) {
            QMessageBox::warning(this, "Register failure", "Passwords don't match each other");
            pass1.clear();
            pass2.clear();
        } else {
            QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-0.-]+\\.[A-Z]{2,4}\\b");
            mailREX.setCaseSensitivity(Qt::CaseInsensitive);
            mailREX.setPatternSyntax(QRegExp::RegExp);

            bool regMat = mailREX.exactMatch(email);

            if(!regMat) {
                QMessageBox::warning(this, "Register failure", "Email format is not correct, please use a correct email format.");
                email.clear();
            } else {
                ////TODO
                //all is correct, register user
            }
        }
    }
}
