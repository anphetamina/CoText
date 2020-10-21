#include "Register.h"
#include "ui_register.h"
#include <QMessageBox>
#include <QIcon>
#include <QDebug>
#include "sslechoclient.h"

Register::Register(QWidget *parent) : QDialog(parent), ui(new Ui::Register) {
    ui->setupUi(this);
    ui->labelPhoto->setVisible(false);
    //ui->dropBox->setAcceptDrops(true);
}

Register::~Register() {
    delete ui;
}

void Register::clearInput() {
    qDebug() << "Input cleared";
    ui->lineEdit_Name->text().clear();
    ui->lineEdit_Surname->text().clear();
    ui->lineEdit_Pass1->text().clear();
    ui->lineEdit_Pass2->text().clear();
    ui->lineEdit_email->text().clear();
}


void Register::on_pushButton_Register_clicked() {
    clearInput();
    bool null = false;
    bool empty = false;
    name = ui->lineEdit_Name->text();
    surname = ui->lineEdit_Surname->text();
    pass1 = ui->lineEdit_Pass1->text();
    pass2 = ui->lineEdit_Pass2->text();
    email = ui->lineEdit_email->text();

    if(name.isNull() || surname.isNull() || pass1.isNull() || pass2.isNull() || email.isNull())
        null = true;
    if(name.isEmpty() || surname.isEmpty() || pass1.isEmpty() || pass2.isEmpty() || email.isEmpty())
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
            
				qDebug() << name, surname;
				//here password is not hashed, it will be hashed in sendRegistration
                client->set_username(email);
                client->set_password(pass1);
                client->sendRegistration(name, surname, email, nickname, pass1, profilePicture);

            }
        }
    }
}


void Register::on_pushButton_BrowseReg_clicked() {
	//QMessageBox::warning(this, "Resize Warn", "The image will be resized in order to be 128x128px");
	
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.jpeg)"));
	if(QString::compare(filename, QString()) != 0) {
		
		//#1. Open Image scale + covert in ARGB
		QFileInfo f(filename);
		QImage img(filename);
		QImage scaledImage = img.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QImage squaredImage = scaledImage.convertToFormat(QImage::Format_ARGB32);
		QPixmap squaredpixmap = QPixmap::fromImage(squaredImage);
		//#2. Crop to a square
		int imgSize = qMin(squaredImage.width(), squaredImage.height());
		
		QRect rect = QRect(
				qAbs(squaredImage.width() - imgSize) / 2,
				qAbs(squaredImage.height() - imgSize) / 2,
				imgSize,
				imgSize);
		squaredImage = squaredImage.copy(rect);
		
		//#3. Create circle clip area
		QPixmap rounded = QPixmap(imgSize, imgSize);
		rounded.fill(Qt::transparent);
		QPainterPath path;
		path.addEllipse(rounded.rect());
		QPainter painter(&rounded);
		painter.setClipPath(path);
		//fill rounded area if needed
		painter.fillRect(rounded.rect(), Qt::transparent);
		/*
		int x = qAbs(pixmap.width() + imgSize)
		painter.drawPixmap()
		*/
		
		
		
		this->profilePicture = squaredImage;
		ui->labelPhoto->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
		ui->labelPhoto->setFixedWidth(squaredImage.width());
		ui->labelPhoto->setFixedHeight(squaredImage.height());
		//ui->labelPhoto->setMask(mask);
		ui->labelPhoto->setPixmap(QPixmap::fromImage(squaredImage));
		
		ui->labelPhoto->setVisible(true);
		
		
		
	} else {
		QMessageBox::warning(this, "File not valid", "The choosen file is not valid, please retry");
	}
	
	qDebug() << filename;
	
	
}

void Register::on_pushButton_Canc_Reg_clicked() {
	this->close();
}

void Register::showError() {
	QMessageBox::warning(this, "Error", "Error from server");
}
void Register::registerOk() {
    qDebug() << "[REGISTER] registerOk";
    //emit closeLogin();
    this->close();
}