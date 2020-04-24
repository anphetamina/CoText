//
// Created by Emanuele Munafò on 17/04/2020.
//

#include "LoginPacket.h"
#include "Packet.h"


/** Login request packet **/
LoginReqPacket::LoginReqPacket(): Packet(PACK_TYPE_LOGIN_REQ){};

LoginReqPacket::LoginReqPacket(QString username, QString hashedPassword): Packet(PACK_TYPE_LOGIN_REQ), username(username),hashedPassword(hashedPassword){};

void LoginReqPacket::writePayload(QDataStream& stream) const
{
    stream << username << hashedPassword;
}

void LoginReqPacket::readPayload(QDataStream& stream)
{
    stream >> username >> hashedPassword;
}

QString LoginReqPacket::getUsername() const
{
    return username;
}
QString LoginReqPacket::gethashedPassword() const
{
    return hashedPassword;
}

/** Login Success packet **/
LoginOkPacket::LoginOkPacket(): Packet(PACK_TYPE_LOGIN_OK){};

LoginOkPacket::LoginOkPacket(User user): Packet(PACK_TYPE_LOGIN_OK), user(user){};

void LoginOkPacket::writePayload(QDataStream& stream) const
{
    stream << user;
}

void LoginOkPacket::readPayload(QDataStream& stream)
{
    stream >> user;
}

User LoginOkPacket::getUser() const
{
    return user;
}


/** Logout request packet **/
LogoutReqPacket::LogoutReqPacket(): Packet(PACK_TYPE_LOGOUT_REQ){};

void LogoutReqPacket::writePayload(QDataStream& stream) const
{
}

void LogoutReqPacket::readPayload(QDataStream& stream)
{
}
