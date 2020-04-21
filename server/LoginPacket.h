//
// Created by Emanuele Munafò on 17/04/2020.
//

#pragma once

#include "PacketDef.h"
#include "User.h"
#include "Packet.h"

class LoginReqPacket : public Packet {
    friend PacketBuilder;
private:
    QString username;
    QString hashedPassword;

protected:
    LoginReqPacket();

    // Costruct LoginRequest message with the username of the account that is trying to setAsLogged
    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

public:
    LoginReqPacket(QString username, QString hashedPassword);
    ~LoginReqPacket() {};
    QString getUsername() const;
    QString gethashedPassword() const;
};

