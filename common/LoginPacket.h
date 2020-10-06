//
// Created by Emanuele Munafò on 17/04/2020.
//

#pragma once

#include "Packet.h"
#include "PacketDef.h"
#include "User.h"

/** Login request packet **/
class LoginReqPacket : public Packet {
    friend PacketBuilder;
private:
    QString username;
    QString hashedPassword;

protected:
    LoginReqPacket();

    // Costruct LoginRequest message with the username of the account that is trying to setAsLogged
    void writePayload(QDataStream &stream) const override;

    void readPayload(QDataStream &stream) override;

public:
    LoginReqPacket(QString username, QString hashedPassword);

    ~LoginReqPacket() {};

    QString getUsername() const;

    QString gethashedPassword() const;
};

/** Login Success packet **/
class LoginOkPacket : public Packet {
    friend PacketBuilder;
private:
    User user;
    //TODO: add list of document accessible by that user
protected:
    LoginOkPacket();

    // Costruct LoginOk message with all the correct info retrieved from DB
    void writePayload(QDataStream &stream) const override;

    void readPayload(QDataStream &stream) override;

public:
    LoginOkPacket(User user);

    ~LoginOkPacket() {};

    User getUser() const;
    //TODO: add getter for list of document accessible by that user

};

/** Logout request packet **/
class LogoutReqPacket : public Packet {
    friend PacketBuilder;

protected:
    LogoutReqPacket();

    // Costruct LogoutRequest (no message is needed, maybe add some security field to avoid replay/ipspoofing?)
    void writePayload(QDataStream &stream) const override;

    void readPayload(QDataStream &stream) override;

public:
    ~LogoutReqPacket() {};

    QString getUsername() const;

    QString gethashedPassword() const;
};
