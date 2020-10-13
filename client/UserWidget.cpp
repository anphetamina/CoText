#include "MainWindow.h"
#include "ui_userwidget.h"
#include "UserWidget.h"
#include "usereditwidget.h"

#include <QDebug>
#include <QPixmap>
#include <QPrinter>
#include <QColorDialog>
#include <QColor>
#include <QCloseEvent>
#include <QtSvg>
#include <QEvent>


UserWidget::UserWidget(QWidget *parent) : QDialog(parent), ui(new Ui::UserWidget) {
	
    ui->setupUi(this);

    qDebug() << ui->label_profilpic;
    
    //get info and fill contents
	User usr = MainWindow::getUser();
	ui->label_profilpic->setPixmap(QPixmap::fromImage(usr.getProfilePic()));
	ui->label_profilpic->setMaximumWidth(48);
	ui->label_profilpic->setMaximumHeight(48);
	ui->Name->setText(usr.getName());
	ui->Surname->setText(usr.getSurname());
	ui->email->setText(usr.getEmail());
	
}

UserWidget::~UserWidget() {
	
	delete ui;
	
}

void UserWidget::on_pushButton_Edit_clicked() {
	UserEditWidget uew;
	//connect(&uew, &Register::closeLogin, this, &Login::loginSuccessful);
	//client->connectToRegister(&regForm);
	uew.setModal(true);
	uew.exec();
}

void UserWidget::on_pushButton_Cancel_clicked() {
	this->close();
}


