//
// Created by Emanuele Munafò on 17/04/2020.
//

#include "LoginPacket.h"

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