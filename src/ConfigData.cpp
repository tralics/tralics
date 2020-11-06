#include "tralics/ConfigData.h"

auto ConfigData::find_list(const std::string &name, bool creat) -> ParamDataList * {
    for (auto &p : *this)
        if (p.name == name) return &p;
    if (!creat) return nullptr;
    emplace_back(name);
    return &back();
}
