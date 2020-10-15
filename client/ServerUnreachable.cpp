#include "ServerUnreachable.h"
#include <ui_ServerUnreachable.h>

ServerUnreachable::ServerUnreachable(Benchmark* b, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerUnreachable)
{
    ui->setupUi(this);
    benchmark = b;
}

ServerUnreachable::~ServerUnreachable()
{
    delete ui;
}

void ServerUnreachable::on_pushButton_tryAgain_clicked()
{
    benchmark->startTimer();
    this->close();
}

void ServerUnreachable::on_pushButton_quit_clicked()
{
    benchmark->stopTimer();
    this->close();
}
