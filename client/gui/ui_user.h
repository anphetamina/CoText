/********************************************************************************
** Form generated from reading UI file 'user.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USER_H
#define UI_USER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>

QT_BEGIN_NAMESPACE

class Ui_User
{
public:
    QGraphicsView *graphicsView;

    void setupUi(QDialog *User)
    {
        if (User->objectName().isEmpty())
            User->setObjectName(QString::fromUtf8("User"));
        User->resize(724, 603);
        graphicsView = new QGraphicsView(User);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(290, 80, 256, 192));

        retranslateUi(User);

        QMetaObject::connectSlotsByName(User);
    } // setupUi

    void retranslateUi(QDialog *User)
    {
        User->setWindowTitle(QCoreApplication::translate("User", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class User: public Ui_User {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USER_H
