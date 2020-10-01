#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:

    void on_pushButton_Reg_clicked();

    void on_pushButton_Login_clicked();

private:
    Ui::Login *ui;
	void closeEvent(QCloseEvent *event) override;
	
};

#endif // LOGIN_H
