#ifndef CMDHANDLER_H
#define CMDHANDLER_H

#include "Commands.h"
#include "multimeteremulator.h"

class CmdHandler
{
public:
    explicit CmdHandler();
    ~CmdHandler();

    bool createResponse(const CMD_TYPE &cmdType,
                        const std::vector<std::string> &params,
                        std::string &response);

private:
    MultimeterEmulator *multimeterEmulator;

};

#endif // CMDHANDLER_H
