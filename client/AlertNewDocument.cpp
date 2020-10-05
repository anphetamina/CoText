#include "AlertNewDocument.h"
#include "ui_AlertNewDocument.h"

AlertNewDocument::AlertNewDocument(QString currentDocument, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertNewDocument)
{
    ui->setupUi(this);
    ui->label->setText("Do you want to close "+ currentDocument+" and open a new document?");
}

AlertNewDocument::~AlertNewDocument()
{
    delete ui;
}

void AlertNewDocument::on_pushButton_ok_clicked()
{
    emit(openNewDocument("Untitled"));
    this->close();
}

void AlertNewDocument::on_pushButton_cancel_clicked()
{
    this->close();
}
