#ifndef CMDPARSER_H
#define CMDPARSER_H

#include <vector>
#include <map>
#include <string>

#include "Commands.h"
#include "cmdhandler.h"
#include "serverdevicelogger.h"

using StringVector = std::vector<std::string>;

class CmdParser
{
public:
    explicit CmdParser();
    ~CmdParser();

    bool checkCmd(const std::string &cmd, std::string &response);

private:
    inline void fillCmdTypeMap();
    inline StringVector stringParse(const std::string &cmd, const std::string &delimiter);

private:
    CmdHandler cmdHandler;
    const std::string spaceDelim{" "};
    const std::string commaDelim{","};

    ServerDeviceLogger *logger;

    std::map<std::string, CMD_TYPE> cmdTypeMap;
};

#endif // CMDPARSER_H
