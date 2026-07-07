#include "control/logic/PlayerController.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include <QIcon>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/image/assets/images/appIcon.png"));

    QQmlApplicationEngine engine;

    PlayerController controller;

    // Make controller become content property
    engine.rootContext()->setContextProperty("playerController", &controller);

    // Load UI
    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();

}