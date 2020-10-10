#include "OpenDocument.h"
#include "ui_OpenDocument.h"
#include "AlertNewDocument.h"
#include "DeletePushButton.h"
#include <QDebug.h>
#include <QtWidgets/QHBoxLayout>

OpenDocument::OpenDocument(QVector<QString> docList, MainWindow* mw, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::OpenDocument),
        documentList(docList)
{
    ui->setupUi(this);
    mainWindow = mw;
    qDebug() << "Doclist = "<<docList;

    repaint();

    /*for(QString document : docList){
        //ui->listWidget->addItem(document);
        QListWidgetItem * item = new QListWidgetItem (document);
        ui->listWidget->addItem ( item );
        QWidget * w = new QWidget();
        w->setLayout ( new QHBoxLayout() );
        DeletePushButton *but = new DeletePushButton ( ui->listWidget->row(item));
        QPixmap pixmap(":/imgs/icons/noun_user login_178831.svg");
        QIcon buttonIcon(pixmap);
        but->setIcon(buttonIcon);
        but->setIconSize(pixmap.rect().size()/5);
        but->setFixedSize(pixmap.rect().size()/5);
        QLabel *lab = new QLabel ();
        w->layout()->addWidget ( lab );
        w->layout()->addWidget ( but );
        w->layout()->setContentsMargins ( 1, 1, 1, 1 );

        connect(but, &DeletePushButton::releasedWithIndex,this, &OpenDocument::buttonPressed);

        //qDebug() << "[OPEN DOCUMENT] create DeletePushButton index = " << ui->listWidget->row(item);

        ui->listWidget->setItemWidget ( item, w );
    }*/
}

void OpenDocument::buttonPressed(int i){
    //qDebug() << "[OPEN DOCUMENT] buttonPressed index = "<< i<< " docName = "<<ui->listWidget->item(i)->text();
    emit documentDeleted(ui->listWidget->item(i)->text());
    documentList.removeOne(ui->listWidget->item(i)->text());
    ui->listWidget->removeItemWidget(ui->listWidget->currentItem());
    ui->listWidget->clear();
    repaint();
}

void OpenDocument::repaint(){
    for(QString document : documentList){
        //ui->listWidget->addItem(document);
        QListWidgetItem * item = new QListWidgetItem (document);
        ui->listWidget->addItem ( item );
        QWidget * w = new QWidget();
        w->setLayout ( new QHBoxLayout() );
        DeletePushButton *but = new DeletePushButton ( ui->listWidget->row(item));
        QPixmap pixmap(":/imgs/icons/noun_user login_178831.svg");
        QIcon buttonIcon(pixmap);
        but->setIcon(buttonIcon);
        but->setIconSize(pixmap.rect().size()/5);
        but->setFixedSize(pixmap.rect().size()/5);
        QLabel *lab = new QLabel ();
        w->layout()->addWidget ( lab );
        w->layout()->addWidget ( but );
        w->layout()->setContentsMargins ( 1, 1, 1, 1 );

        connect(but, &DeletePushButton::releasedWithIndex,this, &OpenDocument::buttonPressed);

        //qDebug() << "[OPEN DOCUMENT] create DeletePushButton index = " << ui->listWidget->row(item);

        ui->listWidget->setItemWidget ( item, w );
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