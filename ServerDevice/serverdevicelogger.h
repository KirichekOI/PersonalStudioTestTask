#ifndef SERVERDEVICELOGGER_H
#define SERVERDEVICELOGGER_H

#include <string>
#include <mutex>

enum class LOG_TYPE {
    LOG_ERROR,
    LOG_WARRING,
    LOG_INFO
};

class ServerDeviceLogger;

class ServerDeviceLoggerDestroyer
{
public:
    ServerDeviceLoggerDestroyer() = default;
    ~ServerDeviceLoggerDestroyer();
    void init(ServerDeviceLogger *_loggerPtr);

private:
    ServerDeviceLogger *loggerPtr;
};

class ServerDeviceLogger
{
    friend class ServerDeviceLoggerDestroyer;
public:
    ServerDeviceLogger(ServerDeviceLogger &other) = delete;
    ServerDeviceLogger& operator=(const ServerDeviceLogger &other) = delete;

    static ServerDeviceLogger *getInstance();

    void logInfo(const LOG_TYPE &logType, const std::string &info);

private:
    ServerDeviceLogger() = default;
    ~ServerDeviceLogger() = default;

    static void setLogFile();
    static std::string setLogFileName();

private:
    const std::string LOG_TYPE_NAMES[3] = {"[ERROR]:", "[WARRING]:", "[INFO]:"};
    static std::string currentLogFile;
    static ServerDeviceLogger *logger;
    static std::mutex mutex;
    static ServerDeviceLoggerDestroyer destroyer;
};

#endif // SERVERDEVICELOGGER_H
