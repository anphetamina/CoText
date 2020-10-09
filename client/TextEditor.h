//
// Created by Antonio Santoro on 06/08/2020.
//

#ifndef COTEXT_CLIENT_TEXTEDITOR_H
#define COTEXT_CLIENT_TEXTEDITOR_H


#include <QDebug>
#include <iostream>
#include <QtGui/QPainter>
#include <QtWidgets/QTextEdit>
#include "../common/SharedEditor.h"
#include "../common/QSymbol.h"
#include "UserHighlighter.h"
#include "../common/User.h"

class UserHighlighter;

namespace Ui { class MainWindow; }

class TextEditor : public QTextEdit {

    Q_OBJECT

public:
    explicit TextEditor(int siteId, Ui::MainWindow &ui, QWidget *parent = nullptr);

	void setSiteId(int siteId);

	int getRow(int position) const;
	int getCol(int row, int position) const;
	
	int getSiteId();
	int getDocId() const;
	QString getDocName() const;
	int getNumChars() const;
	
	int getUserId(int row, int col) const;
	QColor getUserColor(int userId) const;

private:
    QWidget *parent;
    Ui::MainWindow &ui;
    SharedEditor editor;
    
    std::vector<int> index;
    std::atomic<bool> isFromRemote;
    bool isOpenDocumentCursor;
    std::vector<std::vector<QSymbol>> testSymbols;
    int currentSelectedChars;
    
    UserHighlighter highlighter;
    std::map<int, int> cursorMap;
    std::atomic<bool> isUserColorsToggled;

    int documentId;
    QString documentName = "";
    int nChars = 0;

    void insertRow(int pos, int n);
    void deleteRow(int pos, int n);
    void incrementIndex(int pos, int n);
    void decrementIndex(int pos, int n);
    int getPosition(int row, int col);
    void printSymbols();
    bool isNewLine(QChar c);
    //void incrementIndexAtPos(int pos, int n);

public slots:
    void remoteInsert(QSymbol symbol);
    void remoteInsertBlock(std::vector<QSymbol> symbols);
    //void remoteOpenBlock(std::vector<QSymbol> symbols);
    void remoteErase(QSymbol symbol);
    void remoteEraseBlock(std::vector<QSymbol> symbols);
    void updateCursor(int userId, int position);
    void updateAlignment(Qt::Alignment alignment, int position);
    void openDocument(int docId, QString docName, std::vector<std::vector<QSymbol>> symbols);
    void updateCursorMap(QVector<User> onlineUserList);

private slots:
    void selectFont();
    void setFontBold(bool bold);
    void setFontItalic(bool italic);
    void setFontUnderline(bool underline);
    void setFontColor();
    void setTextAlignment(QAction *action);
    void alignmentChange(Qt::Alignment a);
    void currentCharFormatChange(const QTextCharFormat &f);
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
    void textAlignmentChanged(Qt::Alignment alignment, int position, int siteId);
    void setMainWindowTitle(QString title);

};


#endif //COTEXT_CLIENT_TEXTEDITOR_H
