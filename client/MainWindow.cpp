#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Login.h"
//#include "dbConf.h"


#include <QPixmap> //allows to create a qpixmap onj which takes 1 arg
#include <QPrinter>
#include <QColorDialog>
#include <QColor>
#include <QCloseEvent>
#include <QtSvg>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
	QApplication::instance()->setAttribute(Qt::AA_DontShowIconsInMenus, true);
	ui->setupUi(this);
	//this->setCentralWidget(ui->textEdit);
	QPixmap icon(":/appIcon/qPDS.ico");
	//ui->iconLabel->setPixmap(icon);
	// ui->statusbar->addPermanentWidget();
	
}


MainWindow::~MainWindow(){
	delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "CoText", tr("Are your sure to quit?\n"), QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
	if(resBtn != QMessageBox::Yes) {
		event->ignore();
	} else {
		event->accept();
	}
}


void MainWindow::on_actionNew_triggered() {
	currentFileName.clear();
	setWindowTitle("untitled");
	ui->textEdit->setText(QString());
	
}

void MainWindow::on_actionOpen_triggered() {
	QString filename = QFileDialog::getOpenFileName(this, "Choose file to open", "/home", "All files .*");
	if(!filename.isNull()) {
		
		QFile file(filename);
		QString filePath = filename.toUtf8();
		
		currentFileName = filename;
		
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

void MainWindow::Save_as() {
	
	QString filename = QFileDialog::getSaveFileName(this, "Save as");
	QFile file(filename);
	if(!file.open(QIODevice::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, "Warning", "Can not save the file:  "+ file.errorString() + ". Choose a name and save.");
		return;
	}
	currentFileName = filename;
	setWindowTitle(filename);
	QTextStream out(&file);
	QString text = ui->textEdit->toPlainText();
	out << text;
	file.close();
	return;
	
	
}
void MainWindow::on_actionSave_as_triggered() {
	Save_as();
	return;
}



void MainWindow::on_actionSave_triggered() {
	QString filename = currentFileName;
	QFile file(filename);
	
	if(!file.open(QIODevice::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, "Warning", "Can not save the file:  "+ file.errorString()+ ". Choose a name and save.");
		Save_as();
		return;
	} else {
		QTextStream out(&file);
		QString text = ui->textEdit->toPlainText();
		out << text;
		file.close();
		return;
	}
}



void MainWindow::on_actionPrintPDF_triggered() {
	QPrinter printer;
	printer.setOutputFormat(QPrinter::PdfFormat);
	QString filename = QFileDialog::getSaveFileName(this, "Export PDF");
	printer.setOutputFileName(filename);
	
	
	
	
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


