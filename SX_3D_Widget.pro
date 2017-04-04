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
    SX_Camera.hpp \
    SX_Mesh.hpp \
    SX_Model.hpp

FORMS    += \
    SX_3D_Client.ui

DISTFILES +=

RESOURCES += \
    resources.qrc

unix: INCLUDEPATH += $$PWD/../../../../opt/LIBRARYES/glm
unix: DEPENDPATH += $$PWD/../../../../opt/LIBRARYES/glm
unix: INCLUDEPATH += $$PWD/../../../LIBRARYES/Assimp/assimp-3.3.1/include
unix: DEPENDPATH += $$PWD/../../../LIBRARYES/Assimp/assimp-3.3.1/include
unix:!macx: LIBS += -lassimp

win32: INCLUDEPATH += $$PWD/../../../LIBRARYES/glm
win32: INCLUDEPATH += $$PWD/../../../LIBRARYES/Assimp/assimp-3.3.1/include
win32: LIBS += -lopengl32
win32: LIBS += -L$$PWD/../../../LIBRARYES/Assimp/assimp-3.3.1/build/code/ -llibassimp.dll


