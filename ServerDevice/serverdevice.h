#ifndef SERVERDEVICE_H
#define SERVERDEVICE_H

#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <pthread.h>
#include <mutex>

#include "serverdevicelogger.h"

#define BUFSIZE 1024
#define BACKLOG 10

using SA = struct sockaddr_un;

class ServerDevice
{
public:
    explicit ServerDevice();
    ~ServerDevice();

    bool start();
    void stop();

private:
    static void *handleConnectionStatic(void *p);
    void handleConnection();

private:
    std::string svSocketPath{"/tmp/ServerDevice"};
    int serverSocket;
    int clientSocket;
    SA serverAddr;
    SA clientAddr;
    bool created{false};
    std::mutex mutex;

    ServerDeviceLogger *logger;
};

#endif // SERVERDEVICE_H
