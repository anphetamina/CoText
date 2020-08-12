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
    if (user != nullptr)
        delete user;
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
    this->user = user;
}

void Client::logout()
{
    loggedFlag = false;

    if (user != nullptr)
        delete user;

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
