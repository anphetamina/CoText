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
    this->done(0);
}

void MainMenu::closeEvent (QCloseEvent *event)
{
    event->accept();
    emit sendCloseMainWindow();
}