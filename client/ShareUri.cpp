#include "ShareUri.h"
#include "ui_ShareUri.h"

ShareUri::ShareUri(QString URI, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShareUri)
{
    ui->setupUi(this);
    ui->lineEdit_URI->setText(URI);
}

ShareUri::~ShareUri()
{
    delete ui;
}

void ShareUri::on_pushButton_copyLink_clicked()
{
    //ui->lineEdit_URI->copy();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->clear();
    clipboard->setText(ui->lineEdit_URI->text());

    this->close();
}