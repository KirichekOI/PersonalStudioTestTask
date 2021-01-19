#ifndef MULTIMETERCONSTANTS_H
#define MULTIMETERCONSTANTS_H

#include <vector>
#include <string>

constexpr int CHANNEL_CNT{10};
constexpr int RANGE_CNT{4};
constexpr int CHANNEL_RESULT_POINTS_CNT{15};
const std::vector<std::string> responseVector{"fail", "ok"};

#endif // MULTIMETERCONSTANTS_H
