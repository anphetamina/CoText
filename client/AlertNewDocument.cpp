#include "AlertNewDocument.h"
#include "ui_AlertNewDocument.h"

#include <QDebug.h>

AlertNewDocument::AlertNewDocument(QString currentDocument, QString newDoc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertNewDocument)
{
    ui->setupUi(this);
    newDocument = newDoc;
    if(!newDocument.isEmpty()){ //open an existing document
        ui->label->setText("Do you want to close "+ currentDocument+" and open "+ newDocument +" ?");
    }else{  //create new document
        ui->label->setText("Do you want to close "+ currentDocument+" and open a new document ?");
    }
}

AlertNewDocument::~AlertNewDocument()
{
    delete ui;
}

void AlertNewDocument::on_pushButton_ok_clicked()
{
    if(!newDocument.isEmpty()){ //open an existing document
        //qDebug()<<"[ALERT] open an existing document newDocument = "<<newDocument;
        emit(openNewDocument(newDocument));
    }else{  //create new document
        //qDebug()<<"[ALERT] create new document";
        emit(openNewDocument("Untitled"));
    }
    this->close();
}

void AlertNewDocument::on_pushButton_cancel_clicked()
{
    this->close();
}
