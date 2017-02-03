#-------------------------------------------------
#
# Project created by QtCreator 2017-01-27T18:10:28
#
#-------------------------------------------------

QT       += core gui opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SX_3D_Widget
TEMPLATE = app


SOURCES += main.cpp \
    SX_3D_Widget.cpp \
    SX_3D_Client.cpp

HEADERS  += \
    SX_3D_Widget.h \
    SX_3D_Client.h

FORMS    += \
    SX_3D_Client.ui

DISTFILES +=

RESOURCES += \
    resources.qrc

INCLUDEPATH += $$PWD/../../../../opt/LIBRARYES/glm
DEPENDPATH += $$PWD/../../../../opt/LIBRARYES/glm
