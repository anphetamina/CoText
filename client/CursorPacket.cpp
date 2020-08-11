//
// Created by Emanuele Munafò on 22/04/2020.
//

#include "CursorPacket.h"
#include "PacketDef.h"

/** Cursor packet **/
CursorPacket::CursorPacket(): Packet(PACK_TYPE_CURSOR_POS){}
CursorPacket::CursorPacket(qint32 userId, qint32 newPosition): Packet(PACK_TYPE_CURSOR_POS), userId(userId),newPosition(newPosition){};



QString CursorPacket::getuserId() const {
    return userId;
}

QString CursorPacket::getnewPosition() const {
    return newPosition;
}

void CursorPacket::writePayload(QDataStream& stream) const
{
    stream << userId << newPosition;
}

void CursorPacket::readPayload(QDataStream& stream)
{
    stream >> userId >> newPosition;
}
