#-------------------------------------------------
#
# Project created by QtCreator 2012-06-12T18:49:11
#
#-------------------------------------------------

QT       += core gui

TARGET = src
TEMPLATE = app
DEPENDPATH += .

SOURCES += main.cpp\
        mainwindow.cpp \
    model.cpp \
    mesh.cpp \
    kdtree.c \
    glwidget.cpp \
    scene.cpp \
    algebra.cpp \
    primitive.cpp \
    material.cpp \
    scene_lua.cpp \
    rrt.cpp \
    chainMail.cpp

HEADERS  += mainwindow.h \
    DEEP/include/SWIFT.h \
    model.h \
    mesh.h \
    kdtree.h \
    glwidget.h \
    scene.h \
    algebra.h \
    lua488.h \
    scene_lua.h \
    material.h \
    primitive.h \
    rrt.h \
    chainMail.h

QT += opengl

unix:!macx:!symbian: LIBS += -L$$PWD/sw/include/ -lglut

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/sw/include/libglut.a

unix:!macx:!symbian: LIBS += -L$$PWD/sw/include/ -lGLU

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/sw/include/libGLU.a


unix:!macx:!symbian: LIBS += -L$$PWD/swift++-1.2/lib/ -lSWIFT++

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/swift++-1.2/lib/libSWIFT++.linux64.a

unix:!macx:!symbian: LIBS += -L$$PWD/qhull/ -lqhullstatic

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/qhull/libqhullstatic.a

INCLUDEPATH += $$PWD/DEEP/include
DEPENDPATH += $$PWD/DEEP/include

unix:!macx:!symbian: LIBS += -L$$PWD/DEEP/ -lDEEP

INCLUDEPATH += $$PWD/DEEP
DEPENDPATH += $$PWD/DEEP

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/DEEP/libDEEP.a


unix:!macx:!symbian: LIBS += -L$$PWD/sw/include/ -llua5.1

INCLUDEPATH += $$PWD/sw/include
DEPENDPATH += $$PWD/sw/include
