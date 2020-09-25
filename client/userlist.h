#ifndef USERLIST_H
#define USERLIST_H

#include <QDialog>

namespace Ui {
class UserList;
}

class UserList : public QDialog
{
    Q_OBJECT

public:
    explicit UserList(QWidget *parent = nullptr);
    ~UserList();

private:
    Ui::UserList *ui;
};

#endif // USERLIST_H
