//
// Created by Antonio Santoro on 06/08/2020.
//

#ifndef COTEXT_CLIENT_TEXTEDITOR_H
#define COTEXT_CLIENT_TEXTEDITOR_H


#include <QDebug>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stack>
#include <QtWidgets/QTextEdit>
#include "MainWindow.h"
#include "../SharedEditor.h"
//#include "../sslechoclient.h"

namespace Ui { class MainWindow; }


// Forward declaration (im using just pointer and in this way avoid the circular dep. issue)
class SslEchoClient;

class TextEditor : public QObject {

    Q_OBJECT



public:
    explicit TextEditor(QWidget &parent, Ui::MainWindow &ui);
    TextEditor(QWidget &parent, Ui::MainWindow &ui, SslEchoClient *client);

    virtual ~TextEditor();

private:

    QWidget &parent;
    Ui::MainWindow &ui;
    SharedEditor editor;
    SslEchoClient* sslEchoClient;
    QThread *listener;
    std::vector<int> index;

    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);

    /**
     * local
     */
    void insertRow(int pos, int n);
    void deleteRow(int pos, int n);
    void incrementIndex(int pos, int n);
    void decrementIndex(int pos, int n);
    int getRow(int position);
    int getCol(int row, int position);


    /**
     * remote
     */

    int getPosition(int row, int col);



public slots:

    void remoteInsert(Symbol symbol);
    void remoteErase(Symbol symbol);

private slots:

    //void remoteInsert(Symbol symbol);
    //void remoteErase(Symbol symbol);

    /**
     * font style management
     */
    void selectFont();
    void setFontBold(bool bold);
    void setFontUnderline(bool underline);
    void setFontItalic(bool italic);
    void setFontColor();

    void currentCharFormatChanged(const QTextCharFormat &format);


    /**
     * document content management
     */

    void contentsChange(int position, int charsRemoved, int charsAdded);
    void cursorPositionChanged();

signals:

    void sendSymbol(Symbol symbol, int type, int siteId);


};


#endif //COTEXT_CLIENT_TEXTEDITOR_H
