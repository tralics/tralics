#include "tralics/LabelInfo.h"
#include <unordered_map>

auto labinfo(const std::string &s) -> LabelInfo * {
    static std::unordered_map<std::string, LabelInfo> LM;
    return &LM[s];
}
