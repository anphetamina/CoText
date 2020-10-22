#ifndef JOIN_H
#define JOIN_H

#include <QDialog>
#include "MainWindow.h"

namespace Ui {
class Join;
}

class Join : public QDialog
{
    Q_OBJECT

public:
    explicit Join(QWidget *parent = nullptr);
    ~Join();

private slots:
    void on_pushButton_join_clicked();

public slots:
    void closeJoin();
    void showError();

signals:
    void sendJoin(qint32 userId, int docId, QString invCode);

private:
    Ui::Join *ui;
};

#endif // JOIN_H
