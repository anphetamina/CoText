#include "UserList.h"

#include "ui_UserList.h"

UserList::UserList(QWidget *parent, QVector<User> uList, QMap<int, QColor> colorMap) :
    QDialog(parent),
    ui(new Ui::UserList)
{
    ui->setupUi(this);
    this->setWindowTitle("Users working on the document");
    for(int i=0; i<uList.size(); i++){
        ui->listWidget->addItem(uList[i].getEmail());
        ui->listWidget->item(i)->setForeground(colorMap.take(uList[i].getId()));
    }
}

UserList::~UserList()
{
    delete ui;
}
