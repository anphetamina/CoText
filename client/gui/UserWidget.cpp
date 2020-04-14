#include "UserWidget.h"
#include "ui_user.h"
#include "../User.h"

UserWidget::UserWidget(QWidget *parent) : QDialog(parent), ui(new Ui::UserWidget) {
    ui->setupUi(this);
    this->setUserState(OFFLINE);
    this->setTimeBeforeInactive(300);
}

UserWidget::~UserWidget() { delete ui; }
/* GETTERS */
QString UserWidget::getName() { return this->name; }

QString UserWidget::getSurname() { return this->surname; }

QString UserWidget::getEmail() { return this->email; }

QString UserWidget::getNick() { return this->nickname; }

QIcon UserWidget::getProfilePic(){ return this->profilePic; }

QList<User*> UserWidget::getFriends() { return this->friendList; }

int UserWidget::getUserState() { return this->userState; }

int UserWidget::getActualSecondsBeforeInactive() { return this->secondsBeforeInactive; }

/* SETTERS */
void UserWidget::setName(QString _name) { this->name = _name; }

void UserWidget::setSurname(QString _surname) { this->surname = _surname; }

void UserWidget::setProfilePic(QIcon _profilePic) { this->profilePic = _profilePic; }

void UserWidget::setNick(QString _nick) { this->nickname = _nick; }

void UserWidget::setEmail(QString _email) { this->email = _email; }

void UserWidget::addFriend(User* _usrFriend) { this->friendList.append(_usrFriend); }

void UserWidget::setTimeBeforeInactive(int _timer){ this->secondsBeforeInactive = _timer; }

void UserWidget::setUserState(int _state) { this->userState = _state; }
