#include "./gui/MainWindow.h"
#include "./gui/Login.h"

#include <QApplication> //manages: widgets, events, mouse movements, overall looking feel
#include <QThread>
#include <iostream>

//#include <QtCore/QCoreApplication>
#include "sslechoclient.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
	QFile styleFile(":/style/darkTheme.qss");
	styleFile.open(QFile::ReadOnly);
	QString stylesheetString = QLatin1String(styleFile.readAll());
	a.setStyleSheet(stylesheetString);


    // ** Network testing code
    //QCoreApplication a(argc, argv);
    SslEchoClient* client = new SslEchoClient(QUrl(QStringLiteral("wss://localhost:12345")));
    Q_UNUSED(*client);
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
    //w->setWindowIcon(QIcon(":/CoText.ico"));
    //QPixmap pm(":/CoText.ico");
    ////DO SOME RESIZING
    ////TODO

    w->show();

    TextEditor editor(*w, *w->getUi(), client);
    //delete client;
    return a.exec();

}
