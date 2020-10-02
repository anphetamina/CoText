#include "MainWindow.h"
#include "Login.h"

#include <QApplication> //manages: widgets, events, mouse movements, overall looking feel
#include <QThread>
#include <iostream>
#include "sslechoclient.h"

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

	/*
	// user/pass test authentication for now
    QString quser, qpass;
	if (argc > 1) {
        std::string username = argv[1];
        std::string password = argv[2];
        quser = QString::fromStdString(username);
        qpass = QString::fromStdString(password);
        client->set_username(quser);
        client->set_password(qpass);
    }

    /** Login Phase */
	client = new SslEchoClient(QUrl(QStringLiteral("wss://localhost:12345")));
	Q_UNUSED(*client);

	Login* login = new Login();
    MainWindow *w = new MainWindow();

    // Set login GUI options
    login->setWindowTitle("Welcome to CoText!");
    login->setModal(true);
    login->exec();

    // Set MainWindow (Editor) GUI options
    QCoreApplication::setAttribute(Qt::AA_UseStyleSheetPropagationInWidgetStyles, true);
    w->setWindowTitle("Welcome");
    w->setWindowIconText("Co-Text");
    TextEditor editor(0, *w->getUi()); // todo get site id from server
    //editor.setDisabled(true);
    // place the QTextEditor object in the central position of the main window

    w->connectToTextEditor(&editor);

    /**
     * place the QTextEditor object in the central position of the main window
     */
    w->setCentralWidget(&editor);

    // Perform connection (signal/slot)
    client->connectToMainWindow(w);
    //connect the echo client to enable remote operations on the editor
    client->connectToEditor(&editor);
    //client->connectToLoginWindow(login, w); //TODO: use signal/slot for creating/closing diffent windows.tonote: login is a QDialog not QWindow

    while(user == nullptr || !user->isLogged()) {
        QCoreApplication::processEvents();
    }

    // After login show main window
    w->show();

    //delete client;
    return a.exec();
}
