//
// Created by Emanuele Munafò on 22/04/2020.
//

#pragma once

#include "Packet.h"

/** Account Creation request packet **/
class AccountCreationPacket : public Packet {
    friend PacketBuilder;
private:
    QString username;
    QString hashedPassword;
    QString name;
    QString surname;
    QImage profilePic;

public:
    QString getUsername() const;

    QString getHashedPassword() const;

    QString getName() const;

    QString getSurname() const;

    QImage getProfilePic() const;

protected:
    AccountCreationPacket();

    void writePayload(QDataStream &stream) const override;

    void readPayload(QDataStream &stream) override;

public:
    AccountCreationPacket(QString username, QString password, QString name, QString surname, QImage profilePic);

    ~AccountCreationPacket() {};

};

/** Account Update request packet **/
class AccountUpdatePacket : public Packet {
    friend PacketBuilder;

private:
    int id;
    QString username;
    QString hashedPassword;
    QString name;
    QString surname;
    QImage profilePic;
protected:
    AccountUpdatePacket();

    void writePayload(QDataStream &stream) const override;

    void readPayload(QDataStream &stream) override;

public:
    AccountUpdatePacket(int id, QString username, QString password, QString name, QString surname, QImage profilePic);

    ~AccountUpdatePacket() {};

    QString getUsername() const;

    QString getHashedPassword() const;

    QString getName() const;

    QString getSurname() const;

    QImage getProfilePic() const;

    int getId() const;
};

/** Account Creation or update success packet **/
class AccountOkPacket : public Packet {
    friend PacketBuilder;
private:
    User user;
protected:
    AccountOkPacket();

    // Costruct AccountOkPacket message with all the correct info retrieved after (a successful) insert into DB
    void writePayload(QDataStream &stream) const override;

    void readPayload(QDataStream &stream) override;

public:
    AccountOkPacket(User user);

    ~AccountOkPacket() {};

    User getUser() const;
};