#ifndef OPENDOCUMENT_H
#define OPENDOCUMENT_H

#include <QDialog>
#include "TextEditor.h"
#include "MainWindow.h"

namespace Ui {
    class OpenDocument;
}

class OpenDocument : public QDialog
{
    Q_OBJECT

public:
    explicit OpenDocument(QVector<QString> docList, MainWindow* mw, QWidget *parent = nullptr);
    ~OpenDocument();

private slots:
    void on_pushButton_clicked();

signals:
    void sendOpenDocument(QString docName);

public slots:
    void forwardOpenNewDocument(QString docName);

private:
    Ui::OpenDocument *ui;
    MainWindow* mainWindow;
};

#endif // OPENDOCUMENT_H