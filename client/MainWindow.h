#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unistd.h>
#include <cstdlib>
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
#include "TextEditor.h"


//#include <QSqlDatabase>

/* COLOR PALETTE CODE:
    (30, 30, 30) dark
    (37, 37, 37) greydark
    (51, 51, 51) grey light
    (0, 99, 161) blue buttons
 */
class SslEchoClient;

inline User user;
inline SslEchoClient *client = nullptr;


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
        user.setName(usr->getName());
        user.setSurname(usr->getSurname());
        user.setNick(usr->getNick());
        user.setEmail(usr->getNick());
        user.setProfilePic(usr->getProfilePic());
        user.setUserState(20); //ACTIVE USER
    }

    static User getUser() {
    	return user;
    }
    
    void setupStatusBar();
    void setStatusBar(QStatusBar *qSb);

    Ui::MainWindow *getUi() const;

    void connectToTextEditor(TextEditor* te);

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
    void on_actionJoin_triggered();

public slots:
    void updateUserList(QVector<User> newUserList);
    void askUriReceivedMainWindow(QString URI);
    void sendJoinMainWindow(qint32 userId, int docId, QString invCode);

signals:
    void newColorMapReceived(QMap<int, QColor> colorMap);
    void sendAskUriMainWindow(qint32 userId, int docId, QString invCode);

private:
    Ui::MainWindow *ui;
    QStatusBar *statusBar;
    TextEditor *editor;
    QString currentFileName = "";
    QVector<User> userList = {};
    //std::vector<User> userList = {};
    QMap<int, QColor> colorMap = {};
    QVector<QColor> colorList = {QColor(255,0,0),   //red
                                 QColor(0,255,0),   //lime
                                 QColor(0,0,255),   //blue
                                 QColor(255,255,0),   //yellow
                                 QColor(0,255,255),   //aqua
                                 QColor(255,0,255),   //magenta
                                 QColor(210,105,30),   //chocolate
                                 QColor(189,183,107),   //dark kaki
                                 QColor(0,128,0),   //green
                                 QColor(255,127,80),   //coral
                                 QColor(154,205,50),   //yellow green
                                 QColor(102,205,170),   //medium aqua marine
                                 QColor(175,238,238),   //pale turquoise
                                 QColor(147,112,219),   //medium purple
                                 QColor(255,165,0),   //orange
                                 QColor(176,196,222),   //light steel blue
                                 QColor(255,182,193),   //light pink
                                 QColor(0,128,128),   //teal
                                 QColor(0,0,128),   //navy
                                 QColor(128,0,128)   //purple
    }; //is a static array of color

    /*QMap<QColor, bool> colorList = {
            {QColor(255, 0, 0),     false},  //red
            {QColor(0, 255, 0),     false}, //lime
            { QColor(0, 0, 255),     false}, //blue
            {QColor(255, 255, 0),   false},  //yellow
            {QColor(0, 255, 255),   false},  //aqua
            {QColor(255, 0, 255),   false}, //magenta
            {QColor(210, 105, 30),  false},   //chocolate
            {QColor(189, 183, 107), false}, //dark kaki
            {QColor(0, 128, 0),     false}, //green
            {QColor(255, 127, 80),  false},  //coral
            {QColor(154, 205, 50),  false},  //yellow green
            {QColor(102, 205, 170), false}, //medium aqua marine
            {QColor(175, 238, 238), false}, //pale turquoise
            {QColor(147, 112, 219), false},  //medium purple
            {QColor(255, 165, 0),   false},  //orange
            {QColor(176, 196, 222), false},  //light steel blue
            {QColor(255, 182, 193), false}, //light pink
            {QColor(0, 128, 128),   false},  //teal
            {QColor(0, 0, 128),     false},  //navy
            {QColor(128, 0, 128),   false} //purple
    }*/
                                     //is a static array of color

    /*std::multimap<int, QColor> colorMap = {
            {-1, QColor(255, 0, 0)},  //red
            {-1, QColor(0, 255, 0)}, //lime
            {-1, QColor(0, 0, 255)}, //blue
            {-1, QColor(255, 255, 0)},  //yellow
            {-1,QColor(0, 255, 255)},  //aqua
            {-1, QColor(255, 0, 255)}, //magenta
            {-1, QColor(210, 105, 30)},   //chocolate
            {-1, QColor(189, 183, 107)}, //dark kaki
            {-1, QColor(0, 128, 0)}, //green
            {-1, QColor(255, 127, 80)},  //coral
            {-1,QColor(154, 205, 50)},  //yellow green
            {-1,QColor(102, 205, 170)}, //medium aqua marine
            {-1,QColor(175, 238, 238)}, //pale turquoise
            {-1,QColor(147, 112, 219)},  //medium purple
            {-1,QColor(255, 165, 0)},  //orange
            {-1,QColor(176, 196, 222)},  //light steel blue
            {-1,QColor(255, 182, 193)}, //light pink
            {-1,QColor(0, 128, 128)},  //teal
            {-1,QColor(0, 0, 128)},  //navy
            {-1,QColor(128, 0, 128)} //purple
    };*/

    //std::multimap<int, QAction*> actionUserMap;
    QVector<QAction*> actionUserList;
    //QAction* actionUserList;
    //SslEchoClient *client;
    void Save_as();
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

};
#endif // MAINWINDOW_H
