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
        user = nullptr;
        //delete user;
}

QWebSocket* Client::getSocket() const
{
    return websocket;

}

PacketBuffer& Client::getPacketBuffer()
{
    return packetBuffer;
}

User Client::getUser() const
{
    return *user;
}

// Usr get email wrapper
QString Client::getUsername() const
{
    if (user == nullptr)
        return QString::null;
    else return user->getUsername();
}


void Client::setAsLogged(std::unique_ptr<User> &user)
{
    loggedFlag = true;
    this->user = std::move(user);
}

void Client::logout()
{
    loggedFlag = false;

    //if (user != nullptr)
    //    delete user;

    user = nullptr;
}

bool Client::isLogged()
{
    return loggedFlag;
}

int Client::getUserId() const
{
    if (user == nullptr)
        return -1;
    else
        return this->user->getId();
}

bool Client::authenticate(QByteArray token)
{
    return true;
}
