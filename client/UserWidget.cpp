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
    this->setWindowTitle("Personal information");

    //get info and fill contents
	ui->label_profilpic->setPixmap(makeRoundImage(QPixmap::fromImage(user.getProfilePic())));
	ui->label_profilpic->setMaximumWidth(128);
	ui->label_profilpic->setMaximumHeight(128);
	ui->Name->setText(user.getName());
	ui->Surname->setText(user.getSurname());
	ui->email->setText(user.getUsername());
}

UserWidget::~UserWidget() {
	delete ui;
}

void UserWidget::on_pushButton_Edit_clicked() {
	UserEditWidget uew;
	//connect(&uew, &Register::closeLogin, this, &Login::loginSuccessful);
	//client->connectToRegister(&regForm);
	uew.setModal(true);
	this->hide();
	uew.exec();
}

void UserWidget::on_pushButton_Cancel_clicked() {
	this->close();
}

QPixmap UserWidget::makeRoundImage(const QPixmap &orig) {
    // Getting size if the original picture is not square
    int size = qMin(orig.width(), orig.height());

    // Creating circle clip area
    QPixmap rounded = QPixmap(size, size);
    rounded.fill(Qt::transparent);
    QPainterPath path;
    path.addEllipse(rounded.rect());
    QPainter painter(&rounded);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setClipPath(path);

    // Filling rounded area if needed
    painter.fillRect(rounded.rect(), Qt::black);

    // Getting offsets if the original picture is not square
    int x = qAbs(orig.width() - size) / 2;
    int y = qAbs(orig.height() - size) / 2;
    painter.drawPixmap(-x, -y, orig.width(), orig.height(), orig);

    return rounded;
}
