#ifndef USER_H
#define USER_H

#include <QDialog>
#include <QIcon>

#define ACTIVE 20
#define INACTIVE 21
#define DO_NOT_DISTURB 22
#define OFFLINE 19


namespace Ui {
class User;
}

class User : public QDialog {
    Q_OBJECT

public:
    explicit User(QWidget *parent = nullptr);
    ~User();
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
    Ui::User *ui;
    QList<User*> friendList;
    QString name;
    QString surname;
    QString nickname;
    QString email;
    QIcon profilePic;
    int userState;
    int secondsBeforeInactive;
};

#endif // USER_H
