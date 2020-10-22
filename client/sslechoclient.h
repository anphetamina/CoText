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
#include "Register.h"
#include "usereditwidget.h"

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
    void updateAlignmentReceived(Qt::Alignment alignment, QSymbol symbol);
    void updateUserListReceived(QVector<User> onlineUserList, QVector<User> completeUserList);
    void auth(User loggedUser);
    void documentReceived(int docId, QString docName, std::vector<std::vector<QSymbol>> qsymbols);
    void documentNameReceived(QString title);
    void loginSuccessfulReceived();
    void loginFailedReceived();
    void askUriReceived(QString URI);
    void documentListReceived(QVector<QString> documentList);
    void loginFailed();
    void registerSuccessfulReceived();
    void registerFailedReceived();
    void updateSuccessfulReceived();

public slots:
    void sendInsert(std::vector<QSymbol> symbols, int siteId);
    void sendErase(std::vector<QSymbol> symbols, int siteId);
    void sendCursor(int userId, int position);
    void sendAskUri(qint32 userId, int docId, QString invCode);
    void sendAlignment(Qt::Alignment alignment, QSymbol symbol, int siteId);
    void sendAskDocList(qint32 userId);
    void sendDocCreate(QString docName, qint32 userId);
    void quitApp();
    void sendDocumentDeletedSlot(QString docName, quint32 userId);

private Q_SLOTS:
    void onConnected();

    void onTextMessageReceived(QString message);
    void onBinaryMessageReceived(QByteArray message);
    void onSslErrors(const QList<QSslError> &errors);

    void onError(QAbstractSocket::SocketError error);

private:
    QString username, password;
    QWebSocket m_webSocket;
    QWebSocket *pServer;

    void sendPing();
    int loginAttemptCount=0;
    void packetParse(QByteArray rcvd_packet);
    void sendTest();
    void dispatch(PacketHandler rcvd_packet, QWebSocket *pClient);

public:
    void connectToEditor(TextEditor* te);
    void connectToMainWindow(MainWindow* mw);
    void connectToRegister(Register* r);
	void connectToUserEdit(UserEditWidget* uew);
    void sendDocOpen(QString docName, qint32 userId);
    void authenticate(QString username, QString password);
    void set_username(QString username);
    void set_password(QString password);
    void sendLogin();
    void socketDisconnected();
    void connectToLogin(Login *login);
    bool isConnected();
    int getLoginAttemptCount();
    void sendRegistration(QString name, QString surname, QString username, QString nickname, QString password, QImage profilePic);
    void registerUser(QString name, QString surname, QString username, QString nickname, QString password, QImage profilePic);
    void sendUpdateProfile(int uID, QString name, QString surname, QString email, QString password, QImage newPP);
    
    void updateUser(int uID, QString name, QString surname, QString email, QString password, QImage newPP);
};

