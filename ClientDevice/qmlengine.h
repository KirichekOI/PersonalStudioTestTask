#ifndef QMLENGINE_H
#define QMLENGINE_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

class QmlEngine : public QObject
{
    Q_OBJECT
public:
    explicit QmlEngine(QObject *parent = nullptr);

    const QQmlApplicationEngine &getEngine() const { return engine; }

signals:

public slots:

private:
    QQmlApplicationEngine engine;
};

#endif // QMLENGINE_H
