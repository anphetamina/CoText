#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class MainMenu;
}

class MainMenu : public QDialog
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();
    void closeEvent (QCloseEvent *event);

private slots:
    void on_pushButton_newDocument_clicked();
    void on_pushButton_openDocument_clicked();
    void on_pushButton_join_clicked();

public slots:
    void closeMainMenuSlot();

signals:
    void newDocumentClicked();
    void openDocumentClicked();
    void joinClicked();
    void sendCloseMainWindow();

private:
    Ui::MainMenu *ui;
};

#endif // MAINMENU_H
