#include "gui/MainWindow.h"
#include "gui/Login.h"

#include <QApplication> //manages: widgets, events, mouse movements, overall looking feel


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow *w = new MainWindow();

    w->setWindowTitle("Welcome");
    w->setWindowIconText("Co-Text");
    //w->setWindowIcon(QIcon(":/CoText.ico"));



    //QPixmap pm(":/CoText.ico");

    ////DO SOME RESIZING
    ////TODO







    w->show();


    return a.exec();
}
