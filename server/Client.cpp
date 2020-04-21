//
// Created by Emanuele Munafò on 21/04/2020.
//

#include <QWebSocket>
#include "Client.h"


Client::Client(QWebSocket* s) :
        websocket(s), user(nullptr), loggedFlag(false), packetBuffer(PacketBuffer())
{
}

Client::~Client()
{
}

QWebSocket* Client::getSocket() const
{
    return websocket;

}

PacketBuffer& Client::getPacketBuffer()
{
    return packetBuffer;
}

User* Client::getUser() const
{
    return user;
}

// Usr get email wrapper
QString Client::getEmail() const
{
    if (user == nullptr)
        return QString::null;
    else return user->getEmail();
}


void Client::setAsLogged(User* user)
{
    loggedFlag = true;
    user = user;
}

void Client::logout()
{
    loggedFlag = false;
    user = nullptr;
}

bool Client::isLogged()
{
    return loggedFlag;
}

bool Client::authenticate(QByteArray token)
{
    return true;
}
