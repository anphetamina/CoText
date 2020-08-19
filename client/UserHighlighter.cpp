//
// Created by Antonio Santoro on 17/08/2020.
//

#include <QDebug>
#include "UserHighlighter.h"
#include "TextEditor.h"


UserHighlighter::UserHighlighter(const TextEditor &editor, QTextDocument *parent) : QSyntaxHighlighter(parent), document(*parent), editor(editor) {
    setDocument(nullptr);
}

void UserHighlighter::highlightBlock(const QString &text) {

    try {
        for (int i = currentBlock().position(), j = 0; i < currentBlock().position() + text.length(); i++, j++) {
            QTextCharFormat format;
            format.setFontWeight(QFont::Bold);
            const int &row = editor.getRow(i);
            const int &col = editor.getCol(row, i);
            const int &userId = editor.getUserId(row, col);
            const QColor &color = editor.getUserColor(userId);
            format.setForeground(color);
            setFormat(j, 1, format);
        }
    } catch (const std::exception &e) {
        qDebug() << e.what();
    }
}

void UserHighlighter::enable() {
    setDocument(&document);
}

void UserHighlighter::disable() {
    setDocument(nullptr);
}
