#pragma once
#include "Packet.h"
#include "QSymbol.h"

class Message : public Packet {
    friend PacketBuilder;
private:
    int type;
    QSymbol qs;
    int siteId;

public:
    Message(int type, QSymbol qs, int siteId);

    int getType() const;

    QSymbol getQS() const;

    int getSiteId() const;

    int getCounter() const;

protected:
    Message();
    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

};