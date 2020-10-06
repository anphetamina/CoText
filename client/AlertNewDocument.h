#ifndef ALERTNEWDOCUMENT_H
#define ALERTNEWDOCUMENT_H

#include <QDialog>

namespace Ui {
class AlertNewDocument;
}

class AlertNewDocument : public QDialog
{
    Q_OBJECT

public:
    explicit AlertNewDocument(QString currentDocument, QString newDocument, QWidget *parent = nullptr);
    ~AlertNewDocument();

signals:
    void openNewDocument(QString docName);

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::AlertNewDocument *ui;
    QString newDocument;
};

#endif // ALERTNEWDOCUMENT_H
