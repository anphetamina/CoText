//
// Created by Antonio Santoro on 17/08/2020.
//

#include <QDebug>
#include "UserHighlighter.h"

UserHighlighter::UserHighlighter(std::map<int, QColor> &colors, QTextDocument *parent) : QSyntaxHighlighter(parent), document(*parent), positions({}), colors(colors) {
    setDocument(nullptr);
}

void UserHighlighter::highlightBlock(const QString &text) {

    for (int i = currentBlock().position(), j = 0; i < currentBlock().position() + currentBlock().length(); i++, j++) {
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        format.setForeground(colors[positions[i]]);
        setFormat(j, 1, format);
    }
}

void UserHighlighter::updatePosition(int position, int userId) {
    positions[position] = userId;
}

void UserHighlighter::removePosition(int position) {
    positions.erase(position);
}

void UserHighlighter::enable() {
    setDocument(&document);
}

void UserHighlighter::disable() {
    setDocument(nullptr);
}
