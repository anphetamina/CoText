#include "./gui/MainWindow.h"
#include "./gui/Login.h"

#include <QApplication> //manages: widgets, events, mouse movements, overall looking feel
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

    TextEditor editor(*w, *w->getUi());

    return a.exec();
}
