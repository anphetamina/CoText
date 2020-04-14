#include "Register.h"
#include "ui_register.h"
#include "UserWidget.h"


#include <QMessageBox>
#include <QIcon>
#include <QDebug>

Register::Register(QWidget *parent) : QDialog(parent), ui(new Ui::Register) {
    ui->setupUi(this);
    ui->dropBox->setAcceptDrops(true);
}

Register::~Register() {
    delete ui;
}



void Register::clearInput() {
    qDebug() << "Input cleared";
    ui->lineEdit_Name->text().clear();
    ui->lineEdit_Surname->text().clear();
    ui->lineEdit_Nick->text().clear();
    ui->lineEdit_Pass1->text().clear();
    ui->lineEdit_Pass2->text().clear();
    ui->lineEdit_email->text().clear();
}

void Register::dragEnterEvent(QDragEnterEvent *event) {
    qDebug() << "Dragging now...";
    if(event->mimeData()->hasImage()) {
        event->acceptProposedAction();
    } else {
        QMessageBox::warning(this, "Type Error", "Your file does not seem belong to any image format.");
    }

}

void Register::dropEvent(QDropEvent *event) {
    qDebug() << "Something has been dropped";

    //check if the proposedaction is acceptable
    //handle case if not
    //ex. QT::LinkAction ignore if not support links to external sources

    //before calling accept() override setDropAction() with preferred action from Qt::DropAction
    ////in this case replacement drop action is used instead of the proposed action
    if(event->mimeData()->hasImage()) {
        QIcon userIcon = qvariant_cast<QIcon>(event->mimeData()->imageData());
        this->profilePicture = userIcon;

    }else {
        QMessageBox::warning(this, "Type Error", "Your file does not seem belong to any image format.");
    }

}
void Register::on_pushButton_Register_clicked()
{
    clearInput();
    bool null = false;
    bool empty = false;
    name = ui->lineEdit_Name->text();
    surname = ui->lineEdit_Surname->text();
    nickname = ui->lineEdit_Nick->text();
    pass1 = ui->lineEdit_Pass1->text();
    pass2 = ui->lineEdit_Pass2->text();
    email = ui->lineEdit_email->text();





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
                //all is correct, register user into DB

                //1#. Fill User Object instance, set stautus on, start timer
                User *usr = new User();
                usr->setName(name);
                usr->setNick(nickname);
                usr->setSurname(surname);
                usr->setEmail(email);
                usr->setProfilePic(profilePicture);

                MainWindow::setUser(usr);
            }
        }
    }
}
