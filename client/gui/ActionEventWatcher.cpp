#include "ActionEventWatcher.h"

ActionEventWatcher::ActionEventWatcher(QObject *parent) : QObject(parent) { }

bool ActionEventWatcher::eventFilter(QObject *watched, QEvent *event) {
    if(watched == ui->textEdit) {
        ui->textEdit->setStyleSheet("background-color: green;");
        qDebug()<< "press the textEdit";
        return true;
    }

    if(watched == ui->actionLogin && event->type() == QEvent::HoverEnter) {
        qDebug() << "hover entered";
        ui->textEdit->setStyleSheet("background-color: green;");
        ui->actionLogin->setIcon(QIcon(":/imgs/icons/noun_user login_178831_white.svg"));
        return true;
    }

    if(watched == ui->actionLogin && event->type() == QEvent::HoverLeave) {
        ui->actionLogin->setIcon(QIcon(":/imgs/icons/noun_user login_178831.svg"));
        return true;
    }

    return false;
}
