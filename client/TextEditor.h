//
// Created by Antonio Santoro on 06/08/2020.
//

#ifndef COTEXT_CLIENT_TEXTEDITOR_H
#define COTEXT_CLIENT_TEXTEDITOR_H

#include <QDebug>
#include <iostream>
#include <QtGui/QPainter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QLabel>
#include "../common/SharedEditor.h"
#include "../common/QSymbol.h"
#include "UserHighlighter.h"
#include "../common/User.h"
#include "Resync.h"

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
    SharedEditor getEditor() const;
    std::vector<int> getIndex() const;
    void filePrintPdf(QString filename);

    std::atomic<bool> isFromRemote;

private:
    QWidget *parent;
    Ui::MainWindow &ui;
    SharedEditor editor;
    
    std::vector<int> index;
    int currentSelectedChars;
    
    UserHighlighter highlighter;
    std::map<int, std::pair<int, QLabel*>> cursorMap;
    std::atomic<bool> isUserColorsToggled;
    std::atomic<bool> hasLostFocus;
    std::atomic<bool> copiedFromOutside;
    std::atomic<bool> draggedFromOutside;
    std::atomic<bool> mousePressed;
    std::atomic<bool> undoRedoFlag;
    std::atomic<bool> isResyncing = false;

    QFontComboBox *fontComboBox;
    QComboBox *sizeComboBox;

    int documentId;
    QString documentName;

    void insertRow(int pos, int n);
    void deleteRow(int pos, int n);
    void incrementIndex(int pos, int n);
    void decrementIndex(int pos, int n);
    int getPosition(int row, int col);
    void printSymbols(const std::string &functionName);
    int isNewLine(QChar c);
    void insertBlock(const std::vector<QSymbol> &block, int position);
    void resyncWithSharedEditor();

public slots:
    void remoteInsert(QSymbol symbol);
    void remoteInsertBlock(std::vector<QSymbol> symbols);
    void remoteErase(QSymbol symbol);
    void remoteEraseBlock(std::vector<QSymbol> symbols);
    void updateCursor(int userId, int position);
    void updateAlignment(Qt::Alignment alignment, QSymbol symbol);
    void openDocument(int docId, QString docName, std::vector<std::vector<QSymbol>> symbols);
    void updateCursorMap(QVector<User> onlineUserList, QVector<User> completeUserList);
    void paintCursors();
    void resyncTE();

private slots:
    void setFontFamily(const QFont &font);
    void setFontSize(const QString &text);
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
    void clipboardDataChange();
    void textChanged();
    void undo();
    void redo();

protected:
    void focusInEvent(QFocusEvent *e) override;
    void focusOutEvent(QFocusEvent *e) override;
    void dragEnterEvent(QDragEnterEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void insertFromMimeData(const QMimeData *source) override;

signals:
    void symbolsInserted(std::vector<QSymbol> symbols, int siteId);
    void symbolsErased(std::vector<QSymbol> symbols, int siteId);
    void cursorPositionChanged(int userId, int position);
    void textAlignmentChanged(Qt::Alignment alignment, QSymbol symbol, int siteId);
    void sendDocOpenTE(QString docName, qint32 userId);
};


#endif //COTEXT_CLIENT_TEXTEDITOR_H
