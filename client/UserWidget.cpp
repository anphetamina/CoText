#include "MainWindow.h"
#include "ui_userwidget.h"
#include "UserWidget.h"

#include <QDebug>
#include <QPixmap>
#include <QPrinter>
#include <QColorDialog>
#include <QColor>
#include <QCloseEvent>
#include <QtSvg>
#include <QEvent>


UserWidget::UserWidget(QWidget *parent) : QDialog(parent), ui(new Ui::UserWidget) {
	
    ui->setupUi(this);
    //this->setUserState(OFFLINE);
    //this->setTimeBeforeInactive(300);
    qDebug() << ui->label_profilpic;
    
	//installing EventFilter for QWidgets in register
	dynamic_cast<QLabel*>(ui->label_profilpic)->installEventFilter(this);
	
	

    
    
    
  
    
    
    
}

UserWidget::~UserWidget() {
	
	delete ui;
	
}

bool UserWidget::eventFilter(QObject *watched, QEvent *event) {
	
	qDebug()<<"-----Sono entrato nella eventFilter del register" << event->type() << watched;
	//Checkthe watched QObjects
	if(watched == dynamic_cast<QLabel*>(ui->label_profilpic) && event->type() == QEvent::Enter) {
		setCursor(Qt::PointingHandCursor);
		QPixmap *uppp = new QPixmap(92, 92);
		QPixmap upp("/Users/marco_head/Desktop/minime.jpg");
		
		ui->label_profilpic->setPixmap(upp);
		
		
		
		
		
		return true;
	}

}


