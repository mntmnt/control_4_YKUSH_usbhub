QT += quick quickcontrols2
# CONFIG+=qml_debug
# CONFIG += console
TARGET = YepkitUSBSwitch
VERSION = 0.5
CONFIG += c++20  \
          warn_on \
          thread

SOURCES += \
        usbswitch/deviceconnection.cpp \
        usbswitch/deviceenumerator.cpp \
        usbswitch/connector.cpp \
        usbswitch/interactor.cpp \
        usbswitch/usb-upstream-switch.cpp \
        main.cpp

resources.files = qml/main.qml \
        qml/NoDevicePage.qml \
        qml/DeviceControlPage.qml \
        qml/ErrorPage.qml
resources.prefix = /$${TARGET}
RESOURCES += resources

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    usbswitch/constants.h \
    usbswitch/deviceconnection.h \
    usbswitch/deviceenumerator.h \
    usbswitch/connector.h \
    usbswitch/interactor.h \
    usbswitch/types-.h \
    usbswitch/usb-upstream-switch.h

include(hidapi.pri)

DISTFILES += +\

QMAKE_TARGET_COMPANY = "Mag-Instruments, Inc"
QMAKE_TARGET_DESCRIPTION = "Mag-Instruments software"
QMAKE_TARGET_COPYRIGHT = "Mag-Instruments © 2025"
QMAKE_TARGET_PRODUCT = "Yepkit USB Switch Control"
RC_ICONS += UsbIcon.ico

