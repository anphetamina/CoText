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

namespace Ui { class MainWindow; }

class TextEditor : public QObject {

    Q_OBJECT

public:
    explicit TextEditor(QWidget &parent, Ui::MainWindow &ui, SharedEditor &editor, int numWorkers = 3, int numListeners = 1);

    virtual ~TextEditor();

private:

    QWidget &parent;
    Ui::MainWindow &ui;
    SharedEditor &editor;
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

    void remoteInsert(const Symbol& symbol);
    void remoteErase(const Symbol &symbol);
    int getPosition(int row, int col);

    /**
     * concurrency
     */

    int numWorkers;
    int numListeners;
    std::vector<std::thread> workers;
    std::vector<std::thread> listeners;
    std::mutex editorMutex;
    std::mutex messagesMutex;
    std::condition_variable is_empty;
    std::stack<Message> messages;
    bool listening;

private slots:

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

};


#endif //COTEXT_CLIENT_TEXTEDITOR_H
