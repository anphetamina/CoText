#include "MainMenu.h"
#include "ui_MainMenu.h"


MainMenu::MainMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}


void MainMenu::on_pushButton_newDocument_clicked()
{
    emit newDocumentClicked();
}

void MainMenu::on_pushButton_openDocument_clicked()
{
    emit openDocumentClicked();
}

void MainMenu::on_pushButton_join_clicked()
{
    emit joinClicked();
}


void MainMenu::closeMainMenuSlot(){
    this->close();
}

void MainMenu::closeEvent (QCloseEvent *event)
{
    /*QMessageBox::StandardButton resBtn = QMessageBox::question( this, APP_NAME,
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }*/

    event->accept();
    emit sendCloseMainWindow();

}