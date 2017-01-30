/********************************************************************************
** Form generated from reading UI file 'SX_3D_Client.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SX_3D_CLIENT_H
#define UI_SX_3D_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>
#include "SX_3D_Widget.h"

QT_BEGIN_NAMESPACE

class Ui_SX_3D_Client
{
public:
    QGridLayout *gridLayout;
    SX_3D_Widget *widget;

    void setupUi(QWidget *SX_3D_Client)
    {
        if (SX_3D_Client->objectName().isEmpty())
            SX_3D_Client->setObjectName(QStringLiteral("SX_3D_Client"));
        SX_3D_Client->resize(709, 495);
        gridLayout = new QGridLayout(SX_3D_Client);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget = new SX_3D_Widget(SX_3D_Client);
        widget->setObjectName(QStringLiteral("widget"));

        gridLayout->addWidget(widget, 0, 0, 1, 1);


        retranslateUi(SX_3D_Client);

        QMetaObject::connectSlotsByName(SX_3D_Client);
    } // setupUi

    void retranslateUi(QWidget *SX_3D_Client)
    {
        SX_3D_Client->setWindowTitle(QApplication::translate("SX_3D_Client", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class SX_3D_Client: public Ui_SX_3D_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SX_3D_CLIENT_H
