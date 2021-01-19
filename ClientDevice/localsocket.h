#ifndef LOCALSOCKET_H
#define LOCALSOCKET_H

#include <QObject>
#include <QLocalSocket>
#include <QList>
#include <QVariant>

namespace CLIENT_CONSTANTS {
constexpr int CHANNEL_CNT{10};
constexpr int RANGE_CNT{4};
}

class LocalSocket : public QObject
{
    Q_OBJECT
public:
    explicit LocalSocket(QObject *parent = nullptr);
    void setServerName(const QString &_serverName);
    void initStructs();

    QStringList getCmdTypes() {return cmdType;}

signals:
    void printInfoMsg(const QString &msg);
    void initQmlModels(const QStringList &cmdType,
                       const QStringList &channelNameList,
                       const QStringList &rangeNameList);

public slots: 
    void connectToServer();
    void slotSendToServer(const QString &cmd, const QStringList &params);

private slots:
    void slotReadyRead();
    void slotError(QLocalSocket::LocalSocketError error);
    void slotConnected();

private:
    QLocalSocket localSocket;
    QString serverName;
    QStringList cmdType{"start_measure", "set_range",
                        "stop_measure", "get_status", "get_result"};
    QStringList channelNameList;
    QStringList rangeNameList;
};

#endif // LOCALSOCKET_H
