#-------------------------------------------------
#
# Project created by QtCreator 2017-01-27T18:10:28
#
#-------------------------------------------------

QT       += core gui opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SX_3D_Widget
TEMPLATE = app


SOURCES += \
    SX_3D_Widget.cpp \
    SX_3D_Client.cpp \
    main.cpp

HEADERS  += \
    SX_3D_Widget.h \
    SX_3D_Client.h \
    SX_Model.hpp \
    SX_Camera.hpp \
    SX_Mesh.hpp

FORMS    += \
    SX_3D_Client.ui

DISTFILES +=

RESOURCES += \
    resources.qrc

INCLUDEPATH += $$PWD/../../../../opt/LIBRARYES/glm
DEPENDPATH += $$PWD/../../../../opt/LIBRARYES/glm

unix:!macx: LIBS += -lassimp
