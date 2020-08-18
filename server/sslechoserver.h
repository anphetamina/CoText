//
// Created by Emanuele Munafò on 04/04/2020.
//

#ifndef SSLECHOSERVER_H
#define SSLECHOSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtNetwork/QSslError>
#include "Packet.h"
#include "Client.h"
#include "CursorPacket.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class SslEchoServer : public QObject
{
    Q_OBJECT
public:
    explicit SslEchoServer(quint16 port, QObject *parent = nullptr);
    ~SslEchoServer() override;

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
    void onSslErrors(const QList<QSslError> &errors);

private:
    QWebSocketServer *m_pWebSocketServer;
    //QList<QWebSocket *> m_clients;

    // Association of socket and client
    QMap<QWebSocket*, QSharedPointer<Client>> clientMapping;

    //Association of opened doc and user
    QMap<int, QList<QSharedPointer<Client>>> documentMapping;

    void packetParse(QByteArray rcvd_packet);


    void dispatch(PacketHandler rcvd_packet, QWebSocket* pClient);

    bool findAndDeleteFromDoclist(QSharedPointer<Client> client);

    void pruneOldConnectionsIfAny(QSharedPointer<Client> client, QWebSocket* pClient);

    void sendUpdatedOnlineUserByDocId(int docId);

    int getDocIdOpenedByUserId(int userId);
};

#endif //SSLECHOSERVER_H