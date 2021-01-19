#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "mediator.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    Mediator mediator;
    mediator.show();

    return app.exec();
}
