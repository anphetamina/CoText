#include "OpenDocument.h"
#include "ui_OpenDocument.h"

OpenDocument::OpenDocument(QVector<QString> docList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenDocument)
{
    ui->setupUi(this);
    for(QString document : docList){
        ui->listWidget->addItem(document);
    }
}

OpenDocument::~OpenDocument()
{
    delete ui;
}

void OpenDocument::on_pushButton_clicked()
{
    emit(sendOpenDocument(ui->listWidget->currentItem()->text()));  //todo if 2 items have the same name?
    this->close();
}
