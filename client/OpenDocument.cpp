#include "OpenDocument.h"
#include "ui_OpenDocument.h"
#include "AlertNewDocument.h"
#include <QDebug.h>

OpenDocument::OpenDocument(QVector<QString> docList, MainWindow* mw, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::OpenDocument)
{
    ui->setupUi(this);
    mainWindow = mw;
    qDebug() << "Doclist = "<<docList;
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
    if(mainWindow->getTextEditor()->isEnabled()){    //c'è già un documento aperto
        //qDebug()<<"[OPEN DOCUMENT] c'è già un documento aperto = "<<mainWindow->windowTitle();
        AlertNewDocument alert(mainWindow->windowTitle(), ui->listWidget->currentItem()->text());
        connect(&alert, &AlertNewDocument::openNewDocument, this, &OpenDocument::forwardOpenNewDocument);
        alert.setWindowTitle("Alert");
        alert.setModal(true);
        alert.exec();
    }else { //non c'è nessun documento aperto
        emit(sendOpenDocument(ui->listWidget->currentItem()->text()));
    }
    this->close();
}

void OpenDocument::forwardOpenNewDocument(QString docName){
    //qDebug()<<"[OPEN DOCUMENT] forwardOpenNewDocument docName = "<<docName;
    emit(sendOpenDocument(docName));
}