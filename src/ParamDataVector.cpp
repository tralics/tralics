#include "tralics/ParamDataVector.h"

ParamDataVector::ParamDataVector() {
    emplace_back("ur");
    emplace_back("sections");
    emplace_back("profession");
    emplace_back("affiliation");
}

auto ParamDataVector::find_list(const std::string &name, bool creat) -> ParamDataList * {
    for (auto &p : *this)
        if (p.name == name) return &p;
    if (!creat) return nullptr;
    emplace_back(name);
    return &back();
}
