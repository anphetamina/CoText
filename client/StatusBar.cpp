//
// Created by Marco Testa on 2020-10-04.
//


#include "StatusBar.h"

//! [Constructor]
StatusBar::StatusBar(Ui::MainWindow &ui, QWidget *parent): parent(parent), ui(ui), isUserListToggled(false) {
	
	//this->setupSB();
	//this->displaySB();
	
}
StatusBar::~StatusBar() {
	delete this;
}

void StatusBar::setupSB() {
	/**Setup */
	
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
	
	//3.
	nActiveUsers = new QLabel(tr("#activeUsers"));
	nActiveUsers->setMinimumSize(nActiveUsers->sizeHint());
	nActiveUsers->setAlignment(Qt::AlignCenter);
	nActiveUsers->setText(tr("nActiveUsers")); //TODO it has to be the number
	nActiveUsers->setToolTip("#activeUsers");
	
	//4.
	docTitle = new QLabel("Document Title");
	docTitle->setMinimumSize(docTitle->sizeHint());
	docTitle->setAlignment(Qt::AlignCenter);
	docTitle->setText(tr("docTitle"));
	docTitle->setToolTip("This is the current document title");
	
	//5.
	docSize = new QLabel(tr("Document Size"));
	docSize->setMinimumSize(docSize->sizeHint());
	docSize->setAlignment((Qt::AlignRight | Qt::AlignVCenter));
	docSize->setText(tr("size of the document").arg(0));
	docSize->setToolTip(tr("The memory used for the current document.")); //TODO implement the count in KB -> MB
	docSize->setCursor(Qt::PointingHandCursor);
	
	//6.
	nChars = new QLabel("#nChars");
	nChars->setMinimumSize(nChars->sizeHint());
	nChars->setAlignment(Qt::AlignCenter);
	nChars->setText(tr("#chars"));
	nChars->setToolTip("Total amount of chars of the document");
	
	
	
	//7. TODO error messages with icon
	
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
	// TODO handling click on the widget to show up the list of users
	
	//starting from left
	addWidget(docTitle);
	addWidget(docSize);
	addWidget(nChars);
	/**end display */
	
}

void StatusBar::updateDocInfo() {}
void StatusBar::updateUsersInfo() {}
