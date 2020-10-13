#ifndef USEREDITWIDGET_H
#define USEREDITWIDGET_H

#include <QDialog>

namespace Ui {
class UserEditWidget;
}

class UserEditWidget : public QDialog
{
    Q_OBJECT

public:
    explicit UserEditWidget(QWidget *parent = nullptr);
    ~UserEditWidget();
    
    void setOldUsername(QString _oldUsername);
	QString getOldUsername();
	void setNewPP(QImage newPP);
	QImage getNewPP();
	QImage getOldPP();
	void setOldPP(QImage _oldPP);
	
	void clearInput();
    bool checkInput(const QString& name, const QString& surname, const QString& email, const QString &pass, const QString &re_pass);
	bool isPPChanged = false;

private:
    Ui::UserEditWidget *ui;
    QString oldUsername;
    QImage newProfilePic;
    QImage oldProfilePic;
    
    
public slots:
	void updateOk();
    
    
private slots:
	void on_pushButton_Done_clicked();
	void on_pushButton_Cancel_clicked();
	void on_buttonChangePP_clicked();
};

#endif // USEREDITWIDGET_H
