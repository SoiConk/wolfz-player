#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include "control/service/MetadataManager.h"
#include "data/playlist/Queue.h"
#include "data/playlist/History.h"
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Basic");

    app.setWindowIcon(QIcon(":/Blueberry_Wolfz/src/ui/assets/images/appIcon.png"));

    MetadataManager& instance = MetadataManager::getInstance();
    History::getInstance().addList(instance.getHistory());
    Queue::getInstance().addList(instance.getQueue());

    QQmlApplicationEngine engine;

    qDebug() << "Import paths:" << engine.importPathList();

    engine.loadFromModule("Blueberry_Wolfz", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}