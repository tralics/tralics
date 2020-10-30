#pragma once
#include "Xml.h"
#include "enums.h"
#include <string>

struct XmlAction {
    std::string match;      // the name of the object to match
    recur_type  what;       // the type of the action to perform
    long        int_val{0}; // number of results, or xid
    Xml *       xml_val{};  // input or output xml value
    std::string string_val; // name of element ot work on

public:
    XmlAction(std::string M, recur_type w) : match(std::move(M)), what(w) {}
    XmlAction(std::string M, recur_type w, Xml *X) : match(std::move(M)), what(w), xml_val(X) {}
    XmlAction(std::string M, recur_type w, std::string X) : match(std::move(M)), what(w), string_val(std::move(X)) {}

    [[nodiscard]] auto is_ok() const -> bool { return int_val != 0; }

    void mark_found() { int_val = 1; }
};
