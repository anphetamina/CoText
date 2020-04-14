#include "MainWindow.h"
#include "UserWidget.h"
#include "../User.h"

UserWidget::UserWidget(QWidget *parent) : QDialog(parent), ui(new Ui::UserWidget) {
	
    ui->setupUi(this);
    this->setUserState(OFFLINE);
    this->setTimeBeforeInactive(300);
    
}

