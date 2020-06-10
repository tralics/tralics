#pragma once
#include "Bchar.h"

class NameSplitter { // \todo local to BibEntry.cpp
    bchar_type *table;
    Bchar       first_name{};
    Bchar       last_name{};
    Bchar       jr_name{};
    Bchar       main_data{};

public:
    NameSplitter(bchar_type *T) : table(T) {}

    void handle_the_names();
    void handle_one_name(bool ifn, bool iln, int serial);
    auto is_this_other() -> bool;

    static auto want_handle_key(int s, bool last) -> bool;
};
