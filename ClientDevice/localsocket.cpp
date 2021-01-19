#include "localsocket.h"

LocalSocket::LocalSocket(QObject *parent) :
    QObject(parent)
{
    connect(&localSocket, QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::error),
            this, &LocalSocket::slotError);
    connect(&localSocket, &QLocalSocket::connected, this, &LocalSocket::slotConnected);
    connect(&localSocket, &QLocalSocket::readyRead, this, &LocalSocket::slotReadyRead);
}

void LocalSocket::setServerName(const QString &_serverName)
{
    serverName = _serverName;
    localSocket.setServerName(serverName);
}

void LocalSocket::initStructs()
{
    for (auto i = 0; i < CLIENT_CONSTANTS::CHANNEL_CNT; ++i)
        channelNameList.push_back("channel" + QString::number(i));
    for (auto i = 0; i < CLIENT_CONSTANTS::RANGE_CNT; ++i)
        rangeNameList.push_back("range" + QString::number(i));
    emit initQmlModels(cmdType, channelNameList, rangeNameList);
}

void LocalSocket::connectToServer()
{
    localSocket.disconnectFromServer();
    localSocket.connectToServer();
}

void LocalSocket::slotSendToServer(const QString &cmd, const QStringList &params)
{
    if (!localSocket.isOpen()) {
        printInfoMsg("Device not open, first connect to server.");
        return;
    }
    QString request{cmd};
    for (auto &param: params)
        request += " " + param + ",";
    request.remove(request.size() - 1, 1);
    localSocket.write(QByteArray(request.toStdString().c_str()));
}

void LocalSocket::slotReadyRead()
{
    emit printInfoMsg(localSocket.readAll());
}

void LocalSocket::slotError(QLocalSocket::LocalSocketError error)
{
    QString infoMsg;
    switch (error) {
    case QLocalSocket::ServerNotFoundError:
        infoMsg = tr("The host was not found. Please make sure "
                     "that the server is running and that the "
                     "server name is correct.");
        break;
    case QLocalSocket::ConnectionRefusedError:
        infoMsg = tr("The connection was refused by the peer. "
                     "Make sure the fortune server is running, "
                     "and check that the server name "
                     "is correct.");
        break;
    case QLocalSocket::PeerClosedError:
        break;
    default:
        infoMsg = tr("The following error occurred: %1.")
                .arg(localSocket.errorString());
    }
    emit printInfoMsg(infoMsg);
}

void LocalSocket::slotConnected()
{
    emit printInfoMsg("Received the connected() signal.");
}
