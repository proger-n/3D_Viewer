QT       += openglwidgets core gui

qmake
TARGET = ../3DViwerV1.0

pro
contains(CONFIG, windows) {
   LIBS += -lopengl32
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(QtGifImage-master/src/gifimage/qtgifimage.pri)
SOURCES += \
    ../backend/parsing.c \
    ../backend/transformations.c \
    main.cpp \
    mainwindow.cpp \
    rendering.cpp

HEADERS += \
    ../backend/parsing.h \
    ../backend/transformations.h \
    mainwindow.h \
    rendering.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
