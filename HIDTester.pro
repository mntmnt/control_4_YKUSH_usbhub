QT += quick quickcontrols2
# CONFIG+=qml_debug
# CONFIG += console

# On older versions there is a bug on Linux+Wayland: a hidden window doesn't re-appear on further show() call
linux {
    !versionAtLeast(QT_VERSION, 6.11.1) {
        error("On Linux, Qt 6.11.1 or newer is required. Current Qt version: $$QT_VERSION")
    }
}


TARGET = YepkitUSBSwitch
VERSION = 0.5
CONFIG += c++20  \
          warn_on \
          thread

# Enable/Disable system tray support. Comment it to disable
CONFIG += system_tray

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
        qml/ErrorPage.qml \
        qml/AnimatedProgressBorder.qml \
        qml/PortControlsArea.qml

contains(CONFIG, system_tray) {
    QT += widgets
    DEFINES += MY_SYSTEM_TRAY_SUPPORT
    resources.files += qml/SystemTray.qml \
        UsbIcon.ico
}

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
    deploy-data/installRule.sh \
    deploy-data/98-ykshusbhub-udev.rules \
    deploy-data/README_PERMISSION.md

QMAKE_TARGET_COMPANY = "Mag-Instruments, Inc"
QMAKE_TARGET_DESCRIPTION = "Mag-Instruments software"
QMAKE_TARGET_COPYRIGHT = "Mag-Instruments © 2025"
QMAKE_TARGET_PRODUCT = "Yepkit USB Switch Control"
RC_ICONS += UsbIcon.ico

