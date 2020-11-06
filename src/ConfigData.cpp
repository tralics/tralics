#include "tralics/ConfigData.h"

auto ConfigData::find_list(const std::string &name, bool creat) -> ParamDataList * {
    if (auto it = find(name); it != end()) return &it->second;
    if (!creat) return nullptr;
    return &(*this)[name];
}
