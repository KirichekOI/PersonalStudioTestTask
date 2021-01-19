#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>

enum class CMD_TYPE
{
    START_MEASURE,
    SET_RANGE,
    STOP_MEASURE,
    GET_STATUS,
    GET_RESULT
};

const std::vector<std::string> cmdTypeVector{"start_measure", "set_range",
                                             "stop_measure", "get_status", "get_result"};


#endif // COMMANDS_H
