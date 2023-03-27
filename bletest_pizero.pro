QT -= gui
QT += bluetooth network

CONFIG += c++17 console
CONFIG -= app_bundle

DEFINES += SOURCE_PATH=$$PWD
DEFINES += TARGI=$$TARGET

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        bi/ble/bleapi.cpp \
        bi/ble/bleserver.cpp \
        bi/buildnumber.cpp \
        bi/dowork.cpp \
        bi/instance.cpp \
        bi/hwinfo.cpp \
        bi/mcpreader.cpp \
        global.cpp \
        helpers/commandlineparserhelper.cpp \
        helpers/filenamehelper.cpp \
        helpers/wiringpihelper.cpp \
        helpers/logger.cpp \
        helpers/networkhelper.cpp \
        helpers/shortguid.cpp \
        helpers/signalhelper.cpp \
        helpers/textfilehelper.cpp \
        main.cpp

HEADERS += \
    bi/ble/bleapi.h \
    bi/ble/bleserver.h \
    bi/buildnumber.h \
    bi/dowork.h \
    bi/instance.h \
    bi/hwinfo.h \
    bi/mcpreader.h \
    global.h \
    helpers/commandlineparserhelper.h \
    helpers/filenamehelper.h \
    helpers/wiringpihelper.h \
    helpers/logger.h \
    helpers/nameof.h \
    helpers/networkhelper.h \
    helpers/shortguid.h \
    helpers/signalhelper.h \
    helpers/stringify.h \
    helpers/textfilehelper.h

DISTFILES +=

contains(QMAKESPEC,.*linux-rasp-pi\d*-.*){
    message(rpi detected)
    CONFIG += rpi
    DEFINES += RPI
}

unix:rpi:
{
    # egyes
    #LIBS += -L/home/anti/raspi/sysroot/usr/lib -lraspicam -lraspicam_cv
    #LIBS += -L/home/anti/raspi/sysroot/usr/lib/lib -lopencv_dnn -lopencv_gapi -lopencv_highgui -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_video -lopencv_videoio -lopencv_imgcodecs -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_imgproc -lopencv_core
    # kettes
    #LIBS += -L/home/anti/pizero/sysroot/usr/lib/arm-linux-gnueabihf/ -lusb-1.0
    #LIBS += -L/home/anti/pizero/sysroot/opt/vc/lib/ -lmmal -lmmal_core -lmmal_util -lmmal_vc_client -lmmal_components -lvchiq_arm -lvcsm -lcontainers -lvcos -lbcm_host

    LIBS += -L/home/zoli/pizero_bullseye/sysroot/usr/lib/arm-linux-gnueabihf
    LIBS += -L/usr/local/lib -lwiringPi
    message(LIBS added for raspberry_pi)
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:rpi: target.path = /home/pi/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


