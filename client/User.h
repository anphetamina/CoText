//
// Created by Emanuele Munafò on 13/04/2020.
//
#pragma  once
#include <cstdint>
#include <QIcon>
#include <QString>


#define ACTIVE 20
#define INACTIVE 21
#define DO_NOT_DISTURB 22
#define OFFLINE 19


class User  {
    friend QDataStream& operator>>(QDataStream& in, User& user);
    friend QDataStream& operator<<(QDataStream& out, const User& user);
public:
    User();
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
    QList<User*> friendList;
    QString name;
    QString surname;
    QString nickname;
    QString email;
    QIcon profilePic;
    int userState;
    int secondsBeforeInactive;
    uint8_t signature = 0x00;

    void setSignature(uint8_t signature);

    int getSignature();
};

