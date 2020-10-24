#include "resync.h"
#include "ui_resync.h"
#include <QKeyEvent>

Resync::Resync(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Resync)
{
    ui->setupUi(this);
}

Resync::~Resync()
{
    delete ui;
}

void Resync::on_pushButton_join_clicked()
{
    emit resync();
    this->close();
}

//disable the escape key event
void Resync::keyPressEvent(QKeyEvent *e) {
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
    else {
        return;
    }
}