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

    void writePayload(QDataStream &stream) const override;

    void readPayload(QDataStream &stream) override;

};

class BigMessage : public Packet {
    friend PacketBuilder;
private:
    int type;
    QVector<QSymbol> qss;
    int siteId;

public:
    BigMessage(int type, QVector<QSymbol> qss, int siteId);

    int getType() const;

    QVector<QSymbol> getQSS() const;

    int getSiteId() const;

protected:
    BigMessage();

    void writePayload(QDataStream &stream) const override;

    void readPayload(QDataStream &stream) override;

};

class AlignMessage : public Packet {

    friend PacketBuilder;

    friend QDataStream &operator>>(QDataStream &in, AlignMessage &am);

    friend QDataStream &operator<<(QDataStream &out, const AlignMessage &am);

private:
    QSymbol positionStart;
    int delta = 0;
    Qt::Alignment alignment;
    int siteId;

public:
    AlignMessage();

    AlignMessage(QSymbol positionStart, int delta, Qt::Alignment alignment, int siteId);

    QSymbol getPositionStart() const;

    int getDelta() const;

    Qt::Alignment getAlignment() const;

    int getSiteId() const;

protected:

    void writePayload(QDataStream &stream) const override;

    void readPayload(QDataStream &stream) override;

};