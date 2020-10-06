//
// Created by Marco Testa on 2020-10-04.
//

#ifndef COTEXT_CLIENT_STATUSBAR_H
#define COTEXT_CLIENT_STATUSBAR_H

#include <QDebug>
#include <QtGui>
#include <QLabel>
#include <QPixmap>
#include <QtWidgets/QStatusBar>
#include <QMainWindow>

namespace Ui { class MainWindow; }
class MainWindow;

class StatusBar : public QStatusBar {
	Q_OBJECT
	
public:
	explicit StatusBar(Ui::MainWindow &ui, MainWindow* mw, QWidget *parent = nullptr);
	~StatusBar();
	void setupSB();
	void displaySB();
	void updateDocInfo();
	void updateUsersInfo();
	QString calculateDocSize();
	
private:
	QWidget *parent; //pointer to MainWindow
	Ui::MainWindow &ui;
	QLabel *appName = new QLabel(tr("©Co-Text"));
	QLabel *vSeparator;
	MainWindow* mw;
	
	/** Users Info */
	QLabel *userListToggle = new QLabel(tr("userList"));
	QLabel *nActiveUsers;
	std::atomic<bool> isUserListToggled;

	
	/** Document info */
	QLabel *docTitle;
	QLabel *docSize;
	QLabel *nChars;
	
protected:


	
	

public slots:

private slots:

signals:


};


#endif //COTEXT_CLIENT_STATUSBAR_H
