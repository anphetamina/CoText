//
// Created by Emanuele Munafò on 21/04/2020.
//

#pragma once

#include "../common/Packet.h"
#include "User.h"


class Client
{

private:

    QWebSocket* websocket;
    User* user;
    bool loggedFlag;

    PacketBuffer packetBuffer;

public:

    Client(QWebSocket* s);

    ~Client();

    void setAsLogged(User* user);
    void logout();
    bool isLogged();

    bool authenticate(QByteArray token);

    /* getters */
    User getUser() const;
    int getUserId() const;
    QString getUsername() const;
    QWebSocket* getSocket() const;
    PacketBuffer& getPacketBuffer();
};
