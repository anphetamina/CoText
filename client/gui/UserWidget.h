#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QDialog>
#include <QIcon>
#include "../User.h"

#define ACTIVE 20
#define INACTIVE 21
#define DO_NOT_DISTURB 22
#define OFFLINE 19


namespace Ui {
class UserWidget;
}

class UserWidget : public QDialog {
    Q_OBJECT

public:
    explicit UserWidget(QWidget *parent = nullptr);
    ~UserWidget();
    QString getName();
    QString getSurname();
    QString getEmail();
    QString getNick();
    QIcon getProfilePic();

    void setName(QString _name);
    void setSurname(QString _surname);
    void setProfilePic(QIcon _profilePic);
    void setNick(QString _nickname);
    void setEmail(QString _email);
    void addFriend(User * _usrFriend);
    void setTimeBeforeInactive(int _timer);
    void setUserState(int _state);

    QList<User*> getFriends();

    int getUserState();
    int getActualSecondsBeforeInactive();

private:
    Ui::UserWidget *ui;
    QList<User*> friendList;
    QString name;
    QString surname;
    QString nickname;
    QString email;
    QIcon profilePic;
    int userState;
    int secondsBeforeInactive;
};

#endif // USERWIDGET_H
