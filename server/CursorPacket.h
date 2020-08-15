//
// Created by Emanuele Munafò on 17/04/2020.
//

#pragma once

#include "Packet.h"
#include "PacketDef.h"

/** Cursor packet **/
class CursorPacket : public Packet {
    friend PacketBuilder;
private:
    qint32 userId;
    qint32 newPosition;

protected:
    CursorPacket();

    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

public:
    CursorPacket(qint32 userId, qint32 newPosition);
    ~CursorPacket() {};
    qint32 getuserId() const;
    qint32 getnewPosition() const;
};
