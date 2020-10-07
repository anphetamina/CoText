#include "MainWindow.h"
#include "Login.h"

#include <QApplication> //manages: widgets, events, mouse movements, overall looking feel
#include <QThread>
#include <iostream>
#include "sslechoclient.h"
#include <Benchmark.h>

//#include <QtCore/QCoreApplication>

bool closed = false;

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
	//exe icon for mac/linux
    QIcon icon(":/appIcon/CoText.ico");
    a.setWindowIcon(icon);
    //a.setQuitOnLastWindowClosed(false);// Avoid exit the whole app when just one windows (ie. login) was opened and closed
    // Apply custom stylesheet (Theme)
	QFile styleFile(":/style/darkTheme.qss");
	styleFile.open(QFile::ReadOnly);
	QString stylesheetString = QLatin1String(styleFile.readAll());
	a.setStyleSheet(stylesheetString);

    /** Login Phase */
	client = new SslEchoClient(QUrl(QStringLiteral("wss://localhost:12345")));
	Q_UNUSED(*client);

    MainWindow *w = new MainWindow();
    Login *login = new Login();



    // Set MainWindow (Editor) GUI options
    QCoreApplication::setAttribute(Qt::AA_UseStyleSheetPropagationInWidgetStyles, true);
    w->setWindowTitle("Welcome");
    w->setWindowIconText("Co-Text");

    TextEditor* editor = new TextEditor(user.getId(), *w->getUi(), w);
    editor->setDisabled(true);
    w->setCentralWidget(editor);
    w->setTextEditor(editor);
    w->connectToTextEditor(editor);
    client->connectToMainWindow(w);
    client->connectToEditor(editor);
    //client->connectToLoginWindow(login, w); //TODO: use signal/slot for creating/closing diffent windows.tonote: login is a QDialog not QWindow

    Benchmark b = Benchmark();
    b.startTimer();
    while(!client->isConnected()){
        QCoreApplication::processEvents();
        if(b.getTimer() > 6.0){
            qDebug() << "Connection timeout: Server is unreachable.";
            qApp->quit();
            return -1;
        }
    }

    QString quser, qpass;
    if (argc > 1) {
        std::string username = argv[1];
        std::string password = argv[2];
        quser = QString::fromStdString(username);
        qpass = QString::fromStdString(password);
        client->set_username(quser);
        client->set_password(qpass);
        client->sendLogin();
    }else {
        // Set login GUI options
        client->connectToLogin(login);
        w->connectToLogin(login);
        login->setWindowTitle("Welcome to CoText!");
        login->setModal(true);
        login->exec();
    }

    while(!user.isLogged() && login->isVisible()) {
        QCoreApplication::processEvents();
    }
    // Check if the while was broken by the login or the closing of the window
    if(client->getLoginAttemptCount() == 0){
        qApp->quit();
        return -1;
    }

    // After a successful login show main window
    w->show();

    //delete client;
    return a.exec();
}
