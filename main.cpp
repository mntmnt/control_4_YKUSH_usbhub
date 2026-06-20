#ifdef MY_SYSTEM_TRAY_SUPPORT
#include <QApplication>
#else
#include <QGuiApplication>
#endif
#include <QQmlApplicationEngine>

#include <QQmlContext>
//#include <QQuickStyle>
#include "usbswitch/usb-upstream-switch.h"


int main(int argc, char *argv[]) {
//    QQuickStyle::setStyle(QStringLiteral("Imagine"));

#ifdef MY_SYSTEM_TRAY_SUPPORT
    QApplication app(argc, argv);
    // Keep running when the main window is hidden to the system tray.
    QApplication::setQuitOnLastWindowClosed(false);
    constexpr bool systemTraySupport = true;
#else
    QGuiApplication app(argc, argv);
    constexpr bool systemTraySupport = false;
#endif

    const QUrl url(u"qrc:/YepkitUSBSwitch/qml/main.qml"_qs);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("systemTraySupport", systemTraySupport);

    qmlRegisterSingletonType<usbswitch::USBUpstreamSwitch>("com.project.USBSwitch", 1, 0, "USBSwitch", [&app](QQmlEngine * /*engine*/, QJSEngine *) -> QObject * {
        auto encoder = new usbswitch::USBUpstreamSwitch;
        QObject::connect(&app, &QCoreApplication::aboutToQuit,  encoder, &usbswitch::USBUpstreamSwitch::stopService, Qt::DirectConnection);
        return encoder;
    });

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url, &engine](QObject *obj, const QUrl &objUrl) {
        if (! obj && url == objUrl ) {
            Q_ASSERT_X(false, "createing root object", "Failed to load QML");
            QCoreApplication::exit(-1);
        } else {
            auto objects = engine.rootObjects();
            Q_ASSERT( objects.size() == 1 );
        }
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
