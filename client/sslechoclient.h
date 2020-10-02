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
#include "../common/Packet.h"
#include "../common/Message.h"
#include "../common/CursorPacket.h"
#include "../common/DocumentPacket.h"
#include "TextEditor.h"
#include "MainWindow.h"
#include "Login.h"

QT_FORWARD_DECLARE_CLASS(QWebSocket)

class SslEchoClient : public QObject
{
Q_OBJECT
public:
    explicit SslEchoClient(const QUrl &url, QObject *parent = nullptr);
signals:

    void insertReceived(QSymbol qsymbol);
    void eraseReceived(QSymbol symbol);
    void insertBlockReceived(std::vector<QSymbol> symbols);
    void eraseBlockReceived(std::vector<QSymbol> symbols);
    void updateCursorReceived(int userId, int position);

    void updateUserListReceived(QVector<User> userlist);
    void auth(User loggedUser);
    void documentReceived(int docId, QString docName, std::vector<std::vector<QSymbol>> qsymbols);

signals:
    void loginSuccessful();
    void loginFailed();
public slots:

    void sendInsert(std::vector<QSymbol> symbols, int siteId);
    void sendErase(std::vector<QSymbol> symbols, int siteId);
    void sendCursor(int userId, int position);

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onBinaryMessageReceived(QByteArray message);

    void onSslErrors(const QList<QSslError> &errors);

private:
    QString username, password;
    QWebSocket m_webSocket;

    void sendPing();

    QWebSocket *pServer;

    void packetParse(QByteArray rcvd_packet);

    void sendTest();

    void dispatch(PacketHandler rcvd_packet, QWebSocket *pClient);

public:
    void connectToEditor(TextEditor* te);
    void connectToMainWindow(MainWindow* mw);

    void sendDocOpen(QString docName, qint32 userId);
    void authenticate(QString username, QString password);

    void set_username(QString username);

    void set_password(QString password);

    void sendLogin();

    void socketDisconnected();

    void connectToLoginWindow(Login *login, MainWindow *mw);
};

