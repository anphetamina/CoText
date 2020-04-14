/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_User;
    QLineEdit *lineEdit_User;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_Pass;
    QLineEdit *lineEdit_Pass;
    QPushButton *pushButton_Login;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QPushButton *pushButton_Reg;

    void setupUi(QDialog *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QString::fromUtf8("Login"));
        Login->resize(400, 300);
        verticalLayout = new QVBoxLayout(Login);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(Login);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(0, 0));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_User = new QLabel(groupBox);
        label_User->setObjectName(QString::fromUtf8("label_User"));

        horizontalLayout->addWidget(label_User);

        lineEdit_User = new QLineEdit(groupBox);
        lineEdit_User->setObjectName(QString::fromUtf8("lineEdit_User"));
        lineEdit_User->setAutoFillBackground(false);

        horizontalLayout->addWidget(lineEdit_User);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_Pass = new QLabel(groupBox);
        label_Pass->setObjectName(QString::fromUtf8("label_Pass"));

        horizontalLayout_2->addWidget(label_Pass);

        lineEdit_Pass = new QLineEdit(groupBox);
        lineEdit_Pass->setObjectName(QString::fromUtf8("lineEdit_Pass"));

        horizontalLayout_2->addWidget(lineEdit_Pass);


        verticalLayout_2->addLayout(horizontalLayout_2);

        pushButton_Login = new QPushButton(groupBox);
        pushButton_Login->setObjectName(QString::fromUtf8("pushButton_Login"));

        verticalLayout_2->addWidget(pushButton_Login);


        verticalLayout->addWidget(groupBox);

        frame = new QFrame(Login);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        pushButton_Reg = new QPushButton(frame);
        pushButton_Reg->setObjectName(QString::fromUtf8("pushButton_Reg"));

        horizontalLayout_3->addWidget(pushButton_Reg);


        verticalLayout->addWidget(frame);


        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QDialog *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Login", "Login", nullptr));
        label_User->setText(QCoreApplication::translate("Login", "Username", nullptr));
        lineEdit_User->setText(QString());
        lineEdit_User->setPlaceholderText(QCoreApplication::translate("Login", "enter your email", nullptr));
        label_Pass->setText(QCoreApplication::translate("Login", "Password", nullptr));
        lineEdit_Pass->setPlaceholderText(QCoreApplication::translate("Login", "enter your password", nullptr));
        pushButton_Login->setText(QCoreApplication::translate("Login", "Login", nullptr));
        label->setText(QCoreApplication::translate("Login", "Not registered yet?", nullptr));
        pushButton_Reg->setText(QCoreApplication::translate("Login", "Register now!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
