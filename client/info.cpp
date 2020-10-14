#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);
    QString filename(":/appIcon/icons/CoText.png");
    QImage pixmap(filename);
    QImage scaledPixmap = pixmap.scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QImage logo = scaledPixmap.convertToFormat(QImage::Format_ARGB32);
    ui->Logo->setPixmap(QPixmap::fromImage(logo));
}

Info::~Info()
{
    delete ui;
}
