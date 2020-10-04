#ifndef OPENDOCUMENT_H
#define OPENDOCUMENT_H

#include <QDialog>

namespace Ui {
class OpenDocument;
}

class OpenDocument : public QDialog
{
    Q_OBJECT

public:
    explicit OpenDocument(QVector<QString> docList, QWidget *parent = nullptr);
    ~OpenDocument();

private slots:
    void on_pushButton_clicked();

signals:
    void sendOpenDocument(QString docName);

private:
    Ui::OpenDocument *ui;
};

#endif // OPENDOCUMENT_H
