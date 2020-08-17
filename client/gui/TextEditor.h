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
#include "../QSymbol.h"
#include "UserHighlighter.h"

namespace Ui { class MainWindow; }

class TextEditor : public QTextEdit {

    Q_OBJECT

public:
    explicit TextEditor(Ui::MainWindow &ui, QWidget *parent = nullptr);

    const std::map<int, QColor>& getUserColors();

private:

    QWidget *parent;

    /**
     * to access to the toolbar icons
     */
    Ui::MainWindow &ui;

    SharedEditor editor;
    std::vector<int> index;
    std::vector<std::vector<Symbol>> testSymbols;
    int currentSelectedChars;
    UserHighlighter highlighter;

    /**
     * toolbar updates
     */
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

    std::map<int, int> cursors;
    std::map<int, QTextEdit::ExtraSelection> selections;
    std::map<int, QColor> userColors;

    std::atomic<bool> isUserColorsToggled;


public slots:

    void remoteInsert(QSymbol qsymbol);
    void remoteInsertBlock(std::vector<Symbol> symbols);
    void remoteErase(Symbol symbol);
    void remoteEraseBlock(std::vector<Symbol> symbols);

    void updateCursor(int userId, int position);
    void updateSelection(int userId, QTextCursor cursor);

private slots:

    void selectFont();
    void setFontBold(bool bold);
    void setFontColor();
    void updateToolbar(const QTextCharFormat &format);

    void contentsChange(int position, int charsRemoved, int charsAdded);
    void cursorPositionChange();
    void selectionChange();

    void toggleUserColors();

protected:
    void paintEvent(QPaintEvent *e) override;

signals:

    void symbolsInserted(std::vector<QSymbol> symbols, int siteId);
    void symbolsErased(std::vector<QSymbol> symbols, int siteId);
    void cursorPositionChanged(int userId, int position);
    void selectionChanged(int userId, QTextCursor cursor);

};


#endif //COTEXT_CLIENT_TEXTEDITOR_H
