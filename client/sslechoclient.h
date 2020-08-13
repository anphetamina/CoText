//
// Created by Emanuele Munafò on 04/04/2020.
//

#pragma once

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QSslError>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include "Packet.h"
#include "Message.h"
#include "gui/TextEditor.h"

QT_FORWARD_DECLARE_CLASS(QWebSocket)

class SslEchoClient : public QObject
{
Q_OBJECT
public:
    explicit SslEchoClient(const QUrl &url, QObject *parent = nullptr);

signals:

    void insertReceived(Symbol symbol);
    void eraseReceived(Symbol symbol);

public slots:

    void sendInsert(std::vector<Symbol> symbols, int siteId);
    void sendErase(std::vector<Symbol> symbols, int siteId);

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onBinaryMessageReceived(QByteArray message);

    void onSslErrors(const QList<QSslError> &errors);

private:
    QWebSocket m_webSocket;

    void sendPing();

    QWebSocket *pServer;

    void packetParse(QByteArray rcvd_packet);

    void sendLogin();

    void sendTest();

    void authenticate(QString username, QString password);

    void dispatch(PacketHandler rcvd_packet, QWebSocket *pClient);

public:
    void connectToEditor(TextEditor* te);

};

