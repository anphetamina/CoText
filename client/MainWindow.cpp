#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Login.h"
#include "TextEditor.h"
#include "ShareUri.h"
#include "Join.h"
#include "OpenDocument.h"
#include "AlertNewDocument.h"
#include "ChooseName.h"
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
    // ui->statusbar->addPermanentWidget();
    
	
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
    ChooseName chooseName(docList);
    connect(&chooseName, &ChooseName::nameChosen, this, &MainWindow::nameChosenMainWindow);
    chooseName.setWindowTitle("Choose document name");
    chooseName.setModal(true);
    chooseName.exec();
}

void MainWindow::nameChosenMainWindow(QString name){
    if(editor->isEnabled()){    //c'è già un documento aperto
        AlertNewDocument alert(this->windowTitle(), name);
        connect(&alert, &AlertNewDocument::openNewDocument, this, &MainWindow::openNewDocumentMainWindow);
        alert.setWindowTitle("Alert");
        alert.setModal(true);
        alert.exec();
    }else { //non c'è nessun documento aperto
        emit(sendDocCreateMainWindow(name, user.getId()));   //il server poi risponde con DocumentOkPacket e il client nella slot apre il nuovo documento
        docList.append(name);
    }
}

void MainWindow::connectToMainMenu(MainMenu* mainMenu) {
    connect(mainMenu, &MainMenu::newDocumentClicked, this, &MainWindow::newDocumentFromMainMenu);
    connect(mainMenu, &MainMenu::openDocumentClicked, this, &MainWindow::openDocumentFromMainMenu);
    connect(mainMenu, &MainMenu::joinClicked, this, &MainWindow::joinFromMainMenu);
    connect(this, &MainWindow::closeMainMenu, mainMenu, &MainMenu::closeMainMenuSlot);
    connect(mainMenu, &MainMenu::sendCloseMainWindow, this, &MainWindow::closeMainWindow);
}

void MainWindow::newDocumentFromMainMenu() {
    ChooseName chooseName(docList);
    connect(&chooseName, &ChooseName::nameChosen, this, &MainWindow::nameChosenFromMainMenu);
    chooseName.setWindowTitle("Choose document name");
    chooseName.setModal(true);
    chooseName.exec();
}

void MainWindow::nameChosenFromMainMenu(QString name){
    emit closeMainMenu();
    emit(sendDocCreateMainWindow(name, user.getId()));   //il server poi risponde con DocumentOkPacket e il client nella slot apre il nuovo documento
    docList.append(name);
    this->show();
}

void MainWindow::openNewDocumentMainWindow(QString docName){
    QString name(docName);
    int i = 0;
    while (docList.contains(name)) {
        i++;
        name = docName + "" + QString::number(i);
    }
    emit(sendDocCreateMainWindow(name, user.getId()));   //il server poi risponde con DocumentOkPacket e il client nella slot apre il nuovo documento
    docList.append(name);
}

void MainWindow::openDocumentFromMainMenu() {
    //emit(sendAskDocListMainWindow(user.getId())); //todo understand if it's useful
    OpenDocument openDocument(docList, this);
    connect(&openDocument, &OpenDocument::sendOpenDocument, this, &MainWindow::sendOpenDocumentFromMainMenu);
    connect(&openDocument, &OpenDocument::documentDeleted, this, &MainWindow::sendDocumentDeletedMainWindow);
    openDocument.setWindowTitle("Select a document");
    openDocument.setModal(true);
    openDocument.exec();
}

void MainWindow::sendDocumentDeletedMainWindow(QString docName){
    emit sendDocumentDeletedSignal(docName, user.getId());
    docList.removeOne(docName);
}

void MainWindow::sendOpenDocumentFromMainMenu(QString docName){
    emit closeMainMenu();
    emit(sendOpenDocumentSignal(docName, user.getId()));
    this->show();
}
void MainWindow::on_actionOpen_triggered() {
    //emit(sendAskDocListMainWindow(user.getId())); //todo understand if it's useful
    OpenDocument openDocument(docList, this);
    connect(&openDocument, &OpenDocument::sendOpenDocument, this, &MainWindow::sendOpenDocumentMainWindow);
    openDocument.setWindowTitle("Select a document");
    openDocument.setModal(true);
    openDocument.exec();
}

void MainWindow::sendOpenDocumentMainWindow(QString docName){
    //qDebug()<<"[MAIN WINDOW] sendOpenDocumentMainWindow docName = "<<docName;
    emit(sendOpenDocumentSignal(docName, user.getId()));
}

void MainWindow::joinFromMainMenu() {
    Join join;
    join.setWindowTitle("Join a shared document");
    connect(&join, &Join::sendJoin, this, &MainWindow::sendJoinFromMainMenu);
    join.setModal(true);
    join.exec();
}

void MainWindow::sendJoinFromMainMenu(qint32 userId, int docId, QString invCode){
    emit closeMainMenu();
    emit(sendAskUriMainWindow(userId,docId,invCode));
    this->show();
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


void MainWindow::updateUserList(QVector<User> newOnlineUserList, QVector<User> newCompleteUserList){
    for(int j=0; j<20; j++){
        actionUserList[j]->setVisible(false);
    }

    //qDebug() << "[MAIN WINDOW] User list updated";
    onlineUserList = newOnlineUserList;
    completeUserList = newCompleteUserList;
    colorMap.clear();
    //qDebug() << "[MAIN WINDOW] complete user list ";
    for(int i=0; i<newCompleteUserList.size();i++){
        colorMap.insert(newCompleteUserList[i].getId(),colorList.at(i%19));
        //qDebug() << "user = "<<newCompleteUserList[i].getId();
    }

    for(int j=0; j<newOnlineUserList.size();j++){
        actionUserList[j]->setVisible(true);
        if(newOnlineUserList[j].getId() == user.getId()) {
            actionUserList[j]->setText(newOnlineUserList[j].getEmail() + " (YOU)");
        }else {
            actionUserList[j]->setText(newOnlineUserList[j].getEmail());
        }
        ui->rightToolBar->widgetForAction(actionUserList[j])->setStyleSheet("color:"+colorMap[newOnlineUserList[j].getId()].name());
    }
}


void MainWindow::on_actionShare_Uri_triggered() {
    emit(sendAskUriMainWindow(user.getId(), editor->getDocId(), ""));   //todo change userId and docId
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

QColor MainWindow::getUserColor(int userId) const {
    if (colorMap.find(userId) == colorMap.end()) {
        throw std::runtime_error(std::string{} + __PRETTY_FUNCTION__ + ": color for user id not found");
    }

    return colorMap[userId];
}

void MainWindow::setMainWindowTitle(QString title){
    setWindowTitle(title);
}

void MainWindow::sendJoinMainWindow(qint32 userId, int docId, QString invCode){
    emit(sendAskUriMainWindow(userId,docId,invCode));
}

void MainWindow::documentListReceivedMainWindow(QVector<QString> documentList){
    docList = documentList;
}

void MainWindow::setTextEditor(TextEditor* te){
    editor = te;
}

TextEditor* MainWindow::getTextEditor() const{
    return editor;
}

QVector<User> MainWindow::getOnlineUserList() const {
	return onlineUserList;
}

void MainWindow::openDocumentMainWindow(int docId, QString docName, std::vector<std::vector<QSymbol>> qsymbols){
    //add a document joined with an invite inside the doclist
    if(!docList.contains(docName)){
        docList.append(docName);
    }
}

void MainWindow::closeMainWindow(){
    qDebug()<<"[MAIN WINDOW] close ";
    exit(0);
}