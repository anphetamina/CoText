#include "ChooseName.h"
#include "ui_ChooseName.h"

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
        emit(nameChoosen(ui->lineEdit->text()));
        this->close();
    }

}

void ChooseName::on_pushButton_cancel_clicked()
{
    this->close();
}
