#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Login.h"
#include "TextEditor.h"
#include "ShareUri.h"
#include "Join.h"
#include "OpenDocument.h"
#include "AlertNewDocument.h"
#include "ChooseName.h"
#include "sslechoclient.h"
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
    ui->rightToolBar->setVisible(true);

    //installing EventFilter for QToolButtons on the qToolBar
    dynamic_cast<QToolButton *>(ui->toolBar->widgetForAction(ui->actionOpen))->installEventFilter(this);
    dynamic_cast<QToolButton *>(ui->toolBar->widgetForAction(ui->actionNew))->installEventFilter(this);
    dynamic_cast<QToolButton *>(ui->toolBar->widgetForAction(ui->actionShare_Uri))->installEventFilter(this);
    dynamic_cast<QToolButton *>(ui->toolBar->widgetForAction(ui->actionLogout))->installEventFilter(this);
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
    
    setStatusBar(new StatusBar(*ui, this));
    this->qSB = dynamic_cast<StatusBar *>(statusBar());
    qSB->setupSB();
    qSB->displaySB();
}


MainWindow::~MainWindow(){
	 //delete client;
     delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "CoText", tr("Are you sure to quit?\n"), QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if(resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    // qDebug()<<"Sono entrato nella eventFilter" << event->type() << watched;

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

    //Logout
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionLogout)) && event->type() == QEvent::Enter) {
        setCursor(Qt::PointingHandCursor);
        ui->actionLogout->setIcon(QIcon(":/imgs/icons/noun_Exit_1558251_white.svg"));
        return true;
    }
    if(watched == dynamic_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionLogout)) && event->type() == QEvent::Leave) {
        setCursor(Qt::ArrowCursor);
        ui->actionLogout->setIcon(QIcon(":/imgs/icons/noun_Exit_1558251.svg"));
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

void MainWindow::on_actionLogout_triggered() {
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Logout", tr("Are you sure to logout ?\n"),  QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

    if(resBtn == QMessageBox::Yes) {
        user = User();
        this->hide();
        editor->setDisabled(true);

        Login *login = new Login();
        client->connectToLogin(login);
        login->setWindowTitle("Welcome to CoText!");
        login->setModal(true);
        login->exec();

        MainMenu* mainMenu = new MainMenu();
        this->connectToMainMenu(mainMenu);
        mainMenu->setWindowTitle("Main Menu");
        mainMenu->setModal(true);
        mainMenu->exec();
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

void MainWindow::updateUserList(QVector<User> newOnlineUserList, QVector<User> newCompleteUserList){

    //remove users that before were online and now are offline
    removeOldOnlineNowOffline(newOnlineUserList);

    for(int i=0; i<newCompleteUserList.size();i++){
        if(!colorMap.contains(newCompleteUserList[i].getId())){
            colorMap.insert(newCompleteUserList[i].getId(),colorList.at(i%(colorList.size()-1)));
        }

        if(newOnlineUserList.contains(newCompleteUserList[i])){
            QLabel* label = new QLabel();
            QLabel* iconLabel = new QLabel();
            QString text;
            QHBoxLayout *hBox = new QHBoxLayout;


            if(newCompleteUserList[i].getId() == user.getId()) {
                text = "  "+newCompleteUserList[i].getEmail() + " (YOU)  ";
            }else {
                text = "  "+newCompleteUserList[i].getEmail()+"  ";
            }

            if(ui->rightToolBar->findChild<QWidget*>(QString::number(newCompleteUserList[i].getId())) == nullptr){
                label->setText(text);
                label->setStyleSheet("font-weight: bold; color:"+colorMap[newCompleteUserList[i].getId()].name());

                QPixmap orig;
                orig.load(":/imgs/icons/user-group.svg");
                QPixmap background = addImageInRightToolBar(orig, colorMap[newCompleteUserList[i].getId()].name());
                iconLabel->setPixmap(background);

                QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
                spLeft.setHorizontalStretch(1);
                iconLabel->setSizePolicy(spLeft);
                QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);
                spRight.setHorizontalStretch(5);
                label->setSizePolicy(spRight);

                hBox->addWidget( iconLabel);
                hBox->addWidget( label);

                QWidget *w = new QWidget();
                w->setLayout(hBox);
                w->setObjectName(QString::number(newCompleteUserList[i].getId()));
                ui->rightToolBar->addWidget(w);
            }
        }
    }

    onlineUserList = newOnlineUserList;
}

void MainWindow::removeOldOnlineNowOffline(QVector<User> newOnlineUserList){
    QVector<User> toRemove;
    for(User u: onlineUserList){
        if(!newOnlineUserList.contains(u)){
            delete ui->rightToolBar->findChild<QWidget*>(QString::number(u.getId()));
        }
    }
}

QPixmap MainWindow::addImageInRightToolBar(const QPixmap &orig, QColor color) {
    // Getting size if the original picture is not square
    int size = qMin(orig.width(), orig.height());

    // Creating circle clip area
    QPixmap rounded = QPixmap(size, size);
    rounded.fill(Qt::transparent);
    QPainterPath path;
    path.addEllipse(rounded.rect());
    QPainter painter(&rounded);
    painter.setClipPath(path);

    // Filling rounded area if needed
    painter.fillRect(rounded.rect(), Qt::black);

    // Getting offsets if the original picture is not square
    int x = qAbs(orig.width() - size) / 2;
    int y = qAbs(orig.height() - size) / 2;
    painter.drawPixmap(-x, -y, orig.width(), orig.height(), orig);

    QPixmap background = QPixmap(size + 50, size + 50);
    background.fill(Qt::transparent);
    QPainterPath path1;
    path1.addEllipse(background.rect());
    QPainter painter1(&background);
    painter1.setClipPath(path1);

    // Filling rounded area if needed
    painter1.fillRect(background.rect(), color);

    // Getting offsets if the original picture is not square
    x = qAbs(rounded.width() - size - 50) / 2;
    y = qAbs(rounded.height() - size - 50) / 2;
    painter1.drawPixmap(x, y, rounded.width(), rounded.height(), rounded);

    return background.scaled(30,30);
}


void MainWindow::on_actionShare_Uri_triggered() {
    emit(sendAskUriMainWindow(user.getId(), editor->getDocId(), ""));   //todo change userId and docId
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

