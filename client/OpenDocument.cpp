#include "OpenDocument.h"
#include "ui_OpenDocument.h"
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

    repaint();
}

void OpenDocument::buttonPressed(int i){
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Delete document", "Are you sure to delete '"+ui->listWidget->item(i)->text()+"' ?\n", QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

    if(resBtn == QMessageBox::Yes) {
        emit documentDeleted(ui->listWidget->item(i)->text());
        documentList.removeOne(ui->listWidget->item(i)->text());
        ui->listWidget->removeItemWidget(ui->listWidget->currentItem());
        ui->listWidget->clear();
        repaint();
    }
}

void OpenDocument::repaint(){
    for(QString document : documentList){
        if(mainWindow->windowTitle() != document){
            QListWidgetItem * item = new QListWidgetItem (document);
            ui->listWidget->addItem ( item );
            QWidget * w = new QWidget();
            w->setLayout ( new QHBoxLayout() );
            DeletePushButton *but = new DeletePushButton ( ui->listWidget->row(item));
            QString filename(":/imgs/icons/noun_Garbage_2025401.svg");
            QImage pixmap(filename);
            QImage scaledPixmap = pixmap.scaled(22, 22, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QImage garbageImage = pixmap.convertToFormat(QImage::Format_ARGB32);
            
            QPixmap pix(QPixmap::fromImage(garbageImage));
            
            QIcon buttonIcon(pix);
            
            
           
            but->setIcon(buttonIcon);
            but->setIconSize(QSize(22, 22));
            but->setFixedSize(QSize(22, 22));
            QLabel *lab = new QLabel ();
            w->layout()->addWidget ( lab );
            w->layout()->addWidget ( but );
            w->layout()->setContentsMargins ( 1, 1, 1, 1 );

            connect(but, &DeletePushButton::releasedWithIndex,this, &OpenDocument::buttonPressed);

            ui->listWidget->setItemWidget ( item, w );
        }
    }
}

OpenDocument::~OpenDocument()
{
    delete ui;
}

void OpenDocument::on_pushButton_clicked()
{
    if(ui->listWidget->currentItem() != nullptr){

        if(mainWindow->getTextEditor()->isEnabled()){    //c'è già un documento aperto

            QString messageText;
            if(ui->listWidget->currentItem()->text().isEmpty()){    //create new document
                messageText = "Do you want to close "+ mainWindow->windowTitle()+" and open a new document ?";
            }else{                      //open an existing document
                messageText = "Do you want to close '"+ mainWindow->windowTitle()+"' and open '"+ ui->listWidget->currentItem()->text() +"' ?";
            }

            QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Alert", messageText,  QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

            if(resBtn == QMessageBox::Yes) {
                if(ui->listWidget->currentItem()->text().isEmpty()){   //create new document
                    emit sendOpenDocument("Untitled");
                }else{                      //open an existing document
                    emit sendOpenDocument(ui->listWidget->currentItem()->text());
                }
            }

        }else { //non c'è nessun documento aperto
            emit sendOpenDocument(ui->listWidget->currentItem()->text());
        }
    }
}

void OpenDocument::closeOpenDocument(){
    this->close();
}

void OpenDocument::openDocFailedOD(){
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Document opening failed", "You haven't rights to open this document",  QMessageBox::Cancel | QMessageBox::Retry, QMessageBox::Retry);

    if (resBtn == QMessageBox::Retry) {
        ui->pushButton->click();
    }
}