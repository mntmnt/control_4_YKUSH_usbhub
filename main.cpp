#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>
//#include <QQuickStyle>
#include "usbswitch/usb-upstream-switch.h"


int main(int argc, char *argv[]) {
//    QQuickStyle::setStyle(QStringLiteral("Imagine"));

    QGuiApplication app(argc, argv);

    const QUrl url(u"qrc:/YepkitUSBSwitch/main.qml"_qs);
    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<usbswitch::USBUpstreamSwitch>("com.project.USBSwitch", 1, 0, "USBSwitch", [&app](QQmlEngine * /*engine*/, QJSEngine *) -> QObject * {
        auto encoder = new usbswitch::USBUpstreamSwitch;
        QObject::connect(&app, &QGuiApplication::aboutToQuit,  encoder, &usbswitch::USBUpstreamSwitch::stopService, Qt::DirectConnection);
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
