#include "mainwindow.h"
#include "login.h"

#include <QApplication> //manages: widgets, events, mouse movements, overall looking feel


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile styleFile(":/style/darkTheme.qss");
    styleFile.open(QFile::ReadOnly);
    QString stylesheetString = QLatin1String(styleFile.readAll());
    a.setStyleSheet(stylesheetString);

    MainWindow *w = new MainWindow();

    QCoreApplication::setAttribute(Qt::AA_UseStyleSheetPropagationInWidgetStyles, true);




    w->setWindowTitle("Welcome");
    w->setWindowIconText("Co-Text");


    //w->setWindowIcon(QIcon(":/qPDS.ico"));



    QPixmap pm(":/appIcon/CoText.ico");

    ////DO SOME RESIZING
    ////TODO







    w->show();


    return a.exec();
}
