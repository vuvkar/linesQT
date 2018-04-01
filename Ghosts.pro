#-------------------------------------------------
#
# Project created by QtCreator 2018-03-07T23:35:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ghosts
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    Ghost.cpp \
    GameLogic.cpp \
    cell.cpp \
    startscreen.cpp

HEADERS += \
        mainwindow.h \
    Definitions.h \
    Ghost.h \
    GameLogic.h \
    cell.h \
    startscreen.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    Assets/blocker.png \
    Assets/button_highscore.png \
    Assets/button_home.png \
    Assets/button_pause.png \
    Assets/button_play.png \
    Assets/cell_dark.png \
    Assets/cell_light.png \
    Assets/ghost_green.png \
    Assets/ghost_red.png \
    Assets/ghost_white.png \
    Assets/ghost_yellow.png \
    Assets/icon_path.png \
    Assets/panel_objectives.png \
    Assets/preview_1.png \
    Assets/restart_button.png \
    Assets/splash.png

RESOURCES += \
    assets.qrc

win32:RC_ICONS += Assets\icon.ico
