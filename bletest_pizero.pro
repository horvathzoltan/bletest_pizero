QT -= gui
QT += bluetooth

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        bleapi.cpp \
        bleserver.cpp \
        dowork.cpp \
        main.cpp

LIBS += -L/home/zoli/pizero_bullseye/sysroot/usr/lib/arm-linux-gnueabihf

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    bleapi.h \
    bleserver.h \
    dowork.h
