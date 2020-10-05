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
#include <thread>
#include <mutex>
#include "MainWindow.h"
#include "Benchmark.h"

std::mutex ins_mutex;  // protects insert


TextEditor::TextEditor(int siteId, Ui::MainWindow &ui, QWidget *parent) :
    parent(parent),
    ui(ui),
    index({0}),
    editor(SharedEditor(siteId)),
    isFromRemote(false),
    isFromRemoteCursor(false),
    testSymbols({{}}),
    cursorMap({}),
    currentSelectedChars(0),
    highlighter(*this, document()),
    isUserColorsToggled(false) {

    qDebug() << "Current sID: "<< editor.getSiteId();

    /**
     * document default styling
     */
    setAcceptRichText(false);
    alignmentChanged(alignment());

    setStyleSheet("QTextEdit {margin-left: 40px; margin-right: 40px; margin-top: 10px; margin-bottom: 10px; color: white; font-size: 16px; border: hidden}");

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

    connect(this, &QTextEdit::currentCharFormatChanged, this, &TextEditor::currentCharFormatChanged);

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
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, parent);
    if (fontSelected)
        setFont(font);
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
    if (!color.isValid())
        return;
    setTextColor(color);
}

void TextEditor::setTextAlignment(QAction *action) {

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
            block.position() != -1 ? pos = block.position() : pos = textCursor().selectionEnd();
            emit textAlignmentChanged(flag, pos, editor.getSiteId());
            block = block.next();
        }
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

void TextEditor::currentCharFormatChanged(const QTextCharFormat &f) {
    ui.actionBold->setChecked(f.font().bold());
    ui.actionItalic->setChecked(f.font().italic());
    ui.actionUnderline->setChecked(f.font().underline());
}

void TextEditor::contentsChange(int position, int charsRemoved, int charsAdded) {

    if (isFromRemote) {
        isFromRemote = false;
        return;
    }

    /**
     * https://github.com/anphetamina/CoText/issues/32 workaround
     */

    if (document()->characterAt(position+charsAdded) == '\0') {
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
                    QChar addedChar = document()->characterAt(position);
                    QTextCursor qTextCursor = textCursor();
                    qTextCursor.setPosition(position);
                    QSymbol symbol = editor.localInsert(row, col, addedChar, qTextCursor.charFormat());

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
        } catch (const std::exception &e) {
            qDebug() << e.what();
        }

    }

    //printSymbols();
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

    // qDebug() << "received add " << symbol.getC();

    try {
        isFromRemote = true;
        isFromRemoteCursor = true;
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

    // qDebug() << "received del " << symbol.getC();

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

    int last_position = 0;
    QVector<QString> blocks;
    QString buffer_block;
    QTextCharFormat last_cf = QTextCharFormat();
    for (int j = 0; j < symbols.size(); j++) {
        QSymbol symbol = symbols[j];
        //qDebug() << "received add " << symbol.getC();
        try {
            isFromRemote = true;
            isFromRemoteCursor = true;
            std::pair<int, int> pos = editor.remoteInsert(symbol);

            if (pos.first != -1 || pos.second != -1) {

                incrementIndex(pos.first, 1);

                if (symbol.isNewLine()) {
                    insertRow(pos.first, 1);
                }

                // If a new format is found, insert the biffered content
                if(j!=0 && (last_cf != symbol.getCF()) ){
                    QTextCursor cursor(textCursor());
                    cursor.insertText(buffer_block, last_cf);
                    //cursor.setPosition(last_position);
                    //setTextCursor(cursor);
                    buffer_block = "";
                }
                last_cf = symbol.getCF();
                //last_position = getPosition(pos.first, pos.second);
                buffer_block.push_back(symbol.getC()) ;
            }
        } catch (const std::exception &e) {
            qDebug() << e.what();
        }
    }
    // Check if the last buffer_block (last_cf didnt changed almost for sure in the last char)
    if(!buffer_block.isEmpty()){
        QTextCursor cursor(textCursor());
        cursor.insertText(buffer_block, last_cf);
        //cursor.setPosition(last_position);
        //setTextCursor(cursor);
    }
}

void TextEditor::remoteEraseBlock(std::vector<QSymbol> symbols) {
    std::for_each(symbols.begin(), symbols.end(), [this](const QSymbol &it){ remoteErase(it); });
}

void TextEditor::paintEvent(QPaintEvent *e) {

    try {
        QTextEdit::paintEvent(e);
        QPainter painter(viewport());
        QTextCursor cursor(document());
        for (const std::pair<int, int> &c : cursorMap) {
            int position = c.second;
            int count = document()->characterCount();
            QColor color = getUserColor(c.first);
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

                update();
            } else if (position == count) {
                cursor.setPosition(position - 1);
                QRect cRect = cursorRect(cursor);
                painter.drawRect(cRect);

                update();
            }
        }
    } catch (const std::exception &e) {
        qDebug() << e.what();
    }
}

void TextEditor::cursorPositionChange() {

    alignmentChanged(alignment());

    /**
     * this works because in the constructor of TextEditor
     * parent contains a subobject of type MainWindow that is derived from QWidget
     */
    if (MainWindow *mw = dynamic_cast<MainWindow*>(parent)) {

        int userId = mw->getUser().getId();

        if (!isFromRemoteCursor) {
            emit cursorPositionChanged(userId, textCursor().position());
        }

        isFromRemoteCursor = false;

    }
}

/**
 * update the cursor of userId at position
 * @param userId
 * @param position
 */
void TextEditor::updateCursor(int userId, int position) {

    if (cursorMap.find(userId) != cursorMap.end()) {

        cursorMap[userId] = position;

    } else {

        qDebug() << userId << "not found";

    }
}

void TextEditor::selectionChange() {
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

    qDebug() << "invalid color for" << userId;
    return QColor::Invalid;
}

int TextEditor::getUserId(int row, int col) const {
    return editor.getSymbols()[row][col].getSiteId();
}

void TextEditor::openDocument(int docId, QString docName, std::vector<std::vector<QSymbol>> symbols) {

    if (std::any_of(symbols.begin(), symbols.end(), [](const std::vector<QSymbol> &row){
        return std::any_of(row.begin(), row.end(), [](const QSymbol &s){ return !s.isValid(); });
    })) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": document is invalid");
    }

    emit(setMainWindowTitle(docName));
    this->clear();
    this->setDisabled(false);

    Benchmark b = Benchmark("TextEditor::openDocument");
    b.startTimer();
    index.clear();
    index.push_back(0);
    int pos = 0;
    // Prepared code for remoteInsertBlock optimization. As of now its the same as below  code

    for (int i = 0; i < symbols.size(); i++) {
        this->remoteInsertBlock(symbols[i]);
    }
    /*
    parallel_for(symbols.size(), [&](int start, int end){
        for(int i = start; i < end; ++i) {
            this->remoteInsertBlock(symbols[i]);
        }
    } );
    */
    b.stopTimer();

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
    isFromRemote = true;
    isFromRemoteCursor = true;

    QTextBlockFormat f;
    f.setAlignment(alignment);

    QTextCursor c(textCursor());
    c.setPosition(position);
    c.setBlockFormat(f);

}

bool TextEditor::isNewLine(QChar c) {
    return c == QChar::LineFeed || c == QChar::ParagraphSeparator || c == QChar::LineSeparator;
}

void TextEditor::updateCursorMap(QVector<User> onlineUserList) {

    std::vector<int> onlineUserIds{};
    std::for_each(onlineUserList.begin(), onlineUserList.end(), [&](const User &u) { onlineUserIds.push_back(u.getId()); });
    std::sort(onlineUserIds.begin(), onlineUserIds.end());

    std::vector<int> currentUserList{};
    std::for_each(cursorMap.begin(), cursorMap.end(), [&](const std::pair<int, int> &u) { currentUserList.push_back(u.first); });

    std::vector<int> offlineUserIds{};
    std::set_difference(currentUserList.begin(), currentUserList.end(), onlineUserIds.begin(), onlineUserIds.end(), std::inserter(offlineUserIds, offlineUserIds.begin()), [](const int &id1, const int &id2){
        return id1 < id2;
    });

    std::vector<int> newOnlineUserIds{};
    std::set_difference(onlineUserIds.begin(), onlineUserIds.end(), currentUserList.begin(), currentUserList.end(), std::inserter(newOnlineUserIds, newOnlineUserIds.begin()), [](const int &id1, const int &id2){
        return id1 < id2;
    });

    for (auto it : offlineUserIds) {
        cursorMap.erase(it);
    }

    for (auto it : newOnlineUserIds) {

        /**
         * it will not print the new cursor instead wait for the updated valid position
         */
        cursorMap[it] = document()->characterCount()+1;
    }

    /**
     * step necessary to avoid printing multiple cursor for the local user
     */
    cursorMap.erase(user.getId());

}

// todo handle offline case

// todo handle user disconnections
