#include "MainWindow.h"
#include "Login.h"
#include "MainMenu.h"
#include "ServerUnreachable.h"
#include <QApplication> //manages: widgets, events, mouse movements, overall looking feel
#include <QThread>
#include <iostream>
#include "sslechoclient.h"
#include <Benchmark.h>

bool closed = false;

int main(int argc, char *argv[]) {

    int currentExitCode = 0;

    do {

        QApplication a(argc, argv);
        //exe icon for mac/linux
        QIcon icon(":/appIcon/CoText.ico");
        a.setWindowIcon(icon);
        // Apply custom stylesheet (Theme)
        QFile styleFile(":/style/darkTheme.qss");
        styleFile.open(QFile::ReadOnly);
        QString stylesheetString = QLatin1String(styleFile.readAll());
        a.setStyleSheet(stylesheetString);

        QFile f("./client.config");
        QString serverAddr = "wss://localhost:12345";
        if (!f.open(QFile::ReadOnly | QFile::Text)) {
            qInfo("No client.config was found.\nFallback to default value for a localhost server running on default (1235) port: wss://localhost:12345");
        }
        else {
            QTextStream in(&f);
            serverAddr = in.readLine();
            f.close();
        }
        client = new SslEchoClient(QUrl(serverAddr));
        Q_UNUSED(*client);

        MainWindow *w = new MainWindow();
        Login *login =  new Login();

        // Set MainWindow (Editor) GUI options
        QCoreApplication::setAttribute(Qt::AA_UseStyleSheetPropagationInWidgetStyles, true);
        w->setWindowTitle("Welcome");
        w->setWindowIconText("Co-Text");

        TextEditor* editor = new TextEditor(user.getId(), *w->getUi(), w); // tonote: for the multiple document at the same time it would be better to use a better approach
        editor->setDisabled(true);
        w->setCentralWidget(editor);
        w->setTextEditor(editor);
        client->connectToMainWindow(w);
        client->connectToEditor(editor);
        //client->connectToLoginWindow(login, w); //TONOTE (Improvement): use signal/slot for creating/closing diffent windows.tonote: login is a QDialog not QWindow

        Benchmark* b = new Benchmark("main");
        b->startTimer();
        while(!client->isConnected()){
            QCoreApplication::processEvents();
            if(b->getTimer() > 6.0){
                ServerUnreachable* serverUnreachable = new ServerUnreachable(b);
                serverUnreachable->setWindowTitle("Server unreachable");
                serverUnreachable->setModal(true);
                serverUnreachable->exec();
            }

            if(b->isStopped()){
                qDebug() << "Connection timeout: Server is unreachable.";
                qApp->quit();
                return -1;
            }
        }

        // Perform authentication now if user and pass where passed as CLI arg
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
            // Set login GUI options otherwise
            client->connectToLogin(login);
            login->setWindowTitle("Welcome to CoText!");
            login->setModal(true);
            login->exec();
        }

        // Wait until the user received a succesfull message
        while(!user.isLogged() && login->isVisible()) {
            QCoreApplication::processEvents();
        }
        // Check if the while was broken by the login or the closing of the window (and so no auth was tried)
        if(client->getLoginAttemptCount() == 0){
            qApp->quit();
            return -1;
        }

        MainMenu* mainMenu = new MainMenu();
        w->connectToMainMenu(mainMenu);
        mainMenu->setWindowTitle("Main Menu");
        mainMenu->setModal(true);
        mainMenu->exec();

        currentExitCode = a.exec();

    } while( currentExitCode == EXIT_CODE_REBOOT );

    return currentExitCode;

}
