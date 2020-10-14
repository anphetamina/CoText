#include "Login.h"
#include "ui_login.h"
#include "Register.h"
#include "UserWidget.h"
#include "sslechoclient.h"

Login::Login(QWidget *parent) : QDialog(parent), ui(new Ui::Login) {
    ui->setupUi(this);
    ui->errorLabel->setText("");
}

Login::~Login() {
    delete ui;
}

void Login::clearInput() {
	ui->lineEdit_Pass->text().clear();
	ui->lineEdit_User->text().clear();
}

void Login::on_pushButton_Login_clicked()
{
	//clearInput();
	bool checkDone = false;
    QString username = ui->lineEdit_User->text();
    QString password = ui->lineEdit_Pass->text();
    
	//// Do we need a while?
    while(checkInput(username, password) && !checkDone) {
	    //Do login
	    checkDone = true;
	    client->set_username(username);
	    client->set_password(password);
	    client->sendLogin();
    }
}


void Login::on_pushButton_Reg_clicked()
{
    //open Register Form
    clearInput();
    //hide();
    Register regForm;
    connect(&regForm, &Register::closeLogin, this, &Login::loginSuccessful);
    client->connectToRegister(&regForm);
    regForm.setModal(true);
    regForm.exec();
}


bool Login::checkInput(const QString& username, const QString& psw) {
	bool isNull = false;
	bool isEmpty = false;
	
	if(username.isNull() || psw.isNull())
		isNull = true;
	if(username.isEmpty() || psw.isEmpty())
		isEmpty = true;
	
	if(isNull || isEmpty) {
		QMessageBox::warning(this, "Login failure", "You have missed some fields, retry");
		clearInput();
		return false;
		
	} else {
		
		QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-0.-]+\\.[A-Z]{2,4}\\b");
		mailREX.setCaseSensitivity(Qt::CaseInsensitive);
		mailREX.setPatternSyntax(QRegExp::RegExp);
		
		bool regMat = mailREX.exactMatch(username);
		
		if(!regMat) {
			QMessageBox::warning(this, "Login Failure", "Email format is incorrect, retry");
			clearInput();
			return false;
		} else
			return true;
		
	}
}

void Login::loginFailed(){
    ui->errorLabel->setText("Wrong Username or Password !");
    ui->lineEdit_User->setText("");
    ui->lineEdit_Pass->setText("");
}

void Login::loginSuccessful(){
    this->close();
}

