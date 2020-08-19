//
// Created by Emanuele Munafò on 20/04/2020.
//

#include "PingPacket.h"
#include "PacketDef.h"


/*************** LOGIN REQUEST MESSAGE ***************/

PingPacket::PingPacket()
        : Packet(PACK_TYPE_PING)
{
}

PingPacket::PingPacket(QString m_debug_msg)
        : Packet(PACK_TYPE_PING), m_debug_msg(m_debug_msg)
{
}

void PingPacket::writePayload(QDataStream& stream) const
{
    stream << m_debug_msg;
    // Compute payload length

}

void PingPacket::readPayload(QDataStream& stream)
{
    stream >> m_debug_msg;
}

QString PingPacket::getDebugMsg() const
{
    return m_debug_msg;
}
// TODO: add a setter for m_debug_msg