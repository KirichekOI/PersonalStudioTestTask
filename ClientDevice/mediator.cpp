#include "mediator.h"

Mediator::Mediator(QObject *parent) : QObject(parent)
{
    connectObjcts();
    setupObjcts();
}

void Mediator::show()
{

}

void Mediator::setupObjcts()
{
    clientSocket.setServerName("ServerDevice");
    clientSocket.initStructs();
}

void Mediator::connectObjcts()
{
    auto context = qmlEngine.getEngine().rootContext();
    context->setContextProperty("clientSocket", &clientSocket);
}
