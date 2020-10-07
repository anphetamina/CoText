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
	
	//! [Setters]
	void setSiteId(int siteId);
	
	//! [Getters]
	int getRow(int position) const;
	int getCol(int row, int position) const;
	
	int getSiteId();
	int getDocId() const;
	QString getDocName() const;
	int getNumChars() const;
	
	int getUserId(int row, int col) const;
	QColor getUserColor(int userId) const;
	
	//![public attributes]
	std::atomic<bool> isFromRemote;
    std::atomic<bool> isFromRemoteCursor;

    


private:
    QWidget *parent;
    Ui::MainWindow &ui;
    SharedEditor editor;
    
    std::vector<int> index;
    std::vector<std::vector<QSymbol>> testSymbols;
    int currentSelectedChars;
    
    UserHighlighter highlighter;
    std::map<int, int> cursorMap;
    std::atomic<bool> isUserColorsToggled;
    
    //! [private Document attributes]
    int documentId;
    QString documentName = "";
    int nChars = 0;
    //! [end Document attributes]

    void insertRow(int pos, int n);
    void deleteRow(int pos, int n);
    void incrementIndex(int pos, int n);
    void decrementIndex(int pos, int n);
    int getPosition(int row, int col);
    void printSymbols();
    bool isNewLine(QChar c);

public slots:
    void remoteInsert(QSymbol symbol);
    void remoteInsertBlock(std::vector<QSymbol> symbols);
    void remoteErase(QSymbol symbol);
    void remoteEraseBlock(std::vector<QSymbol> symbols);
    void updateCursor(int userId, int position);
    void updateAlignment(Qt::Alignment alignment, int position);
    void openDocument(int docId, QString docName, std::vector<std::vector<QSymbol>> symbols);
    void updateCursorMap(QVector<User> onlineUserList);
    //void loginSuccessful();

private slots:
    void selectFont();
    void setFontBold(bool bold);
    void setFontItalic(bool italic);
    void setFontUnderline(bool underline);
    void setFontColor();
    void setTextAlignment(QAction *action);
    void alignmentChanged(Qt::Alignment a);
    void currentCharFormatChanged(const QTextCharFormat &f);
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
