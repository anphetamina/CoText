//
// Created by Marco Testa on 2020-10-04.
//


#include <ui_mainwindow.h>
#include "StatusBar.h"
#include "MainWindow.h"

//! [Constructor]
StatusBar::StatusBar(Ui::MainWindow &ui, MainWindow* mw, QWidget *parent): mw(mw), parent(parent), ui(ui), isUserListToggled(false) {
	
	//this->setupSB();
	//this->displaySB();
}

StatusBar::~StatusBar() {
	delete this;
}

void StatusBar::setupSB() {
	
	/**Setup */
	this->setFixedHeight(22);
	//0. Define separator to use multiple times (matter of graphics)
	vSeparator = new QLabel(tr("verticalSeparator"));
	vSeparator->setAlignment(Qt::AlignCenter);
	QPixmap vSepPM(QPixmap(":imgs/icons/verticalSeparator.svg"));
	vSeparator->setPixmap(vSepPM);
	
	//1.
	appName->setMinimumSize(appName->sizeHint());
	appName->setAlignment(Qt::AlignCenter);
	appName->setText(tr("©Co-Text"));
	appName->setToolTip("Name of the application");
	appName->setCursor(Qt::PointingHandCursor);
	
	//2.
	userListToggle->setMinimumSize(userListToggle->sizeHint());
	userListToggle->setAlignment(Qt::AlignCenter);
	userListToggle->setText("activeUsers");
	userListToggle->setToolTip("List of active users on this Document");
	userListToggle->setCursor(Qt::PointingHandCursor);
	QPixmap userListPM(QPixmap(":imgs/icons/user-group_whiteSB.svg"));
	userListToggle->setPixmap(userListPM);
	userListToggle->setFixedWidth(20);
	
	connect(userListToggle, &ClickableLabel::clicked, this, &StatusBar::showActiveUsersList);
	
	
	//3.
	nActiveUsers = new QLabel(tr("#activeUsers"));
	nActiveUsers->setMinimumSize(nActiveUsers->sizeHint());
	nActiveUsers->setFixedWidth(22);
	nActiveUsers->setAlignment(Qt::AlignCenter);
	nActiveUsers->setText("1");
	nActiveUsers->setToolTip("#activeUsers");
	
	//4.
	docTitleLabel = new QLabel("DocTitleLabel");
	docTitleLabel->setMinimumSize(docTitleLabel->sizeHint());
	docTitleLabel->setAlignment(Qt::AlignLeft);
	docTitleLabel->setText("Current document: ");
	
	docTitle = new QLabel("Document Title");
	docTitle->setMinimumSize(docTitle->sizeHint());
	docTitle->setAlignment(Qt::AlignLeft);
	docTitle->setText("Untitled");
	docTitle->setFixedWidth(120);
	docTitle->setToolTip("This is the current document title");
	
	/** end setup */
	
	
}

void StatusBar::displaySB() {
	/** Display */
	this->clearMessage();
	
	//this function append starting from the right
	addPermanentWidget(userListToggle);
	addPermanentWidget(nActiveUsers);
	addPermanentWidget(vSeparator);
	addPermanentWidget(appName);
	
	//starting from left
	addWidget(docTitleLabel);
	addWidget(docTitle);
	/**end display */
	
}


void StatusBar::updateDocInfo(QString title) {
	this->docTitle->setText("'"+title+"'");
}
void StatusBar::updateUsersInfo(QString n) {
	this->nActiveUsers->setText(n);
}

void StatusBar::showActiveUsersList() {
	Ui::MainWindow *ui = mw->getUi();
	qDebug() << "ShowActiveUserList";
	if(ui->rightToolBar->isVisible())
		ui->rightToolBar->setVisible(false);
	else
		ui->rightToolBar->setVisible(true);
}

