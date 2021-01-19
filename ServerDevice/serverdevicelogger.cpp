#include "serverdevicelogger.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>

ServerDeviceLogger *ServerDeviceLogger::logger{nullptr};
std::mutex ServerDeviceLogger::mutex;
std::string ServerDeviceLogger::currentLogFile{};
ServerDeviceLoggerDestroyer ServerDeviceLogger::destroyer;

ServerDeviceLogger *ServerDeviceLogger::getInstance()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (logger == nullptr){
        logger = new ServerDeviceLogger();
        destroyer.init(logger);
    }
    setLogFile();
    return logger;
}

void ServerDeviceLogger::logInfo(const LOG_TYPE &logType, const std::string &info)
{
    std::lock_guard<std::mutex> lock(mutex);
    std::ofstream fs(currentLogFile, std::ios_base::app);
    if (!fs.good()) {
        std::cerr <<"ERROR: Cannot open log file: " << currentLogFile <<std::endl;
        return;
    }
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S")
        << "-->"
        << LOG_TYPE_NAMES[static_cast<int>(logType)]
        << "-->"
        << info
        << std::endl;
    fs << oss.str();
    std::cout << oss.str();
    std::cout.flush();
    fs.flush();
    fs.close();
}

void ServerDeviceLogger::setLogFile()
{
    currentLogFile = setLogFileName();
    std::ofstream fs(currentLogFile, std::ios_base::app);
    if (!fs.good())
        std::cerr << "ERROR: Cannot open log file: " << currentLogFile <<std::endl;
}

std::string ServerDeviceLogger::setLogFileName()
{
    std::string fileName;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d%m%Y");
    fileName = oss.str();
    return fileName.append(".log");
}

ServerDeviceLoggerDestroyer::~ServerDeviceLoggerDestroyer()
{
    delete loggerPtr;
}

void ServerDeviceLoggerDestroyer::init(ServerDeviceLogger *_loggerPtr)
{
    loggerPtr = _loggerPtr;
}
