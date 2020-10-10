//
// Created by dimar on 10/10/2020.
//

#ifndef COTEXT_CLIENT_DELETEPUSHBUTTON_H
#define COTEXT_CLIENT_DELETEPUSHBUTTON_H


#include <QtWidgets/QPushButton>

class DeletePushButton : public QPushButton{

Q_OBJECT

public:
    DeletePushButton(int i);

signals:
    void releasedWithIndex(int i);

public slots:
    void releasedReceived();

private:
    int index;
};


#endif //COTEXT_CLIENT_DELETEPUSHBUTTON_H
