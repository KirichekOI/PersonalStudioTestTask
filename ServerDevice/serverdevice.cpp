#include <iostream>

#include "serverdevice.h"
#include "cmdparser.h"

ServerDevice::ServerDevice()
{
    logger = ServerDeviceLogger::getInstance();
    serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        logger->logInfo(LOG_TYPE::LOG_ERROR, " error while creating server socket.");
        return;
    }

    if ( svSocketPath.length() > sizeof (serverAddr.sun_path) - 1 ) {
        logger->logInfo(LOG_TYPE::LOG_ERROR, " Server socket path too long: " + svSocketPath);
        return;
    }

    if ( remove(svSocketPath.c_str()) == -1 &&  errno != ENOENT) {
        logger->logInfo(LOG_TYPE::LOG_ERROR, " remove-" + svSocketPath);
        return;
    }

    memset(&serverAddr, 0, sizeof (SA));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, svSocketPath.c_str(), sizeof (serverAddr.sun_path) - 1);

    if (bind(serverSocket, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof (SA)) == -1) {
        logger->logInfo(LOG_TYPE::LOG_ERROR, " error while binding.");
        return;
    }

    if (listen(serverSocket, BACKLOG) == -1) {
        logger->logInfo(LOG_TYPE::LOG_ERROR, " error while listening.");
        return;
    }

    created = true;
}

ServerDevice::~ServerDevice()
{
    stop();
}

bool ServerDevice::start()
{
    if (!created) {
        logger->logInfo(LOG_TYPE::LOG_ERROR, "Server socket not created.");
        return false;
    }
    logger->logInfo(LOG_TYPE::LOG_INFO, "Server started.");

    for(;;) {
        clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == -1) {
            logger->logInfo(LOG_TYPE::LOG_ERROR, " acccept failed.");
            return false;
        }

        pthread_t t;
        pthread_create(&t, nullptr, &ServerDevice::handleConnectionStatic, this);
    }

    return true;
}

void ServerDevice::stop()
{
    logger->logInfo(LOG_TYPE::LOG_INFO, "Server stopped.");
}

void *ServerDevice::handleConnectionStatic(void *p)
{
    static_cast<ServerDevice*>(p)->handleConnection();
    return nullptr;
}

void ServerDevice::handleConnection()
{
    mutex.lock();
    int newClientSocket = clientSocket;
    mutex.unlock();

    ServerDeviceLogger *logger = ServerDeviceLogger::getInstance();
    logger->logInfo(LOG_TYPE::LOG_INFO, " Started client cocket = " + std::to_string(newClientSocket));

    char buffer[BUFSIZE];
    int bytesRead;
    int msgSize{0};
    std::string response;
    CmdParser parser;

    memset(buffer, 0, BUFSIZE);
    while ( (bytesRead = read(newClientSocket, buffer, sizeof(buffer))) > 0 ) {
        logger->logInfo(LOG_TYPE::LOG_INFO, " Received msg from client = " + std::to_string(newClientSocket)
                        + " " + std::string(buffer, bytesRead));
        response.clear();
        parser.checkCmd(std::string(buffer, bytesRead), response);
        write(newClientSocket, response.c_str(), response.size());
//        write(newClientSocket,"I got your message",18);
    }

    if (bytesRead == -1) {
        logger->logInfo(LOG_TYPE::LOG_ERROR, " Error while received msg from client = " + std::to_string(newClientSocket)
                        + " bytesRead = -1");
    }
    else if (bytesRead == 0) {
        logger->logInfo(LOG_TYPE::LOG_INFO, " Close connection with client = " + std::to_string(newClientSocket));
        close(newClientSocket);
        pthread_cancel(pthread_self());
    }
}
