//
// Created by Antonio Santoro on 06/08/2020.
//

#include <ui_mainwindow.h>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QFontDialog>
#include <QThread>
#include <QRandomGenerator>
#include <QtWidgets/QLabel>
#include "../common/Shuffler.h"
#include "TextEditor.h"


TextEditor::TextEditor(int siteId, Ui::MainWindow &ui, QWidget *parent) :
    parent(parent),
    ui(ui),
    index({0}),
    editor(SharedEditor(Shuffler::getInstance()->getRandomInt())), // todo get site id from server
    isFromRemote(false),
    testSymbols({{}}),
    cursors({}),
    currentSelectedChars(0),
    selections({}),
    userColors({}),
    highlighter(*this, document()),
    isUserColorsToggled(false) {


    userColors[editor.getSiteId()] = QColor::fromRgb(QRandomGenerator::global()->generate()); // todo get from connected user list
    document()->setDocumentMargin(50); // todo better margins
    setTextColor(Qt::white);
    qDebug() << "Current sID: "<< editor.getSiteId();

    /**
    * font styling connections
    */

    connect(this, &QTextEdit::currentCharFormatChanged, this, &TextEditor::updateToolbar);
    
    connect(ui.actionFont, &QAction::triggered, this, &TextEditor::selectFont);
    connect(ui.actionBold, &QAction::triggered, this, &TextEditor::setFontBold);
    connect(ui.actionTextColor, &QAction::triggered, this, &TextEditor::setFontColor);

    /**
     * document content connections
     */

    connect(document(), &QTextDocument::contentsChange, this, &TextEditor::contentsChange);

    connect(this, &QTextEdit::cursorPositionChanged, this, &TextEditor::cursorPositionChange);
    connect(this, &QTextEdit::selectionChanged, this, &TextEditor::selectionChange);

    connect(ui.actionToggle_user_colors, &QAction::triggered, this, &TextEditor::toggleUserColors);


    /**
     * testing code
     */

    /*QTextCharFormat f;
    f.setFontWeight(QFont::Bold);
    for (int i = 0; i < 200; i++) {
        for (int j = 0; j < 40; j++) {
            QSymbol s = editor.localInsert(i, j,'a', f);
            testSymbols[i].push_back(s);
        }
        QSymbol s = editor.localInsert(i, editor.getSymbols()[i].size(), QChar::LineFeed, f);
        testSymbols[i].push_back(s);
        testSymbols.emplace_back();
    }
    editor.clear();

    openDocument(testSymbols);*/
}

void TextEditor::selectFont() {
    // todo per character
    /*bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, &parent);
    if (fontSelected)
        setFont(font);*/
}

void TextEditor::setFontBold(bool bold) {
    bold ? setFontWeight(QFont::Bold) :
    setFontWeight(QFont::Normal);
}

void TextEditor::setFontColor() {
    // todo per character
    /*QColor col = QColorDialog::getColor(textColor(), &parent);
    if (!col.isValid())
        return;
    setTextColor(col);
    colorChanged(col);*/
}

/**
 * given the current format updates the icons on the editor toolbar in the main window
 * @param format
 */
void TextEditor::updateToolbar(const QTextCharFormat &format) {
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void TextEditor::fontChanged(const QFont &f) {
    ui.actionBold->setChecked(f.bold());
    ui.actionItalic->setChecked(f.italic());
    ui.actionUnderline->setChecked(f.underline());
}

void TextEditor::colorChanged(const QColor &c) {
    // todo fix icon in the toolbar
    QPixmap pix(16, 16);
    pix.fill(c);
    ui.actionTextColor->setIcon(pix);
}

void TextEditor::contentsChange(int position, int charsRemoved, int charsAdded) {

    if (isFromRemote) {
        isFromRemote = false;
        return;
    }

    /**
     * workaround for https://github.com/anphetamina/CoText/issues/22
     */
    if (position == 0 && charsAdded >= 1 && charsRemoved >= 1) {
        int oldSize = charsRemoved-1;
        int newSize = charsAdded-1;
        int diff = std::abs(newSize - oldSize);
        charsRemoved = currentSelectedChars;
        charsAdded = std::abs(charsRemoved - diff);
    }

    /**
     * first solution exploits a vector of indexes that keeps track of the initial position
     * in terms of 1-dimensional numbers
     */


    if (charsRemoved > 0) {

        try {
            int endPosition = position + charsRemoved - 1;
            int startRow = getRow(position);
            int startCol = getCol(startRow, position);
            int endRow = getRow(endPosition);
            int endCol = getCol(endRow, endPosition);

            int oldSize = editor.getSymbols().size();

            std::vector<QSymbol> erasedSymbols = editor.localErase(startRow, startCol, endRow, endCol);

            int newSize = editor.getSymbols().size();

            decrementIndex(startRow, charsRemoved);
            deleteRow(startRow, oldSize - newSize);

            emit symbolsErased(erasedSymbols, editor.getSiteId());
        } catch (const std::exception &e) {
            qDebug() << e.what();
            undo();
        }

    }

    if (charsAdded > 0) {

        try {
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

            std::vector<QSymbol> insertedSymbols;

            while (charsAdded > 0) {
                try {
                    QChar addedChar = document()->characterAt(position++);
                    QSymbol symbol = editor.localInsert(row, col, addedChar, currentCharFormat());

                    if (addedChar == QChar::LineFeed || addedChar == QChar::ParagraphSeparator || addedChar == QChar::LineSeparator) {
                        newRows++;
                    }

                    insertedSymbols.push_back(symbol);

                    /**
                     * if it reaches the end of the line go in the next one
                     */
                    if (col == editor.getSymbols()[row].size()) {
                        row++;
                        col = 0;
                    }

                    col++;
                    charsAdded--;
                } catch (const std::exception &e) {
                    textCursor().deleteChar();
                    throw;
                }
            }

            incrementIndex(pos, n);
            insertRow(pos, newRows);

            emit symbolsInserted(insertedSymbols, editor.getSiteId());
        } catch (const std::exception &e) {
            qDebug() << e.what();
        }


    }

    /**
     * second solution maps a 1-dimensional index (position) to a 2-dimensional one
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
//            QChar addedChar = document()->characterAt(pos);
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


    printSymbols();

}

/**
 * insert n rows after pos
 * @param pos
 * @param n
 */
void TextEditor::insertRow(int pos, int n) {

    if (pos < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": pos is negative");
    } else if (n < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": n is negative");
    }

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

    if (pos < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": pos is negative");
    } else if (n < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": n is negative");
    }

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

    if (pos < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": pos is negative");
    } else if (n < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": n is negative");
    }

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

    if (pos < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": pos is negative");
    } else if (n < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": n is negative");
    }

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
 * @return the relative offset from the starting index of row
 */
int TextEditor::getCol(int row, int position) const {

    if (position < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": position is negative");
    } else if (row < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": row is negative");
    }

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
int TextEditor::getRow(int position) const {

    if (position < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": position is negative");
    }

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
void TextEditor::remoteInsert(QSymbol symbol) {

    try {

        isFromRemote = true;
        std::pair<int, int> pos = editor.remoteInsert(symbol);
        if (pos.first != -1 || pos.second != -1) {

            incrementIndex(pos.first, 1);

            if (symbol.isNewLine()) {
                insertRow(pos.first, 1);
            }

            int position = getPosition(pos.first, pos.second);

            if (position < 0 || position > document()->characterCount()) {
                throw std::runtime_error(std::string{} + __PRETTY_FUNCTION__ + ": invalid cursor position");
            }

            int oldPosition = textCursor().position();
            QTextCursor cursor(textCursor());
            cursor.setPosition(position);

            cursor.insertText(symbol.getC(), symbol.getCF());

            /**
             * this step is necessary due to the cursor changing position
             * when an operation is done when the text cursor is in the same position
             * as of the remote cursor
             */

            cursor.setPosition(oldPosition);
            setTextCursor(cursor);

        }
    } catch (const std::exception &e) {
        qDebug() << e.what();
    }

}
/**
 * erase symbol received from the server
 * @param symbol
 */
void TextEditor::remoteErase(QSymbol symbol) {

    try {
        isFromRemote = true;
        std::pair<int, int> pos = editor.remoteErase(symbol);
        if (pos.first != -1 || pos.second != -1) {

            decrementIndex(pos.first, 1);
            if (symbol.getC() == '\n') {
                deleteRow(pos.first, 1);
            }

            int position = getPosition(pos.first, pos.second);

            if (position < 0 || position > document()->characterCount()) {
                throw std::runtime_error(std::string{} + __PRETTY_FUNCTION__ + ": invalid cursor position");
            }

            QTextCursor cursor(textCursor());
            cursor.setPosition(position);
            cursor.deleteChar();
        }
    } catch (const std::exception &e) {
        qDebug() << e.what();
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

void TextEditor::remoteInsertBlock(std::vector<QSymbol> symbols) {
    // todo
}

void TextEditor::remoteEraseBlock(std::vector<QSymbol> symbols) {
    // todo
}

void TextEditor::paintEvent(QPaintEvent *e) {

    try {
        QTextEdit::paintEvent(e);
        QPainter painter(viewport());
        QTextCursor cursor(document());
        for (const std::pair<int, int> &c : cursors) {
            int position = c.second;
            int count = document()->characterCount();
            QColor color = userColors[c.first];
            painter.setPen(color);
            if (position < count) {
                cursor.setPosition(position);
                QRect cRect = cursorRect(cursor);
                painter.drawRect(cRect);

                /*QRect lRect(cRect.left(), cRect.top(), 50, 10);
                painter.fillRect(lRect, color);
                painter.drawRect(lRect);
                painter.setPen(Qt::white);
                QRect boundingRect;
                painter.drawText(lRect, 0, tr("TEST"), &boundingRect);*/

                document()->drawContents(&painter);
            } else if (position == count) {
                cursor.setPosition(position - 1);
                QRect cRect = cursorRect(cursor);
                painter.drawRect(cRect);

                document()->drawContents(&painter);
            }
        }
    } catch (const std::exception &e) {
        qDebug() << e.what();
    }


}

void TextEditor::cursorPositionChange() {
    // todo change with user id
    emit cursorPositionChanged(editor.getSiteId(), textCursor().position());
}

/**
 * update the cursor of userId at position
 * @param userId
 * @param position
 */
void TextEditor::updateCursor(int userId, int position) {
    cursors[userId] = position;
    if(!userColors[userId].isValid()) {
        userColors[userId] = QColor::fromRgb(QRandomGenerator::global()->generate()); // todo change with different colors
    }
}

void TextEditor::selectionChange() {
    int selectionEnd = textCursor().selectionEnd();
    int selectionStart = textCursor().selectionStart();
    if (selectionStart != selectionEnd) {
        currentSelectedChars = selectionEnd - selectionStart;
//        QTextCursor selection = cursorForPosition(QPoint(selectionStart, selectionStart));
//        selection.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, currentSelectedChars);
//        emit selectionChanged(editor.getSiteId(), selection);
    } else {
        currentSelectedChars = 0;
//        emit selectionChanged(editor.getSiteId(), QTextCursor());
    }
}

void TextEditor::updateSelection(int userId, QTextCursor cursor) {
    /**
     * unused
     */
    QTextCharFormat format;
    // todo change color setting
    format.setUnderlineColor(QColor::fromRgb(QRandomGenerator::global()->generate()));
    QTextEdit::ExtraSelection es;
    es.cursor = cursor;
    es.format = format;
    selections[userId] = es;
    QList<QTextEdit::ExtraSelection> extraSelections;
    for (const auto &selection : selections) {
        extraSelections.push_back(selection.second);
    }
    setExtraSelections(extraSelections);
}

void TextEditor::toggleUserColors() {
    if (isUserColorsToggled) {
        highlighter.disable();
    } else {
        highlighter.enable();
    }
    isUserColorsToggled = !isUserColorsToggled;
}

QColor TextEditor::getUserColor(int userId) const {

    if (userColors.find(userId) == userColors.end()) {
        throw std::runtime_error(std::string{} + __PRETTY_FUNCTION__ + ": color for user id not found");
    }

    return userColors.at(userId);
}

int TextEditor::getUserId(int row, int col) const {
    return editor.getSymbols()[row][col].getSiteId();
}

void TextEditor::openDocument(std::vector<std::vector<QSymbol>> symbols) {

    if (std::any_of(symbols.begin(), symbols.end(), [](const std::vector<QSymbol> &row){
        return std::any_of(row.begin(), row.end(), [](const QSymbol &s){ return !s.isValid(); });
    })) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": document is invalid");
    }

    index.clear();
    index.push_back(0);
    int pos = 0;
    for (int i = 0; i < symbols.size(); i++) {
        for (int j = 0; j < symbols[i].size(); j++, ++pos) {
            QSymbol symbol = symbols[i][j];
            isFromRemote = true;
            textCursor().insertText(symbol.getC());
            setCurrentCharFormat(symbol.getCF());
            textCursor().movePosition(QTextCursor::Right);
        }
        index.push_back(pos);
    }
    editor.setSymbols(symbols);
}

void TextEditor::printSymbols() {
    std::cout << "---" << std::endl;
    const auto& symbols = editor.getSymbols();
    for (int i = 0; i < symbols.size(); i++) {
        std::cout << "[" << index[i] << "] ";
        for (int j = 0; j < symbols[i].size(); j++) {
            const QSymbol &s = symbols[i][j];
            s.isNewLine() ? std::cout << std::endl : std::cout << s.getC().toLatin1();
        }
    }
    std::cout << std::endl;
}

// todo handle offline case

// todo handle user disconnections
