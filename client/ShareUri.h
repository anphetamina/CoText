#ifndef SHAREURI_H
#define SHAREURI_H

#include <QDialog>
#include <QClipboard>

namespace Ui {
class ShareUri;
}

class ShareUri : public QDialog
{
    Q_OBJECT

public:
    explicit ShareUri(QString URI, QWidget *parent = nullptr);
    ~ShareUri();

private slots:
    void on_pushButton_copyLink_clicked();

private:
    Ui::ShareUri *ui;
};

#endif // SHAREURI_H
