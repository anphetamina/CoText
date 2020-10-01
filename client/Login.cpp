#include "Login.h"
#include "ui_login.h"
#include "Register.h"
#include "UserWidget.h"
#include "sslechoclient.h"



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
	
	//SslEchoClient* client = new SslEchoClient(QUrl(QStringLiteral("wss://localhost:12345")), this);
	
	client->set_username(username);
	client->set_password(password);
	client->sendLogin();
	//client->authenticate(username, password);
	
}



void Login::on_pushButton_Reg_clicked()
{
    //open Register Form
    hide();
    Register regForm;
    regForm.setModal(true);
    regForm.exec();


}

void Login::closeEvent(QCloseEvent *event) {
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "CoText", tr("Are your sure to quit?\n"), QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
	if(resBtn != QMessageBox::Yes) {
		event->ignore();
	} else {
		event->accept();
	}
}
