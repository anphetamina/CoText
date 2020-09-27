#ifndef USERLIST_H
#define USERLIST_H

#include <QDialog>
#include <User.h>

namespace Ui {
class UserList;
}

class UserList : public QDialog
{
    Q_OBJECT

public:
    explicit UserList(QWidget *parent = nullptr, QVector<User> userList = {});
    ~UserList();

private slots:

private:
    Ui::UserList *ui;
};

#endif // USERLIST_H
