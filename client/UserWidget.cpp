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
	ui->label_profilpic->setMaximumWidth(100);
	ui->label_profilpic->setMaximumHeight(100);
	ui->Name->setText(user.getName());
	ui->Surname->setText(user.getSurname());
	ui->email->setText(user.getEmail());
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
    painter.setClipPath(path);

    // Filling rounded area if needed
    painter.fillRect(rounded.rect(), Qt::black);

    // Getting offsets if the original picture is not square
    int x = qAbs(orig.width() - size) / 2;
    int y = qAbs(orig.height() - size) / 2;
    painter.drawPixmap(-x, -y, orig.width(), orig.height(), orig);

    /*QPixmap background = QPixmap(size + 50, size + 50);
    background.fill(Qt::transparent);
    QPainterPath path1;
    path1.addEllipse(background.rect());
    QPainter painter1(&background);
    painter1.setClipPath(path1);

    // Filling rounded area if needed
    painter1.fillRect(background.rect(), color);

    // Getting offsets if the original picture is not square
    x = qAbs(rounded.width() - size - 50) / 2;
    y = qAbs(rounded.height() - size - 50) / 2;
    painter1.drawPixmap(x, y, rounded.width(), rounded.height(), rounded);*/

    return rounded;
}
