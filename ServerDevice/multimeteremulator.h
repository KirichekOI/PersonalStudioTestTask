#ifndef MULTIMETEREMULATOR_H
#define MULTIMETEREMULATOR_H

#include <string>
#include <vector>
#include <map>
#include <mutex>

#include "multimeterconstants.h"
#include "serverdevicelogger.h"

enum class STATE {
    ERROR_STATE,
    IDLE_STATE,
    MEASURE_STATE,
    BUSY_STATE
};

class MultimeterEmulator;

class MultimeterEmulatorDestroyer
{
public:
    MultimeterEmulatorDestroyer() = default;
    ~MultimeterEmulatorDestroyer();

    void init(MultimeterEmulator *_multimeterEmulatorPtr);

private:
    MultimeterEmulator *multimeterEmulatorPtr;
};

class MultimeterEmulator
{
    friend class MultimeterEmulatorDestroyer;
public:
    MultimeterEmulator(MultimeterEmulator &other) = delete;
    MultimeterEmulator& operator=(MultimeterEmulator &other) = delete;

    static MultimeterEmulator *getInstance();

    void startMeasure(const std::string &channelName, std::string &response);
    void setRange(const std::string &channelName, const std::string &rangeName, std::string &response);
    void stopMeasure(const std::string &channelName, std::string &response);
    void getStatus(const std::string &channelName, std::string &response);
    void getResult(const std::string &channelName, std::string &response);

private:
    MultimeterEmulator() = default;
    ~MultimeterEmulator() = default;

    inline static void initStructs();

    inline STATE generateRndState();
    inline bool isErrorState(std::string &response);

    inline bool generateRndResult(const std::string &rangeName, std::string &result);
    inline std::string getStateName(const STATE &state);

private:
    static MultimeterEmulator *multimeter;
    static std::mutex mutex;
    static MultimeterEmulatorDestroyer destroyer;

    static std::vector<std::string> rangeNameVector;
    static std::map<std::string, std::pair<float, float>> rangeMap;
    static std::vector<std::string> channelNameVector;
    static std::map<std::string, STATE> stateNameMap;
    //map key = channel name, value = range, state
    static std::map<std::string, std::pair<std::string, STATE>> channelsCurrentStateMap;
    //map key = channel name, value = rnd float
    static std::map<std::string, float> channelsResultMap;

    static ServerDeviceLogger *logger;
};

#endif // MULTIMETEREMULATOR_H
