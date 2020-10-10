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
    void documentDeleted(QString docName);

public slots:
    void forwardOpenNewDocument(QString docName);
    void buttonPressed(int i);

private:
    Ui::OpenDocument *ui;
    MainWindow* mainWindow;
    QVector<QString> documentList;

    void repaint();
};

#endif // OPENDOCUMENT_H