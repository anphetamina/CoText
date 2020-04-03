#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:
    void on_pushButton_Register_clicked();

private:
    QString name;
    QString surname;
    QString nickname;
    QString pass1;
    QString pass2;
    QString email;
    Ui::Register *ui;
    void clearInput();
};

#endif // REGISTER_H
