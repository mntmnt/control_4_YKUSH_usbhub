# Force Switch to static version
STATIC_HIDAPI=static

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
    SOURCES += hidapi/linux/hid.c
    LIBS += -ludev -lrt
}
