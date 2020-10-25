#include "usereditwidget.h"
#include "ui_usereditwidget.h"
#include "MainWindow.h"
#include "sslechoclient.h"

UserEditWidget::UserEditWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserEditWidget)
{
    ui->setupUi(this);
	client->connectToUserEdit(this);
	this->setWindowTitle("Edit User Dialog");
    User usr = MainWindow::getUser();
    clearInput();
    this->setOldUsername(usr.getEmail());
    this->setOldPP(usr.getProfilePic());
	ui->LnewPP->setVisible(false);
    ui->lineEditName->setText(usr.getName());
    ui->lineEditSurname->setText(usr.getSurname());
    ui->lineEditEmail->setText(usr.getEmail());
    //the password is hashed
    ui->lineEditPass->setText(usr.getPassword());
    ui->lineEditRePass->setText(usr.getPassword());
    
}

UserEditWidget::~UserEditWidget()
{
    delete ui;
}

void UserEditWidget::setOldUsername(QString _oldUsername) {
	this->oldUsername = _oldUsername;
}
QString UserEditWidget::getOldUsername() {
	return this->oldUsername;
}

void UserEditWidget::setNewPP(QImage newPP) {
	this->newProfilePic = newPP;
}

QImage UserEditWidget::getNewPP() {
	return this->newProfilePic;
}
QImage UserEditWidget::getOldPP() {
	return this->oldProfilePic;
}
void UserEditWidget::setOldPP(QImage _oldPP) {
	this->oldProfilePic = _oldPP;
}

void UserEditWidget::clearInput() {
	ui->lineEditName->text().clear();
	ui->lineEditSurname->text().clear();
	ui->lineEditEmail->text().clear();
	ui->lineEditPass->text().clear();
	ui->lineEditRePass->text().clear();
}

void UserEditWidget::on_pushButton_Done_clicked() {
	User usr = MainWindow::getUser();
	bool checkDone = false;
	
	QString name = ui->lineEditName->text();
	QString surname = ui->lineEditSurname->text();
	QString email = ui->lineEditEmail->text();
	QString password = ui->lineEditPass->text();
	QString re_pass = ui->lineEditRePass->text();
	
	
	while(checkInput(name, surname, email, password, re_pass) && !checkDone) {
		checkDone = true;
		
		if(isPPChanged)
			client->sendUpdateProfile(usr.getId(), name, surname, email, password, getNewPP());
		else
			client->sendUpdateProfile(usr.getId(), name, surname, email, password, getOldPP());
		
		
	}
	
	//QMessageBox::information(this, "Completed", "User Edit completed");
	//this->close();
	
}

bool UserEditWidget::checkInput(const QString &name, const QString &surname, const QString &email, const QString &pass, const QString &re_pass) {
	bool isNull = false;
	bool isEmpty = false;
	
	if(name.isNull() || surname.isNull() || email.isNull())
		isNull = true;
	if(name.isEmpty() || surname.isEmpty() || email.isEmpty())
		isEmpty = true;
	
	if(isNull || isEmpty) {
		QMessageBox::warning(this, "Update Failure", "Fill all the fields: if you don't want to modify some, just re-insert what it is shown in the placeholder");
		clearInput();
		return false;
	} else {
		
		if(pass != re_pass) {
			QMessageBox::warning(this, "Edit failure", "Passwords don't match each other");
		} else {
			QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-0.-]+\\.[A-Z]{2,4}\\b");
			mailREX.setCaseSensitivity(Qt::CaseInsensitive);
			mailREX.setPatternSyntax(QRegExp::RegExp);
			
			bool regMat = mailREX.exactMatch(email);
			if(!regMat) {
				QMessageBox::warning(this, "Update Failure", "Email format is incorrect, retry");
				clearInput();
				return false;
			} else
				return true;
		}
	}
	return false;
}

void UserEditWidget::on_pushButton_Cancel_clicked() {
	
	this->close();
}

void UserEditWidget::on_buttonChangePP_clicked() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.jpeg)"));
	if(QString::compare(filename, QString()) != 0) {
		
		//#1. Open Image scale + covert in ARGB
		QFileInfo f(filename);
		QImage pixmap(filename);
		QImage scaledPixmap = pixmap.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QImage newPixmap = scaledPixmap.convertToFormat(QImage::Format_ARGB32);
		
		//#2. Crop to a square
		int imgSize = std::min(newPixmap.width(), newPixmap.height());
		QRect rect = QRect(
				(newPixmap.width() - imgSize) / 2,
				(newPixmap.height() - imgSize) / 2,
				imgSize,
				imgSize);
		newPixmap = newPixmap.copy(rect);
		
		setNewPP(newPixmap);
		ui->LnewPP->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
		ui->LnewPP->setFixedWidth(newPixmap.width());
		ui->LnewPP->setFixedHeight(newPixmap.height());
		ui->LnewPP->setPixmap(QPixmap::fromImage(newPixmap));
		ui->LnewPP->setVisible(true);
		this->isPPChanged = true;
		
		
	} else {
		QMessageBox::warning(this, "File not valid", "The choosen file is not valid, please retry");
	}
}

void UserEditWidget::updateOk() {
	this->close();
}