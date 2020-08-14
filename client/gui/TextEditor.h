//
// Created by Antonio Santoro on 06/08/2020.
//

#ifndef COTEXT_CLIENT_TEXTEDITOR_H
#define COTEXT_CLIENT_TEXTEDITOR_H


#include <QDebug>
#include <iostream>
#include <QtWidgets/QTextEdit>
#include <QtGui/QPainter>
#include "../SharedEditor.h"

namespace Ui { class MainWindow; }

class TextEditor : public QTextEdit {

    Q_OBJECT

public:
    explicit TextEditor(Ui::MainWindow &ui, QWidget *parent = nullptr);

private:

    QWidget *parent;

    /**
     * to access to the toolbar icons
     */
    Ui::MainWindow &ui;
    SharedEditor editor;
    std::vector<int> index;

    std::vector<std::vector<Symbol>> testSymbols;

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

    std::atomic<bool> isFromRemote;
    int getPosition(int row, int col);

    QPainter painter;


public slots:

    void remoteInsert(Symbol symbol);
    void remoteInsertBlock(std::vector<Symbol> symbols);
    void remoteErase(Symbol symbol);
    void remoteEraseBlock(std::vector<Symbol> symbols);

private slots:

    /**
     * font style management
     */


    void selectFont();
    void setFontBold(bool bold);
    void setFontColor();
    void updateToolbar(const QTextCharFormat &format);


    /**
     * document content management
     */

    void contentsChange(int position, int charsRemoved, int charsAdded);

signals:

    void symbolsInserted(std::vector<Symbol> symbols, int siteId);
    void symbolsErased(std::vector<Symbol> symbols, int siteId);

};


#endif //COTEXT_CLIENT_TEXTEDITOR_H
