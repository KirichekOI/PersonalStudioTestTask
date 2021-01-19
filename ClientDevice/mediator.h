#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QObject>
#include "qmlengine.h"
#include "localsocket.h"

class Mediator : public QObject
{
    Q_OBJECT
public:
    explicit Mediator(QObject *parent = nullptr);


    void show();
signals:

public slots:

private:
    void setupObjcts();
    void connectObjcts();

private:
    QmlEngine qmlEngine;
    LocalSocket clientSocket;
};

#endif // MEDIATOR_H
