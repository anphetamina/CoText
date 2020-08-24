//
// Created by Antonio Santoro on 06/08/2020.
//

#ifndef COTEXT_CLIENT_TEXTEDITOR_H
#define COTEXT_CLIENT_TEXTEDITOR_H


#include <QDebug>
#include <iostream>
#include <QtWidgets/QPlainTextEdit>
#include <QtGui/QPainter>
#include "../common/SharedEditor.h"
#include "../common/QSymbol.h"
#include "UserHighlighter.h"

class UserHighlighter;

namespace Ui { class MainWindow; }

class TextEditor : public QPlainTextEdit {

    Q_OBJECT

public:
    explicit TextEditor(int siteId, Ui::MainWindow &ui, QWidget *parent = nullptr);

    int getRow(int position) const;
    int getCol(int row, int position) const;
    QColor getUserColor(int userId) const;
    int getUserId(int row, int col) const;


private:

    QWidget *parent;

    /**
     * to access to the toolbar icons
     */
    Ui::MainWindow &ui;

    SharedEditor editor;
    std::vector<int> index;
    std::vector<std::vector<QSymbol>> testSymbols;
    int currentSelectedChars;
    UserHighlighter highlighter;

    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);

    void insertRow(int pos, int n);
    void deleteRow(int pos, int n);
    void incrementIndex(int pos, int n);
    void decrementIndex(int pos, int n);

    std::atomic<bool> isFromRemote;
    int getPosition(int row, int col);

    std::map<int, int> cursors;
    std::map<int, QTextEdit::ExtraSelection> selections;
    std::map<int, QColor> userColors;

    std::atomic<bool> isUserColorsToggled;

    void printSymbols();


public slots:

    void remoteInsert(QSymbol symbol);
    void remoteInsertBlock(std::vector<QSymbol> symbols);
    void remoteErase(QSymbol symbol);
    void remoteEraseBlock(std::vector<QSymbol> symbols);

    void updateCursor(int userId, int position);
    void updateSelection(int userId, QTextCursor cursor);

    void openDocument(std::vector<std::vector<QSymbol>> symbols);

private slots:

    void mergeFormat(const QTextCharFormat &format);
    void selectFont();
    void setFontBold(bool bold);
    void setFontItalic(bool italic);
    void setFontUnderline(bool underline);
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
