#include <algorithm>
#include <sstream>
#include <iterator>

#include "cmdparser.h"

CmdParser::CmdParser()
{
    logger = ServerDeviceLogger::getInstance();
    fillCmdTypeMap();
}

CmdParser::~CmdParser()
{

}

bool CmdParser::checkCmd(const std::string &cmd, std::string &response)
{
    auto cmdVector = stringParse(cmd, spaceDelim);
    for (auto &str: cmdVector) {
        if (str.back() == commaDelim.front())
            str.pop_back();
    }

    auto it = cmdTypeMap.find(cmdVector.front());
    if (it == cmdTypeMap.end())
        return false;

    cmdVector.erase(cmdVector.begin());
    if ( !cmdHandler.createResponse(it->second, cmdVector, response) )
        return false;

    return true;
}

void CmdParser::fillCmdTypeMap()
{
    int index{0};
    for (auto &str: cmdTypeVector) {
        cmdTypeMap.emplace(str, static_cast<CMD_TYPE>(index));
        ++index;
    }
}

StringVector CmdParser::stringParse(const std::string &cmd, const std::string &delimiter)
{
    size_t pos = 0;
    std::string token;
    StringVector result;
    std::string str(cmd);
    while ((pos = str.find(delimiter)) != std::string::npos) {
        result.emplace_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    result.emplace_back(str);
    return result;
}
