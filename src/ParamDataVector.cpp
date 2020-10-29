#include "tralics/ParamDataVector.h"

ParamDataVector::ParamDataVector() {
    push_back(new ParamDataList("ur"));
    push_back(new ParamDataList("sections"));
    push_back(new ParamDataList("profession"));
    push_back(new ParamDataList("affiliation"));
}

auto ParamDataVector::find_list(const std::string &name, bool creat) -> ParamDataList * {
    for (size_t i = 0; i < size(); i++) // \todo range based
        if (at(i)->name == name) return at(i);
    if (!creat) return nullptr;
    auto *res = new ParamDataList(name);
    push_back(res);
    return res;
}
