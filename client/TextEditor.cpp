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
    qDebug() << "Current sID: "<< editor.getSiteId();


    setAcceptRichText(false);
    alignmentChanged(alignment());

    /**
     * each alignment option is mutual exclusive
     */
    QActionGroup *alignGroup = new QActionGroup(this);
    alignGroup->addAction(ui.actionAlign_left);
    alignGroup->addAction(ui.actionAlign_right);
    alignGroup->addAction(ui.actionAlign_center);
    alignGroup->addAction(ui.actionJustify);

    connect(alignGroup, &QActionGroup::triggered, this, &TextEditor::setTextAlignment);

    /**
    * font styling connections
    */
    
    connect(ui.actionFont, &QAction::triggered, this, &TextEditor::selectFont);
    connect(ui.actionBold, &QAction::triggered, this, &TextEditor::setFontBold);
    connect(ui.actionItalic, &QAction::triggered, this, &TextEditor::setFontItalic);
    connect(ui.actionUnderline, &QAction::triggered, this, &TextEditor::setFontUnderline);
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

void TextEditor::mergeFormat(const QTextCharFormat &format) {
    QTextCursor c = textCursor();
    int start = c.selectionStart();
    int end = c.selectionEnd();
    for (int i = start; i < end; i++) {
        c.setPosition(i);
        c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        c.mergeCharFormat(format);
        c.clearSelection();
    }
}

void TextEditor::selectFont() {
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, parent);
    if (fontSelected) {
        QTextCharFormat f;
        f.setFont(font);
        mergeFormat(f);
    }
}

void TextEditor::setFontBold(bool bold) {
    QTextCharFormat f;
    bold ? f.setFontWeight(QFont::Bold) : f.setFontWeight(QFont::Normal);
    mergeFormat(f);
}

void TextEditor::setFontItalic(bool italic) {
    QTextCharFormat f;
    f.setFontItalic(italic);
    mergeFormat(f);
}

void TextEditor::setFontUnderline(bool underline) {
    QTextCharFormat f;
    f.setFontUnderline(underline);
    mergeFormat(f);
}

void TextEditor::setFontColor() {
    QColor color = QColorDialog::getColor(currentCharFormat().foreground().color(), parent);
    if (!color.isValid())
        return;

    QTextCharFormat f;
    f.setForeground(color);
    mergeFormat(f);

    colorChanged(color);
}

void TextEditor::setTextAlignment(QAction *action) {

    if (action == ui.actionAlign_left) {
        setAlignment(Qt::AlignLeft);
        emit textAlignmentChanged(Qt::AlignLeft, textCursor().position());
    } else if (action == ui.actionAlign_right) {
        setAlignment(Qt::AlignRight);
        emit textAlignmentChanged(Qt::AlignRight, textCursor().position());
    } else if (action == ui.actionAlign_center) {
        setAlignment(Qt::AlignHCenter);
        emit textAlignmentChanged(Qt::AlignHCenter, textCursor().position());
    } else if (action == ui.actionJustify) {
        setAlignment(Qt::AlignJustify);
        emit textAlignmentChanged(Qt::AlignJustify, textCursor().position());
    }

}

void TextEditor::alignmentChanged(Qt::Alignment alignment) {
    if (alignment & Qt::AlignLeft) {
        ui.actionAlign_left->setChecked(true);
    } else if (alignment & Qt::AlignCenter) {
        ui.actionAlign_center->setChecked(true);
    } else if (alignment & Qt::AlignRight) {
        ui.actionAlign_right->setChecked(true);
    } else if (alignment & Qt::AlignJustify) {
        ui.actionJustify->setChecked(true);
    }
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
    // todo remove?
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
     * workaround for https://github.com/anphetamina/CoText/issues/32
     */
    if (charsAdded >= 1 && charsRemoved >= 1 && position+charsRemoved > index.back()+static_cast<int>(editor.getSymbols()[index.size()-1].size()-1)) {
        charsRemoved--;
        charsAdded--;
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

                    if (isNewLine(addedChar)) {
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
            if (isNewLine(symbol.getC())) {
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

    updateToolbar(currentCharFormat());
    alignmentChanged(alignment());

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

void TextEditor::updateAlignment(Qt::Alignment alignment, int position) {
    document()->findBlock(position).blockFormat().setAlignment(alignment);
}

bool TextEditor::isNewLine(QChar c) {
    return c == QChar::LineFeed || c == QChar::ParagraphSeparator || c == QChar::LineSeparator;
}

// todo handle offline case

// todo handle user disconnections