#ifndef SERVERDISCONNECTED_H
#define SERVERDISCONNECTED_H

#include <QDialog>

namespace Ui {
class ServerDisconnected;
}

class ServerDisconnected : public QDialog
{
    Q_OBJECT

public:
    explicit ServerDisconnected(QWidget *parent = nullptr);
    ~ServerDisconnected();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ServerDisconnected *ui;
};

#endif // SERVERDISCONNECTED_H
