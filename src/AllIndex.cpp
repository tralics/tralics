#include "tralics/AllIndex.h"
#include "tralics/MainClass.h"
#include "tralics/Stack.h"

// By default, this is a glossary and a main index
AllIndex::AllIndex() {
    emplace_back("glossary", "Glossary", 6);
    emplace_back("default", "Index", 5);
}

// Returns the index location associated to the name S
// If S is not found, the main index is used
auto AllIndex::find_index(const std::string &s) -> OneIndex & {
    for (size_t i = 0; i < size(); i++)
        if (at(i).name == s) return at(i);
    return at(1);
}

void AllIndex::new_index(const std::string &s, const std::string &title) {
    for (size_t i = 0; i < size(); i++)
        if (at(i).name == s) return;
    auto id = the_stack.next_xid(nullptr).value;
    emplace_back(s, title, id);
}

// Case \printglossary or \printindex[foo].
// Marks the place where to insert the index
void AllIndex::mark_print(OneIndex &g) {
    Xml *mark = new Xml(std::string(), nullptr);
    Xml *Foo  = new Xml(std::string(), mark);
    the_stack.add_last(Foo);
    g.position = mark;
}
