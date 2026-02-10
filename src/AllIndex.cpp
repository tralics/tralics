#include "tralics/AllIndex.h"
#include "tralics/MainClass.h"
#include "tralics/Stack.h"

// By default, this is a glossary and a main index
AllIndex::AllIndex() {
    emplace_back("glossary", "Glossary", the_stack.elt_from_id(6));
    emplace_back("default", "Index", the_stack.elt_from_id(5));
}

auto AllIndex::the_index() -> AllIndex & {
    static AllIndex I;
    return I;
}

// Returns the index location associated to the name S
// If S is not found, the main index is used
auto AllIndex::find(const std::string &s) -> OneIndex & {
    auto out = std::find_if(begin(), end(), [&](const auto &i) { return i.name == s; });
    return out != end() ? *out : (*this)[1];
}

void AllIndex::insert(const std::string &s, const std::string &title) {
    if (std::any_of(begin(), end(), [&](const auto &i) { return i.name == s; })) return;
    auto *elt = the_stack.elt_from_id(the_stack.next_xid(nullptr));
    emplace_back(s, title, elt);
}
