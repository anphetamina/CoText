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


private:
    Ui::UserWidget *ui;

};

#endif // USERWIDGET_H
