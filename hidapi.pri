# Force Switch to static version
STATIC_HIDAPI=static
HIDAPI_OVER_LIBUSB_4_LINUX=yes

# Enable LOG of hidapi
#DEFINES += DEBUG_PRINTF
# Switch to DDK
#DEFINES += HIDAPI_USE_DDK

INCLUDEPATH += "hidapi/hidapi"

win32 {
    SOURCES += hidapi/windows/hid.c
    # #pragma comment (lib, "Setupapi.lib")
    win32:*-g++* {
        LIBS += -lsetupapi
    } else {
        LIBS += /LIB:Setupapi.lib
    }
    # -lhid # This one for DDK. For non-DDK it is not required
}

macx {
    LIBS += -framework IOKit -framework CoreFoundation -framework AppKit
    SOURCES += hidapi/mac/hid.c
}

linux {
    # Unfortunately, it appears that the original combination of VID&PID is in a blacklist
    # so the only way to interact with it using the hidapi library is using libusb backend
    contains(HIDAPI_OVER_LIBUSB_4_LINUX,yes) {
       message("using libusb backend for linux")
       CONFIG += link_pkgconfig
       PKGCONFIG += libusb-1.0
       SOURCES += hidapi/libusb/hid.c
       LIBS += -lusb-1.0
    } else {
        message("using default linux backend")
        SOURCES += hidapi/linux/hid.c
        LIBS += -ludev -lrt
    }
}
