//
// Created by Antonio Santoro on 06/08/2020.
//

#include <ui_mainwindow.h>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QFontDialog>
#include <QThread>
#include <QRandomGenerator>
#include <QScrollBar>
#include "../common/Shuffler.h"
#include "TextEditor.h"
#include <mutex>
#include "Benchmark.h"
#include "MainWindow.h"
#include "../common/PacketDef.h"


TextEditor::TextEditor(int siteId, Ui::MainWindow &ui, QWidget *parent) :
    parent(parent),
    ui(ui),
    index({0}),
    editor(SharedEditor(siteId)),
    isFromRemote(false),
    testSymbols({{}}),
    cursorMap({}),
    currentSelectedChars(0),
    highlighter(*this, document()),
    isUserColorsToggled(false),
    hasLostFocus(false),
    copiedFromOutside(true),
    draggedFromOutside(true),
    mousePressed(false),
    documentId(0),
    documentName(QString()) {

    setAcceptRichText(false);

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

    connect(ui.actionBold, &QAction::triggered, this, &TextEditor::setFontBold);
    connect(ui.actionItalic, &QAction::triggered, this, &TextEditor::setFontItalic);
    connect(ui.actionUnderline, &QAction::triggered, this, &TextEditor::setFontUnderline);
    connect(ui.actionTextColor, &QAction::triggered, this, &TextEditor::setFontColor);

    connect(this, &QTextEdit::currentCharFormatChanged, this, &TextEditor::currentCharFormatChange);

    if (MainWindow *mw = dynamic_cast<MainWindow*>(parent)) {
        fontComboBox = mw->fontComboBox; // hmmm
        sizeComboBox = mw->sizeComboBox;

        connect(fontComboBox, &QFontComboBox::currentFontChanged, this, &TextEditor::setFontFamily);
        connect(sizeComboBox, &QFontComboBox::textActivated, this, &TextEditor::setFontSize);
    }

    /**
     * document content connections
     */

    connect(document(), &QTextDocument::contentsChange, this, &TextEditor::contentsChange);

    connect(this, &QTextEdit::cursorPositionChanged, this, &TextEditor::cursorPositionChange);
    connect(this, &QTextEdit::selectionChanged, this, &TextEditor::selectionChange);

    connect(ui.actionToggle_user_colors, &QAction::triggered, this, &TextEditor::toggleUserColors);

    connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &TextEditor::clipboardDataChange);

    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &TextEditor::paintCursors);
    connect(this, &QTextEdit::textChanged, this, &TextEditor::paintCursors);

    /**
     * action connections
     */

    connect(this, &QTextEdit::undoAvailable, ui.actionUndo, &QAction::setEnabled);
    connect(this, &QTextEdit::redoAvailable, ui.actionRedo, &QAction::setEnabled);
    ui.actionCopy->setEnabled(false);
    connect(this, &QTextEdit::copyAvailable, ui.actionCopy, &QAction::setEnabled);
    ui.actionCut->setEnabled(false);
    connect(this, &QTextEdit::copyAvailable, ui.actionCut, &QAction::setEnabled);

    ui.actionUndo->setEnabled(document()->isUndoAvailable());
    ui.actionRedo->setEnabled(document()->isUndoAvailable());

    if (const QMimeData *md = QApplication::clipboard()->mimeData()) {
        ui.actionPaste->setEnabled(md->hasText());
    }

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

void TextEditor::setFontFamily(const QFont &font) {
    QTextCharFormat f;
    f.setFontFamily(font.family());
    mergeCurrentCharFormat(f);
}

void TextEditor::setFontSize(const QString &text) {
    qreal pointSize = text.toFloat();
    if (text.toFloat() > 0) {
        QTextCharFormat f;
        f.setFontPointSize(pointSize);
        mergeCurrentCharFormat(f);
    }
}

void TextEditor::setFontBold(bool bold) {
    bold ? setFontWeight(QFont::Bold) : setFontWeight(QFont::Normal);
}

void TextEditor::setFontItalic(bool italic) {
    QTextEdit::setFontItalic(italic);
}

void TextEditor::setFontUnderline(bool underline) {
    QTextEdit::setFontUnderline(underline);
}

void TextEditor::setFontColor() {
    QColor color = QColorDialog::getColor(textColor(), parent);
    if (!color.isValid()) {
        return;
    }
    setTextColor(color);
}

void TextEditor::setTextAlignment(QAction *action) {

    /**
     * this prevent from erasing the entire block
     */
    document()->blockSignals(true);

    Qt::Alignment flag;
    bool hasChanged = true;

    if (action == ui.actionAlign_left) {
        setAlignment(Qt::AlignLeft);
        flag = Qt::AlignLeft;
    } else if (action == ui.actionAlign_right) {
        setAlignment(Qt::AlignRight);
        flag = Qt::AlignRight;
    } else if (action == ui.actionAlign_center) {
        setAlignment(Qt::AlignHCenter);
        flag = Qt::AlignHCenter;
    } else if (action == ui.actionJustify) {
        setAlignment(Qt::AlignJustify);
        flag = Qt::AlignJustify;
    } else {
        hasChanged = false;
    }

    if (hasChanged) {
        QTextBlock block = document()->findBlock(textCursor().selectionStart());
        int pos = 0;
        while (block.isValid() && block.position() <= textCursor().selectionEnd()) {
            block.position() != -1 ? pos = block.position()-1 : pos = document()->findBlock(textCursor().selectionEnd()).position()-1;

            try {

                if (pos < 0) {
                    qDebug() << "first block encountered ( pos = " << pos << ")";
                    QSymbol symbol(QChar(), FIRST_ROW, {}, QTextCharFormat());
                    emit textAlignmentChanged(flag, symbol, editor.getSiteId());
                } else {
                    int row = getRow(pos);
                    int col = getCol(row, pos);
                    QSymbol symbol = editor.getSymbol(row, col);
                    emit textAlignmentChanged(flag, symbol, editor.getSiteId());
                }


            } catch (const std::exception &e) {
                qDebug() << "[EXCEPTION]"  << __PRETTY_FUNCTION__ << e.what();
            }


            block = block.next();
        }
    }

    document()->blockSignals(false);

}

void TextEditor::alignmentChange(Qt::Alignment alignment) {
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

void TextEditor::currentCharFormatChange(const QTextCharFormat &f) {

    QTextCharFormat format{f};

    /**
     * the format toolbar will be updated with the current selected text format
     */
    if (textCursor().hasSelection()) {
        QTextCursor cursor(document());
        cursor.setPosition(textCursor().selectionEnd());
        format = cursor.charFormat();
    }


    if (fontComboBox) {
        fontComboBox->blockSignals(true);
        fontComboBox->setCurrentIndex(fontComboBox->findText(QFontInfo(format.font()).family()));
        fontComboBox->blockSignals(false);
    }

    if (sizeComboBox) {
        sizeComboBox->blockSignals(true);
        sizeComboBox->setCurrentText(QString::number(format.font().pointSize()));
        sizeComboBox->blockSignals(false);
    }

    ui.actionBold->setChecked(format.font().bold());
    ui.actionItalic->setChecked(format.font().italic());
    ui.actionUnderline->setChecked(format.font().underline());
}

void TextEditor::contentsChange(int position, int charsRemoved, int charsAdded) {

    qDebug() << "contentsChange";

    if (document()->signalsBlocked()) {
        document()->blockSignals(false);
        return;
    }

    /**
     * https://github.com/anphetamina/CoText/issues/32 workaround
     */

    while (document()->characterAt(position+charsAdded) == '\0' && charsRemoved > 0) {
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
            qDebug() << "[EXCEPTION]" << "TextEditor::contentsChange charsRemoved" << e.what();
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
            std::vector<QSymbol> insertedLineFeeds;

            while (charsAdded > 0) {
                try {
                    QChar addedChar = document()->characterAt(position);

                    QTextCursor c(document());
                    c.setPosition(position + 1);

                    QSymbol symbol = editor.localInsert(row, col, addedChar, c.charFormat());

                    int aNewLine = isNewLine(addedChar);
                    if (aNewLine != -1) {
                        newRows++;
                        if (aNewLine == 0) {
                            insertedLineFeeds.push_back(symbol);
                        }
                    }

                    insertedSymbols.push_back(symbol);

                    /**
                     * if it reaches the end of the line go in the next one
                     */
                    if (col == editor.getSymbols()[row].size()) {
                        row++;
                        col = 0;
                    }

                    position++;
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

            for (const QSymbol& sym : insertedLineFeeds) {
                emit textAlignmentChanged(document()->findBlock(position-1).blockFormat().alignment(), sym, editor.getSiteId());
            }
        } catch (const std::exception &e) {
            qDebug() << "[EXCEPTION]"  << "TextEditor::contentsChange charsAdded" << e.what();
        }

    }

    printSymbols(__PRETTY_FUNCTION__);
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

    index.erase(index.begin() + pos + 1, index.begin() + pos + n + 1);

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
        return static_cast<int>(index.size())-1;
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


    /**
     * block updates
     */
    document()->blockSignals(true);

    textCursor().clearSelection();

    try {

        std::pair<int, int> pos = editor.remoteInsert(symbol);
        if (pos.first != -1 || pos.second != -1) {

            incrementIndex(pos.first, 1);

            if (symbol.isNewLine()) {
                insertRow(pos.first, 1);
            }

            int position = getPosition(pos.first, pos.second);

            if (position < 0 || position > document()->characterCount()) {
                throw std::runtime_error(": invalid cursor position");
            }

            QTextCursor cursor(document());
            cursor.setPosition(position);

            cursor.insertText(symbol.getC(), symbol.getCF());

            cursorPositionChange();
        }
    } catch (const std::exception &e) {
        qDebug() << "[EXCEPTION]"  << __PRETTY_FUNCTION__ << e.what();
    }

    printSymbols(__PRETTY_FUNCTION__);

    document()->blockSignals(false);

}

/**
 * erase symbol received from the server
 * @param symbol
 */
void TextEditor::remoteErase(QSymbol symbol) {

    document()->blockSignals(true);
    textCursor().clearSelection();

    try {

        std::pair<int, int> pos = editor.remoteErase(symbol);
        if (pos.first != -1 || pos.second != -1) {

            decrementIndex(pos.first, 1);

            if (symbol.isNewLine()) {
                deleteRow(pos.first, 1);
            }

            int position = getPosition(pos.first, pos.second);

            if (position < 0 || position > document()->characterCount()) {
                throw std::runtime_error(": invalid cursor position");
            }

            QTextCursor cursor(document());
            cursor.setPosition(position);
            cursor.deleteChar();

            cursorPositionChange();
        }
    } catch (const std::exception &e) {
        qDebug() << "[EXCEPTION]"  << __PRETTY_FUNCTION__ << e.what();
    }

    printSymbols(__PRETTY_FUNCTION__);

    document()->blockSignals(false);

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

    if (symbols.empty()) {
        return;
    }

    document()->blockSignals(true);

    textCursor().clearSelection();

    try {

        std::pair<int, int> firstPos = std::make_pair(-1, -1);
        std::pair<int, int> lastPos = std::make_pair(-1, -1);
        int lineCount = 0;

        for (int j = 0; j < symbols.size(); j++) {
            QSymbol symbol = symbols[j];

            std::pair<int, int> pos = editor.remoteInsert(symbol);

            if (pos.first != -1 || pos.second != -1) {

                if (symbol.isNewLine()) {
                    lineCount++;
                }

                if (firstPos.first == -1 || firstPos.second == -1) {
                    firstPos = pos;
                }
                lastPos = pos;

            }
        }

        int firstLine = firstPos.first;
        int firstIndex = firstPos.second;
        int lastLine = lastPos.first;
        int lastIndex = lastPos.second;

        if (firstLine == -1 || firstIndex == -1 || lastLine == -1 || lastIndex == -1) {
            document()->blockSignals(false);
            return;
        }


        std::vector<QSymbol> block = editor.getBlock(firstLine, firstIndex, lastLine, lastIndex);
        incrementIndex(firstLine, block.size());
        insertRow(firstLine, lineCount);

        int lastPosition = getPosition(firstLine, firstIndex);

        int diffSize = static_cast<int>(block.size() - symbols.size());
        QTextCursor cursor(document());
        cursor.setPosition(lastPosition);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, diffSize);
        cursor.removeSelectedText();

        //std::cout << __PRETTY_FUNCTION__ << document()->toPlainText().toStdString() << std::endl;

        insertBlock(block, lastPosition);

    } catch (const std::exception &e) {
        qDebug() << "[EXCEPTION]"  << "TextEditor::remoteInsertBlock" << __PRETTY_FUNCTION__ << e.what();
    }

    document()->blockSignals(false);

}


void TextEditor::remoteEraseBlock(std::vector<QSymbol> symbols) {

    if (symbols.empty()) {
        return;
    }

    document()->blockSignals(true);

    try {
        std::pair<int, int> firstPos = std::make_pair(-1, -1);
        std::pair<int, int> lastPos = std::make_pair(-1, -1);

        for (int i = 0; i < symbols.size(); i++) {
            std::pair<int, int> pos = editor.getPos(symbols[i]);

            if (pos.first != -1 || pos.second != -1) {

                if (firstPos.first == -1 || firstPos.second == -1) {
                    firstPos = pos;
                }

                lastPos = pos;
            }

        }

        int firstLine = firstPos.first;
        int firstIndex = firstPos.second;
        int lastLine = lastPos.first;
        int lastIndex = lastPos.second;

        if (firstLine == -1 || firstIndex == -1 || lastLine == -1 || lastIndex == -1) {
            document()->blockSignals(false);
            return;
        }

        std::vector<QSymbol> oldBlock = editor.getBlock(firstLine, firstIndex, lastLine, lastIndex);

        std::vector<QSymbol> erasedBlock{};

        int lineCount = 0;
        for (int i = 0; i < symbols.size(); i++) {
            std::pair<int, int> pos = editor.remoteErase(symbols[i]);

            if (pos.first != -1 || pos.second != -1) {
                if (symbols[i].isNewLine()) {
                    lineCount++;
                }
                erasedBlock.push_back(symbols[i]);
            }
        }

        std::vector<QSymbol> diffBlock{};
        std::set_difference(oldBlock.begin(), oldBlock.end(), erasedBlock.begin(), erasedBlock.end(), std::inserter(diffBlock, diffBlock.begin()));

        decrementIndex(firstLine, diffBlock.size());
        deleteRow(firstLine, lineCount);

        QTextCursor cursor(document());
        int position = getPosition(firstLine, firstIndex);
        cursor.setPosition(position);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, oldBlock.size());
        cursor.removeSelectedText();

        //std::cout << __PRETTY_FUNCTION__ << document()->toPlainText().toStdString() << std::endl;

        insertBlock(diffBlock, position);
    } catch (const std::exception &e) {
        qDebug() << "[EXCEPTION]"  << "TextEditor::remoteEraseBlock" << __PRETTY_FUNCTION__ << e.what();
    }

    document()->blockSignals(false);

}

void TextEditor::insertBlock(const std::vector<QSymbol> &block, int position) {
    QTextCursor cursor(document());
    cursor.setPosition(position);
    QTextCharFormat lastCF = QTextCharFormat();
    QString bufferString;

    for (int i = 0; i < block.size(); i++) {
        if (i != 0 && block[i].getCF() != lastCF) {

            cursor.setPosition(position);
            cursor.insertText(bufferString, lastCF);

            position += bufferString.size();
            bufferString.clear();
        }

        bufferString.append(block[i].getC());
        lastCF = block[i].getCF();
    }

    if (!bufferString.isEmpty()) {
        cursor.setPosition(position);
        cursor.insertText(bufferString, lastCF);
    }

    cursorPositionChange();

    printSymbols(__PRETTY_FUNCTION__);
}

void TextEditor::cursorPositionChange() {

    alignmentChange(alignment());

    int userId = MainWindow::getUser().getId();

    emit cursorPositionChanged(userId, textCursor().position());

}

/**
 * update the cursor of userId at position
 * @param userId
 * @param position
 */
void TextEditor::updateCursor(int userId, int position) {

    if (cursorMap.find(userId) != cursorMap.end()) {

        cursorMap[userId].first = position;

    } else {

        qDebug() << userId << "not found";

    }
}

void TextEditor::selectionChange() {

    /**
     * this condition is true when the cursor was selecting a text and now no more
     * causing the update of the current format change
     */
    if (currentSelectedChars != 0 && !textCursor().hasSelection()) {
        currentCharFormatChange(currentCharFormat());
    }

    int selectionEnd = textCursor().selectionEnd();
    int selectionStart = textCursor().selectionStart();

    if (selectionStart != selectionEnd) {
        currentSelectedChars = selectionEnd - selectionStart;
    } else {
        currentSelectedChars = 0;
    }
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

    if (MainWindow *mw = dynamic_cast<MainWindow*>(parent)) {

        return mw->getUserColor(userId);

    }

    qDebug() << "color for" << userId << "not found";
    return QColorConstants::White;
}

int TextEditor::getUserId(int row, int col) const {
    return editor.getSymbols()[row][col].getSiteId();
}

void TextEditor::openDocument(int docId, QString docName, std::vector<std::vector<QSymbol>> symbols) {

    document()->blockSignals(true);

    try {
        if (std::any_of(symbols.begin(), symbols.end(), [](const std::vector<QSymbol> &row){
            return std::any_of(row.begin(), row.end(), [](const QSymbol &s){ return !s.isValid(); });
        })) {
            throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": document is invalid");
        }

        this->setSiteId(user.getId());
        qDebug() << "[DOC_OPENED] : updating siteId "<< this->getSiteId();
        documentId = docId;
        documentName = docName;

        if (document()->characterCount() > 1 || !editor.getSymbols()[0].empty()) {
            editor.clear();

            this->clear();

        }

        this->setDisabled(false);

        Benchmark b = Benchmark("TextEditor::openDocument");
        b.startTimer();

        index.clear();
        index.push_back(0);

        for (int i = 0; i < symbols.size(); i++) {
            this->remoteInsertBlock(symbols[i]);
        }

        b.stopTimer();

        alignmentChange(alignment());
        currentCharFormatChange(currentCharFormat());
        setFocus();
    } catch (const std::exception &e) {
        qDebug() << "[EXCEPTION]" << __PRETTY_FUNCTION__ << e.what();
    }

    document()->blockSignals(false);

}

SharedEditor TextEditor::getEditor() const {
    return editor;
}

std::vector<int> TextEditor::getIndex() const {
    return index;
}


void TextEditor::printSymbols(const std::string &functionName) {
    QString fn = QString::fromStdString(functionName);
    qDebug()  << "--- " << fn << " ---" ;
    const auto& symbols = editor.getSymbols();
    for (int i = 0; i < symbols.size(); i++) {
        QString sline = QString();
        for (int j = 0; j < symbols[i].size(); j++) {
            const QSymbol &s = symbols[i][j];
            sline.append(isNewLine(s.getC()) == -1 ? s.getC() : QChar::LineFeed);
        }
        qDebug() << "[" << index[i] << "] " << sline;
    }
    qDebug();
}

void TextEditor::updateAlignment(Qt::Alignment align, QSymbol symbol) {
    document()->blockSignals(true);

    QTextBlockFormat f;
    f.setAlignment(align);

    try {
        QTextCursor c(document());
        int position = 0;

        /**
         * the position for the alignment update should be computed only
         * if the symbol id is not equal to the special symbol one
         * used to manage the first block of the document
         */
        if (symbol.getId() != FIRST_ROW) {
            std::pair<int, int> pos = editor.getPos(symbol);
            position = getPosition(pos.first, pos.second)+1;

            if (pos.first == -1 || pos.second == -1) {
                throw std::runtime_error("symbol not found");
            } else if (position > document()->characterCount()) {
                throw std::out_of_range(std::to_string(position) + " greater than character count " + std::to_string(document()->characterCount()));
            }
        }


        c.setPosition(position);
        c.setBlockFormat(f);

        alignmentChange(alignment());
    } catch (const std::exception &e) {
        qDebug() << "[EXCEPTION]"  << "TextEditor::updateAlignment" << __PRETTY_FUNCTION__ << e.what();
    }

    document()->blockSignals(false);
}

/**
 *
 * @param c
 * @return 1 if c == QChar::LineFeed, 0 if c == QChar::ParagraphSeparator || c == QChar::LineSeparator, else -1
 */
int TextEditor::isNewLine(QChar c) {
    int res = -1;
    if (c == QChar::LineFeed) {
        res = 1;
    } else if (c == QChar::ParagraphSeparator || c == QChar::LineSeparator) {
        res = 0;
    }
    return res;
}

void TextEditor::updateCursorMap(QVector<User> onlineUserList, QVector<User> completeUserList /*ignored*/) {

    std::vector<int> onlineUserIds{};
    std::for_each(onlineUserList.begin(), onlineUserList.end(), [&](const User &u) { onlineUserIds.push_back(u.getId()); });
    std::sort(onlineUserIds.begin(), onlineUserIds.end());

    std::vector<int> currentUserList{};
    std::for_each(cursorMap.begin(), cursorMap.end(), [&](const std::pair<int, std::pair<int, QLabel*>> &u) { currentUserList.push_back(u.first); });

    std::vector<int> offlineUserIds{};
    std::set_difference(currentUserList.begin(), currentUserList.end(), onlineUserIds.begin(), onlineUserIds.end(), std::inserter(offlineUserIds, offlineUserIds.begin()), [](const int &id1, const int &id2){
        return id1 < id2;
    });

    std::vector<int> newOnlineUserIds{};
    std::set_difference(onlineUserIds.begin(), onlineUserIds.end(), currentUserList.begin(), currentUserList.end(), std::inserter(newOnlineUserIds, newOnlineUserIds.begin()), [](const int &id1, const int &id2){
        return id1 < id2;
    });

    for (auto it : offlineUserIds) {
        cursorMap[it].second->clear();
        cursorMap[it].second->setEnabled(false);
        cursorMap[it].second->close();

        cursorMap.erase(it);
    }

    for (auto it : newOnlineUserIds) {
        cursorMap[it].first = document()->characterCount()-1;
        cursorMap[it].second = new QLabel(this);
        cursorMap[it].second->setTextInteractionFlags(Qt::NoTextInteraction);
    }

    /**
     * step necessary to avoid printing multiple cursor for the local user
     */
    cursorMap.erase(user.getId());

    paintCursors();
}

void TextEditor::paintCursors() {
    QTextCursor cursor(document());
    for (const std::pair<int, std::pair<int, QLabel*>> &c : cursorMap) {
        int position = std::min(c.second.first, document()->characterCount()-1);
        cursor.setPosition(position);
        QRect cRect = cursorRect(cursor);
        //QPoint centerY = cRect.center();

        int width = cRect.width() + 1;
        int height = cRect.height();
        //int height = QFontMetrics(cursor.charFormat().font()).height();
        //cRect.setHeight(height);
        //cRect.moveCenter(centerY);

        QPixmap pixmap(width, height);
        pixmap.fill(getUserColor(c.first));

        c.second.second->move(cRect.left(), cRect.top());
        c.second.second->resize(width, height);
        c.second.second->setPixmap(pixmap);
        c.second.second->show();
    }
}

void TextEditor::setSiteId(int siteId){
    this->editor.setSiteId(siteId);
}

int TextEditor::getSiteId(){
    return this->editor.getSiteId();
}

int TextEditor::getDocId() const{
    return documentId;
}

QString TextEditor::getDocName() const {
	return documentName;
}

int TextEditor::getNumChars() const {
	return document()->characterCount();
}

void TextEditor::clipboardDataChange() {

    if (const QMimeData *md = QApplication::clipboard()->mimeData()) {

        ui.actionPaste->setEnabled(md->hasText());

        if (hasLostFocus) {
            qDebug() << "clipboard changed outside";
            copiedFromOutside = true;
        } else {
            qDebug() << "clipboard changed inside";
            copiedFromOutside = false;
        }
    }
}

void TextEditor::focusInEvent(QFocusEvent *e) {
    hasLostFocus = e->lostFocus();
    QTextEdit::focusInEvent(e);
}

void TextEditor::focusOutEvent(QFocusEvent *e) {
    hasLostFocus = e->lostFocus();
    mousePressed = false;
    QTextEdit::focusOutEvent(e);
}

void TextEditor::dragEnterEvent(QDragEnterEvent *e) {
    draggedFromOutside = !mousePressed;
    qDebug() << "dragged from" << (draggedFromOutside ? "outside" : "inside");
    QTextEdit::dragEnterEvent(e);
}

void TextEditor::mousePressEvent(QMouseEvent *e) {
    mousePressed = true;
    QTextEdit::mousePressEvent(e);
}

void TextEditor::mouseReleaseEvent(QMouseEvent *e) {
    mousePressed = false;
    QTextEdit::mouseReleaseEvent(e);
}

void TextEditor::insertFromMimeData(const QMimeData *source) {
    if (copiedFromOutside || draggedFromOutside) {
        qDebug() << "paste format disabled";
        setAcceptRichText(false);
    } else {
        qDebug() << "paste format enabled";
        setAcceptRichText(true);
    }
    QTextEdit::insertFromMimeData(source);
}

QString TextEditor::getText() const{
    QString text;
    const auto& symbols = editor.getSymbols();
    for (int i = 0; i < symbols.size(); i++) {
        for (int j = 0; j < symbols[i].size(); j++) {
            const QSymbol &s = symbols[i][j];
            if(s.isNewLine()){
                text = text + "\r\n";
            }else{
                text = text + s.getC().toLatin1();
            }
        }
    }
    return text;
}


