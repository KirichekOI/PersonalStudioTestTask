#include "cmdhandler.h"

CmdHandler::CmdHandler()
{
    multimeterEmulator = MultimeterEmulator::getInstance();
}

CmdHandler::~CmdHandler()
{

}

bool CmdHandler::createResponse(const CMD_TYPE &cmdType, const std::vector<std::string> &params, std::string &response)
{
    bool result{false};
    switch (cmdType) {
    case CMD_TYPE::START_MEASURE : {
        multimeterEmulator->startMeasure(params.front(), response);
    }
        break;
    case CMD_TYPE::SET_RANGE : {
        multimeterEmulator->setRange(params.front(), params.back(), response);
    }
        break;
    case CMD_TYPE::STOP_MEASURE : {
        multimeterEmulator->stopMeasure(params.front(), response);
    }
        break;
    case CMD_TYPE::GET_STATUS : {
        multimeterEmulator->getStatus(params.front(), response);
    }
        break;
    case CMD_TYPE::GET_RESULT : {
        multimeterEmulator->getResult(params.front(), response);
    }
        break;
    }
    return result;
}
