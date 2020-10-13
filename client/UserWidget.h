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





private:
    Ui::UserWidget *ui;

private slots:
	void on_pushButton_Edit_clicked();
	void on_pushButton_Cancel_clicked();
	
};

#endif // USERWIDGET_H
