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
                client->sendRegistration(name, surname, email, nickname, pass1, profilePicture);

            }
        }
    }
}


void Register::on_pushButton_BrowseReg_clicked() {
	QMessageBox::warning(this, "Resize Warn", "The image will be resized in order to be 128x128px");
	
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
		
		/*
		//! Try to make label circular
		//#3. Create the output image with the same dimensions and an alpha channel + make completely transparent
		QImage output = QImage(imgSize, imgSize, QImage::Format_ARGB32);
		output.fill(Qt::transparent);
		
		//Create a texture brush and paint a circle with the original image onto the output image
		QBrush qBrush = QBrush(newPixmap);
		QPainter qPainter = QPainter();
		
		
		qPainter.setBrush(qBrush);
		qPainter.setPen(Qt::NoPen);
		qPainter.setRenderHint(QPainter::Antialiasing, true);
		qPainter.drawEllipse(0, 0, imgSize, imgSize);
		qPainter.end();
		
		//Convert the image to a pixmap and rescale it. Take pixel ratio into account to get a sharp image on retina display
		auto pr = QWindow().devicePixelRatio();
		QPixmap pm = QPixmap::fromImage(output);
		imgSize  *=  pr;
		pm = pm.scaled(imgSize, imgSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		
		pm.setDevicePixelRatio(pr);
		//! end make label circular
		*/
		
		
		//this->profilePicture = pm;
		//QPixmap circ(":/images/CircleMask128x128.png");
		//QPixmap mask = circ.createMaskFromColor(Qt::white, Qt::MaskOutColor);
		
		this->profilePicture = newPixmap;
		ui->labelPhoto->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
		ui->labelPhoto->setFixedWidth(newPixmap.width());
		ui->labelPhoto->setFixedHeight(newPixmap.height());
		//ui->labelPhoto->setMask(mask);
		ui->labelPhoto->setPixmap(QPixmap::fromImage(newPixmap));
		
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
    emit closeLogin();
    this->close();
}