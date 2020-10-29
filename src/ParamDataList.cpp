#include "tralics/ParamDataList.h"

// We may add a special slot at the end
void ParamDataList::check_other() {
    if (!empty() && (std::islower(name[0]) != 0)) push_back({"Other", "Other"});
}

void ParamDataList::keys_to_buffer(std::string &B) const {
    for (const auto &i : *this) B += " " + i.key;
}
