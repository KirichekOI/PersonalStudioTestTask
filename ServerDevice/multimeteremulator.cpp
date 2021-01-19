#include <algorithm>
#include <random>
#include <chrono>
#include "multimeteremulator.h"

MultimeterEmulator* MultimeterEmulator::multimeter{nullptr};
std::mutex MultimeterEmulator::mutex;
MultimeterEmulatorDestroyer MultimeterEmulator::destroyer;
std::vector<std::string> MultimeterEmulator::rangeNameVector;
std::map<std::string, std::pair<float, float>> MultimeterEmulator::rangeMap;
std::vector<std::string> MultimeterEmulator::channelNameVector;
std::map<std::string, STATE> MultimeterEmulator::stateNameMap;
std::map<std::string, std::pair<std::string, STATE>> MultimeterEmulator::channelsCurrentStateMap;
std::map<std::string, float> MultimeterEmulator::channelsResultMap;
ServerDeviceLogger* MultimeterEmulator::logger{nullptr};

MultimeterEmulatorDestroyer::~MultimeterEmulatorDestroyer()
{
    delete multimeterEmulatorPtr;
}

void MultimeterEmulatorDestroyer::init(MultimeterEmulator *_multimeterEmulatorPtr)
{
    multimeterEmulatorPtr = _multimeterEmulatorPtr;
}

MultimeterEmulator *MultimeterEmulator::getInstance()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (multimeter == nullptr) {
        multimeter = new MultimeterEmulator();
        logger = ServerDeviceLogger::getInstance();
        destroyer.init(multimeter);
    }
    initStructs();
    return multimeter;
}

void MultimeterEmulator::startMeasure(const std::string &channelName, std::string &response)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = channelsCurrentStateMap.find(channelName);
    if (it == channelsCurrentStateMap.end()) {
        response = responseVector.front();
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName);
        return;
    }

    if (isErrorState(response)) {
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName
                         + " generated random error state");
        return;
    }

    if (it->second.second == STATE::ERROR_STATE || it->second.second == STATE::BUSY_STATE || it->second.second == STATE::MEASURE_STATE) {
        response = responseVector.front();
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName +
                        "; current state = " + getStateName(it->second.second));
        return;
    }

    it->second.second = STATE::MEASURE_STATE;
    response = responseVector.back();
    logger->logInfo(LOG_TYPE::LOG_INFO, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName +
                    "; current state = " + getStateName(it->second.second));
}

void MultimeterEmulator::setRange(const std::string &channelName, const std::string &rangeName, std::string &response)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = channelsCurrentStateMap.find(channelName);
    if (it == channelsCurrentStateMap.end()) {
        response = responseVector.front() + ", " + rangeName;
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName
                         + " rangeName = " + rangeName);
        return;
    }

    if (isErrorState(response)) {
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName
                         + " generated random error state");
        return;
    }

    if (it->second.second == STATE::ERROR_STATE || it->second.second == STATE::BUSY_STATE || it->second.second == STATE::MEASURE_STATE) {
        response = responseVector.front() + ", " + rangeName;
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName +
                        "; current state = " + getStateName(it->second.second));
        return;
    }

    auto rangeIt = std::find(rangeNameVector.begin(), rangeNameVector.end(), rangeName);
    if (rangeIt == rangeNameVector.end()) {
        response = responseVector.front() + ", " + rangeName;
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName);
        return;
    }

    it->second.first = rangeName;
    response = responseVector.back() + ", " + rangeName;
    logger->logInfo(LOG_TYPE::LOG_INFO, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName);

}

void MultimeterEmulator::stopMeasure(const std::string &channelName, std::string &response)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = channelsCurrentStateMap.find(channelName);
    if (it == channelsCurrentStateMap.end()) {
        response = responseVector.front();
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName);
        return;
    }

    if (isErrorState(response)) {
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName
                         + " generated random error state");
        return;
    }

    if (it->second.second == STATE::ERROR_STATE || it->second.second == STATE::BUSY_STATE || it->second.second == STATE::IDLE_STATE) {
        response = responseVector.front();
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName +
                        "; current state = " + getStateName(it->second.second));
        return;
    }

    it->second.second = STATE::IDLE_STATE;
    response = responseVector.back();
    logger->logInfo(LOG_TYPE::LOG_INFO, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName +
                    "; current state = " + getStateName(it->second.second));
}

void MultimeterEmulator::getStatus(const std::string &channelName, std::string &response)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = channelsCurrentStateMap.find(channelName);
    if (it == channelsCurrentStateMap.end()) {
        response = responseVector.front();
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName);
        return;
    }

    if (isErrorState(response)) {
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName
                         + " generated random error state");
        return;
    }

    std::string state = getStateName(it->second.second);
    response = responseVector.back() + ", " + state;
    logger->logInfo(LOG_TYPE::LOG_INFO, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName +
                    "; current state = " + getStateName(it->second.second));
}

void MultimeterEmulator::getResult(const std::string &channelName, std::string &response)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = channelsCurrentStateMap.find(channelName);
    if (it == channelsCurrentStateMap.end()) {
        response = responseVector.front();
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName);
        return;
    }

    auto result = generateRndState();
    if (result == STATE::ERROR_STATE || result == STATE::BUSY_STATE) {
        std::string state;
        for (auto &name: stateNameMap) {
            if (name.second == result) {
                state = name.first;
                break;
            }
        }
        response = responseVector.front() + ", " + state;
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName
                         + " generated random state = " + state);
        return;
    }

    if (it->second.second != STATE::MEASURE_STATE) {
        response = responseVector.front();
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName
                         + " current state = " + getStateName(it->second.second));
        return;
    }

    std::string resultValue;
    auto ok = generateRndResult(it->second.first, resultValue);
    if (!ok) {
        response = responseVector.front();
        logger->logInfo(LOG_TYPE::LOG_ERROR, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName
                         + " error generating random value");
        return;
    }
    response = responseVector.back() + ", " + resultValue;
    logger->logInfo(LOG_TYPE::LOG_INFO, std::string(__FUNCTION__) + " response = " + response + " channelName = " + channelName);

}

void MultimeterEmulator::initStructs()
{
    auto factor{10000.0};
    auto minValue{0.0000001};
    auto maxValue{0.0};
    auto indx{0};
    for (auto i = 0; i < RANGE_CNT; ++i) {
        rangeNameVector.emplace_back("range" + std::to_string(i));
        maxValue = minValue*factor;
        rangeMap.emplace(rangeNameVector.back(), std::make_pair(minValue, maxValue));
        minValue = maxValue;
        if (indx == 0)
            factor /= 10;
        ++indx;
    }

    for (auto i = 0; i < CHANNEL_CNT; ++i)
        channelNameVector.emplace_back("channel" + std::to_string(i));

    stateNameMap.emplace("error_state", STATE::ERROR_STATE);
    stateNameMap.emplace("idle_state", STATE::IDLE_STATE);
    stateNameMap.emplace("measure_state", STATE::MEASURE_STATE);
    stateNameMap.emplace("busy_state", STATE::BUSY_STATE);

    //set range0, state = idle_state for all channels
    for (auto &channel: channelNameVector)
        channelsCurrentStateMap.emplace(channel, std::make_pair(rangeNameVector.front(), STATE::IDLE_STATE));

    for (auto &channel: channelNameVector)
        channelsResultMap.emplace(channel, minValue);

}

STATE MultimeterEmulator::generateRndState()
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto rng = std::default_random_engine{seed};
    auto result = static_cast<int>(STATE::ERROR_STATE) + rng()%(static_cast<int>(STATE::BUSY_STATE) - static_cast<int>(STATE::ERROR_STATE) + 1);
    return static_cast<STATE>(result);
}

bool MultimeterEmulator::isErrorState(std::string &response)
{
    auto result = generateRndState();
    if (result == STATE::ERROR_STATE) {
        std::string state;
        for (auto &name: stateNameMap) {
            if (name.second == result) {
                state = name.first;
                break;
            }
        }
        response = responseVector.front() + ", " + state;
        return true;
    }
    return false;
}

bool MultimeterEmulator::generateRndResult(const std::string &rangeName, std::string &result)
{
    auto it = rangeMap.find(rangeName);
    if (it == rangeMap.end())
        return false;
    auto min = it->second.first;
    auto max = it->second.second - min;
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(min, max);
    result = std::to_string(distr(eng));
    return true;
}

std::string MultimeterEmulator::getStateName(const STATE &state)
{
    std::string stateStr;
    for (auto &name: stateNameMap) {
        if (name.second == state) {
            stateStr = name.first;
            break;
        }
    }
    return  stateStr;
}
