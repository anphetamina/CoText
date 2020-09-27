#include "MainWindow.h"
#include "Login.h"

#include <QApplication> //manages: widgets, events, mouse movements, overall looking feel
#include <QThread>
#include <iostream>

//#include <QtCore/QCoreApplication>
#include "sslechoclient.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    
    //exe icon for mac/linux
    QIcon icon(":/appIcon/CoText.ico");
    a.setWindowIcon(icon);
    //exe icon for Win
    
    //apply custom stylesheet
	QFile styleFile(":/style/darkTheme.qss");
	styleFile.open(QFile::ReadOnly);
	QString stylesheetString = QLatin1String(styleFile.readAll());
	a.setStyleSheet(stylesheetString);


	// user/pass test authentication for now
    QString quser, qpass;
	if (argc > 1) {
        std::string username = argv[1];
        std::string password = argv[2];
        quser = QString::fromStdString(username);
        qpass = QString::fromStdString(password);
    }else{
	    quser = QString::fromStdString("test@test.test");
	    qpass = QString::fromStdString("test");
	}
    // ** Network testing code

    //QCoreApplication a(argc, argv);
    SslEchoClient* client = new SslEchoClient(QUrl(QStringLiteral("wss://localhost:12345")));
    Q_UNUSED(*client);
    client->set_username(quser);
    client->set_password(qpass);
    // ** End of network testing code

    /*QThread *listener = new QThread();
    client->moveToThread(listener);
    listener->start();

    qRegisterMetaType<QList<QSslError>>("QList<QSslError>");*/

    // Set GUI options
    MainWindow *w = new MainWindow();
	QCoreApplication::setAttribute(Qt::AA_UseStyleSheetPropagationInWidgetStyles, true);
	
	w->setWindowTitle("Welcome");
    
    w->setWindowIconText("Co-Text");

    client->connectToMainWindow(w);
    //w->setWindowIcon(QIcon(":/CoText.ico"));
    //QPixmap pm(":/CoText.ico");
    ////DO SOME RESIZING
    ////TODO


    TextEditor editor(0, *w->getUi()); // todo get site id from server

    /**
     * place the QTextEditor object in the central position of the main window
     */
    w->setCentralWidget(&editor);

    /**
     * connect the echo client to enable remote operations on the editor
     */
    client->connectToEditor(&editor);


    w->show();

    //delete client;
    return a.exec();

}
