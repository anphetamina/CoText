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
    void on_tabWidget_tabCloseRequested(int index);

public slots:
    void updateUserList(QVector<User> newUserList);
    void askUriReceivedMainWindow(QString URI);
    void sendJoinMainWindow(qint32 userId, int docId, QString invCode);
    void documentListReceivedMainWindow(QVector<QString> documentList);
    void sendOpenDocumentMainWindow(QString docName);
    void openDocumentMainWindow(int docId, QString docName, std::vector<std::vector<QSymbol>> qsymbols);

signals:
    void newColorMapReceived(QMap<int, QColor> colorMap);
    void sendAskUriMainWindow(qint32 userId, int docId, QString invCode);
    void sendDocCreateMainWindow(QString docName, qint32 userId);
    void sendAskDocListMainWindow(qint32 user);
    void sendOpenDocumentSignal(QString docName, qint32 user);

private:
    Ui::MainWindow *ui;
    QString currentFileName = "";
    QVector<QString> docList;
    QVector<QAction*> actionUserList;
    QVector<User> userList = {};
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

    void Save_as();
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
};
#endif // MAINWINDOW_H
