//
// Created by Antonio Santoro on 17/08/2020.
//

#ifndef COTEXT_CLIENT_USERHIGHLIGHTER_H
#define COTEXT_CLIENT_USERHIGHLIGHTER_H


#include <QSyntaxHighlighter>
#include <QTextCharFormat>



class TextEditor;

class UserHighlighter : public QSyntaxHighlighter {

    Q_OBJECT

public:
    UserHighlighter(const TextEditor &editor, QTextDocument *parent = 0);

    void enable();
    void disable();

protected:
    void highlightBlock(const QString &text) override;

private:

    QTextDocument &document;
    const TextEditor &editor;

};


#endif //COTEXT_CLIENT_USERHIGHLIGHTER_H
