#-------------------------------------------------
#
# Project created by QtCreator 2018-02-16T21:33:55
#
#-------------------------------------------------

QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Project3
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS DEBUG

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -L/usr/lib/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu/qca-qt5/crypto -lqca-qt5
INCLUDEPATH += src/

SOURCES += \
        external/csprng.cpp \
        external/crc32.cpp \
        src/Main.cpp \
        src/MainWindow.cpp \
        src/Utils.cpp \
    src/screens/Screen.cpp \
    src/screens/ScreenDebug.cpp \
    src/screens/ScreenMain.cpp \
    src/screens/ScreenPincode.cpp \
    src/screens/ScreenOptions.cpp \
    src/screens/ScreenThanks.cpp \
    src/screens/ScreenBalance.cpp \
    src/screens/ScreenWithdrawSelection.cpp \
    src/screens/ScreenWithdrawConfirm.cpp \
    src/screens/ScreenWithdrawCustom.cpp \
    src/Arduino.cpp \
    src/Session.cpp \
    src/Database.cpp

HEADERS += \
        external/csprng.hpp \
        external/crc32.h \
        src/MainWindow.hpp \
        src/Utils.hpp \
    src/screens/Screen.hpp \
    src/screens/ScreenDebug.hpp \
    src/screens/ScreenMain.hpp \
    src/screens/ScreenPincode.hpp \
    src/screens/ScreenOptions.hpp \
    src/screens/ScreenThanks.hpp \
    src/screens/ScreenBalance.hpp \
    src/screens/ScreenWithdrawSelection.hpp \
    src/screens/ScreenWithdrawConfirm.hpp \
    src/screens/ScreenWithdrawCustom.hpp \
    src/Arduino.hpp \
    src/Session.hpp \
    src/Database.hpp

FORMS += \
        ui/MainWindow.ui \
    ui/ScreenDebug.ui \
    ui/ScreenMain.ui \
    ui/ScreenPincode.ui \
    ui/ScreenOptions.ui \
    ui/ScreenThanks.ui \
    ui/ScreenBalance.ui \
    ui/ScreenWithdrawSelection.ui \
    ui/ScreenWithdrawConfirm.ui \
    ui/ScreenWithdrawCustom.ui
