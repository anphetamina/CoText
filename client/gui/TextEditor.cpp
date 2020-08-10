//
// Created by Antonio Santoro on 06/08/2020.
//

#include <ui_mainwindow.h>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QFontDialog>
#include "TextEditor.h"

TextEditor::TextEditor(QWidget &parent, Ui::MainWindow &ui, SharedEditor &editor) : parent(parent), ui(ui), editor(editor), index({0}) {


    /**
    * font styling connections
    */

    connect(ui.textEdit, &QTextEdit::currentCharFormatChanged, this, &TextEditor::currentCharFormatChanged);
    connect(ui.textEdit, &QTextEdit::cursorPositionChanged, this, &TextEditor::cursorPositionChanged);

    connect(ui.actionFont, &QAction::triggered, this, &TextEditor::selectFont);
    connect(ui.actionBold, &QAction::triggered, this, &TextEditor::setFontBold);
    connect(ui.actionItalic, &QAction::triggered, this, &TextEditor::setFontItalic);
    connect(ui.actionUnderline, &QAction::triggered, this, &TextEditor::setFontUnderline);
    connect(ui.actionTextColor, &QAction::triggered, this, &TextEditor::setFontColor);

    /**
     * document content connections
     */

    connect(ui.textEdit->document(), &QTextDocument::contentsChange, this, &TextEditor::contentsChange);
}

void TextEditor::selectFont() {
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, &parent);
    if (fontSelected)
        ui.textEdit->setFont(font);
}

void TextEditor::setFontBold(bool bold) {
    bold ? ui.textEdit->setFontWeight(QFont::Bold) :
    ui.textEdit->setFontWeight(QFont::Normal);
}

void TextEditor::setFontUnderline(bool underline) {
    ui.textEdit->setFontUnderline(underline);
}

void TextEditor::setFontItalic(bool italic) {
    ui.textEdit->setFontItalic(italic);
}

void TextEditor::setFontColor() {
    QColor col = QColorDialog::getColor(ui.textEdit->textColor(), &parent);
    if (!col.isValid())
        return;
    ui.textEdit->setTextColor(col);
    colorChanged(col);
}

void TextEditor::currentCharFormatChanged(const QTextCharFormat &format) {
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void TextEditor::cursorPositionChanged() {
    QTextCursor cursor = ui.textEdit->textCursor();
}

void TextEditor::fontChanged(const QFont &f) {
    ui.actionBold->setChecked(f.bold());
    ui.actionItalic->setChecked(f.italic());
    ui.actionUnderline->setChecked(f.underline());
}

void TextEditor::colorChanged(const QColor &c) {
    QPixmap pix(16, 16);
    pix.fill(c);
    ui.actionTextColor->setIcon(pix);
}

void TextEditor::contentsChange(int position, int charsRemoved, int charsAdded) {


    /**
     * QTextEdit bug
     * pasting >= characters gives wrong number of added and removed chars
     */
    if (charsAdded >= 1 && charsRemoved >= 1) {
        charsAdded--;
        charsRemoved--;
    }

    /**
     * first solution maps a 1-dimensional index (position) to a 2-dimensional one
     */

//
//    if (charsRemoved > 0) {
//
//        int startLine = 0, startIndex = 0, acc = 0;
//
//        /**
//         * find the 2-dimensional index given the 1-dimensional one (position)
//         * the equal has been introduced because the index points to a non-free position
//         */
//        while ((acc = editor.getSymbols()[startLine].size()+acc) <= position) {
//            startLine++;
//            startIndex = acc;
//        }
//        startIndex = position - startIndex;
//
//        int endLine = startLine;
//        int endIndex = startIndex + charsRemoved - 1;
//        int diff = charsRemoved - editor.getSymbols()[endLine].size() + startIndex;
//
//        /**
//         * iterate until the remaining characters to remove are positive
//         * and compute the difference line by line
//         * the ending index is the number of the remaining character -1 in the end line
//         */
//        while (diff > 0) {
//            endIndex = diff - 1;
//            diff -= editor.getSymbols()[++endLine].size();
//        }
//
////        qDebug() << "start line" << startLine << "start index" << startIndex << "end line" << endLine << "end index" << endIndex;
//
//        editor.localErase(startLine, startIndex, endLine, endIndex);
//    }
//
//    if (charsAdded > 0) {
//
//        int line = 0, index = 0, acc = 0;
//
//        /**
//         * first search for the x,y position inside the symbols
//         * starting given the 1-dimensional index (position)
//         */
//        while ((acc = editor.getSymbols()[line].size()+acc) < position) {
//            line++;
//            index = acc;
//        }
//        index = position - index;
//
//        int pos = position;
//        for (int i = 0; i < charsAdded; ++i) {
//            QChar addedChar = ui.textEdit->document()->characterAt(pos);
//
//            if (addedChar == QChar::LineFeed || addedChar == QChar::ParagraphSeparator) {
//                editor.localInsert(line, index, '\n');
//            } else {
//                editor.localInsert(line, index, addedChar.toLatin1());
//            }
//
//            /**
//             * index is always incremented due to the fact that if a line ends with a '\n'
//             * the character is inserted at the beginning of the next line
//             * thus the index must be incremented anyway
//             */
//            if (index == editor.getSymbols()[line].size()) {
//                line++;
//                index = 0;
//            }
//            index++;
//
//            pos++;
//        }
//    }


    /**
     * second solution exploits a vector of indexes that keeps track of the initial position
     * in terms of 1-dimensional numbers
     */


    if (charsRemoved > 0) {
        int endPosition = position + charsRemoved - 1;
        int startRow = getRow(position);
        int startCol = getCol(startRow, position);
        int endRow = getRow(endPosition);
        int endCol = getCol(endRow, endPosition);

        int oldSize = editor.getSymbols().size();

        editor.localErase(startRow, startCol, endRow, endCol);

        int newSize = editor.getSymbols().size();

        decrementIndex(startRow, charsRemoved);
        deleteRow(startRow, oldSize - newSize);


    }

    if (charsAdded > 0) {

        /**
         * get row and col from the index
         * save the number of chars to increment the index rows right after
         * save the number of new rows to insert
         */
        int row = getRow(position);
        int col = getCol(row, position);
        int n = charsAdded;
        int pos = row;
        int newRows = 0;

        while (charsAdded > 0) {
            QChar addedChar = ui.textEdit->document()->characterAt(position++);

            if (addedChar == QChar::LineFeed || addedChar == QChar::ParagraphSeparator) {
                editor.localInsert(row, col, '\n');
                newRows++;
            } else {
                editor.localInsert(row, col, addedChar.toLatin1());
            }

            /**
             * if it reaches the end of the line go in the next one
             */
            if (col == editor.getSymbols()[row].size()) {
                row++;
                col = 0;
            }

            col++;
            charsAdded--;
        }

        incrementIndex(pos, n);
        insertRow(pos, newRows);
    }


    /**
     * print the current document in the console
     */
    const auto& symbols = editor.getSymbols();
    for (int i = 0; i < symbols.size(); i++) {
        std::cout << "[" << index[i] << "] ";
        for (int j = 0; j < symbols[i].size(); j++) {
            std::cout << symbols[i][j].getC();
        }
    }

    std::cout << std::endl << "---" << std::endl;
}

/**
 * insert n rows after pos
 * @param pos
 * @param n
 */
void TextEditor::insertRow(int pos, int n) {
    if (pos >= index.size()) {
        return;
    }
    for (int i = pos; i < n + pos; i++) {
        index.emplace(index.begin() + i + 1, index[i] + editor.getSymbols()[i].size());
    }
}

/**
 * delete n rows after pos
 * @param pos
 * @param n
 */
void TextEditor::deleteRow(int pos, int n) {
    if (pos+n >= index.size() || n == 0) {
        return;
    }
    index.erase(index.begin() + pos + 1, index.begin() + pos + n + 1);
    if (pos+1 >= index.size()) {
        return;
    }
    for (int i = pos; i < index.size()-1; i++) {
        index[i+1] = index[i] + editor.getSymbols()[i].size();
    }
}

/**
 * increments by n all the rows after pos
 * @param pos
 * @param n
 */
void TextEditor::incrementIndex(int pos, int n) {
    if (pos+1 >= index.size()) {
        return;
    }

    for (int i = pos+1; i < index.size(); i++) {
        index[i] += n;
    }
}

/**
 * decrement n rows after pos
 * @param pos
 * @param n
 */
void TextEditor::decrementIndex(int pos, int n) {
    if (pos+1 >= index.size()) {
        return;
    }

    for (int i = pos+1; i < index.size(); i++) {
        index[i] -= n;
    }
}

/**
 * @param row
 * @param position
 * @return the relative offset from a given row
 */
int TextEditor::getCol(int row, int position) {
    return position - index[row];
}

/**
 *
 * @param position
 * @return the row matching the 1-dimensional index (position)
 * e.g.
 * 1) antonio\n
 * 2) santoro\n
 * 3)
 * for position 8 returns row 2
 */
int TextEditor::getRow(int position) {

    auto it = std::lower_bound(index.begin(), index.end(), position);
    if (it == index.end()) {
        return index.size()-1;
    }
    int row = it - index.begin();
    if (index[row] > position) {
        return row-1;
    }
    return row;
}

/**
 * insert symbol received from the server
 * @param symbol
 */
void TextEditor::remoteInsert(Symbol symbol) {
    std::pair<int, int> pos = editor.remoteInsert(symbol);
    if (pos != std::make_pair(-1, -1)) {
        int position = getPosition(pos.first, pos.second);
        ui.textEdit->textCursor().setPosition(position);
        ui.textEdit->textCursor().insertText(QChar::fromLatin1(symbol.getC()));
    }
}

/**
 * erase symbol received from the server
 * @param symbol
 */
void TextEditor::remoteErase(Symbol symbol) {
    std::pair<int, int> pos = editor.remoteErase(symbol);
    if (pos != std::make_pair(-1, -1)) {
        int position = getPosition(pos.first, pos.second);
        ui.textEdit->textCursor().setPosition(position);
        ui.textEdit->textCursor().deleteChar();
    }
}

/**
 *
 * @param row
 * @param col
 * @return the absolute position mapped from (row,col)
 */
int TextEditor::getPosition(int row, int col) {
    int pos = index[row];
    pos += col;
    return pos;
}
