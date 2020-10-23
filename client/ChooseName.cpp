#include "ChooseName.h"
#include "ui_ChooseName.h"

#include <QDebug.h>
#include <QtSvg/QtSvg>

ChooseName::ChooseName(QVector<QString> dList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseName)
{
    ui->setupUi(this);
    ui->label->setVisible(false);
    QPalette palette = ui->label->palette();
    palette.setColor(ui->label->backgroundRole(), Qt::red);
    palette.setColor(ui->label->foregroundRole(), Qt::red);
    ui->label->setPalette(palette);

    docList = dList;
}

ChooseName::~ChooseName()
{
    delete ui;
}

void ChooseName::on_pushButton_ok_clicked()
{
    if(docList.contains(ui->lineEdit->text())){
        ui->label->setVisible(true);
    }else{
       emit nameChosen(ui->lineEdit->text());
    }
}

void ChooseName::on_pushButton_cancel_clicked()
{
    this->close();
}

void ChooseName::closeChooseName(){
    this->close();
}

void ChooseName::openDocFailedCN(){
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Document opening failed", "You haven't rights to open this document",  QMessageBox::Cancel | QMessageBox::Retry, QMessageBox::Retry);

    if(resBtn == QMessageBox::Cancel) {
        ui->lineEdit->setText("");
    } else if (resBtn == QMessageBox::Retry) {
        ui->pushButton_ok->click();
    }
}
