//
// Created by Emanuele Munafò on 22/04/2020.
//
#pragma once

#include "Packet.h"

/** Account Creation request packet **/
class AccountCreationPacket : public Packet{
    friend PacketBuilder;
private:
    QString username;
    QString hashedPassword;
    QString name;
    QString surname;
    QIcon profilePic;

public:
    QString getUsername() const;
    QString getHashedPassword() const;
    QString getName() const;
    QString getSurname() const;
    QIcon getProfilePic() const;

protected:
    AccountCreationPacket();
    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;
public:
    AccountCreationPacket(QString username,QString password, QString name, QString surname, QIcon profilePic);
    ~AccountCreationPacket(){};

};

/** Account Update request packet **/
class AccountUpdatePacket : public Packet{
    friend PacketBuilder;

private:
    QString username;
    QString hashedPassword;
    QString name;
    QString surname;
    QIcon profilePic;
protected:
    AccountUpdatePacket();
    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;
public:
    AccountUpdatePacket(QString username,QString password, QString name, QString surname, QIcon profilePic);
    ~AccountUpdatePacket(){};
    QString getUsername() const;
    QString getHashedPassword() const;
    QString getName() const;
    QString getSurname() const;
    QIcon getProfilePic() const;
};

/** Account Creation or update success packet **/
class AccountOkPacket : public Packet {
    friend PacketBuilder;
private:
    User user;
    //TODO: add list of document accessible by that user
protected:
    AccountOkPacket();

    // Costruct AccountOkPacket message with all the correct info retrieved after (a successful) insert into DB
    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

public:
    AccountOkPacket(User user);
    ~AccountOkPacket() {};
    User getUser() const;
    //TODO: add getter for list of document accessible by that user
};