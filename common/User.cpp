//
// Created by Emanuele Munafò on 13/04/2020.
//
#include "User.h"
#include <QDebug>

User::User() {

}

User::User(int id, QString username, QString name, QString surname) : id(id), username(username), name(name),
                                                                   surname(surname) {};

User::~User() {}

/* GETTERS */
QString User::getName() { return this->name; }

QString User::getSurname() { return this->surname; }

QString User::getUsername() { return this->username; }

int User::getId() const { return this->id; }

QString User::getPassword()  { return this->hashedPsw; }

QImage User::getProfilePic() { return this->profilePic; }

QList<User *> User::getFriends() { return this->friendList; }

int User::getUserState() { return this->userState; }

int User::getActualSecondsBeforeInactive() { return this->secondsBeforeInactive; }

int User::getSignature() { return this->signature; }

bool User::isLogged() { return this->id > 0 ? 1 : 0; }


/* SETTERS */
void User::setName(QString _name) { this->name = _name; }

void User::setSurname(QString _surname) { this->surname = _surname; }

void User::setProfilePic(QImage _profilePic) { this->profilePic = _profilePic; }

void User::setPassword(QString _hashedPassword) { this->hashedPsw = _hashedPassword; }

void User::setUsername(QString _email) { this->username = _email; }

void User::setId(int _id) { this->id = _id; }

void User::addFriend(User *_usrFriend) { this->friendList.append(_usrFriend); }

void User::setTimeBeforeInactive(int _timer) { this->secondsBeforeInactive = _timer; }

void User::setUserState(int _state) { this->userState = _state; }

void User::setSignature(uint8_t signature) { this->signature = signature; }

QDataStream &operator>>(QDataStream &in, User &user) {
    in >> user.id >> user.name >> user.surname >> user.username
       >> user.profilePic
       >> user.hashedPsw
       >> user.userState
       >> user.secondsBeforeInactive
       >> user.userState
       >> user.signature;

    return in;
}

QDataStream &operator<<(QDataStream &out, const User &user) {
    out << user.id << user.name << user.surname << user.username
        << user.profilePic
        << user.hashedPsw
        << user.userState
        << user.secondsBeforeInactive
        << user.userState
        << user.signature;
    return out;
}

bool User::operator== (User u) const{
    return id==u.id && name == u.name && surname == u.surname && username == u.username;
}


