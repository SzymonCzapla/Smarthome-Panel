QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ac.cpp \
    actuator.cpp \
    device.cpp \
    devicewidget.cpp \
    heater.cpp \
    hvac.cpp \
    hvacwidget.cpp \
    lamp.cpp \
    lampwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    networkhandler.cpp \
    newdevicewindow.cpp \
    rgblamp.cpp \
    rgblampwidget.cpp \
    sensor.cpp \
    thermostat.cpp \
    weatherstation.cpp

HEADERS += \
    ac.h \
    actuator.h \
    device.h \
    devicewidget.h \
    heater.h \
    hvac.h \
    hvacwidget.h \
    lamp.h \
    lampwidget.h \
    mainwindow.h \
    networkhandler.h \
    newdevicewindow.h \
    rgblamp.h \
    rgblampwidget.h \
    sensor.h \
    thermostat.h \
    weatherstation.h

FORMS += \
    RGBLampWidget.ui \
    hvacwidget.ui \
    lampwidget.ui \
    mainwindow.ui \
    newdevicewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    SmartHome.pro.user

RESOURCES += \
    resources.qrc
