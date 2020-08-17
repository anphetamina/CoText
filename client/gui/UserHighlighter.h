//
// Created by Antonio Santoro on 17/08/2020.
//

#ifndef COTEXT_CLIENT_USERHIGHLIGHTER_H
#define COTEXT_CLIENT_USERHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>


class UserHighlighter : public QSyntaxHighlighter {

    Q_OBJECT

public:
    UserHighlighter(const std::map<int, QColor> &colors, QTextDocument *parent = 0);

    void updatePosition(int position, int userId);
    void removePosition(int position);

    void enable();
    void disable();

protected:
    void highlightBlock(const QString &text) override;

private:

    QTextDocument &document;
    std::map<int, int> positions;
    const std::map<int, QColor> &colors;

};


#endif //COTEXT_CLIENT_USERHIGHLIGHTER_H
