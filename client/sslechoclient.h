//
// Created by Emanuele Munafò on 04/04/2020.
//

#ifndef SSLECHOCLIENT_H
#define SSLECHOCLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QSslError>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>

QT_FORWARD_DECLARE_CLASS(QWebSocket)

class SslEchoClient : public QObject
{
Q_OBJECT
public:
    explicit SslEchoClient(const QUrl &url, QObject *parent = nullptr);

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onBinaryMessageReceived(QByteArray message);

    void onSslErrors(const QList<QSslError> &errors);

private:
    QWebSocket m_webSocket;

    void sendPing();

    void packetParse(QByteArray rcvd_packet);

    void sendLogin();
};

#endif // SSLECHOCLIENT_H