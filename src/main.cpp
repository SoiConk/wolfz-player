#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/Blueberry_Wolfz/src/ui/assets/images/appIcon.png"));

    QQmlApplicationEngine engine;

    // Load UI
    engine.load(QUrl(QStringLiteral("qrc:/Blueberry_Wolfz/src/ui/Main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();

}