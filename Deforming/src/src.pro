#-------------------------------------------------
#
# Project created by QtCreator 2012-07-04T13:34:23
#
#-------------------------------------------------

QT       += core gui

TARGET = src
TEMPLATE = app
DEPENDPATH += .



SOURCES += main.cpp\
        mainwindow.cpp \
    primitive.cpp \
    material.cpp \
    scene_lua.cpp \
    mesh.cpp \
    algebra.cpp \
    scene.cpp \
    glwidget.cpp \
    ray.cpp \
    chainMail.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    lua488.h \
    scene_lua.h \
    material.h \
    primitive.h \
    mesh.h \
    scene.h \
    algebra.h \
    ray.h \
    chainMail.h

QT += opengl

unix:!macx:!symbian: LIBS += -L$$PWD/sw/include/ -lglut

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/sw/include/libglut.a

unix:!macx:!symbian: LIBS += -L$$PWD/sw/include/ -lGLU

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/sw/include/libGLU.a





unix:!macx:!symbian: LIBS += -L$$PWD/sw/include/ -llua5.1

INCLUDEPATH += $$PWD/sw/include
DEPENDPATH += $$PWD/sw/include
