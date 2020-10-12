//
// Created by Emanuele Munafò on 22/04/2020.
//

#include "AccountPacket.h"
#include "PacketDef.h"

/** Account Creation Request packet **/
AccountCreationPacket::AccountCreationPacket() : Packet(PACK_TYPE_ACC_CREATE) {}

AccountCreationPacket::AccountCreationPacket(QString username, QString password, QString name, QString surname,
                                             QImage profilePic) : Packet(PACK_TYPE_ACC_CREATE), username(username),
                                                                 hashedPassword(password), name(name),
                                                                 surname(surname), profilePic(profilePic) {};


QString AccountCreationPacket::getUsername() const {
    return username;
}

QString AccountCreationPacket::getHashedPassword() const {
    return hashedPassword;
}

QString AccountCreationPacket::getName() const {
    return name;
}

QString AccountCreationPacket::getSurname() const {
    return surname;
}

QImage AccountCreationPacket::getProfilePic() const {
    return profilePic;
};

void AccountCreationPacket::writePayload(QDataStream &stream) const {
    stream << username << hashedPassword << name << surname << profilePic;
}

void AccountCreationPacket::readPayload(QDataStream &stream) {
    stream >> username >> hashedPassword >> name >> surname >> profilePic;
}

/** Account Update Request packet **/
// TOIMPROVE: Maybe an aCCount packet and child class AccountUpdate and AccountCreate?
AccountUpdatePacket::AccountUpdatePacket() : Packet(PACK_TYPE_ACC_UPDATE) {};

AccountUpdatePacket::AccountUpdatePacket(QString username, QString password, QString name, QString surname,
                                         QImage profilePic) : Packet(PACK_TYPE_ACC_UPDATE), username(username),
                                                             hashedPassword(hashedPassword), name(name),
                                                             surname(surname), profilePic(profilePic) {};

QString AccountUpdatePacket::getUsername() const {
    return username;
}

QString AccountUpdatePacket::getHashedPassword() const {
    return hashedPassword;
}

QString AccountUpdatePacket::getName() const {
    return name;
}

QString AccountUpdatePacket::getSurname() const {
    return surname;
}

QImage AccountUpdatePacket::getProfilePic() const {
    return profilePic;
};

void AccountUpdatePacket::writePayload(QDataStream &stream) const {
    stream << username << hashedPassword << name << surname << profilePic;
}

void AccountUpdatePacket::readPayload(QDataStream &stream) {
    stream >> username >> hashedPassword >> name >> surname >> profilePic;
}

/** Account Creation/Update Success packet **/
AccountOkPacket::AccountOkPacket() : Packet(PACK_TYPE_ACC_OK) {};

AccountOkPacket::AccountOkPacket(User user) : Packet(PACK_TYPE_ACC_OK), user(user) {};

void AccountOkPacket::writePayload(QDataStream &stream) const {
    stream << user;
}

void AccountOkPacket::readPayload(QDataStream &stream) {
    stream >> user;
}

User AccountOkPacket::getUser() const {
    return user;
}