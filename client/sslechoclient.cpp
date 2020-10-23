//
// Created by Emanuele Munafò on 04/04/2020.
//

#include "sslechoclient.h"
#include "../common/PingPacket.h"
#include "../common/LoginPacket.h"
#include "Login.h"
#include "Register.h"
#include "usereditwidget.h"
#include "ServerDisconnected.h"
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>
#include <AccountPacket.h>
#include <Benchmark.h>

QT_USE_NAMESPACE

//! [constructor]
 SslEchoClient::SslEchoClient(const QUrl &url, QObject* parent) :
        QObject(parent)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &SslEchoClient::onConnected);
    connect(&m_webSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),
            this, &SslEchoClient::onSslErrors);
    connect(&m_webSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));

    m_webSocket.open(QUrl(url));
}
//! [constructor]

//! [onConnected]
void SslEchoClient::onConnected()
{
    qDebug() << "WebSocket connected";
    // Set callback for text message
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &SslEchoClient::onTextMessageReceived);

    // And set callback for binary msg
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived,
            this, &SslEchoClient::onBinaryMessageReceived);

    connect(&m_webSocket, &QWebSocket::disconnected, this, &SslEchoClient::socketDisconnected);

    this->sendPing();
    //this->sendTest();
}
//! [onConnected]

//! [onTextMessageReceived]
void SslEchoClient::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message;
}

//! [onTextMessageReceived]
void SslEchoClient::onBinaryMessageReceived(QByteArray message)
{
    //qDebug() << "Message received:" << message;
    this->packetParse(message);
}

void SslEchoClient::onSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors);
    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root to the CA store.
    m_webSocket.ignoreSslErrors();
}
//! [onTextMessageReceived]
void SslEchoClient::onError(QAbstractSocket::SocketError error)
{
    std::cout << error << std::endl;
}

//! [socketDisconnected]
void SslEchoClient::socketDisconnected()
{
    qDebug() << "Server closed the connection.\n[HINT]Duplicated instance with the same user?";
    ServerDisconnected* serverDisconnected = new ServerDisconnected();
    connect(serverDisconnected, &ServerDisconnected::quitClicked, this, &SslEchoClient::quitApp);
    serverDisconnected->setWindowTitle("Server disconnected");
    serverDisconnected->setModal(true);
    serverDisconnected->exec();
}

void SslEchoClient::quitApp(){
    qApp->exit(-2);
}

void SslEchoClient::sendPing() {
    // Create buffer
    PingPacket pp = PingPacket("test");

    pp.send(m_webSocket);
    qDebug() << "Ping sent";
}

void SslEchoClient::sendTest() {
    qDebug() << "[NETWORK] ** Network Test Start ** ";
    this->sendLogin();
    qDebug() << "[NETWORK] ** Sending Message packet ** ";
    std::vector<Identifier> sym_position;
    QString test("test_qstring");
    QChar qc = test.at(0);//t
    QTextCharFormat cf = QTextCharFormat();
    QSymbol qs = QSymbol(qc, test, sym_position, cf);
    Message msg = Message(MSG_INSERT_SYM, qs, 3);
    //msg.send(m_webSocket);
    qDebug() << "[NETWORK] ** Network Test Packet were all sent ** ";
}

void SslEchoClient::set_username(QString username){
    this->username = username;
}
void SslEchoClient::set_password(QString password){
    this->password = password;
}
void SslEchoClient::authenticate(QString username, QString password) {
    loginAttemptCount++;
    qDebug() << "[NETWORK] ** Sending login req packet ** ";
    QString hashedPassword = password;
    LoginReqPacket lrp = LoginReqPacket(username, hashedPassword);
    lrp.send(m_webSocket);
}
void SslEchoClient::sendLogin(){
    qDebug() << "sendLogin username = "<<username<<" password = "<<password;
    this->authenticate(username, password);
}

void SslEchoClient::registerUser(QString name, QString surname, QString username, QString nickname, QString password, QImage profilePic){
	qDebug() << "[NETWORK] ** Sending registerReqPacket : AccountCreationPacket()";
	QString hashedPassword = password;
	AccountCreationPacket acp = AccountCreationPacket(username, hashedPassword, name, surname, profilePic);
	acp.send(m_webSocket);
}

void SslEchoClient::sendRegistration(QString _name, QString _surname, QString _username, QString _nickname, QString _password, QImage _profilePic) {
	//username is the field Email
	this->registerUser(_name, _surname, _username, _nickname, _password, _profilePic);
}

void SslEchoClient::sendUpdateProfile(int uID, QString name, QString surname, QString email, QString password,
                                      QImage newPP) {
	
	this->updateUser(uID, name, surname, email, password, newPP);
}

void SslEchoClient::updateUser(int uID, QString name, QString surname, QString email, QString password, QImage newPP) {

	AccountUpdatePacket aup = AccountUpdatePacket(uID, email, password, name, surname, newPP);
	
}

int SslEchoClient::getLoginAttemptCount(){
    return this->loginAttemptCount;
}


void SslEchoClient::packetParse(QByteArray rcvd_packet) {

    // Create a new packet buffer (used to w8 and receive for the full packet)
    PacketBuffer* pBuffer = new PacketBuffer();
    //qDebug() << rcvd_packet;
    //Create a data stream (used to deserialize the rcvd bytearray  to a structured packet)
    QDataStream streamRcv(&rcvd_packet, QIODevice::ReadOnly);

    // If the packet buffer is empty parse (deserialize) the headers field (MAGIC_VAL|Flags|type|payloadLen)
    if (pBuffer->getDataSize() == 0) {
        streamRcv >> *pBuffer;
    }
    QByteArray payload = rcvd_packet.mid(4+sizeof(quint32));//header+Payoadlen skip
    pBuffer->append(payload);

    if (pBuffer->isComplete()) {

        QDataStream dataStream(pBuffer->bufferPtr(), QIODevice::ReadWrite);
        quint8 mType = (quint8) pBuffer->getType();

        try {
            // Create an empty packet and read the fields by deserializing the data stream into a structured Packet
            PacketHandler packetH = PacketBuilder::Container(mType);
            packetH->read(dataStream);
            // Clear the buffer when a full packet is received (we are ready for the next one!)
            pBuffer->clearBuffer();

            if (mType == PACK_TYPE_PING || mType <= PACK_TYPE_LAST_CODE) {
//                qDebug() << "---";
//                qDebug() << "[INFO] Parsed new packet. Type: " << mType;
                QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
                dispatch(packetH, pClient);
            } else {
                qDebug() << "---";
                qDebug() << "[ERROR] Unknown packet type!\nUknType: " << mType;
            }
        }
        catch (std::exception me)//(MyException& me)
        {
            qDebug() << me.what();
            pBuffer->clearBuffer();
            //socketAbort(m_webSocket);				// Terminate connection with the client
        }
    }
}

void SslEchoClient::dispatch(PacketHandler rcvd_packet, QWebSocket* pClient) {
    //qDebug() << rcvd_packet.get();  // print packet as hex
    //qDebug() << "New packet type= " << rcvd_packet->getType();
    switch (rcvd_packet->getType()) {

        // Remeber to add {} scope to avoid jump from switch compilation error
        case (PACK_TYPE_PING): {
            PingPacket *ping = dynamic_cast<PingPacket *>(rcvd_packet.get());
            qDebug() << "[PING] Debug text: " << ping->getDebugMsg();
            break;
        }

        case(PACK_TYPE_LOGIN_OK): {
            LoginOkPacket* loginOk = dynamic_cast<LoginOkPacket*>(rcvd_packet.get());
            User loggedUser = loginOk->getUser();
            if ( loggedUser.isLogged() ){
                qDebug() << "[AUTH] Logged in as: " << loggedUser.getEmail();
                emit loginSuccessfulReceived();
            }
            else {
                qDebug() << "[AUTH] FAILED. See the server for the log.";
                emit loginFailedReceived();
            }
            pServer = qobject_cast<QWebSocket *>(sender());
	        //emit auth(loggedUser);
	        user = loggedUser;

	       // qDebug() << "USER LOGGED " << user.getId() << " " << user.getEmail() << " profilePic = "<<user.getProfilePic();

	        break;
        }
        
        case(PACK_TYPE_ACC_OK): {

        	AccountOkPacket* registerOk = dynamic_cast<AccountOkPacket*>(rcvd_packet.get());
        	User registeredUser = registerOk->getUser();
        	if(registeredUser.isLogged()) {
        		qDebug() << "[REGISTER AUTH] Registered as: " << registeredUser.getEmail();
        		sendLogin();
		        emit registerSuccessfulReceived();
        	} else {
        		qDebug() << "[REGISTER AUTH] FAILED. See the server for the log";
		        emit registerFailedReceived();
        	}

        	break;
	        
        }
        
	    case(PACK_TYPE_ACC_UPDATE): {
		    AccountUpdatePacket* updateOk = dynamic_cast<AccountUpdatePacket*>(rcvd_packet.get());
		    User loggedUser;
		    loggedUser.setEmail(updateOk->getUsername());
		    loggedUser.setName(updateOk->getName());
		    loggedUser.setSurname(updateOk->getSurname());
		    loggedUser.setProfilePic(updateOk->getProfilePic());
		    
		    if(loggedUser.isLogged()) {
			    qDebug() << "[REGISTER AUTH] Logged in as: " << loggedUser.getEmail();
			    emit updateSuccessfulReceived();
		    }
		    
		    pServer = qobject_cast<QWebSocket *>(sender());
		    user = loggedUser;
		    break;
	    }

        case (PACK_TYPE_MSG): {
            Message *msg = dynamic_cast<Message *>(rcvd_packet.get());
            qDebug() << "PACK_TYPE_MSG messaggio ricevuto: siteID:" << msg->getSiteId() << " char: " << msg->getQS().getC();
            switch (msg->getType()) {
                case(MSG_INSERT_SYM): {
                    emit insertReceived(msg->getQS());
                    qDebug() << "received add symbol";
                    break;
                }

                case(MSG_ERASE_SYM): {
                    emit eraseReceived(msg->getQS());
                    qDebug() << "received del symbol";
                    break;
                }
            }
            break;
        }
        case (PACK_TYPE_BIGMSG): {
            BigMessage *msg = dynamic_cast<BigMessage *>(rcvd_packet.get());
            switch (msg->getType()) {
                case(MSG_INSERT_SYM): {
                    QVector<QSymbol> syms = msg->getQSS();
                    std::vector<QSymbol> symbols(syms.begin(), syms.end());
                    qDebug() << "received add block";
                    emit insertBlockReceived(symbols);
                    break;
                }

                case(MSG_ERASE_SYM): {
                    QVector<QSymbol> syms = msg->getQSS();
                    std::vector<QSymbol> symbols(syms.begin(), syms.end());
                    qDebug() << "received del block";
                    emit eraseBlockReceived(symbols);
                    break;
                }
            }
            break;
        }
        case (PACK_TYPE_ALIGN): {
            AlignMessage *am = dynamic_cast<AlignMessage *>(rcvd_packet.get());
            emit updateAlignmentReceived(am->getAlignment(), am->getPositionStart());
            break;
        }
        case (PACK_TYPE_CURSOR_POS): {
            CursorPacket *cp = dynamic_cast<CursorPacket *>(rcvd_packet.get());
            emit updateCursorReceived(cp->getuserId(), cp->getnewPosition());
            break;
        }
        case (PACK_TYPE_DOC_OK): {
            DocumentOkPacket *doc = dynamic_cast<DocumentOkPacket *>(rcvd_packet.get());
            if(doc->getdocId() > 0) {
                qDebug() << "[OPEN_DOC] (" << doc->getdocName() << ") with id " << doc->getdocId();
                QVector<QVector<QSymbol>> qsymbols = doc->getqsymbols();
                std::vector<std::vector<QSymbol>> symbols  = toVector(qsymbols);
                emit documentNameReceived(doc->getdocName());
                emit documentReceived(doc->getdocId(), doc->getdocName(), symbols);
                emit joinSucceeded();
                emit joinSucceeded();
            } else if (doc->getdocId() == PACK_TYPE_DOC_INV_ERROR){
                emit joinFailed();
            }else if (doc->getdocId() == PACK_TYPE_DOC_OPEN_ERROR){
                emit openDocFailed();
                qDebug() << "[OPEN_DOC] FAILED (No permission for " << doc->getdocName() << ") with docId " << doc->getdocId();
            }
            break;
        }
        case (PACK_TYPE_DOC_LIST): {
            DocumentListPacket *docList = dynamic_cast<DocumentListPacket *>(rcvd_packet.get());
            qDebug() << "[DOC_LIST] Received : " << docList->getdocList();
            emit(documentListReceived(docList->getdocList()));
            break;
        }
        case (PACK_TYPE_DOC_ASKSURI): {
            // When a client receive this it means it was a response to an invite for ANOTHER CLIENT (that will SEND **NOT receive** a similar packet
            DocumentAskSharableURIPacket *docInvite = dynamic_cast<DocumentAskSharableURIPacket *>(rcvd_packet.get());
            emit(askUriReceived(docInvite->getURI()));
            break;
        }
        case (PACK_TYPE_DOC_USERLIST): {
            // When a client receive this it means that some user just went online/offline
            DocumentBeaconOnlineUsers *bou = dynamic_cast<DocumentBeaconOnlineUsers *>(rcvd_packet.get());
            qDebug() << "[DOC] Online userlist updated for DocId: " << bou->getdocId();
            emit updateUserListReceived(bou->getOnlineUserList(), bou->getCompleteUserList());
            break;
        }
    }
}

void SslEchoClient::sendInsert(std::vector<QSymbol> symbols, int siteId) {
    if (symbols.size() > 1) {
        QVector<QSymbol> syms(symbols.begin(), symbols.end());
        BigMessage msg = BigMessage(MSG_INSERT_SYM, syms, siteId);
        msg.send(*pServer);
        qDebug() << "sent add block";
    } else {
        QSymbol symbol = symbols.front();
        Message msg = Message(MSG_INSERT_SYM, symbol, siteId);
        msg.send(*pServer);
        qDebug() << "sent add " << ((symbol.isNewLine()) ? "LF" : QString(symbol.getC()));
    }
}

void SslEchoClient::sendErase(std::vector<QSymbol> symbols, int siteId) {
    if (symbols.size() > 1) {
        QVector<QSymbol> syms(symbols.begin(), symbols.end());
        BigMessage msg = BigMessage(MSG_ERASE_SYM, syms, siteId);
        msg.send(*pServer);
        qDebug() << "sent del block";
    } else {
        QSymbol symbol = symbols.front();
        Message msg = Message(MSG_ERASE_SYM, symbol, siteId);
        msg.send(*pServer);
        qDebug() << "sent del " << ((symbol.isNewLine()) ? "LF" : QString(symbol.getC()));
    }
}

void SslEchoClient::sendCursor(qint32 userId, qint32 position) {
    CursorPacket cp = CursorPacket(userId, position);
    cp.send(*pServer);
}

void SslEchoClient::sendDocOpen(QString docName, qint32 userId) {
    if(!pServer->isValid()) // if u call this and login wasnt performed
        return;
    DocumentOpenPacket dop = DocumentOpenPacket(docName, userId );
    dop.send(*pServer);
}

void SslEchoClient::sendAskUri(qint32 userId, int docId, QString invCode) {
    //qDebug() << "[SSL ECHO CLIENT] sendAskUri userID = "<< userId << " invCode = " << invCode;
    DocumentAskSharableURIPacket sup = DocumentAskSharableURIPacket(docId, userId,invCode);
    sup.send(*pServer);
}

void SslEchoClient::sendAlignment(Qt::Alignment alignment, QSymbol symbol, int siteId) {
    AlignMessage am = AlignMessage(symbol, 0, alignment, siteId);
    am.send(*pServer);
}

void SslEchoClient::connectToEditor(TextEditor* te) {

    connect(this, &SslEchoClient::insertReceived, te, &TextEditor::remoteInsert);
    connect(this, &SslEchoClient::eraseReceived, te, &TextEditor::remoteErase);
    connect(this, &SslEchoClient::insertBlockReceived, te, &TextEditor::remoteInsertBlock);
    connect(this, &SslEchoClient::eraseBlockReceived, te, &TextEditor::remoteEraseBlock);
    connect(this, &SslEchoClient::updateCursorReceived, te, &TextEditor::updateCursor);
    connect(this, &SslEchoClient::updateCursorReceived, te, &TextEditor::paintCursors);
    connect(this, &SslEchoClient::documentReceived, te, &TextEditor::openDocument);
    connect(this, &SslEchoClient::updateAlignmentReceived, te, &TextEditor::updateAlignment);
    connect(this, &SslEchoClient::updateUserListReceived, te, &TextEditor::updateCursorMap);

    connect(te, &TextEditor::symbolsInserted, this, &SslEchoClient::sendInsert);
    connect(te, &TextEditor::symbolsErased, this, &SslEchoClient::sendErase);
    connect(te, &TextEditor::cursorPositionChanged, this, &SslEchoClient::sendCursor);
    connect(te, &TextEditor::textAlignmentChanged, this, &SslEchoClient::sendAlignment);

}

void SslEchoClient::connectToMainWindow(MainWindow* mw) {
    connect(this, &SslEchoClient::updateUserListReceived, mw, &MainWindow::updateUserList);
    connect(mw, &MainWindow::sendAskUriMainWindow, this, &SslEchoClient::sendAskUri);
    connect(this, &SslEchoClient::askUriReceived, mw, &MainWindow::askUriReceivedMainWindow);
    connect(mw, &MainWindow::sendDocCreateMainWindow, this, &SslEchoClient::sendDocCreate);
    connect(mw, &MainWindow::sendAskDocListMainWindow, this, &SslEchoClient::sendAskDocList);
    connect(this, &SslEchoClient::documentListReceived, mw, &MainWindow::documentListReceivedMainWindow);
    connect(mw, &MainWindow::sendOpenDocumentSignal, this, &SslEchoClient::sendDocOpen);
    connect(this, &SslEchoClient::documentReceived, mw, &MainWindow::openDocumentMainWindow);
    connect(mw, &MainWindow::sendDocumentDeletedSignal, this, &SslEchoClient::sendDocumentDeletedSlot);
    connect(this, &SslEchoClient::documentNameReceived, mw, &MainWindow::setMainWindowTitle);
    connect(this, &SslEchoClient::joinFailed, mw, &MainWindow::joinFailedMW);
    connect(this, &SslEchoClient::joinSucceeded, mw, &MainWindow::joinSucceededMW);
}

void SslEchoClient::connectToRegister(Register* r) {
	connect(this, &SslEchoClient::registerFailedReceived, r, &Register::showError);
	connect(this, &SslEchoClient::registerSuccessfulReceived, r, &Register::registerOk);
}

void SslEchoClient::connectToUserEdit(UserEditWidget* uew) {
	connect(this, &SslEchoClient::updateSuccessfulReceived, uew, &UserEditWidget::updateOk);
}

void SslEchoClient::sendDocumentDeletedSlot(QString docName, quint32 userId) {
    if(!pServer->isValid()) // if u call this and login wasnt performed
        return;
    DocumentDelPacket ddp = DocumentDelPacket(docName, userId);
    ddp.send(*pServer);
}

void SslEchoClient::sendAskDocList(qint32 userId) {
    if(!pServer->isValid()) // if u call this and login wasnt performed
        return;
    DocumentAskListPacket dalp = DocumentAskListPacket(userId );
    dalp.send(*pServer);
}

void SslEchoClient::sendDocCreate(QString docName, qint32 userId) {

    if(!pServer->isValid())
        return;
    DocumentCreatePacket dcp = DocumentCreatePacket(docName, userId );
    dcp.send(*pServer);

    DocumentOpenPacket dop = DocumentOpenPacket(docName, userId );
    dop.send(*pServer);

}

bool SslEchoClient::isConnected(){
    return m_webSocket.state() == QAbstractSocket::ConnectedState;
}

void SslEchoClient::connectToLogin(Login* login){
    connect(this, &SslEchoClient::loginFailedReceived, login, &Login::loginFailed);
    connect(this, &SslEchoClient::loginSuccessfulReceived, login, &Login::loginSuccessful);
    
}

/*
 * Dont delete pls. Possible enhancement
void SslEchoClient::connectToLoginWindow(Login* login, MainWindow* mw) {//Qdialog as of now
    // This would both close the login window and open the main one
    connect(this, &SslEchoClient::loginSuccessful, login, &Login::close);
    connect(this, &SslEchoClient::loginSuccessful, mw, &MainWindow::show);

    // If login failed, just close the login window
    connect(this, &SslEchoClient::loginFailed, login, &MainWindow::close);

    // If login failed, or the main window was closed, quit the application
    // This is needed, because we explicitly set the QGuiApplication::quitOnLastWindowClosed property to false.
    //QObject::connect(w, MainWindow::closed, &a, &QCoreApplication::quit);
    //QObject::connect(&login, &Login::loginFailed, &a, &QCoreApplication::quit);
}

*/
//    // Save the secret key that will be used

