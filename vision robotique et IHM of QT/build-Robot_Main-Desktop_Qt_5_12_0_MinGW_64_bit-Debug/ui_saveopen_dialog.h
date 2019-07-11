/********************************************************************************
** Form generated from reading UI file 'saveopen_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEOPEN_DIALOG_H
#define UI_SAVEOPEN_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SaveOpen_Dialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label_Image;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_SubImage;
    QSpacerItem *verticalSpacer;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_WidthMinus;
    QPushButton *pushButton_WidthPlus;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_HeightPlus;
    QPushButton *pushButton_HeightMinus;
    QFrame *line;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_Up;
    QPushButton *pushButton_Down;
    QPushButton *pushButton_Left;
    QPushButton *pushButton_Right;
    QDialogButtonBox *buttonBox;
    QFrame *line_2;

    void setupUi(QDialog *SaveOpen_Dialog)
    {
        if (SaveOpen_Dialog->objectName().isEmpty())
            SaveOpen_Dialog->setObjectName(QString::fromUtf8("SaveOpen_Dialog"));
        SaveOpen_Dialog->resize(315, 251);
        gridLayout = new QGridLayout(SaveOpen_Dialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_Image = new QLabel(SaveOpen_Dialog);
        label_Image->setObjectName(QString::fromUtf8("label_Image"));
        label_Image->setCursor(QCursor(Qt::CrossCursor));

        gridLayout->addWidget(label_Image, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        label_SubImage = new QLabel(SaveOpen_Dialog);
        label_SubImage->setObjectName(QString::fromUtf8("label_SubImage"));

        verticalLayout->addWidget(label_SubImage);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        line_3 = new QFrame(SaveOpen_Dialog);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_WidthMinus = new QPushButton(SaveOpen_Dialog);
        pushButton_WidthMinus->setObjectName(QString::fromUtf8("pushButton_WidthMinus"));

        horizontalLayout->addWidget(pushButton_WidthMinus);

        pushButton_WidthPlus = new QPushButton(SaveOpen_Dialog);
        pushButton_WidthPlus->setObjectName(QString::fromUtf8("pushButton_WidthPlus"));

        horizontalLayout->addWidget(pushButton_WidthPlus);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pushButton_HeightPlus = new QPushButton(SaveOpen_Dialog);
        pushButton_HeightPlus->setObjectName(QString::fromUtf8("pushButton_HeightPlus"));

        verticalLayout_2->addWidget(pushButton_HeightPlus);

        pushButton_HeightMinus = new QPushButton(SaveOpen_Dialog);
        pushButton_HeightMinus->setObjectName(QString::fromUtf8("pushButton_HeightMinus"));

        verticalLayout_2->addWidget(pushButton_HeightMinus);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout);

        line = new QFrame(SaveOpen_Dialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        pushButton_Up = new QPushButton(SaveOpen_Dialog);
        pushButton_Up->setObjectName(QString::fromUtf8("pushButton_Up"));

        gridLayout_2->addWidget(pushButton_Up, 2, 1, 1, 1);

        pushButton_Down = new QPushButton(SaveOpen_Dialog);
        pushButton_Down->setObjectName(QString::fromUtf8("pushButton_Down"));

        gridLayout_2->addWidget(pushButton_Down, 4, 1, 1, 1);

        pushButton_Left = new QPushButton(SaveOpen_Dialog);
        pushButton_Left->setObjectName(QString::fromUtf8("pushButton_Left"));

        gridLayout_2->addWidget(pushButton_Left, 3, 0, 1, 1);

        pushButton_Right = new QPushButton(SaveOpen_Dialog);
        pushButton_Right->setObjectName(QString::fromUtf8("pushButton_Right"));

        gridLayout_2->addWidget(pushButton_Right, 3, 2, 1, 1);

        buttonBox = new QDialogButtonBox(SaveOpen_Dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_2->addWidget(buttonBox, 7, 0, 1, 3);

        line_2 = new QFrame(SaveOpen_Dialog);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_2, 5, 0, 1, 3);


        verticalLayout->addLayout(gridLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 1, 1, 1);


        retranslateUi(SaveOpen_Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SaveOpen_Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SaveOpen_Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SaveOpen_Dialog);
    } // setupUi

    void retranslateUi(QDialog *SaveOpen_Dialog)
    {
        SaveOpen_Dialog->setWindowTitle(QApplication::translate("SaveOpen_Dialog", "Dialog", nullptr));
        label_Image->setText(QApplication::translate("SaveOpen_Dialog", "No Image", nullptr));
        label_SubImage->setText(QApplication::translate("SaveOpen_Dialog", "\345\234\250\345\233\276\347\211\207\344\270\255\346\213\226\346\213\275\351\274\240\346\240\207\344\273\245\351\200\211\346\213\251\351\207\207\346\240\267\345\214\272\345\237\237", nullptr));
        pushButton_WidthMinus->setText(QApplication::translate("SaveOpen_Dialog", "\345\256\275--", nullptr));
        pushButton_WidthPlus->setText(QApplication::translate("SaveOpen_Dialog", "\345\256\275++", nullptr));
        pushButton_HeightPlus->setText(QApplication::translate("SaveOpen_Dialog", "\351\253\230++", nullptr));
        pushButton_HeightMinus->setText(QApplication::translate("SaveOpen_Dialog", "\351\253\230--", nullptr));
        pushButton_Up->setText(QApplication::translate("SaveOpen_Dialog", "\342\206\221", nullptr));
        pushButton_Down->setText(QApplication::translate("SaveOpen_Dialog", "\342\206\223", nullptr));
        pushButton_Left->setText(QApplication::translate("SaveOpen_Dialog", "\342\206\220", nullptr));
        pushButton_Right->setText(QApplication::translate("SaveOpen_Dialog", "\342\206\222", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SaveOpen_Dialog: public Ui_SaveOpen_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEOPEN_DIALOG_H
