#pragma once
#include "Symbol.h"
#include "Packet.h"
#include "QSymbol.h"

class Message : public Packet{
    friend PacketBuilder;
private:
    int type;
    //Symbol s;
    QSymbol qs;
    int siteId;
public:
    Message(int type, QSymbol qs, int siteId);
    Message(int type, Symbol s, int siteId);

    int getType() const;

    Symbol getS() ;
    QSymbol getQS() const;

    int getSiteId() const;

    int getCounter() const;
    ~Message() {};
protected:
    Message();
    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

};