#ifndef CHOOSENAME_H
#define CHOOSENAME_H

#include <QDialog>

namespace Ui {
class ChooseName;
}

class ChooseName : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseName(QVector<QString> dList, QWidget *parent = nullptr);
    ~ChooseName();

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();

signals:
    void nameChoosen(QString name);

private:
    Ui::ChooseName *ui;
    QVector<QString> docList;
};

#endif // CHOOSENAME_H
