#include "MainWindow.h"
#include "ui_userwidget.h"
#include "UserWidget.h"


UserWidget::UserWidget(QWidget *parent) : QDialog(parent), ui(new Ui::UserWidget) {
	
    ui->setupUi(this);
    //this->setUserState(OFFLINE);
    //this->setTimeBeforeInactive(300);
    
}
UserWidget::~UserWidget() {
	delete ui;
}

