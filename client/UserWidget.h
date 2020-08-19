#ifndef USERWIDGET_H
#define USERWIDGET_H


#include <QDialog>
#include <QIcon>
#include "../common/User.h"
#include <QEvent>

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

private slots:



private:
    Ui::UserWidget *ui;
	bool eventFilter(QObject *watched, QEvent *event) override;

};

#endif // USERWIDGET_H
