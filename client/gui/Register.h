#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QIcon>
#include "MainWindow.h"

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
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    //void dragLeaveEvent(QDragLeaveEvent *event) override;



private:
    QString name;
    QString surname;
    QString nickname;
    QString pass1;
    QString pass2;
    QString email;
    QIcon profilePicture;
    Ui::Register *ui;
    void clearInput();
};

#endif // REGISTER_H
