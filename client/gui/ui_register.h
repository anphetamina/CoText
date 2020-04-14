/********************************************************************************
** Form generated from reading UI file 'register.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTER_H
#define UI_REGISTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Register
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBoxRegister;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_Pass1;
    QLineEdit *lineEdit_Pass1;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_Email;
    QLineEdit *lineEdit_email;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_Nick;
    QLineEdit *lineEdit_Nick;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout;
    QLabel *label_Name;
    QLineEdit *lineEdit_Name;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_Surname;
    QLineEdit *lineEdit_Surname;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_Pass2;
    QLineEdit *lineEdit_Pass2;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_5;
    QFrame *line;
    QWidget *dropBox;
    QLabel *label;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *pushButton_Register;
    QPushButton *pushButton_Canc_Reg;

    void setupUi(QDialog *Register)
    {
        if (Register->objectName().isEmpty())
            Register->setObjectName(QString::fromUtf8("Register"));
        Register->resize(593, 521);
        verticalLayout = new QVBoxLayout(Register);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBoxRegister = new QGroupBox(Register);
        groupBoxRegister->setObjectName(QString::fromUtf8("groupBoxRegister"));
        gridLayout = new QGridLayout(groupBoxRegister);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_Pass1 = new QLabel(groupBoxRegister);
        label_Pass1->setObjectName(QString::fromUtf8("label_Pass1"));

        horizontalLayout_7->addWidget(label_Pass1);

        lineEdit_Pass1 = new QLineEdit(groupBoxRegister);
        lineEdit_Pass1->setObjectName(QString::fromUtf8("lineEdit_Pass1"));

        horizontalLayout_7->addWidget(lineEdit_Pass1);


        gridLayout->addLayout(horizontalLayout_7, 2, 3, 1, 2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_7, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_Email = new QLabel(groupBoxRegister);
        label_Email->setObjectName(QString::fromUtf8("label_Email"));

        horizontalLayout_3->addWidget(label_Email);

        lineEdit_email = new QLineEdit(groupBoxRegister);
        lineEdit_email->setObjectName(QString::fromUtf8("lineEdit_email"));

        horizontalLayout_3->addWidget(lineEdit_email);


        gridLayout->addLayout(horizontalLayout_3, 1, 3, 1, 2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_Nick = new QLabel(groupBoxRegister);
        label_Nick->setObjectName(QString::fromUtf8("label_Nick"));

        horizontalLayout_2->addWidget(label_Nick);

        lineEdit_Nick = new QLineEdit(groupBoxRegister);
        lineEdit_Nick->setObjectName(QString::fromUtf8("lineEdit_Nick"));

        horizontalLayout_2->addWidget(lineEdit_Nick);


        gridLayout->addLayout(horizontalLayout_2, 1, 1, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_8, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_Name = new QLabel(groupBoxRegister);
        label_Name->setObjectName(QString::fromUtf8("label_Name"));

        horizontalLayout->addWidget(label_Name);

        lineEdit_Name = new QLineEdit(groupBoxRegister);
        lineEdit_Name->setObjectName(QString::fromUtf8("lineEdit_Name"));

        horizontalLayout->addWidget(lineEdit_Name);


        gridLayout->addLayout(horizontalLayout, 0, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_Surname = new QLabel(groupBoxRegister);
        label_Surname->setObjectName(QString::fromUtf8("label_Surname"));

        horizontalLayout_4->addWidget(label_Surname);

        lineEdit_Surname = new QLineEdit(groupBoxRegister);
        lineEdit_Surname->setObjectName(QString::fromUtf8("lineEdit_Surname"));

        horizontalLayout_4->addWidget(lineEdit_Surname);


        gridLayout->addLayout(horizontalLayout_4, 0, 3, 1, 2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_Pass2 = new QLabel(groupBoxRegister);
        label_Pass2->setObjectName(QString::fromUtf8("label_Pass2"));

        horizontalLayout_5->addWidget(label_Pass2);

        lineEdit_Pass2 = new QLineEdit(groupBoxRegister);
        lineEdit_Pass2->setObjectName(QString::fromUtf8("lineEdit_Pass2"));

        horizontalLayout_5->addWidget(lineEdit_Pass2);


        gridLayout->addLayout(horizontalLayout_5, 2, 1, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 2, 5, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_9, 2, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 2, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 0, 5, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 1, 5, 1, 1);


        verticalLayout->addWidget(groupBoxRegister);

        line = new QFrame(Register);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        dropBox = new QWidget(Register);
        dropBox->setObjectName(QString::fromUtf8("dropBox"));
        label = new QLabel(dropBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 351, 20));

        verticalLayout->addWidget(dropBox);

        line_2 = new QFrame(Register);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        pushButton_Register = new QPushButton(Register);
        pushButton_Register->setObjectName(QString::fromUtf8("pushButton_Register"));
        pushButton_Register->setAutoDefault(true);
        pushButton_Register->setFlat(false);

        horizontalLayout_6->addWidget(pushButton_Register);

        pushButton_Canc_Reg = new QPushButton(Register);
        pushButton_Canc_Reg->setObjectName(QString::fromUtf8("pushButton_Canc_Reg"));

        horizontalLayout_6->addWidget(pushButton_Canc_Reg);


        verticalLayout->addLayout(horizontalLayout_6);


        retranslateUi(Register);

        pushButton_Register->setDefault(false);


        QMetaObject::connectSlotsByName(Register);
    } // setupUi

    void retranslateUi(QDialog *Register)
    {
        Register->setWindowTitle(QCoreApplication::translate("Register", "Dialog", nullptr));
        groupBoxRegister->setTitle(QCoreApplication::translate("Register", "Register Form", nullptr));
        label_Pass1->setText(QCoreApplication::translate("Register", "Password", nullptr));
        lineEdit_Pass1->setPlaceholderText(QCoreApplication::translate("Register", "re-type password", nullptr));
        label_Email->setText(QCoreApplication::translate("Register", "Email", nullptr));
        label_Nick->setText(QCoreApplication::translate("Register", "Nick", nullptr));
        lineEdit_Nick->setPlaceholderText(QCoreApplication::translate("Register", "choose a nickname", nullptr));
        label_Name->setText(QCoreApplication::translate("Register", "Name", nullptr));
        label_Surname->setText(QCoreApplication::translate("Register", "Surname", nullptr));
        label_Pass2->setText(QCoreApplication::translate("Register", "Password", nullptr));
        lineEdit_Pass2->setPlaceholderText(QCoreApplication::translate("Register", "enter password", nullptr));
        label->setText(QCoreApplication::translate("Register", "Drop an Image in this area to load as your profile picture", nullptr));
        pushButton_Register->setText(QCoreApplication::translate("Register", "Register", nullptr));
        pushButton_Canc_Reg->setText(QCoreApplication::translate("Register", "Cancel", nullptr));
#if QT_CONFIG(shortcut)
        pushButton_Canc_Reg->setShortcut(QCoreApplication::translate("Register", "Return", nullptr));
#endif // QT_CONFIG(shortcut)
    } // retranslateUi

};

namespace Ui {
    class Register: public Ui_Register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_H
