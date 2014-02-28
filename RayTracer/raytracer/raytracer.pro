#-------------------------------------------------
#
# Project created by QtCreator 2012-05-14T23:10:50
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = raytracer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    algebra.cpp \
    a4.cpp \
    scene_lua.cpp \
    scene.cpp \
    ray.cpp \
    primitive.cpp \
    polyroots.cpp \
    mesh.cpp \
    material.cpp \
    main.cpp \
    light.cpp \
    image.cpp \


HEADERS += \
    algebra.h \
    a4.h \
    scene_lua.h \
    scene.h \
    ray.h \
    primitive.h \
    polyroots.h \
    mesh.h \
    material.h \
    lua488.h \
    light.h \
    image.h \

unix:!macx:!symbian: LIBS += -lpng


unix:!macx:!symbian: LIBS += -L$$PWD/sw/include/ -llua5.1

INCLUDEPATH += $$PWD/sw/include
DEPENDPATH += $$PWD/sw/include

QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp
