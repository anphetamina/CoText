#include "UserList.h"
#include "ui_UserList.h"
#include "DBConfClient.h"

UserList::UserList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserList)
{
    ui->setupUi(this);
    QStringList userlist = getUserlistFromDocument(12);
    qDebug() << "[USERLIST] Userlist = " <<  userlist;
    for(int i=0; i<userlist.size(); i++){
        ui->listWidget->addItem(userlist.at(i));
    }
}

UserList::~UserList()
{
    delete ui;
}
