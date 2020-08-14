#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unistd.h>
#include <stdlib.h>
#include <QPointingDeviceUniqueId>
#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QWidget>
#include <QEvent>
#include <QtGui/QTextCharFormat>

#include "UserWidget.h"

//#include <QSqlDatabase>

/* COLOR PALETTE CODE:
    (30, 30, 30) dark
    (37, 37, 37) greydark
    (51, 51, 51) grey light
    (0, 99, 161) blue buttons


 */

static User *user = nullptr;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    static void setUser(User* usr){
        user = new User();
        user->setName(usr->getName());
        user->setSurname(usr->getSurname());
        user->setNick(usr->getNick());
        user->setEmail(usr->getNick());
        user->setProfilePic(usr->getProfilePic());
        user->setUserState(20); //ACTIVE USER
    }

    Ui::MainWindow *getUi() const;


private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_actionExit_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionLogin_triggered();

    void on_actionPrintPDF_triggered();

    void on_actionSave_triggered();

    void on_actionShare_Uri_triggered();
    
    void on_actionSettings_triggered();


private:

    Ui::MainWindow *ui;
    QString currentFileName = "";
    void Save_as();
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

};
#endif // MAINWINDOW_H
