#ifndef SERVERUNREACHABLE_H
#define SERVERUNREACHABLE_H

#include <QDialog>
#include <Benchmark.h>

namespace Ui {
class ServerUnreachable;
}

class ServerUnreachable : public QDialog
{
    Q_OBJECT

public:
    explicit ServerUnreachable(Benchmark* b, QWidget *parent = nullptr);
    ~ServerUnreachable();

private slots:
    void on_pushButton_tryAgain_clicked();

    void on_pushButton_quit_clicked();

private:
    Ui::ServerUnreachable *ui;
    Benchmark* benchmark;
};

#endif // SERVERUNREACHABLE_H
