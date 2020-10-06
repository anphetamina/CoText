#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Login.h"
#include "TextEditor.h"
#include "ShareUri.h"
#include "Join.h"
#include <QPixmap> //allows to create a qpixmap onj which takes 1 arg
#include <QPrinter>
#include <QColorDialog>
#include <QColor>
#include <QCloseEvent>
#include <QtSvg>
#include <QEvent>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {

    QApplication::instance()->setAttribute(Qt::AA_DontShowIconsInMenus, true);

    ui->setupUi(this);
    ui->rightToolBar->setVisible(false);
    //installing EventFilter for QToolButtons on the qToolBar
    dynamic_cast<QToolButton *>(ui->toolBar->widgetForAction(ui->actionLogin))->installEventFilter(this);
    dynamic_cast<QToolButton *>(ui->toolBar->widgetForAction(ui->actionOpen))->installEventFilter(this);
    dynamic_cast<QToolButton *>(ui->toolBar->widgetForAction(ui->actionNew))->installEventFilter(this);
    dynamic_cast<QToolButton *>(ui->toolBar->widgetForAction(ui->actionShare_Uri))->installEventFilter(this);
    dynamic_cast<QToolButton *>(ui->toolBar->widgetForAction(ui->actionExit))->installEventFilter(this);
    dynamic_cast<QToolButton *>(ui->toolBar->widgetForAction(ui->actionSettings))->installEventFilter(this);
    dynamic_cast<QToolButton *>(ui->toolBar->widgetForAction(ui->actionJoin))->installEventFilter(this);

    //this->setCentralWidget(ui->textEdit);
    QPixmap icon(":/appIcon/CoText.ico");
    //ui->iconLabel->setPixmap(icon);

    
	
	ui->actionCut->setShortcut(QKeySequence::Cut);
    ui->actionCopy->setShortcut(QKeySequence::Copy);
    ui->actionPaste->setShortcut(QKeySequence::Paste);
    //ui->actionRemove->setShortcut(QKeySequence::Delete);

    // What about using a container?
    actionUserList.insert(0,ui->actionUser0);
    actionUserList.insert(1,ui->actionUser1);
    actionUserList.insert(2,ui->actionUser2);
    actionUserList.insert(3,ui->actionUser3);
    actionUserList.insert(4,ui->actionUser4);
    actionUserList.insert(5,ui->actionUser5);
    actionUserList.insert(6,ui->actionUser6);
    actionUserList.insert(7,ui->actionUser7);
    actionUserList.insert(8,ui->actionUser8);
    actionUserList.insert(9,ui->actionUser9);
    actionUserList.insert(10,ui->actionUser10);
    actionUserList.insert(11,ui->actionUser11);
    actionUserList.insert(12,ui->actionUser12);
    actionUserList.insert(13,ui->actionUser13);
    actionUserList.insert(14,ui->actionUser14);
    actionUserList.insert(15,ui->actionUser15);
    actionUserList.insert(16,ui->actionUser16);
    actionUserList.insert(17,ui->actionUser17);
    actionUserList.insert(18,ui->actionUser18);
    actionUserList.insert(19,ui->actionUser19);
    
    setStatusBar(new StatusBar(*ui, this));
    this->qSB = dynamic_cast<StatusBar *>(statusBar());
    qSB->setupSB();
    qSB->displaySB();
    
    
	
	
	
	
    /*actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser0));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser1));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser2));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser3));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser4));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser5));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser6));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser7));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser8));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser9));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser10));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser11));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser12));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser13));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser14));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser15));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser16));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser17));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser18));
    actionUserMap.insert(std::pair<int,QAction*>(-1,ui->actionUser19));*/
}


MainWindow::~MainWindow(){
	 //delete client;
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


bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    // qDebug()<<"Sono entrato nella eventFilter" << event->type() << watched;

    //Login
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionLogin)) && event->type() == QEvent::Enter) {
        setCursor(Qt::PointingHandCursor);
        ui->actionLogin->setIcon(QIcon(":/imgs/icons/noun_user login_178831_white.svg"));
        return true;
    }
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionLogin)) && event->type() == QEvent::Leave) {
        setCursor(Qt::ArrowCursor);
        ui->actionLogin->setIcon(QIcon(":/imgs/icons/noun_user login_178831.svg"));
        return true;
    }

    //Open
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionOpen)) && event->type() == QEvent::Enter) {
        setCursor(Qt::PointingHandCursor);
        ui->actionOpen->setIcon(QIcon(":/imgs/icons/noun_Folder_2047792_white.svg"));
        return true;
    }
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionOpen)) && event->type() == QEvent::Leave) {
        setCursor(Qt::ArrowCursor);
        ui->actionOpen->setIcon(QIcon(":/imgs/icons/noun_Folder_2047792.svg"));
        return true;
    }

    //New Document
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionNew)) && event->type() == QEvent::Enter) {
        setCursor(Qt::PointingHandCursor);
        ui->actionNew->setIcon(QIcon(":/imgs/icons/noun_new document_2429385_white.svg"));
        return true;
    }
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionNew)) && event->type() == QEvent::Leave) {
        setCursor(Qt::ArrowCursor);
        ui->actionNew->setIcon(QIcon(":/imgs/icons/noun_new document_2429385.svg"));
        return true;
    }

    //Share Uri
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionShare_Uri)) && event->type() == QEvent::Enter) {
        setCursor(Qt::PointingHandCursor);
        ui->actionShare_Uri->setIcon(QIcon(":/imgs/icons/noun_url_887536_white.svg"));
        return true;
    }
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionShare_Uri)) && event->type() == QEvent::Leave) {
        setCursor(Qt::ArrowCursor);
        ui->actionShare_Uri->setIcon(QIcon(":/imgs/icons/noun_url_887536.svg"));
        return true;
    }

    //Exit
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionExit)) && event->type() == QEvent::Enter) {
        setCursor(Qt::PointingHandCursor);
        ui->actionExit->setIcon(QIcon(":/imgs/icons/noun_Exit_1558251_white.svg"));
        return true;
    }
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionExit)) && event->type() == QEvent::Leave) {
        setCursor(Qt::ArrowCursor);
        ui->actionExit->setIcon(QIcon(":/imgs/icons/noun_Exit_1558251.svg"));
        return true;
    }

    //Settings
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionSettings)) && event->type() == QEvent::Enter) {
        setCursor(Qt::PointingHandCursor);
        ui->actionSettings->setIcon(QIcon(":/imgs/icons/noun_Settings_2324598_white.svg"));
        return true;
    }
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionSettings)) && event->type() == QEvent::Leave) {
        setCursor(Qt::ArrowCursor);
        ui->actionSettings->setIcon(QIcon(":/imgs/icons/noun_Settings_2324598.svg"));
        return true;
    }

    //Join to a shared document (invite)
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionJoin)) && event->type() == QEvent::Enter) {
        setCursor(Qt::PointingHandCursor);
        ui->actionJoin->setIcon(QIcon(":/imgs/icons/join_white.svg"));
        return true;
    }
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionJoin)) && event->type() == QEvent::Leave) {
        setCursor(Qt::ArrowCursor);
        ui->actionJoin->setIcon(QIcon(":/imgs/icons/join_grey.svg"));
        return true;
    }

    return false;
}


void MainWindow::on_actionNew_triggered() {
    currentFileName.clear();
    setWindowTitle("untitled");

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
//        ui->textEdit->setText(text);
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
//    QString text = ui->textEdit->toPlainText();
//    out << text;
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
//        QString text = ui->textEdit->toPlainText();
//        out << text;
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
//    ui->textEdit->copy();

}

void MainWindow::on_actionPaste_triggered() {
//    ui->textEdit->paste();
}

void MainWindow::on_actionCut_triggered() {

//    ui->textEdit->cut();
}

void MainWindow::on_actionUndo_triggered() {
//    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered() {
//    ui->textEdit->redo();
}


void MainWindow::on_actionLogin_triggered()
{
    //Modal approach to create loginForm (cannot access mainWindow at same time)
    //It is on the stack, if we want it on the heap just have the pointer on mainwindow.h and call new here

    Login login;
    login.setModal(true);
    login.exec();

    //hide(); //hide the MainWindow
    //lf = new loginform(this); //giving also the parent class
    //lf->show();
}


void MainWindow::updateUserList(QVector<User> newUserList){
    for(int j=0; j<20; j++){
        actionUserList[j]->setVisible(false);
    }

    qDebug() << "User list updated";
    userList = newUserList;
    colorMap.clear();
    for(int i=0; i<newUserList.size();i++){
        colorMap.insert(newUserList[i].getId(),colorList.at(i%19));
        actionUserList[i]->setVisible(true);
        if(newUserList[i].getId() == user.getId()) {
            actionUserList[i]->setText(newUserList[i].getEmail() + " (YOU)");
        }else {
            actionUserList[i]->setText(newUserList[i].getEmail());
        }
        ui->rightToolBar->widgetForAction(actionUserList[i])->setStyleSheet("color:"+colorList.at(i).name());
    }

    emit newColorMapReceived(colorMap);

    /*std::vector<User> newUserListStd = newUserList.toStdVector();
    //users that before were online and now are offline
    QVector<User> usersNoMoreOnline;
    std::set_difference (userList.begin(), userList.end(), newUserListStd.begin(), newUserListStd.end(), usersNoMoreOnline.begin());
    for(int i=0; i<usersNoMoreOnline.size(); i++){
        //std::map<int,QColor> stdMap = colorMap.toStdMap();
        auto nodeHandler = colorMap.extract(usersNoMoreOnline[i].getId());
        nodeHandler.key() = -1;
        colorMap.insert(std::move(nodeHandler));
        //colorMap = QMultiMap(QMap(stdMap));

        actionUserMap.find(usersNoMoreOnline[i].getId())->second->setVisible(false);

       // std::map<int,QAction*> stdMap2 = actionUserMap.toStdMap();
        auto nodeHandler2 = actionUserMap.extract(usersNoMoreOnline[i].getId());
        nodeHandler2.key() = -1;
        actionUserMap.insert(std::move(nodeHandler2));
        //actionUserMap = QMultiMap(QMap(stdMap2));
    }

   ///users that before were offline and now are online
    QVector<User> newUsersOnline;
    std::set_difference ( newUserList.begin(), newUserList.end(), userList.begin(), userList.end(), newUsersOnline.begin());
    for(int i=0; i<newUsersOnline.size(); i++){
        std::map<int,QColor> stdMap = colorMap.toStdMap();
        auto nodeHandler = stdMap.extract(-1);
        nodeHandler.key() = newUsersOnline[i].getId();
        stdMap.insert(std::move(nodeHandler));
        colorMap = QMultiMap(QMap(stdMap));

        std::map<int,QAction*> stdMap2 = actionUserMap.toStdMap();
        auto nodeHandler2 = stdMap2.extract(-1);
        nodeHandler2.key() = newUsersOnline[i].getId();
        stdMap2.insert(std::move(nodeHandler2));
        actionUserMap = QMultiMap(QMap(stdMap2));

        actionUserMap.take(newUsersOnline[i].getId())->setVisible(true);
        actionUserMap.take(newUsersOnline[i].getId())->setText(usersNoMoreOnline[i].getEmail());
        ui->rightToolBar->widgetForAction(actionUserMap.take(newUsersOnline[i].getId()))->setStyleSheet("color:"+colorMap.take(newUsersOnline[i].getId()).name());
    }

    userList = newUserListStd;*/
}


void MainWindow::on_actionShare_Uri_triggered() {
    emit(sendAskUriMainWindow(user.getId(), 12, ""));   //todo change userId and docId
    //qDebug() << "[MAIN WINDOW] sendAskUriMainWindow userId = "<< user.getId();
}

void MainWindow::askUriReceivedMainWindow(QString URI) {
    ShareUri shareUri(URI);
    shareUri.setWindowTitle("Share with other people");
    shareUri.setModal(true);
    shareUri.exec();
}

void MainWindow::on_actionSettings_triggered() {

    UserWidget uw;
    uw.setModal(true);
    uw.exec();
}

void MainWindow::on_actionJoin_triggered() {
    Join join;
    join.setWindowTitle("Join a shared document");
    connect(&join, &Join::sendJoin, this, &MainWindow::sendJoinMainWindow);
    join.setModal(true);
    join.exec();
}


Ui::MainWindow *MainWindow::getUi() const {
    return ui;
}

void MainWindow::connectToTextEditor(TextEditor* te) {
    connect(this, &MainWindow::newColorMapReceived, te, &TextEditor::updateColorMap);
}

void MainWindow::sendJoinMainWindow(qint32 userId, int docId, QString invCode){
    emit(sendAskUriMainWindow(userId,docId,invCode));
}