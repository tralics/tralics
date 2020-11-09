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
auto AllIndex::find_index(const std::string &s) -> size_t {
    for (size_t i = 0; i < size(); i++)
        if (at(i).name == s) return i;
    return 1;
}

void AllIndex::new_index(const std::string &s, const std::string &title) {
    for (size_t i = 0; i < size(); i++)
        if (at(i).name == s) return;
    auto id = the_main->the_main_stack->next_xid(nullptr).value;
    emplace_back(s, title, id);
}

// Case \printglossary or \printindex[foo].
// Marks the place where to insert the index
void AllIndex::mark_print(size_t g) {
    Xml *mark = new Xml(std::string(), nullptr);
    Xml *Foo  = new Xml(std::string(), mark);
    the_main->the_main_stack->add_last(Foo);
    at(g).position = mark;
}
