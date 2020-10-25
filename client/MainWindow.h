#ifndef MAINWINDOW_H
#define MAINWINDOW_H


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
#include "ChooseName.h"

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#else 
#include <unistd.h>
#endif


/* COLOR PALETTE CODE:
    (30, 30, 30) dark
    (37, 37, 37) greydark
    (51, 51, 51) grey light
    (0, 99, 161) blue buttons
 */
class SslEchoClient;


static int const EXIT_CODE_REBOOT = -123456789;

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
		user.setPassword(usr->getPassword());
		user.setEmail(usr->getEmail());
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
    void on_actionPrintPDF_triggered();
    void on_actionShare_Uri_triggered();
    void on_actionUserInfo_triggered();
    void on_actionJoin_triggered();
    void on_actionNew_triggered();
	void on_actionCoTextInfo_triggered();

public slots:
    void updateUserList(QVector<User> newOnlineUserList, QVector<User> newCompleteUserList);
    void askUriReceivedMainWindow(QString URI);
    void sendJoinMainWindow(qint32 userId, int docId, QString invCode);
    void sendOpenDocumentMainWindow(QString docName);
    void documentListReceivedMainWindow(QVector<QString> documentList);
    void setMainWindowTitle(QString title);
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
    void joinFailedMW();
    void joinSucceededMW();
    void updateUserInToolbarMW();

signals:
    void sendCloseDocumentMainWindow();
    void sendAskUriMainWindow(qint32 userId, int docId, QString invCode);
    void sendAskDocListMainWindow(qint32 user);
    void sendOpenDocumentSignal(QString docName, qint32 user);
    void sendDocCreateMainWindow(QString docName, qint32 userId);
    void closeMainMenu();
    void sendDocumentDeletedSignal(QString docName, quint32 userId);
    void closeJoinMW();
    void showJoinErrorMW();
    void closeChooseNameMW();
    void closeOpenDocumentMW();

private:
    Ui::MainWindow *ui;
    StatusBar *qSB;
    TextEditor * editor;
    QVector<QString> docList;
    QVector<User> onlineUserList = {};        //list of all the user online on this document
    QMap<int, QColor> colorMap = {};
    QVector<QColor> colorList = {QColor(193,26,26),       //red
                                 QColor(63,204,63),       //lime
                                 QColor(24,24,182),       //blue
                                 QColor(222,222,36),     //yellow
                                 QColor(25,224,224),     //aqua
                                 QColor(198,33,198),     //magenta
                                 QColor(243,149,82),    //chocolate
                                 QColor(195,187,91),   //dark kaki
                                 QColor(20,115,20),       //green
                                 QColor(243,115,69),    //coral
                                 QColor(154,205,50),    //yellow green
                                 QColor(102,205,170),   //medium aqua marine
                                 QColor(154,218,218),   //pale turquoise
                                 QColor(120,89,183),   //medium purple
                                 QColor(238,161,21),     //orange
                                 QColor(176,196,222),   //light steel blue
                                 QColor(135,173,183),   //light pink
                                 QColor(33,141,141),     //teal
                                 QColor(23,23,122),       //navy
                                 QColor(138,24,138)      //purple
    }; //is a static array of color

    void Save_as();
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void removeOldOnlineNowOffline(QVector<User> newOnlineUserList);
    QPixmap addImageInRightToolBar(const QPixmap &orig, QColor color);
    void createChooseName(bool isromMainMenu);
    void openDocument(bool isFromMainMenu);
    void addUserInRightToolbar(User u);

};
#endif // MAINWINDOW_H
