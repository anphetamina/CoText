//
// Created by Marco Testa on 2020-10-12.
//

#include "ClickableLabel.h"

ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f) : QLabel(parent){}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *ev) {
	emit clicked();
}