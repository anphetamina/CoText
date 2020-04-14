//
// Created by Emanuele Munafò on 13/04/2020.
//
#include "User.h"


User::User() {

}

User::~User() { }
/* GETTERS */
QString User::getName() { return this->name; }

QString User::getSurname() { return this->surname; }

QString User::getEmail() { return this->email; }

QString User::getNick() { return this->nickname; }

QIcon User::getProfilePic(){ return this->profilePic; }

QList<User*> User::getFriends() { return this->friendList; }

int User::getUserState() { return this->userState; }

int User::getActualSecondsBeforeInactive() { return this->secondsBeforeInactive; }
int User::getSignature() { return this->_signature; }


/* SETTERS */
void User::setName(QString _name) { this->name = _name; }

void User::setSurname(QString _surname) { this->surname = _surname; }

void User::setProfilePic(QIcon _profilePic) { this->profilePic = _profilePic; }

void User::setNick(QString _nick) { this->nickname = _nick; }

void User::setEmail(QString _email) { this->email = _email; }

void User::addFriend(User* _usrFriend) { this->friendList.append(_usrFriend); }

void User::setTimeBeforeInactive(int _timer){ this->secondsBeforeInactive = _timer; }

void User::setUserState(int _state) { this->userState = _state; }

void User::setSignature(uint8_t signature) { this->_signature = signature; }