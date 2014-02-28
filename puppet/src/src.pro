######################################################################
# Automatically generated by qmake (2.01a) Thu Apr 16 21:18:10 2009
######################################################################

TEMPLATE = app
TARGET = 


# Input
HEADERS += algebra.h \
           glwidget.h \
           lua488.h \
           mainwindow.h \
           material.h \
           primitive.h \
           scene.h \
           scene_lua.h \
    trackball.h
SOURCES += algebra.cpp \
           glwidget.cpp \
           main.cpp \
           mainwindow.cpp \
           material.cpp \
           primitive.cpp \
           scene.cpp \
           scene_lua.cpp \
    trackball.cpp
		   
QT += opengl




win32: LIBS += -L$$PWD/include/ -llua51

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32: PRE_TARGETDEPS += $$PWD/include/lua51.lib



unix|win32: LIBS += -LGL/-lglut32
