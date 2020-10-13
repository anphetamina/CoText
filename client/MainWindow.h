#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unistd.h>
#include <cstdlib>
#include <QPointingDeviceUniqueId>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QWidget>
#include <QEvent>
#include <QtGui/QTextCharFormat>
#include <QtWidgets/QFontComboBox>
#include "UserWidget.h"
#include "TextEditor.h"
#include "StatusBar.h"
#include "Login.h"
#include "MainMenu.h"


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
    
	//! [Setters]
	static void setUser(User* usr){
		user.setName(usr->getName());
		user.setSurname(usr->getSurname());
		user.setNick(usr->getNick());
		user.setEmail(usr->getNick());
		user.setProfilePic(usr->getProfilePic());
		user.setUserState(20); //ACTIVE USER
	}
	
	void setTextEditor(TextEditor* te);
	
	
	//! [Getters]
	Ui::MainWindow *getUi() const;
	TextEditor* getTextEditor() const;
	QColor getUserColor(int userId) const;
    static User getUser() {
    	return user;
    }
    
    QVector<User> getOnlineUserList() const;

    void connectToMainMenu(MainMenu* mainMenu);

    QFontComboBox *fontComboBox;
    QComboBox *sizeComboBox;
    

private slots:
    void on_actionOpen_triggered();
    void on_actionLogout_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionCut_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionPrintPDF_triggered();
    void on_actionShare_Uri_triggered();
    void on_actionSettings_triggered();
    void on_actionJoin_triggered();
    void on_actionNew_triggered();

public slots:
    void updateUserList(QVector<User> newOnlineUserList, QVector<User> newCompleteUserList);
    void askUriReceivedMainWindow(QString URI);
    void sendJoinMainWindow(qint32 userId, int docId, QString invCode);
    void sendOpenDocumentMainWindow(QString docName);
    void documentListReceivedMainWindow(QVector<QString> documentList);
    void setMainWindowTitle(QString title);
    void openNewDocumentMainWindow(QString docName);
    void nameChosenMainWindow(QString name);
    void newDocumentFromMainMenu();
    void openDocumentFromMainMenu();
    void joinFromMainMenu();
    void nameChosenFromMainMenu(QString name);
    void sendOpenDocumentFromMainMenu(QString docName);
    void sendJoinFromMainMenu(qint32 userId, int docId, QString invCode);
    void openDocumentMainWindow(int docId, QString docName, std::vector<std::vector<QSymbol>> qsymbols);
    void sendDocumentDeletedMainWindow(QString docName);
    void closeMainWindow();

signals:
    void sendAskUriMainWindow(qint32 userId, int docId, QString invCode);
    void sendAskDocListMainWindow(qint32 user);
    void sendOpenDocumentSignal(QString docName, qint32 user);
    void sendDocCreateMainWindow(QString docName, qint32 userId);
    void closeMainMenu();
    void sendDocumentDeletedSignal(QString docName, quint32 userId);

private:
    Ui::MainWindow *ui;
    StatusBar *qSB;
    TextEditor *editor;
    QVector<QString> docList;
    QVector<User> onlineUserList = {};        //list of all the user online on this document
    QMap<int, QColor> colorMap = {};
    QVector<QColor> colorList = {QColor(255,0,0),       //red
                                 QColor(0,255,0),       //lime
                                 QColor(0,0,255),       //blue
                                 QColor(255,255,0),     //yellow
                                 QColor(0,255,255),     //aqua
                                 QColor(255,0,255),     //magenta
                                 QColor(210,105,30),    //chocolate
                                 QColor(189,183,107),   //dark kaki
                                 QColor(0,128,0),       //green
                                 QColor(255,127,80),    //coral
                                 QColor(154,205,50),    //yellow green
                                 QColor(102,205,170),   //medium aqua marine
                                 QColor(175,238,238),   //pale turquoise
                                 QColor(147,112,219),   //medium purple
                                 QColor(255,165,0),     //orange
                                 QColor(176,196,222),   //light steel blue
                                 QColor(255,182,193),   //light pink
                                 QColor(0,128,128),     //teal
                                 QColor(0,0,128),       //navy
                                 QColor(128,0,128)      //purple
    }; //is a static array of color

    void Save_as();
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void removeOldOnlineNowOffline(QVector<User> newOnlineUserList);
    QPixmap addImageInRightToolBar(const QPixmap &orig, QColor color);

};
#endif // MAINWINDOW_H
