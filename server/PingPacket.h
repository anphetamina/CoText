//
// Created by Emanuele Munafò on 20/04/2020.
//
#pragma once

#include "Packet.h"

class PingPacket : public Packet {
    friend PacketBuilder;//or use public costructor
private:
    QString m_debug_msg;

protected:

    PingPacket();

    // Costruct LoginRequest message with the username of the account that is trying to setAsLogged

    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

public:
    PingPacket(QString m_debug_msg);
    ~PingPacket() {};
    QString getDebugMsg() const;
};

