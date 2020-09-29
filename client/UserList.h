#ifndef USERLIST_H
#define USERLIST_H

#include <QDialog>
#include <User.h>
#include <QtCore/QMap>

namespace Ui {
class UserList;
}

class UserList : public QDialog
{
    Q_OBJECT

public:
    explicit UserList(QWidget *parent = nullptr, QVector<User> uList = {}, QMap<int, QColor> colorMap = {});
    ~UserList();

private slots:

private:
    Ui::UserList *ui;
};

#endif // USERLIST_H
