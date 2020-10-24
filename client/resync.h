#ifndef RESYNC_H
#define RESYNC_H

#include <QDialog>

namespace Ui {
class Resync;
}

class Resync : public QDialog
{
    Q_OBJECT

public:
    explicit Resync(QWidget *parent = nullptr);
    ~Resync();

signals:
    void resync();

private slots:
    void on_pushButton_join_clicked();

private:
    Ui::Resync *ui;

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // RESYNC_H
