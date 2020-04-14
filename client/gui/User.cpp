#include "user.h"
#include "ui_user.h"

User::User(QWidget *parent) : QDialog(parent), ui(new Ui::User) {
    ui->setupUi(this);
    this->setUserState(OFFLINE);
    this->setTimeBeforeInactive(300);
}

User::~User() { delete ui; }
/* GETTERS */
QString User::getName() { return this->name; }

QString User::getSurname() { return this->surname; }

QString User::getEmail() { return this->email; }

QString User::getNick() { return this->nickname; }

QIcon User::getProfilePic(){ return this->profilePic; }

QList<User*> User::getFriends() { return this->friendList; }

int User::getUserState() { return this->userState; }

int User::getActualSecondsBeforeInactive() { return this->secondsBeforeInactive; }

/* SETTERS */
void User::setName(QString _name) { this->name = _name; }

void User::setSurname(QString _surname) { this->surname = _surname; }

void User::setProfilePic(QIcon _profilePic) { this->profilePic = _profilePic; }

void User::setNick(QString _nick) { this->nickname = _nick; }

void User::setEmail(QString _email) { this->email = _email; }

void User::addFriend(User* _usrFriend) { this->friendList.append(_usrFriend); }

void User::setTimeBeforeInactive(int _timer){ this->secondsBeforeInactive = _timer; }

void User::setUserState(int _state) { this->userState = _state; }
