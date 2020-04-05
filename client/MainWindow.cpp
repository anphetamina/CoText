#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Login.h"


#include <QPixmap> //allows to create a qpixmap onj which takes 1 arg



MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    //this->setCentralWidget(ui->textEdit);
    QPixmap icon(":/appIcon/CoText.ico");
    //ui->iconLabel->setPixmap(icon);
   // ui->statusbar->addPermanentWidget();



            


}

MainWindow::~MainWindow(){
     delete ui;
}


void MainWindow::on_actionNew_triggered() {
    currentFile.clear();
    setWindowTitle("untitled");
    ui->textEdit->setText(QString());

}

void MainWindow::on_actionOpen_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, "Choose file to open", "/home", "All files .*");
    if(!filename.isNull()) {

        QFile file(filename);
        QString filePath = filename.toUtf8();

        currentFile = filename;

        //checks
        if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "Warning", "Can not open file:  "+ file.errorString());
            return;
        }

        setWindowTitle((filename));
        QTextStream in(&file);
        QString text = in.readAll();
        ui->textEdit->setText(text);
        file.close();

    } else {
        //QMessageBox::warning(this, "Warning - filename Null", "The file selected is invalid");
        return;
    }

}

void MainWindow::on_actionSave_as_triggered() {
    QString filename = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(filename);

    //checks
    if(!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Can not save the file:  "+ file.errorString());
        return;
    }
    currentFile = filename;
    setWindowTitle(filename);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();


}
/*
void MainWindow::on_actionPrint_triggered() {
    QPrinter printer;
    printer.setPrinterName("Printer Name");
    QPrintDialog pDialog(&printer, this);

    if(pDialog.exec() == QDialog::Rejected) {
        QMessageBox::warning(this, "Warning", "Can not access the printer");
        return;
    }

    ui->textEdit->print(&printer);



}
*/

void MainWindow::on_actionExit_triggered() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Exit", "Do you really want to quit the application?", QMessageBox::Yes | QMessageBox::No);


    if(reply == QMessageBox::Yes) {
        QApplication::quit();
    } else {
        return;
    }

}

void MainWindow::on_actionCopy_triggered() {
    ui->textEdit->copy();

}

void MainWindow::on_actionPaste_triggered() {
    ui->textEdit->paste();
}

void MainWindow::on_actionCut_triggered() {

    ui->textEdit->cut();
}

void MainWindow::on_actionUndo_triggered() {
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered() {
    ui->textEdit->redo();
}



void MainWindow::on_actionLogin_triggered()
{
    //Modal approach to create loginForm (cannot access mainWindow at same time)
    //It is on the stack, if we want it on the heap just have the pointer on mainwindow.h and call new here
    //Connecting to MySql Database
    /*
    if(!dbConfigure()) {
        QMessageBox::information(this, "DB-Connection", "Database not connected");
    } else {
        QMessageBox::information(this, "DB-Connection", "Database connected");
        Login login;
        login.setModal(true);
        login.exec();
    }
    */

    Login login;
    login.setModal(true);
    login.exec();


    //hide(); //hide the MainWindow

    //lf = new loginform(this); //giving also the parent class

    //lf->show();

}
