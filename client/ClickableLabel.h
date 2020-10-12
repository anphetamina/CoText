//
// Created by Marco Testa on 2020-10-12.
//

#ifndef COTEXT_CLIENT_CLICKABLELABEL_H
#define COTEXT_CLIENT_CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>


class ClickableLabel : public QLabel {
	Q_OBJECT

public:
	explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
	~ClickableLabel();
	
signals:
	void clicked();
	
protected:
	void mouseReleaseEvent(QMouseEvent *ev);
	
	
};


#endif //COTEXT_CLIENT_CLICKABLELABEL_H
