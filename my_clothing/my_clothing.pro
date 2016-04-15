#-------------------------------------------------
#
# Project created by QtCreator 2016-01-16T16:42:24
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = my_flag
TEMPLATE = app


SOURCES += main.cpp\
    sim.cpp \
    window.cpp \
    openglwidget.cpp \
    spring.cpp \
    flag.cpp \
    particle.cpp \
    ball.cpp \
    camera.cpp \
    ground.cpp \
    person.cpp \
    limb.cpp

HEADERS  += \
    sim.h \
    foundation.h \
    window.h \
    ui_MainWindow.h \
    openglwidget.h \
    spring.h \
    flag.h \
    particle.h \
    ball.h \
    camera.h \
    ground.h \
    person.h \
    limb.h

FORMS    += \
    MainWindow.ui

# Libraries
win32: LIBS += -lglu32
win32: LIBS += -lopengl32
win32: LIBS += -lglut

DISTFILES += \
    ../grass.bmp
